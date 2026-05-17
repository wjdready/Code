"""
Blender RPC 服务器 - 允许外部程序发送代码到 Blender 执行
使用方法：
1. 在 Blender 的 Scripting 工作区运行此脚本
2. 服务器会在 localhost:8888 启动
3. 从 VSCode 或其他工具发送代码到 Blender 执行
"""

import bpy
import socket
import threading
import json

HOST = 'localhost'
PORT = 8888

class BlenderRPCServer(bpy.types.Operator):
    bl_idname = "wm.blender_rpc_server"
    bl_label = "Blender RPC Server"

    _timer = None
    _server_thread = None
    _running = False
    _commands = []
    _lock = threading.Lock()

    def execute_code(self, code):
        """在主线程中执行代码"""
        import sys
        from io import StringIO

        # 捕获 stdout
        old_stdout = sys.stdout
        sys.stdout = captured_output = StringIO()

        try:
            # 创建执行环境
            exec_globals = {
                'bpy': bpy,
                'C': bpy.context,
                'D': bpy.data,
                '__name__': '__main__',  # 让 if __name__ == "__main__" 生效
            }

            # 执行代码
            exec(code, exec_globals)

            # 获取输出
            output = captured_output.getvalue()

            return {
                "status": "success",
                "message": "代码执行成功",
                "output": output if output else None
            }
        except Exception as e:
            import traceback
            error_msg = traceback.format_exc()
            output = captured_output.getvalue()
            print(f"执行错误:\n{error_msg}", file=old_stdout)
            return {
                "status": "error",
                "message": str(e),
                "traceback": error_msg,
                "output": output if output else None
            }
        finally:
            sys.stdout = old_stdout

    def server_loop(self):
        """服务器循环（在独立线程中运行）"""
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server.bind((HOST, PORT))
        server.listen(5)
        server.settimeout(1.0)  # 设置超时以便可以检查 _running 标志

        print(f"{'='*60}")
        print(f"Blender RPC 服务器已启动")
        print(f"监听地址: {HOST}:{PORT}")
        print(f"{'='*60}")

        while self._running:
            try:
                client, addr = server.accept()
                print(f"收到连接: {addr}")

                # 接收数据
                data = b''
                while True:
                    chunk = client.recv(4096)
                    if not chunk:
                        break
                    data += chunk
                    if len(chunk) < 4096:
                        break

                if data:
                    try:
                        request = json.loads(data.decode('utf-8'))
                        code = request.get('code', '')

                        # 将命令添加到队列
                        with self._lock:
                            self._commands.append((client, code))
                    except json.JSONDecodeError:
                        # 如果不是 JSON，直接当作代码执行
                        with self._lock:
                            self._commands.append((client, data.decode('utf-8')))
                else:
                    client.close()

            except socket.timeout:
                continue
            except Exception as e:
                print(f"服务器错误: {e}")

        server.close()
        print("RPC 服务器已停止")

    def modal(self, context, event):
        """在主线程中处理命令队列"""
        if event.type == 'TIMER':
            with self._lock:
                while self._commands:
                    client, code = self._commands.pop(0)

                    print(f"\n{'='*60}")
                    print("执行代码:")
                    print(code[:200] + ('...' if len(code) > 200 else ''))
                    print(f"{'='*60}\n")

                    result = self.execute_code(code)

                    # 发送结果回客户端
                    try:
                        response = json.dumps(result, ensure_ascii=False)
                        client.sendall(response.encode('utf-8'))
                    except:
                        pass
                    finally:
                        client.close()

                    if result['status'] == 'success':
                        print("✓ 执行成功")
                    else:
                        print(f"✗ 执行失败: {result['message']}")

        return {'PASS_THROUGH'}

    def execute(self, context):
        self._running = True

        # 启动服务器线程
        self._server_thread = threading.Thread(target=self.server_loop, daemon=True)
        self._server_thread.start()

        # 添加定时器处理命令队列
        wm = context.window_manager
        self._timer = wm.event_timer_add(0.1, window=context.window)
        wm.modal_handler_add(self)

        return {'RUNNING_MODAL'}

    def cancel(self, context):
        self._running = False

        if self._server_thread:
            self._server_thread.join(timeout=2.0)

        wm = context.window_manager
        wm.event_timer_remove(self._timer)

        print("RPC 服务器已关闭")

def register():
    bpy.utils.register_class(BlenderRPCServer)

def unregister():
    bpy.utils.unregister_class(BlenderRPCServer)

if __name__ == "__main__":
    register()
    bpy.ops.wm.blender_rpc_server()

"""
VSCode 到 Blender 的客户端工具
用于从 VSCode 发送代码到 Blender 执行
"""

import socket
import json
import sys

HOST = 'localhost'
PORT = 8888

def send_code(code):
    """发送代码到 Blender 执行"""
    try:
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect((HOST, PORT))

        # 发送代码
        request = json.dumps({'code': code})
        client.sendall(request.encode('utf-8'))

        # 接收结果
        response = b''
        while True:
            chunk = client.recv(4096)
            if not chunk:
                break
            response += chunk

        client.close()

        # 解析结果
        result = json.loads(response.decode('utf-8'))
        return result

    except ConnectionRefusedError:
        return {
            'status': 'error',
            'message': '无法连接到 Blender。请确保 blender_rpc_server.py 正在运行。'
        }
    except Exception as e:
        return {
            'status': 'error',
            'message': str(e)
        }

def send_file(filepath):
    """发送整个文件到 Blender 执行"""
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            code = f.read()
        return send_code(code)
    except Exception as e:
        return {
            'status': 'error',
            'message': f'读取文件失败: {e}'
        }

def send_selection(code):
    """发送选中的代码到 Blender 执行"""
    return send_code(code)

if __name__ == '__main__':
    # 设置 UTF-8 输出
    import sys
    if sys.platform == 'win32':
        import codecs
        sys.stdout = codecs.getwriter('utf-8')(sys.stdout.buffer, 'strict')

    if len(sys.argv) < 2:
        print("用法:")
        print("  python blender_client.py <file.py>  # 执行整个文件")
        print("  python blender_client.py -c '<code>'  # 执行代码片段")
        sys.exit(1)

    if sys.argv[1] == '-c':
        code = sys.argv[2]
        result = send_code(code)
    else:
        filepath = sys.argv[1]
        result = send_file(filepath)

    # 显示输出
    if result.get('output'):
        print(result['output'])

    # 显示状态信息
    if result['status'] == 'success':
        if not result.get('output'):
            print("✓ 执行成功")
    else:
        print(f"✗ 错误: {result['message']}")
        if result.get('traceback'):
            print(result['traceback'])

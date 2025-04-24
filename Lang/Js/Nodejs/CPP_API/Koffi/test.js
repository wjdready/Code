const koffi = require('koffi');

// 加载多个共享库
const msvcrt = koffi.load('msvcrt');
const user32 = koffi.load('user32.dll');

// 声明数学函数
const pow = msvcrt.func('double pow(double, double)');
const sin = msvcrt.func('double sin(double)');
const rand = msvcrt.func('int rand()');
const srand = msvcrt.func('void srand(unsigned int)');

// 声明Windows API函数
const MessageBox = user32.func('int MessageBoxA(void *hwnd, const char *text, const char *caption, unsigned int type)');

// 测试数学函数
srand(Date.now()); // 设置随机种子
console.log('2的3次方:', pow(2, 3));
console.log('sin(π/2):', sin(Math.PI/2));
console.log('随机数:', rand());

// 测试弹窗
MessageBox(null, 'Hello from Node.js!', 'Koffi测试', 0);


ES6 模块的使用

## 多个导出内容

下面演示同时导出多个内容以及包含默认内容

##### index.html

```html
<!DOCTYPE html>
<html>

<head>
    <title>Demo</title>
    <script type="module" src="main.js"></script>
</head>

<body>
    Hello, World
</body>

</html>
```

> 需要注意的是 script 标签的 type 属性必须为 module, 才能正常使用 module 功能

##### module.js

```js
export const message = 'Hello World!'
// 默认导出部分
export default message

// 导出函数
export function greet(name) {
    return `Hello, ${name}!`
}

// 导出类
export class Person {
    constructor(name) {
        this.name = name
    }

    sayHello() {
        console.log(`Hello, my name is ${this.name}.`)
    }
}
```

##### main.js

```js
// 可以同时导入多个模块
import { greet, Person } from './module.js'
// 导入默认部分, 可以看到这时可以更改导出名称
import msg from './module.js'

console.log(msg)
console.log(greet('John'))

const person = new Person('Alice')
person.sayHello()
```

> 浏览器在处理 `main.js` 时会自动获取 `./module.js`, 因而不必在 html 中指定 src
> 这样即使有多个 js, 也只需入口脚本即可


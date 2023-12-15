

## 浏览器本地存储

localStorage 可以将数据保存到本地，即使页面被删除后仍然有效

sessionStorage API 和 localStorage 一致，但是窗口关闭后保存内容会被删除

```js
let person = { name: 'Tom', age: 18 }

localStorage.setItem('name', 'Jim')
localStorage.setItem('Tom', JSON.stringify(person))

let name = localStorage.getItem('name')
let p1 = localStorage.getItem('Tom')

console.log(name)
console.log(JSON.parse(p1))
```

> `localStorage` 和 `sessionStorage` 只能存字符串, 因此对象需要进行 JSON 转换
> 此外还有 `removeItem`, `clear` 等函数

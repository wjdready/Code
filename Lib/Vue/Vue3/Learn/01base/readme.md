

官方文档:
https://cn.vuejs.org/guide/quick-start.html

## HelloWorld

借助 script 标签直接通过 CDN 来使用 Vue 在创建 Demo 时会非常方便

> 当然我们也可以通过 wget 将 vuejs 下载到本地, 然后使用本地连接 ./ 即可

Vue 提供了全局构建版本和 ES 模块构建版本，这里使用 ES 模块构建版本, 这样我们就可以以模块的形式使用 vue

我们可以使用导入映射表 (Import Maps) 来告诉浏览器如何定位到导入的 vue，从而 import CDN 模块时可以简写为 'vue' 即可

```html
<div id="app">{{ message }}</div>

<script type="importmap">
  {
    "imports": {
      "vue": "https://unpkg.com/vue@3/dist/vue.esm-browser.js"
    }
  }
</script>

<script type="module">
    import { createApp, ref } from 'vue'

    createApp({
        setup() {
            const message = ref('Hello Vue!')
            return {
                message
            }
        }
    }).mount('#app')
</script>
```

> 这里我们导入 createApp 函数来创建 vue 实例
> 并传入一个组件对象，其中包含一个名为 setup 的方法, vue 在创建实例时将调用该方法
> 
> 在 setup 方法中，我们定义了一个名为 message 的响应式变量
> 使用 ref 函数将其包装为响应式对象。响应式变量可以在模板中进行双向绑定，当  message  的值发生变化时，相关的界面也会自动更新
>
> 最后，我们通过调用 mount('#app') 方法将应用程序挂载到具有 id 为 'app' 的 HTML 元素上。这将使 Vue 应用程序在该元素中进行渲染。 


## 拆分模块

index.html

```html
<script type="module">
    import { createApp, ref } from 'vue'
    import App from './App.js'

    const app = createApp(App)
    app.mount('#app')
</script>

<div id="app"></div>
```

App.js

```js
import { ref } from 'vue'
export default {
    setup() {
        const count = ref(0)
        return { count }
    },
    template: `<div>count is {{ count }}</div>`
}
```

## 单文件组件

```
npm i @vue/compiler-sfc -s -D
```
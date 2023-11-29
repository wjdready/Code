
vue2 官方教程: https://v2.cn.vuejs.org/v2/guide/

借助 script 标签直接通过 CDN 来使用 Vue 在创建 Demo 时会非常方便

> 当然我们也可以通过 wget 将 vuejs 下载到本地, 然后使用本地连接 ./ 即可

Vue 提供了全局构建版本和 ES 模块构建版本，这里使用 ES 模块构建版本, 这样我们就可以以模块的形式使用 vue

> 需要注意的是使用模块版本需要设置脚本类型为 `module`

## Hello World

该 Demo 也是作为后续示例的模板

index.html 

```html
<script type="module">
    import Vue from 'https://cdn.jsdelivr.net/npm/vue@2.7.14/dist/vue.esm.browser.js'
    
    Vue.config.productionTip = false

    var app = new Vue({
        el: '#app',
        data: {
            message: 'Hello Vue!'
        }
    })
</script>

<div id="app">
    {{ message }}
</div>
```

Vue 构造函数接收一个配置对象, 这里直接传入了一个匿名对象.

对象中 `el` 用于指定当前 Vue 实例为哪个容器服务，值通常为 css 选择器字符串.

`data` 用于导出数据，让 `el` 所指定的容器能够使用，容器中使用插值语法 `{{ mydata }}` 来对数据进行引用, 双花括号中的内容为 js 表达式

> 每个 Vue 实例只能对应一个容器，而且每个容器只能用于一个 Vue 实例
> 
> 那么如果容器中有非常多的数据要显示，data 中定义的数据岂不是会非常多和复杂呢？
>
> Vue 通过组件的方式来解决这个问题, 组件后面会讲到


## 模板语法

插值语法，形式为 `{{ name }}`, 只能用于解析标签体中的内容

指令语法，可用于解析标签属性，标签体内容，绑定事件等

比如通过指令语法 v-bind 来动态修改标签属性值:

```html
<a v-bind:href="url">link</a>
<a :href="url">link</a>
<a :href="url.toUpperCase()">link</a>
```

> 可以看到和插值语法一样，被解析的部分是支持 js 表达式的
> 这意味着我们可以在配置对象 data 中导出一个 obj, 然后再通过表达式取出数据, 比如 obj.name, obj.age

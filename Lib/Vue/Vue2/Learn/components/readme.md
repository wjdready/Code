

## 组件化

```html
<script type="module">
    import Vue from 'https://cdn.jsdelivr.net/npm/vue@2.7.14/dist/vue.esm.browser.js'
    Vue.config.productionTip = false

    // 创建组件
    const comp1 = Vue.extend({
        template: '<p>{{ msg }}</p>',
        data() {
            return { msg: 'I am comp1' }
        }
    })

    const comp2 = Vue.extend({
        template: '<p>{{ msg }}</p>',
        data() {
            return { msg: 'I am comp2' }
        }
    })

    // 全局注册, 所有实例都可访问和使用
    Vue.component('comp2', comp2)

    const vm = new Vue({
        // 局部注册, 只有当前实例可访问和使用
        components: {
            comp1
        },
    }).$mount('#app')

</script>

<div id="app">
    <!-- 通过组件标签引用组件 -->
    <comp1></comp1>
    <comp2></comp2>
</div>
```

通过静态方法 extend 创建组件，组件配置对象中 `template` 属性用于设置组件的 html 模板

data 属性必须是函数形式，这是为了对 data 中的内容进行深拷贝，以便复用组件时，每个数据都是独立的

组件没有 `el` 属性，因为组件是可拆卸的，并由 vm 管理

> 需要注意的是组件命名, 多个单词命名时支持 my-comp 和 MyComp 两者写法
> my-comp 可直接使用 `<my-comp></my-comp>`, 而 MyComp 需要脚手架支持
> 引用组件 `<comp1/>` 的写法需要脚手架支持
> `const comp1 = Vue.extend({})` 可简写为 `const comp1 = {};`, vue 在初始化时会自动执行 `extend`


## 组件嵌套

```html
<script type="module">
    import Vue from 'https://cdn.jsdelivr.net/npm/vue@2.7.14/dist/vue.esm.browser.js'
    Vue.config.productionTip = false

    // 创建组件
    const comp1 = Vue.extend({
        template: '<p>{{ msg }}</p>',
        data() {
            return { msg: 'I am comp1' }
        }
    })

    const comp2 = Vue.extend({
        template: '<p>{{ msg }}</p>',
        data() {
            return { msg: 'I am comp2' }
        }
    })

    const app = Vue.extend({
        template: `
        <div>
            <comp1></comp1>
            <comp2></comp2>
        </div>
        `,
        components: {comp1, comp2},
    })

    const vm = new Vue({
        template: '<app></app>',
        components: { app },
    }).$mount('#root')
</script>

<div id="root">
</div>
```

> 可以看到实例中只需放入 app 组件，由 app 组件管理和组织多个组件
> app 中可以不放数据而是纯粹的结构


## 组件本质

执行 extend 方法后返回一个组件，而组件其实是一个构造函数，以便通过 new 来构造出组件实例。以后为了方便，称组件实例为 `vc`。

在 vm 或 vc 实例中都有数组 `children` 用来保存当前实例已经实例化的组件。放在 components 配置项中是组件组件构造函数，也就是组件模板。当在 `template` 解析时发现有引用到组件，vue 将从构造函数中构造对应的组件，并将组件实例存放在当前组件自身的 `children` 数组变量中。

Vue 和 VueComponent 都继承自更加基础的 Vue 原型，这一点通过 `VueComponent.prototype.__proto__ === Vue.prototype` 值为 true 可以确定，vue 通过更改 VueComponent 的隐式原型链，将其指向更加其更加基础的 Vue 原型从而实现继承。

也就是说，vc 和 vm 大部分功能都是相同的，仅有少数区别。


## 单文件组件

脚手架

```sh
npm install vue@cli -g
vue create hello-world
```

main.js

```js
import Vue from 'vue'
import App from './App.vue'

Vue.config.productionTip = false

new Vue({
    render: h => h(App)
}).$mount('#app')
```
> 入口文件 main.js 创建了 vue 实例并引入 App 组件
> 
> 因为采用脚手架构建的工程默认引入的是 runtime 版本，因此不包含模板解析器，不能使用 `template` 配置元素引入组件。但是在脚手架中的工程，import 时已经完成模板解析，这时就要使用 render 函数。
> 
> 配置对象中的 render 函数在页面载入时被调用，它会传入一个 createElement 函数用来从组件中生成 html 元素以渲染到页面。
> 
> 因为 render 函数内不需要引用到任何 vm 实例中的数据，因此可直接使用箭头函数的写法也没有关系:
> ```js
> render: (createElement) => {
>     return createElement(App)
> }
> ```
> 又因为函数中只有一个返回语句，故可以进一步简写成 `render: h => h(App)`


App.vue

```html
<template>
    <div>
        <HelloWorld></HelloWorld>
    </div>
</template>
  
<script>
import HelloWorld from './components/HelloWorld.vue'

export default {
    name: 'App',
    components: {
        HelloWorld
    }
}
</script>
  
<style></style>
```

> 一般来说 App 被定义为负责管理所有其他组件，故这里不需要包含任何数据
> 
> App 组件 首先 import 其他组件，然后注册到自身的 components 属性中，最后在自己的模板结构中引入组件

components/HelloWorld.vue

```html
<template>
    <div>
        {{ msg }}
    </div>
</template>

<script>
export default {
    name: 'HelloWorld',
    data() {
        return { msg: 'Hello World' }
    }
}
</script>

<style>
</style>
```

> 更多的组件被统一放到 `components` 文件夹，并按层级管理

##### 关于不同版本的 Vue：
vue.js 与 vue.runtime.xxx.js 的区别：

* vue.js 是完整版的 Vue，包含：核心功能+模板解析器 
* vue.runtime,xxx.js 是运行版的Vue，只包含：核心功能；没有模板解析器 

因为 vue.runtime,xxx.js 没有模板解析器，所以不能使用 template 配置项，需要使用 render 函数接收到的 createElement 函数去指定具体内容


## 配置 vue 工程

配置文件: vue.config.js
参考: https://cli.vuejs.org/zh/config/

一些有用的配置:

```json
lintOnSave: false   // 关闭保存文件时的语法检测
```

## ref 属性

App.vue

```html
<template>
    <div>
        <h1 ref="title">Hehe</h1>
        <HelloWorld ref="helloworld"></HelloWorld>
        <button @click="show">button</button>
    </div>
</template>

<script>
import HelloWorld from './components/HelloWorld.vue'

export default {
    name: 'App',
    components: {
        HelloWorld
    },
    methods: {
        show() {
            console.log(this.$refs.title)
            console.log(this.$refs.helloworld)
        }
    }
}
</script>
```

> 在标签上定义的 ref 属性, 在 js 中可通过 vue 实例的 `$refs` 访问，当标签是 html 内置标签时，`this.$refs.xxx` 获取的是标签 DOM 本身
> 
> 然而当标签是组件时，则获取的是标签所引入的组件实例，这也是 vue 多个组件间通信的常用方法


## props 配置


当引入组件时， 可通过设置一些标签属性来作为参数传入组件，比如:

```html
<HelloWorld name="Jim" :age="12" />
<HelloWorld name="Tom"/>
```

在组件配置对象上，可使用 props 来接收引入组件时配置的属性:

HelloWorld.vue

```html
<template>
    <div>
        I am {{ name }}, {{ myage }} years old.
        <button @click="add">add</button>
    </div>
</template>

<script>
export default {
    name: 'HelloWorld',
    data() {
        return { myage: this.age }
    },
    methods: {
        add() {
            this.myage++;
        }
    },
    props: {
        name: {
            type: String,
            required: true
        },
        age: {
            type: Number,
            required: false,
            default: 18
        }
    }
}
</script>
```

props 还支持另外 2 种简写方法:

* 如果不需要默认值和 `required`, 则可写为 `props: {name:String, age: Number}`
* 如果不要求数据类型, 则可简化为 `props: ['name', 'age']`

> 组件对 props 中的数据是只读的，上例为了对 age 进行改变，维护了自己 myage, 传入的 age 只是作为 myage 的初始值.
>
> 另外值得注意的是, 引用 HelloWorld 组件时，对 age 属性进行了一个 v-bind 绑定，这是为了使最终传入 age 的值是一个 Number 类型，因为 v-bind 接收的是一个 js 表达式, `18` 刚好是一个常量表达式, 如果没有 v-bind 则传入的属性将被解析为字符串。与组件期望的 Number 不符。


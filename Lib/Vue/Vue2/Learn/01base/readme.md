
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

指令语法，可用于解析标签属性，标签体内容，绑定事件等.

指令语法有很多，下面将逐个介绍

#### v-bind 单向数据绑定

指令语法 v-bind 可用来单向动态修改标签属性值:

```html
<a v-bind:href="url">link</a>
<!-- 或简写为 -->
<a :href="url">link</a>
<a :href="url.toUpperCase()">link</a>
```

> 可以看到和插值语法一样，被解析的部分是支持 js 表达式的
> 这意味着我们可以在配置对象 data 中导出一个 obj, 然后再通过表达式取出数据, 比如 obj.name, obj.age

#### v-model 双向数据绑定

```html
<input type="text" v-model:value="name"/>
<!-- 或简写为如下, 因为 v-model 默认收集的就是 value 值 -->
<input type="text" v-model="name"/>
```

双向数据绑定意味着当输入框内数据发送改变，data 中的数据也会发生改变

双向数据绑定一般都应用在表单类元素上，如 input、select 等

#### el 与 data 的两种写法

vue 中提供给用户调用的方法都以 `$` 符号开头, 通过调用 `$mount()` 方法可以动态地修改 vue 所挂载地元素, 这时配置对象中 `el` 可以为空，因为我们将会通过 `$mount()` 方法进行挂载

另外配置对象中的 `data` 也可以是一个函数，然后返回数据对象, 对于组件来说必须使用函数的方法返回数据对象

```js
var app = new Vue({
    data() {
        return {
            name: 'Hello Vue!'
        }
    }
})
app.$mount('#app')
```

> 注意，由 vue 所管理的函数不能使用箭头函数, 因为这时 this 就不再是 vue 实例，从而导致引用失败

#### MVMM 模型

vue 采用 MVVM 模型, vue 实例可以看作是模型中的 VM 部分， 以后说 vm 即为 vue 实例

data 中的所有属性最后都出现在了 vue 实例上
vue 实例所包含的所有属性 (包括原型上的所有属性)，在 vue 模板部分都可以直接使用.

例如我们可以直接在模板部分直接引用 `$mount`, 这会将 `$mount` 方法的源码展示出来

```js
// app 是新建的 vue 实例, 这里直接使用 data 中的 name
console.log(app.name)

// 模板部分直接引用 vue 实例中的内容
<div id="app">
    {{ $mount }}
</div>
```


#### 数据代理

当创建一个实例并传入 data 时，vue 会在实例中创建一个 _data 属性用来存放 data

同时为 data 中的每一个属性在实例也创建相同的属性，然后通过 Object.defineProperty 设置 set 和 get 函数将 _data 中的属性与实例创建的属性关联起来

类似以下实现

```js
class MyVue {
    _create_link(data) {
        const propertyKeys = Object.getOwnPropertyNames(data);
        for (const propertyKey of propertyKeys) {
            Object.defineProperty(this, propertyKey, {
                get() {
                    return data[propertyKey];
                },
                set(value) {
                    data[propertyKey] = value
                },
            });
        }
    }

    constructor(obj) {
        this._data = obj.data
        this._create_link(obj.data)
    }
}

const vm = new MyVue({
    data: {
        name: 'Jim',
        age: 12
    }
})

console.log(vm._data.name)  // Jim
vm._data.name = 'Tom'
console.log(vm.name)        // Tom
```


## 事件处理

```js
new Vue({
    data: {
        name: 'Hello Vue!'
    },
    methods: {
        showInfo(msg, event) {
            alert(msg)
            console.log(event)
        }
    }
}).$mount('#app')

<div id="app">
    <button v-on:click="showInfo(name)">show</button>
    <button @click="showInfo(name, $event)">show</button>
</div>
```


和 data 类似, 模型中用到的函数放在一个专门的配置属性 `methods` 中, 同时 methods 中的方法也将被映射到 vm 实例中, 可通过示例本身直接访问到

可以看到事件回调函数支持自定义传参，同时对于拥有默认参数的，如 button 可通过占位符 `$` 来访问，并且随意更改默认参数的位置

> @事件="xxx" 后面的 xxx 其实是一个 js 表达式, 这意味着可以直接执行简单的语句, 比如令 data 中的 x++，不一定非得调用函数 

## 事件修饰符

在事件类型后面添加 `.修饰内容` 可更改事件的一些行为

vue 中用到的修饰符有:

1. prevent：阻止默认事件（常用）
2. stop：阻止事件冒泡（常用）
3. once：事件只触发一次（常用）
4. capture：使用事件的捕获模式
5. self：只有 event.target 是当前操作的元素时才触发事件
6. passive：事件的默认行为立即执行，无需等待事件回调执行完毕
7. native: 使用DOM原生事件

```html
<a href="http://www.baidu.com" @click.prevent="showInfo">show</a>
```

> 修饰符连写, 比如 `@click.prevent.stop` 既又阻止默认事件又阻止冒泡，

## 键盘事件

keyup 和 keydown 事件, 回调参数 event 中会包含按键名和按键码。每来按一次按钮触发一次，但是我们可以通过 `e.key` 来筛选

vue 提供了按键筛选的事件修饰符, 并对为常用按键码设置了别名:

常用的包括 `enter` 、`delete`、`esc`、`space`、`up`、`down`、`left`、`right`

Vue 未提供别名的按键，可以使用按键原始的 key 值去绑定，但注意要转为kebab-case（短横线命名）

系统修饰键（用法特殊）：ctrl、alt、shift、meta
1. 配合 keyup 使用：按下修饰键的同时，再按下其他键，随后释放其他健，事件才被触发
2. 配合 keydown 使用：正常触发事件

```js
<input type="text" @keyup.enter="showInfo"/>
```

> 同理需要多个按钮同时按才触发可写成比如 `@keyup.ctrl.y`


## 计算属性

```js
new Vue({
    data: {
        name: ''
    },
    computed: {
        greet: {
            get() {
                return 'Hello ' + this.name + "!"
            },
            set(value) {
                this.name = value
            }
        }
    }
}).$mount('#app')

<div id="app">
    <input type="text" v-model="name"/> {{ greet }}
</div>
```

配置在 `computed` 中的属性会被 vue 解析为对象，该对象需提供 `get` 和 `set` 方法，vue 在将该属性关联到 vm 模型上时会做一个数据代理，使用的 get 和 set 方法就是该对象提供的方法。

这样一来当访问或修改该数据会调用用户提供的 `get` 和 `set` 方法

和在 methods 中通过定义一个有 return 值的 `greet()` 方法，然后在视图部分直接调用 `greet()` 以获取计算后的数据不同的是，使用 `computed` 时 vue 会进行优化。

如果使用 `methods` 实现, 那么当在视图中多个地方调用到 `greet()` 方法时，每次刷新视图都会把各个地方的 `greet()` 都执行一遍，而使用 `computed` 实现，vue 会建立缓存机制，保证了只需读取一次，同时还能监听 `greet` 的 get 中引用的数据，保证了其中数据只要发生变化就会刷新视图更新数据。

###### 计算属性的简写

如果只需使用 get 方法用来展示数据，还可简写为方法的形式:

```js
computed: {
    greet() {
        return 'Hello ' + this.name + "!"
    }
}
```

## 监视属性

watch 配置属性可以用来对某些数据进行监视，当数据发生变化时，会触发回调函数。

```js
const vm = new Vue({
    data: {
        name: {
            a: 65,
            b: 'Good'
        }
    },
    watch: {
        // 监视整个对象
        name: {
            // 初始化时让 handler 调用一下
            immediate: true,
            // 开启深度监视
            deep: true,
            // 数据发送变化时调用
            handler(newval, oldval) {
                console.log(newval.a, oldval)
            }
        }
    }
}).$mount('#app')

// 监视指定对象
vm.$watch('name.a', function (newval, oldval) {
    console.log(newval, oldval)
});

<div id="app">
    <button @click="name.a++">{{String.fromCharCode(name.a)}}</button>
</div>
```

> vue 为了效率，需通过 `deep: true` 开启深度监视整个对象, 这时，对象中任一数据发生变化都会触发回调
> 若不开启深度检测，则 vue 仅仅监测对象本身是否被赋予新值
> 
> 当然对数据的监视也是可以动态设置的，比如获取 vue 实例再通过调用 `$watch` 方法实现监视
>
> 和 computed 属性类似，这里用到了简写形式
> 也就是说，当监视对象的配置中只需设置 handler 方法时，可以直接设置监视对象为函数


## 监视属性 VS 计算属性

由于计算属性的 get 回调需要一个返回值，当需要执行异步操作时，返回的结果无法一下子给出，实现起来会非常麻烦。

而监视属性的 handler 回调不需要返回值，执行异步时操作时，值的改变可以在函数返回后的任一时刻发生，因此实现起来就比较自然

```js
const vm = new Vue({
    data: {
        a: 'a',
        b: 'b',
    },

    watch: {
        a(val) {
            setTimeout(() => {
                this.b = 'b' + val
            }, 1000);
        }
    },

    computed: {
        c() {
            return 'c' + this.a
        },
    },
}).$mount('#app')

<div id="app">
    <button @click="a=a+'1'">change</button><br/>{{b}}<br/>{{c}}
</div>
```

> 值得注意的是 setTimeout 应传入箭头函数，因为箭头函数没有自己的 this 绑定，它会捕获当前上下文的 this 值
> 在这里是 vm 实例，从而可以在回调函数中通过 this 访问到 vm 中的数据
>
> 而如果传入 function 形式, 这时 this 绑定的是 window, 就无法访问到 vue 实例中数据了


## 绑定 class 样式

前面说过 v-bind 中绑定的内容实际接收的是 js 表达式, 但在此之前 vue 会先对绑定内容进行类型判断

如果是数组，则 vue 会解析数组中的字符串并将它们拼接到一起
如果是对象，则解析对象中的布尔值，标记为 true 的属性才会被接纳，最终也是生成一个字符串

如下所示:

```js
const vm = new Vue({
    data: {
        classStr: 'bg radius size',
        classArr: [`bg`, `radius`, `size`],
        classObj: {
            bg: true,
            radius: true,
            size: true
        },
    }
}).$mount('#app')

<div id="app">
    <!-- 字符串写法 -->
    <div class="basic" :class="classStr">Test</div><br/>

    <!-- 数组写法 -->
    <div class="basic" :class="classArr">Test</div><br/>

    <!-- 对象写法 -->
    <div class="basic" :class="classObj">Test</div><br/>
</div>
```

* 对于字符串写法, 我们可以任意改变 data 中的 classStr 值，从而改变样式
* 对于数组，我们可以通过 push 或 shift 等方法来移除数组中的元素，从而达到动态改变样式
* 对象写法就像是预先定义了一些全局性主题，通过设置布尔值来使用某个主题，然后关闭某个主题

> 当然还可以使用函数的方法，例如 `:class="classFun()"` 而 `classFun()` 将返回一个样式字符串

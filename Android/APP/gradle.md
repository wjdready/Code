

Gradle 脚本是基于 Groovy 语言的，因此可以使用 Groovy 语言的特性和语法。Gradle 类似于 Java，可以使用 Java 库。

https://www.cnblogs.com/hellozjf/p/18473250

Groovy & Gradle 简单入门:

```groovy
// 支持弱类型定义 def

// 闭包是用 { } 括起来的一段代码块
def createClosure() {
    def str = 'hello';
    // 返回一个接收参数，有返回值的闭包
    return { name -> 
        "$str, $name"
    }
}
def c = createClosure();
println c('groovy')

// 函数调用时参数可以省略括号, 例如
println 'hello'
// 因此类似于 
buildscript { 
}
// 实际是一个接收闭包的函数，例如
def test(Closure closure) {
    def url = closure.call()
    println url
}
def url(param) {
   return param
}
test {
    url 'https://www.baidu.com'
}

// 调试和打印
// 创建一个 task 可以调用 app 里面的 main 方法并传递参数
task mydebug(type: JavaExec) {
    classpath = sourceSets.main.runtimeClasspath
    main = 'org.example.App'
    args = ['arg1', 'arg2', 'arg3']
}

// 当然也可以只打印信息
task mydebug() {
    println 'hello'
}

// 然后通过 gradle mydebug 执行

// 创建工程
mkdir demo && cd demo
gradle init
```

## 配置 Gradle 镜像源

参考: https://www.cnblogs.com/Chary/articles/18657340

```groovy

allprojects {
    repositories {
        maven { url 'https://maven.aliyun.com/repository/central/' }
        maven { url 'https://maven.aliyun.com/repository/google/' }
        maven { url 'https://maven.aliyun.com/repository/public/' }
        mavenLocal()
        // ... 各个模块里面的 build.gradle 里面 implementation 之类的依赖, 就是从这里的仓库查找的.
    }
}

buildscript {
    repositories {
        maven { url 'https://maven.aliyun.com/repository/central/' }
        maven { url 'https://maven.aliyun.com/repository/google/' }
        maven { url 'https://maven.aliyun.com/repository/public/' }
        mavenLocal()
        // ...
    }
    dependencies {
        // ... 这里的库是就是从buildscript里的仓库找的.
    }
}
```

### 全局和局部配置

似乎无效

https://blog.csdn.net/PingGuoAiShangMaiD/article/details/141535669

```shell
# Maven中央仓库的镜像
maven.repo.url=https://repo1.maven.org/maven2
# 阿里云的Maven中央仓库镜像
mavenCentralUrl=https://maven.aliyun.com/repository/public
# JCenter的镜像地址（注意：JCenter已经停止服务）
jcenter.url=https://jcenter.bintray.com/
# 其他常用国内源
maven.aliyun=https://maven.aliyun.com/repository/public
```


# QPushButton



## 一些常用功能

#### 长按后一直做动作

只要设置 setAutoRepeat 就可以了

```cpp
button->setAutoRepeat(true);
```

当然还可以设置触发动作的间隔时间, 以及按钮在开始发出信号之前等待的时间

```cpp
button->setAutoRepeatInterval(ms);
button->setAutoRepeatDelay(ms)
```

---
mytags: myblog
title: 利用 for 实现一些宏
date: 2023-02-04 18:07:24
categories: [C语言, 宏]
tags: [宏]
---

本文主要介绍利用 for 实现一些宏
<!-- more -->

for 有一个非常好的特性就是 for 括号分三段: ( 声明部分; 判断表达式; 结尾表达式)

利用这个特性我们可以写一些比较有意思的宏, 比如

* 线程锁自动进入和退出
  利用 for 循环执行结束后会自动执行结尾表达式, 简化资源锁地使用
  ```c
  #define into_mutex_env(lock) \
    for(int x = ({mutex_lock(lock); 0}); x < 1; x++, mutex_unlock(lock))

  /* 在进入大括号时自动加锁, 退出时自动解锁 */
  into_mutex_env(somelock) {
      // do something
  }
  ```

* 利用 for 声明部分定义变量然后在大括号中使用, [参考](https://stackoverflow.com/questions/75338894/how-to-declare-multiple-different-type-variables-in-one-line-in-c-language)
  因为声明部分的表达式只能定义一种类型的变量, 为了让 for 只执行一次, 这里将指针当作 int 使用
  ```c
  #define ui_style_to_set(ui_style, part_n, part_id) \
      ui_style.styles[part_n].part = part_id;        \
      for (lv_style_t *this = &(ui_style.styles[part_n].style), *p = NULL; (int)p < 1; (int)p++)
  
  ui_style_to_set(styles.msgbox, 0, LV_MSGBOX_PART_BG)
  {
      lv_style_set_radius(this, LV_STATE_DEFAULT, 5);
      lv_style_set_border_width(this, LV_STATE_DEFAULT, 0);
      lv_style_set_bg_color(this, LV_STATE_DEFAULT, LV_COLOR_WHITE);
      lv_style_set_text_color(this, LV_STATE_DEFAULT, LV_COLOR_BLACK);
      lv_style_set_shadow_color(this, LV_STATE_DEFAULT, LV_COLOR_BLACK);
      lv_style_set_shadow_width(this, LV_STATE_DEFAULT, 10);
      lv_style_set_shadow_ofs_x(this, LV_STATE_DEFAULT, 5);
      lv_style_set_shadow_ofs_y(this, LV_STATE_DEFAULT, 5);
  }
  ```

* 用于断言
  ```c
  void assert_failed(void)
  {
      printf("assert failed\n");
  }

  #define assert(flag)    \
      for(int i = 0, flag = 0; i < 1; i++, ({if (flag != 0); assert_failed();}))

  int main(int argc, char const *argv[])
  {
      printf("Hello World\n");
    
      assert(res) {
          // do something
          res = 0;
      }

      return 0;
  }
  ```
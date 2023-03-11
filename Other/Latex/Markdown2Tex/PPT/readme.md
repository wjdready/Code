

[参考](https://github.com/alexeygumirov/pandoc-beamer-how-to)

# 基本使用

## YAML 块
下面列出了常用 YAML 块的可选项

```YAML
title: "My wonderful presentation"
author: "Alexey Gumirov"
institute: "My home office"
topic: "Pandoc how-to"
theme: "Frankfurt"
colortheme: "beaver"
fonttheme: "professionalfonts"
mainfont: "Hack Nerd Font"
fontsize: 10pt
urlcolor: red
linkstyle: bold
aspectratio: 169
titlegraphic: img/aleph0.png
logo: img/aleph0-small.png
date:
lang: en-US
section-titles: false
toc: true
header-includes:
    - \usepackage{multicol}
```


## 分级

使用 `--slide-level 2` 设置分级方式, 默认为 1, 即 markdown 1 级标题 `#` 就代表一个幻灯片页面, 而如果 `--slide-level 2` 的话， 1 级标题 `#` 会创建一个 Section 页, 耳二级标题才是一个幻灯片页.

## 列控制

有时候需要将页面分成多列以展示, 可以使用 `::: columns` 

每一列可设置占比: :::: {.column width=30%}

双等分

```md
## 双等分

::: columns

:::: column
Left column text.

Another text line.
::::

:::: column
- Item 1.
- Item 2.
- Item 3.
::::

:::
```

## 三等分 30:40:30 

::: columns

:::: {.column width=30%}
Left column text.

Another text line.
::::

:::: {.column width=40%}
Middle column list:

1. Item 1.
2. Item 2.
::::

:::: {.column width=30%}
Right column list:

- Item 1.
- Item 2.
::::

:::

## 行列共同布局

对于 `--slide-level 2` 选项, `##` 二级标题就代表一个幻灯片页的开始, 而 `###` 这时就是一个块, 它可以作为一行, 也可以放在列中

```md
## Fancy layout

### Proposal

- Point A
- Point B

::: columns

:::: column

### Pros

- Good
- Better
- Best

::::

:::: column

### Cons

- Bad
- Worse
- Worst

::::

:::

### Conclusion

- Let's go for it!
- No way we go for it!
```


### 增量

[参考](https://www.uv.es/wikibase/doc/cas/pandoc_manual_2.7.3.wiki?162)

增量时

```md
::: incremental

- Eat spaghetti
- Drink wine

:::
```

非增量时

```md
::: nonincremental

- Eat spaghetti
- Drink wine

:::
```

非增量中也可使用旧式增量方法

```md
::: nonincremental

>- Eat spaghetti
>- Drink wine

:::
```

### 插入停顿 （非常重要非常好)

```
content before the pause

. . .

content after the pause

```

[More information](https://pandoc.org/MANUAL.html#incremental-lists)


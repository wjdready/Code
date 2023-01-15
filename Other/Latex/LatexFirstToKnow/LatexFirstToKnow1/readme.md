# Latex 入门（一） 


## 1. 程序发展介绍

高德纳设计的 TeX 原本只是一个相对简单的程序, 命令 tex 就会调用最基本的 TeX 程序。它相对简单的格式 Plain TeX 进行排版。tex 读入 TeX 源文档, 输出一种称为 “设备无关” 的（DeVice Independent）格式, 即 DVI 文件, DVI 文件在过去是 TeX 的标准输出格式, 但功能比较受限, 不能嵌入字体和图形等, 在 PostScript 和 PDF 流行之后, DVI 格式就主要成为一种到 PS 或 PDF 格式的中间格式了。

程序 Dvips 将 DVI 文件转换为 PostScript 文件, 可以直接拿到支持 PostScript 的打印机上打印, 也可以通过 GhostScript 的 ps2pdf 或 Adobe Acrobat 提供的 Distiller 等程序再从 PostScript 文件转换为 PDF 文件。PDF 流行以后又有了能把 DVI 文件直接转换为 PDF 文件的 dvipdf 程序, 之后出现了更为先进的 dvipdfm 和 dvipdfmx, 可以支持更丰富的 PDF 功能和东亚字体等, 现在新的发行版中主要还在使用的是 dvipdfmx（常写做 DVIPDFMx）。这类把 DVI 文件转换为其他实用格式的程序常被称为 TeX 输出的驱动（driver）。

除了最初的 TeX 程序, 后来有许多人对 TeX 进行了扩展。先是有了 eTeX, 后来在 eTeX 的基础上, Hàn Thế Thành 设计了能直接输出 PDF 格式的 pdfTeX。不过 pdfTeX 程序也保留了输出 DVI 格式的能力, 因而现在很多输出DVI 格式的命令内部也是使用的 pdfTeX 程序。pdfTeX 的后继是 LuaTeX, 这是一种把脚本语言 Lua 和 TeX 结合起来的程序。eTeX 的另一发展则是 XeTex 程序, 它将中间层 DVI 格式扩充为更强大的 xdv 格式, 一般会直接调用 dvipdfmx 的后继 xdvipdfmx, 直接输出 PDF 格式。LuaTeX和 XeTex 都将原来 TeX 支持的 ACSII 编码改为 UTF-8 编码, 并且可以更方便地使用各种字体。TeX 程序连同这些扩展常被称为不同的 TeX 引擎（engine）。

不同的引擎都可以编译 Plain TeX、LATeX 或是 ConTeXt 等不同格式的文 档, 不同的组合就使用不同的命令

| 引擎:命令:格式 | Plain TeX  | LaTex    | ConTeXt  | 输出 |
| -------------- | ---------- | -------- | -------- | ---- |
| TeX/eTeX       | tex / etex | 不支持   | 不支持   | DVI  |
| pdfTeX         | tex        | latex    | 不支持   | DVI  |
| pdfTeX         | pdftex     | pdflatex | texexec  | PDF  |
| xeTex          | xetex      | xelatex  | 特殊参数 | PDF  |
| LuaTex         | luatex     | lualatex | context  | PDF  |

> 用 latex 和 pdflatex 命令排版在处理中文时都使用 CJK 宏包的机制，而 xelatex 则使用新的 xeCJK 宏包的机制。功能上 xelatex 最为方便，尤其是在处理中文时；而用 pdflatex 编译，一些宏包的兼容性更好一些。

## 2. Hello LaTex

```tex
\documentclass{article}

\begin{document}
    Hello \LaTeX
\end{document}
```

对于中文, 可通过使用 ctexart 文档类型来支持:

```tex
\documentclass[UTF8]{ctexart}

\begin{document}
    你好 \LaTeX
\end{document}
```

> UTF8 表明了中文所使用的编码,  其对应于 .tex 文本文档保存编码格式。

编译
```
pdflatex demo.tex
```

## 3. 基本文章框架
```tex
\documentclass[UTF8]{ctexart}

\title{杂谈勾股定理}
\author{张三}
\date{\today}

\bibliographystyle{plain}

\begin{document}

\maketitle
\tableofcontents

\section{勾股定理在古代}

\section{勾股定理在近代形式}

\bibliography{math}

\end{document}
```

> `\bibliographystyle` 声明参考文献的格式
> `\maketitle` 输出标题, 包含了三要素:标题，作者，日期
> `\tableofcontents`  输出目录
> `\bibliographystyle{math}` 则是提示 TeX 从文献数据库 math 中 获取文献信息，并输出参考文献列表

在 `\begin{document}` 之前的部分称为导言区（preamble），导言区通常用来对文档的性质做一些设置，或自定义一些命令。

> 注意这里要生成目录至少需要编译两次，让 LaTex 有机会读完整个论文来计算目录结构


## 4. 正文内容

### 4.1 换行与空格

需要注意一下几点

* 使用空行分段。单个换行并不会使文字另起一段，而只是起到使源代码更易读的作用。并且, 空行只起分段作用，使用很多空行不会起任何增大段间距的作用；
* 段前不用打空格，LaTex 会自动完成文字的缩进；
* 通常汉字后面的空格会被忽略，其他符号后面的空格则保留。单个的换行就相当于一个空格，因此源代码中大段文字可以安全地分成短行。
* 空格只起分隔单词或符号的作用，使用很多空格并不起任何增大字词间距的作用。

使用 xelatex 编译文档时，ctexart 文档类会调用 xeCJK 宏包，自动处理汉字与其他符号之间的距离，无论你有没有在它们之间加上正确的空格，这是十分方便的。不过，在源代码中仍然可以给汉字与其他符号之间加上一个空格，这会令代码更加清晰。

### 4.2 命令与环境

一个 LaTex 命令（宏）的格式为：
* 无参数： `\command`
* 有 n 个参数： `\comand<arg1><arg2>...<argn>`
* 有可选参数： `\command[opt1, opt2...optn]<arg1><arg2>...<argn>`

如果命令的参数不止一个字符（不包括空格），就必须用花括号括起来。可选参数如果出现，则用方括号括起来。

常用命令:
* `\footnote{内容}` 输出脚注
* `\emph{内容}` 强调内容
* `\zihao{-5}` 设置后文字号小 5 ('-' 表示小)
* `\kaishu` 设置后文字体字体楷书

`\zihaoh{字体大小}` 与 `\kaishu` 命令会影响后面的所有文字，直到整个分组结束，这种命令又称为声明（declaration）。 分组限定了声明的作用范围。

一个 LaTex 环境就是一个分组（group）, 最大的分组是表示正文的 document 环境，也可以用成对的花括号 { } 产生一个分组。

环境格式:
```tex
\begin{<环境名>}

内容

\end{<环境名>}
```
有的环境也有参数或可选参数，格式为：
```tex
\begin{<环境名>}[<可选参数>]<其他参数>

内容

\end{<环境名>}
```

常用环境:
* 引用环境 quote
* 摘要环境 abstract
* 定理环境
  定理环境是一类环境，在使用前需要先在导言区做定义：
  * `\newtheorem{thm}{定理}` 这就定义了一个 thm 的环境。
  定理环境可以有一个可选参数，就是定理的名字。
  * `\begin{thm}[勾股定理]`

### 4.3 数学公式

夹在行文中的公式称为“正文公式”（in-text formula）或“行内公式”（inline formula），可用一对美元符号 \$ \$ 括起来表示。

对比较长或比较重要的公式，一般则单独居中写在一行；为了方便引用，经常还给公式编号。这种公式被称作 “显示公式” 或 “列表公式”（displayed formula），使用 equation 环境就可以方便地输入这种公式。

### 4.4 使用图表

在 LaTex 中使用插图有两种途径，一是插入事先准备好的图片，二是使用 LaTex 代码直接在文档中画图。

插图功能不是由 LaTex 的内核直接提供，而是由 graphicx 宏包提供的。要使用 graphicx 宏包的插图功能，需要在源文件的导言区使用 \usepackage 命令引入宏包：

```tex
\usepackage{graphicx}
```

引入 graphicx 宏包后，就可以使用 \includegraphics 命令插图了。
```tex
\includegraphics[width=3cm]{xiantu.pdf}
```
> 这里 \includegraphics 有两个参数，方括号中的可选参数 width=3cm 设置图形在文档中显示的宽度为 3 cm，而第二个参数 xiantu.pdf 则是图形的文件名（放在源文件所在目录）
> 可选参数还有 scale=<缩放因子>, height=<高度> 等。

插入的图形就是一个有内容的矩形盒子，在正文中和一个很大的字符没有多少区别。为了不影响文字的排版，除了一些很小的标志图形，我们很少把插图直接夹在文字之中，而是使用单独的环境列出。而且很大的图形如果固定位置，会给分页造成困难。因此，通常都把图形放在一个可以变动相对位置的环境中，称为浮动体（float）。

figure 环境就是插图使用的浮动体环境。

```tex
\begin{figure}[ht]

\centering
\includegraphics[scale=0.6]{xiantu.pdf}

\caption{宋赵爽在《周髀算经》注中作的弦图（仿制），该图给出了勾股定 理的一个极具对称美的证明。}

\label{fig:xiantu}

\end{figure}
```

> 可选参数 [ht]，表示浮动体可以出现在环境周围的文本所在处（here）和一页的顶部（top）；
> figure 环境内部相当于普通的段落（默认没有缩进）；
> \centering 表示后面的内容居中；
> \caption 命令给插图加上自动编号和标题；
> \label 命令则给图形定义一个标签，使用这个标签就可以在文章的其他地方引用 \caption 产生的编号；

### 4.5 制作表格

制作表格，需要确定的是表格的行、列对齐模式和表格线，这是由 tabular 环境完成的：

```tex
\begin{table}[H]
    \begin{tabular}{|rrr|}
        \hline
        直角边 $a$ & 直角边 $b$ & 斜边 $c$\\
        \hline
        3 & 4 & 5 \\
        5 & 12 & 13 \\
        \hline
    \end{tabular}%
    \qquad
    ($a^2 + b^2 = c^2$)
\end{table}
```

> tabular 环境有一个参数，里面声明了表格中列的模式。
> |rrr| 表示表格有三列，都是右对齐，且在第一列前面和第三列后面各有一条垂直的表格线。
> 在 tabular 环境内部，行与行之间用命令 \\ 隔开，每行内部的表项则用符号 & 隔开。
> 表格中的横线则是用命令 \hline 产生的

表格与 \includegraphics 命令得到的插图一样，都是一个比较大的盒子。一般也放在浮动环境中，即 table 环境, 参数与大体的使用格式也与 figure 环境差不多，只是 \caption 命令得到的标题是 “表” 而不是 “图”。不过这里并没有给表格加标题，也没有把内容居中，而是把表格和一个公式并排排开，中间使用一个 \qquad 分隔。

> \qquad 产生长为 2 em（大约两个 “M” 的宽度）的空白。

因为我们已经使用 \qquad 生成足够长度的空格了，所以再用 \end{tabular} 后的注释符 % 取消换行产生的一个多余的空格，这正好达到我们预想的效果。

>  table 环境，在表示位置的参数处使用了 [H]，表示 “就放在这里，不浮动”。

[H] 选项并不是标准 LaTex 的 table 环境使用的参数，而是由 float 宏包提供的特殊功能。因此要让上面的代码正确运行，还要在导言区使用 \usepackage{float}。在这种表格很小（不影响分页），行文又要求连贯的场合，float 宏包的这种不浮动的图表环境是很有用的。


## 5. 自动化工具

### 5.1 参考文献处理工具

BibTex  是一个专用于处理 LaTex 文档文献列表的程序。 BibTex 使用的参考文献数据库其实就是一个后缀为 .bib 的文件。其内容格式如下:
```php
@BOOK{Kline,
    title = {古今数学思想},
    publisher = {上海科学技术出版社},
    year = {2002},
    author = {克莱因}
}
@ARTICLE{quanjing,
    author = {曲安京},
    title = {商高、赵爽与刘徽关于勾股定理的证明},
    journal = {数学传播},
    year = {1998},
    volume = {20},
    number = {3}
}

@BOOK{Shiye,
    title = {几何的有名定理},
    publisher = {上海科学技术出版社},
    year = {1986},
    author = {矢野健太郎}
}
```

一个文献数据库文件的格式并不复杂，每则文献包括类型、引用标签、标题、作者、出版年、出版社等信息，可以直接手工输入。在现实中，BibTeX 数据库经常并不需要我们自己录入，而可以从相关学科的网站直接下载或是从其他类型的文献数据库转换得到。即使是在需要我们自己录入的情况下，使用 JabRef 这种软件来管理也更方便，不易出错。

使用 BibTex 处理文献时, 编译步骤为:

```php
xelatex demo.tex
bibtex demo.aux
xelatex demo.tex
xelatex demo.tex
```
> 第一次运行 xelatex 为 BibTex 准备好辅助文件，确定数据库中的哪些文献将被列出来。
> 然后 bibtex 处理辅助文件 gougu.aux，从文献数据库中选取文献，按指定的格式生成文献列表的 LaTex 代码
> 后面两次 xelatex 再读入文献列表代码并生成正确的引用信息。

对文献数据库的引用如下:

```
\cite{文献标签}
```

> 文献标签是 .bib 中每个条目第一行出现的东西；
> 只有被引用的文献才会出现在参考文献列表中，如果要在列表中显示并不直接引用的文献，可以使用 \nocite{文献标签} 命令，一般是把它放在 \bibliography{文献数据库} 之前。

```tex
\nocite{Shiye}
\bibliography{math}
```

### 5.2 目录和引用

目录也是自动从章节命令中提取并写入目录文件中的，我们在提纲中就使用了 \tableofcontents 命令，它将在第二次 xelatex 编译时生效。

引用不仅限于参考文献。图表、公式的编号，只要事先设定了标签，同样可以通过辅助文件为中介引用。基本的交叉引用命令是 \ref{引用标签}，它以标签为参数，得到被引用的编号。

```tex
图 \ref{fig:xiantu} 是我国古代对勾股定理的一种证明 \cite{quanjing}。
```

公式编号的引用也可照此办理，不过需要先在公式中定义标签。

```tex
\begin{equation}\label{eq:gougu}
AB^2 = BC^2 + AC^2.
\end{equation}
```

而后在正文中以 (\ref{eq:gougu}) 引用。实际中引用公式非常常用，数学宏包 amsmath 就定义了 \eqref 命令，专门用于公式的引用，并能产生括号。

```tex
% 导言区使用 \usepackage{amsmath}
满足式 \eqref{eq:gougu} 的整数称为\emph{勾股数}
```


## 6. 设计文章的格式

绝大部分设计工作是在文章的导言区通过一些命令定义和参数设定来完成的，但往往相当复杂，好在其中的大多数工作可以通过使用一些宏包来简化，前面已经用到过 graphicx、float、amsmath 几种宏包完成一些工作，这里也要用到几种。

* 设计页面尺寸可以使用 geometry 宏包:
  ```tex
  \usepackage{geometry}
  \geometry{a6paper, centering, scale=0.8}
  ```
  > 这是最简单的设定方式，定义页面使用 A6 纸大小，版心居中，长宽占页面的 0:8 倍。

* 改变图表标题格式可以使用 caption 宏包:
  ```tex
  \usepackage[format=hang,font=small,textfont=it]{caption}
  ```
  > 设定图表所有标题使用悬挂对齐方式（即编号向左突出），整体用小字号，而标题文本使用斜体（对汉字来说就是楷书）。

* 增加目录的项目则可以用 tocbibind 宏包:
  ```tex
  \usepackage[nottoc]{tocbibind}
  ```
  > 宏包默认会在目录中加入目录项本身、参考文献、索引等项目。这里使用 nottoc 选项取消了在目录中显示目录本身

* 标题和作者的字体可以直接在命令中设定
  ```tex
  \title{\heiti 杂谈勾股定理}
  \author{\kaishu 张三}
  \date{\today}
  ```
  > 其中 \heiti 是和 \kaishu 类似的中文字体命令，把字体切换为黑体

### 6.1 自定义环境
正文中表示引用的 quote 环境里面还夹杂着字体命令，这种散落在各处的格式设置很难看清，而且不方便修改。为了解决这个问题，可以利用 \newenvironment 命令定义一个新的环境，在原来 quote 的基础上再增加格式控制：

```tex
\newenvironment{myquote} {
    \begin{quote}\kaishu\zihao{-5}
} {
\end{quote} 
}
```

> 这里，\newenvironment 有三个参数，第一个参数是环境的名字，后两个参数分别是在环境开始和末尾处的代码，因此，就可以用新环境。


### 6.2 自定义命令
原来数学公式中角度的单位 ^\circ 也很不直观，可以用 \newcommand 命令定义一个新的命令 \degree：

```tex
\newcommand\degree{^\circ}
```
> \newcommand 命令的两个参数分别是新命令和新命令的定义，于是我们就可以用 \$90\degree\$ 来代替原来不直观的 \$90^\circ\$ 了。


我们还是使用自定义的环境 myquote 和自定义的命令 \degree 代替了文中出现的特殊格式控制。类似地，在设定插图标题的字体时，并没有把字体、字号的命令塞进 \caption 命令的参数中，而是使用 caption 宏包统一设置。这样看起来比最 “直接” 的做法要多绕一道弯子，但好处是更清晰和更容易修改格式。这在只有一两页的内容上可能看不出什么特别的好处，但当你开始编写和维护几十上百页的长文档时，在设计阶段所付出的精力就会得到回报了。

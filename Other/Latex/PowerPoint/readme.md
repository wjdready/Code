
# 基本使用

[参考](https://www.overleaf.com/learn/latex/Beamer)

```tex
%!TEX program = xelatex

\documentclass{beamer}
\usepackage[UTF8]{ctex}

% 使用的主题
\usetheme{Madrid}

\title[USB 协议介绍与学习分享]{USB 协议介绍与学习分享}
\author[Jim]{Jim}
\date[\today]{\today}

\begin{document}

% 首页
\begin{frame}
    \titlepage
\end{frame}

\begin{frame}
	\frametitle{\textbackslash pause 使用}

	在这组幻灯片中 \pause
	
	这段文件将部分可见 \pause
	
	现在所有文字都可以看见了
\end{frame}

\begin{frame}
    \frametitle{item 中的尖括号}

    \begin{itemize}
        \item<1-> 该文字在 1+ 页均可见
        \item<2-3> 该文字在 2-3 页均可见
        \item<3> 该文字仅在第 3 页均可见
        \item<4-> 该文字在 4+ 页均可见
    \end{itemize}
\end{frame}

\end{document}
```

## 幻灯片页
Beamer 中通常会用一对 \begin{frame}、\end{frame} 来制作一页幻灯片（slide），例如:

```tex
\begin{frame}
	\frametitle{标题名称}
	主体内容
\end{frame}
```

## \pause

添加 \pause 会将 \pause 语句前面内容单独在新的一页 slide 上显示，实现了类似 PPT 动画中的点击页面出现下一段文字的效果。


使用 tikz 插入背景图片 (记得 \usepackage{tikz})

```tex
\newcommand\Background{%
\begin{tikzpicture}[remember picture,overlay]
\node[inner sep=0pt,outer sep=0pt,opacity=0.5]
  at (current page.center)
  {\includegraphics[width=\paperwidth,height=\paperheight]{logo}};
\end{tikzpicture}%
}
```


## 插入 logo 

```tex
\logo{\includegraphics[height=1cm]{logo}}
```

## 添加目录功能

在导言区添加, 作用是在每一个章节插入一个目录

```tex
\AtBeginSection[]
{
  \begin{frame}
    \frametitle{Table of Contents}
    \tableofcontents[currentsection]
  \end{frame}
}
```

使用上， 首先在 titlepage 后插入目录页

```tex
\begin{frame}
    \frametitle{Table of Contents}
    \tableofcontents
\end{frame}
```

然后再需要插入目录页的地方插入一个 Section, Section 在 \begin{frame}...\end{frame} 的环境外面

```tex 
\begin{frame}
...
\end{frame}

\section{First section}

\begin{frame}
...
\end{frame}
```


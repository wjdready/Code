

lex 命令可以通过给定的描述脚本来生成词法分析器(scanner), 该词法分析器为 C 语言源码, 需要编译后才能运行.

其中描述脚本的格式如下

file.l 
```
%{
declarations
%}

%%
rules
%%

auxiliary procedures
```
声明部分将被直接插入到输出的 C 源代码

规则部分是一行行的正则表达式, 每一个正则表达式后面可以跟一个 C 表达式用于处理匹配结果, 对于多行 C 表达式需使用 { } 括起来

辅助部分也将直接插入到输出的 C 代码, 可用于调试和后续处理

example:

```c
%{
#include <math.h>
#include <stdio.h>
%}

DIGIT    [0-9]
ID       [a-z][a-z0-9]*

%%

{DIGIT}+ {
    printf("An integer: %s (%d)\n", yytext,
    atoi(yytext));
}

{DIGIT}+"."{DIGIT}*    {
    printf("A float: %s (%g)\n", yytext, atof(yytext));
}

if|then|begin|end|procedure|function {
    printf("A keyword: %s\n", yytext);
}

{ID} printf("An identifier: %s\n", yytext);

"+"|"-"|"*"|"/" printf("An operator: %s\n", yytext);

"==" printf("An Equal : %s\n", yytext);

"=" printf("An = : %s\n", yytext);

"{"[^}\n]*"}" /* eat up one-line comments */

[ \t\n]+ /* eat up white space */

. printf("Unrecognized character: %s\n", yytext);

%%

int yywrap(void)
{
    return 0;
}

int main(int argc, char *argv[])
{
    ++argv, --argc;  /* skip over program name */
    if (argc > 0)
        yyin = fopen(argv[0], "r");
    else
        yyin = stdin;

    yylex();
}
```

测试运行

    $ lex file.l
    $ gcc lex.yy.c

    $ echo if a == 12 then x = 12.5 | ./a.exe
    A keyword: if
    An identifier: a
    An Equal : ==
    An integer: 12 (12)
    A keyword: then
    An identifier: x
    An = : =
    A float: 12.5 (12.5)

##### 参考文献

[https://docs.oracle.com/cd/E88353_01/html/E37839/lex-1.html](https://docs.oracle.com/cd/E88353_01/html/E37839/lex-1.html)


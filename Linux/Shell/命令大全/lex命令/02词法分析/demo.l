%{

%}

DIGIT    [0-9]

%%
-?{DIGIT}+                  { printf("DIGIT %s\n", yytext);}
"+"|"-"|"*"|"/"             { printf("Operator: %s\n", yytext); }
.
%%

int main(int argc, char **argv)
{
    yylex();
}

int yywrap()
{
    return 1;
}

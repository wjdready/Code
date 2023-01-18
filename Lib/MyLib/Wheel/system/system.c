#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include<sys/wait.h> 

int mysystem(const char *command)
{
    const char *ch = command;
    char *argv[100] = {NULL};
    char arg[100];

    int argc = 0;
    int i = 0;

    while(*ch != '\0') {
        // 忽略空格
        while(*ch == ' ')
            ch++;
        if(*ch == '\0')
            break;
        // 获取命令和参数
        while(*ch != '\0' && *ch != ' ')
            arg[i++] = *ch++;
        arg[i] = '\0';
        i = 0;

        // 将命令和参数添加到指针数组
        argv[argc++] = strdup(arg);
    }

    int pid = fork();
    // 在子进程中创建
    if(pid == 0) {
        execvp(argv[0], argv);
    }

    int stat;
    int ret;
    // 等待子进程退出
    wait(&stat);
    // 获取返回值
    if (WIFEXITED(stat)) 
        ret = WEXITSTATUS(stat);
    else if (WIFSIGNALED(stat))
        ret = WTERMSIG(stat);
    else if(WIFSTOPPED(stat))
        ret = WSTOPSIG(stat);
    return ret;
}

int main()
{
    printf("return %d\n", mysystem("   ls    -l    "));
}
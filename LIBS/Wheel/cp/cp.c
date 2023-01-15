#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>

int getFileNameWithDir(char *filename, const char *dir, const char *file)
{

    int i, j;

    // 加入文件夹名
    for(i=0; dir[i] != '\0'; i++)
        filename[i] = dir[i];

    // 去掉 / 字符
    while(filename[--i] == '/')
        ;
    filename[++i] = '/';
    // 加入文件名
    int len = strlen(file);
    char tmpBuf[1024];
    j = 0;
    // 提取 例如从 home/hello.c 中提取 hello.c 
    for(int k=len-1; file[k] != '/' && k>=0; k--)
        tmpBuf[j++] = file[k];

    while(j >=0 )
        filename[++i] = tmpBuf[--j];

    filename[++i] = '\0';
}



int main(int argc, char const *argv[])
{
    if(argc < 2) {
        printf("cp: 缺少了文件操作数\n"
                "尝试 'cp --help' 来获取更多信息.\n");
        return 1;
    }
    else if(argc == 2) {
        printf("cp: 在'%s' 后缺少了要操作的目标文件\n"
                "尝试 'cp --help' 来获取更多信息.\n", argv[1]);
        return 1;
    } 

    // 获取最后一个参数是文件还是文件夹
    int state;
    struct stat st_buf;
    state = stat(argv[argc-1], &st_buf);
    
    if (S_ISREG(st_buf.st_mode) && argc > 3) {
        printf("cp: 目标'%s' 不是目录\n", argv[argc-1]);
        return 1;
    }

    int fd_read;
    int fd_write;
    
    // 文件名缓冲区
    char filename[1024];

    // 读入字符缓冲区
    char buffer[512];
    int readBytes;

    for(int i=1; i<argc-1; i++) {
        // 以只读的方式打开
        fd_read = open(argv[i], O_RDONLY);
        if(fd_read < 0) 
            printf("cp: 无法获取'%s' 的文件状态(stat): 没有那个文件或目录\n", argv[i]);
        else {
            // 如果目标是文件夹
            if(S_ISDIR (st_buf.st_mode)) {
                // 根据文件夹和文件名，拼成完整的要写入的文件名
                getFileNameWithDir(filename, argv[argc-1], argv[i]);
            }else {
                strcpy(filename, argv[argc-1]);
            }
            
            fd_write = creat(filename,0777);
            
            // 创建失败
            if(fd_write < 0) {
                printf("cp: 无法创建普通文件'%s'\n", filename);
            // 创建成功，则依次写入
            } else {
                while((readBytes = read(fd_read, buffer, 512)) > 0) {
                    write(fd_write, buffer, readBytes);
                }
                close(fd_write);
            }
            close(fd_read);
        }
    }
    return 0;
}



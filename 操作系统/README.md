操作系统概念
========

 
## fork() 函数简介

fork系统调用用于创建一个新进程，称为子进程，它与进行fork()调用的进程(父进程)并发运行。创建新的子进程后，两个进程都将执行fork()系统调用之后的下一条指令。子进程使用相同的PC(程序计数器)，相同的CPU寄存器，相同的打开文件，这些文件在父进程中使用。

```C 
fork()
````

它不接受任何参数并返回一个整数值。
下面是fork()返回的不同值。
**负值**：创建子进程失败。
**零**：返回到新创建的子进程。
**正值**：返回给父亲或调用者。该值包含新创建子进程的进程ID。



![图示](https://media.geeksforgeeks.org/wp-content/cdn-uploads/Fork_in_C.jpg)


### 代码

1. 预测以下程序的运行结果：

    ```C
    #include <stdio.h> 
    #include <sys/types.h> 
    #include <unistd.h> 
    int main()
    {
        // 在此指令后生成两个运行相同程序的进程
        fork();

        printf("Hello world!\n"); 
        return 0; 
    }
    ```
    
    **运行结果**：
    ```
    Hello world!
    Hello world!
    ```

2. 计算打印hello的次数。

    ```C
    #include <stdio.h> 
    #include <sys/types.h> 
    int main() 
    { 
        fork(); 
        fork(); 
        fork(); 
        printf("hello\n"); 
        return 0; 
    } 
    ```
    **运行结果**：
    ```
    hello
    hello
    hello
    hello
    hello
    hello
    hello
    hello
    ```
    **解释**
    > hello打印的次数等于创建的进程数。进程总数=$2^n$，其中n是fork系统调用的数量。所以这里$n=3$，$2^3=8$。
    让我们为这三行添加一些标签名称：
    ```C
    fork ();   // Line 1
    fork ();   // Line 2
    fork ();   // Line 3
 
    // 一共 7 个子进程，它们中每一个分别创建于这三行中的哪一个呢？

          L1        // 有 1 个进程创建于Line 1 的fork() P1
        /     \     
      L2      L2    // 有 2 个进程创建于Line 2 的fork() P2~P3
     /  \    /  \   
    L3  L3  L3  L3  // 有 4 个进程创建于Line 3 的fork() P4~P7
                    
    ```
    > 因此总共有八个进程(新子进程和一个原始进程)。
    如果我们想要将进程之间的关系表示为树形层次结构，它将如下所示：
    主要进程：p0。
    第一个分叉创建的进程：P1。
    第二个分叉创建的进程：P2，P3。
    第三个分叉创建的进程：P4，P5，P6，P7
    ```
               P0
            /   |   \
          P1    P4   P2
        /  \          \
       P3    P6         P5
      /
    P7
    ```

3. 预测以下程序的运行结果：

    ```C
    #include <stdio.h> 
    #include <sys/types.h> 
    #include <unistd.h> 
    void forkexample() 
    { 
        // 子进程，因为返回值为零
        if (fork() == 0) 
            printf("Hello from Child!\n"); 

        // 父进程，因为返回值非零
        else
            printf("Hello from Parent!\n"); 
    }

    int main() 
    { 
        forkexample(); 
        return 0; 
    } 
    ```
    **运行结果**
    ```C
    1.
    Hello from Child!
    Hello from Parent!
        (或者)
    2.
    Hello from Parent!
    Hello from Child!
    ```
    **解释**
    > 在上面的代码中，创建子进程，fork()在子进程中返回0，向父进程返回正整数。
    在这里，两个输出是都是有可能的，因为父进程和子进程同时运行。所以我们不知道OS首先把控制权交给哪个进程父进程或子进程。
    **重要提示**：父进程和子进程运行相同的程序，但这并不意味着它们是相同的。OS为这两个进程分配不同的数据和状态，并且控制这些进程的流程也可以不同。请参见下一个示例

    
4. 预测以下程序的运行结果：
    ```C
    #include <stdio.h> 
    #include <sys/types.h> 
    #include <unistd.h> 

    void forkexample() 
    { 
        int x = 1; 

        if (fork() == 0) 
            printf("Child has x = %d\n", ++x); 
        else
            printf("Parent has x = %d\n", --x); 
    } 

    int main() 
    { 
        forkexample(); 
        return 0; 
    } 
    ```

    **运行结果**
    ```C
    Parent has x = 0
    Child has x = 2
        (或者)
    Child has x = 2
    Parent has x = 0
    ```

    **解释**
    > 这里，一个进程中的全局变量变化不会影响另外两个进程，因为两个进程的数据/状态不同。而且父进程和子进程同时运行，所以可以有两个输出。



### `fork()` vs `exec()`

fork系统调用创建一个新进程。fork()创建的新进程是当前进程(返回值除外)的副本。exec()系统调用用新程序替换当前进程。



练习：

1. 一个进程执行以下代码
    ```C
    for (i = 0; i < n; i++) 
	    fork(); 
    ```
    创建的子进程总数为：
    (A) n
    (B) 2^n – 1
    (C) 2^n
    (D) 2^(n+1) – 1;
    [查看此题答案](http:://www.baidu.com)
2. 考虑以下代码片段
    ```C
    if (fork() == 0) { 
        a = a + 5; 
        printf("%d, %d\n", a, &a); 
    } else { 
        a = a –5; 
        printf("%d, %d\n", a, &a); 
    } 
    ```
    设u，v是父进程打印的值，x，y是子进程打印的值。下列哪一项是正确的？
    (A) u = x + 10 and v = y
    (B) u = x + 10 and v != y
    (C) u + 10 = x and v = y
    (D) u + 10 = x and v != y

    [查看此题答案](http:://www.baidu.com)
3. 预测以下程序的输出
    ```C
    #include <stdio.h> 
    #include <unistd.h> 
    int main() 
    { 
        fork(); 
        fork() && fork() || fork(); 
        fork(); 

        printf("forked\n"); 
        return 0; 
    }
    ```
    [查看此题答案](http:://www.baidu.com)
插入排序
========

插入排序是一种简单的排序算法，其工作方式与我们手中扑克牌的排序方式相同。

**算法演示**

    // 排序大小为n的arr[]
    insertionSort(arr，n)
    从i=1到n-1的循环
        拾取元素arr[i]并将其插入排序序列arr[0…i-1]

**例子**
![插入排序](https://media.geeksforgeeks.org/wp-content/uploads/insertionsort.png)

**另一个例子**

`12, 11, 13, 5, 6`

让我们循环i=1(数组的第二个元素)到4(数组的最后一个元素)。

* i=1，由于11小于12，请移动12并在12之前插入11
  `11, 12, 13, 5, 6`
  
* i=2，13将保持在其位置，因为 A[0..i-1] 中的所有元素都小于13
  `11, 12, 13, 5, 6`

* i=3，5将移动到开头，并且从11到13的所有其他元素将移动到其当前位置的前面一个位置。
  `5, 11, 12, 13, 6`

* i=4，6将移动到5之后的位置，从11到13的元素将移动到其当前位置的前面一个位置。
  `5, 6, 11, 12, 13`

代码实现
```C
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


int *getRandArray(int max, int min, int n)
{
    srand(time(NULL));
    int *array = (int *)malloc(sizeof(int) * n);
    for(int i=0; i<n; i++) 
        array[i] = rand() % max + min;
    return array;
}

void printArray(int *array, int n)
{
    for(int i=0; i<n; i++)
        printf("%d ", array[i]);
    printf("\n\n");
}


void insertionSort(int *arr, int n)
{
    int i, key, j;
    // 从第一个元素开始
    for (i = 1; i < n; i++) {
        key = arr[i];
        j = i - 1;

        // 将arr[0..i-1]中大于key的元素移动到其当前位置之后的一个位置>>
        while(j>=0 && arr[j]>key) {
            arr[j+1] = arr[j];
            j--;
        }
        // 这时候前面的位置就留空，将key插入
        arr[j+1] = key;
    }
}

int main()
{
    int len = 30;
    int *arr = getRandArray(len, 1, len);
    printArray(arr, len);

    // 排序
    insertionSort(arr, len);
    printArray(arr, len);
    
    free(arr);
    return 0;
}
```

**时间复杂度**：O(n*2)。

**辅助空间**：O(1)。

**边界情况**：如果元素按相反的顺序排序，插入排序需要最长的时间进行排序。并且当元素已经被排序时，它需要最小的时间(n的阶数)。

**算法范式**：增量方法。

**就地排序**：是。

**稳定**：是。

**在线**：是。

**用途**：当元素数量较小时，使用插入排序。当输入数组几乎被排序时，它也是有用的即：只有几个元素被错误地放在完整的大有序数组中。

**什么是折半插入排序？**
我们可以使用折半搜索法来减少常规插入排序中的比较次数。折半插入排序使用二分法搜索来查找在每次迭代中插入选定项的适当位置。在正常插入中，排序在最坏情况下采用O(I)(第i次迭代)。我们可以通过使用二分搜索将其减少到O(Logi)。作为一个整体，该算法仍然具有O(N2)的运行最坏情况运行时间，这是因为每次插入都需要一系列的交换。有关实施，[请参阅此内容](https://youtu.be/OGzPmgsI-pQ)

**如何实现链表的插入排序？**
下面是链表的简单插入排序算法。
* 创建空的排序(或结果)列表。
* 遍历给定的列表，对每个节点执行以下操作。
  在排序列表或结果列表中以排序方式插入当前节点。
* 将给定链表的表头改为排序(或结果)表头。
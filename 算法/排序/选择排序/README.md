选择排序
========

选择排序算法通过重复查找未排序部分的最小元素(考虑升序)并将其放在开头来对数组进行排序。该算法在给定的数组中维护两个子数组：
* 已经排序的子数组。
* 未排序的剩余子阵列。

在选择排序的每次迭代中，从未排序的子数组中拾取最小元素(考虑升序)并将其移动到排序的子数组。

以下示例说明了上述步骤：

    ARR[]=64 25 12 22 11。

    //查找arr[0...4]中的最小元素。
    //并将其放在开头。
    11 25 12 22 64。

    //查找arr[1...4]中的最小元素。
    //并将其放在arr[1...4]的开头。
    11 12 25 22 64。

    //查找arr[2...4]中的最小元素。
    //并将其放在arr[2...4]的开头。
    11 12 22 25 64。

    //查找arr[3...4]中的最小元素。
    //并将其放在arr[3...4]的开头。
    11 12 22 25 64

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

void swap(int *xp, int *yp)  
{  
    int temp = *xp;  
    *xp = *yp;  
    *yp = temp;  
}  


void selectionSort(int arr[], int n)
{
    int i, j, min_idx;
    // 逐个移动未排序数组的边界
    for(int i=0; i<n-1; i++) {
        min_idx = i;
        // 在未排序数组中的查找最小元素
        for(int j=i+1; j<n; j++) {
            if(arr[j] < arr[min_idx])
                min_idx = j;
        }
        // 将找到的最小元素与第一个元素交换
        swap(&arr[min_idx], &arr[i]);
    }
}

int main()
{
    int len = 30;
    int *arr = getRandArray(100, 1, len);
    printArray(arr, len);

    // 排序
    selectionSort(arr, len);
    printArray(arr, len);

    free(arr);
    return 0;
}
```

**时间复杂度**：O(N2)，因为有两个嵌套循环。

**辅助空间**：O(1)。
选择排序的好处是它从来不会进行超过O(N)的交换，并且在内存写入是一个代价高昂的操作时非常有用。

**练习：**
* 使用选择排序对字符串数组进行排序。

**稳定性**：默认实现不稳定。但是，可以使其稳定。

**就地完成**：是的，它不需要额外的空间。
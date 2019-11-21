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


void insert(int *arr, int n)
{
    int i, j, key;
    for(i=0; i<n-1; i++) {

        // 取出第 i+1 个
        key = arr[i+1];
        
        // key 和 arr[0...i] 逐个比较，然后右移
        for(j=i; j>=0 && key<arr[j]; j--) 
            arr[j+1] = arr[j];

        arr[j+1] = key;
    }
}


int main()
{
    int len = 30;
    int *arr = getRandArray(len, 1, len);
    printArray(arr, len);

    // 排序
    insert(arr, len);
    printArray(arr, len);
    
    free(arr);
    return 0;
}
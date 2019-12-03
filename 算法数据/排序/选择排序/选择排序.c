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
    int len = 50000;
    int *arr = getRandArray(len, 1, len);
    // printArray(arr, len);

    // 排序
    selectionSort(arr, len);
    // printArray(arr, len);

    free(arr);
    return 0;
    
}
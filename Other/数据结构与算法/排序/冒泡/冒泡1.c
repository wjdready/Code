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

void bubbleSort(int arr[], int n)
{
    int i, j;
    // 进行n次遍历
    for(i=0; i<n-1; i++) {
        // 每次遍历最后一个元素已经归位
        for(j=0; j<n-1-i; j++) {
            if(arr[j] > arr[j+1]) 
                swap(&arr[j], &arr[j+1]);
        }
    // n 次遍历后，n个元素都归位，排序完毕
    }
}


int main()
{
    int len = 50000;
    int *arr = getRandArray(len, 1, len);
    // printArray(arr, len);

    // 排序
    bubbleSort(arr, len);
    // printArray(arr, len);

    free(arr);
    return 0;
}
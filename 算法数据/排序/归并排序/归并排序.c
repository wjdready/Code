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


void merge(int arr[], int l, int m, int r)
{
    // 计算左右两边需要的辅助空间大小
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    // 创建辅助空间
    int *L = (int*)malloc(sizeof(int)*n1);
    int *R = (int*)malloc(sizeof(int)*n2);

    //将左右两边的值拷贝到各自的辅助空间
    for(i=0; i<n1; i++)
        L[i] = arr[l+i];
    for(j=0; j<n2; j++)
        R[j] = arr[m+1+j];

    // 将左右辅助空间的值按大小比较放到原空间
    for(i=0,j=0,k=l; i<n1 && j<n2; k++)
        arr[k] = (L[i] <= R[j]) ? L[i++] : R[j++];
    
    // 拷贝剩余内容
    while(i<n1) 
        arr[k++] = L[i++];
    while(j<n2)
        arr[k++] = R[j++];

    // 释放辅助空间
    free(L);
    free(R);
}


void mergeSort(int arr[], int l, int r) 
{ 
    if (l < r) 
    { 

        int m = l+(r-l)/2; 

        mergeSort(arr, l, m); 
        mergeSort(arr, m+1, r); 
  
        merge(arr, l, m, r); 
    } 
} 

int main()
{
    int len = 5000000;
    int *arr = getRandArray(len, 1, len);
    // printArray(arr, len);

    // 排序
    mergeSort(arr, 0, len-1);
    // printArray(arr, len);

    free(arr);
    return 0;
}
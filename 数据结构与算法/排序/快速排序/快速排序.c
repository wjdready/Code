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

void swap(int *arr, int i, int j)
{
    int t = arr[i];
    arr[i] = arr[j];
    arr[j] = t;
}


void q_sort(int *arr, int left, int right)
{
    if(left >= right)
        return;
    int last = left;
    for(int i=left+1; i<=right; i++)
        if(arr[i] < arr[left]) 
            swap(arr, i, ++last);
    swap(arr, last, left);
    q_sort(arr, left, last-1);
    q_sort(arr, last+1, right);
}

int compare(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}

int main()
{
    int len = 50;
    int *arr = getRandArray(len, 1, len);
    // printArray(arr, len);

    // 排序
    // q_sort(arr, 0, len-1);
    qsort(arr, len, sizeof(int), compare);
    printArray(arr, len);

    free(arr);
    return 0;
}
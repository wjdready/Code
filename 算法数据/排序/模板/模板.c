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


void sort(int *arr, int n)
{

}

int main()
{
    int len = 10;
    int *arr = getRandArray(len, 1, len);
    printArray(arr, len);

    // 排序
    sort(arr, len);
    printArray(arr, len);

    free(arr);
    return 0;
}
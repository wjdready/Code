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


void bsort(int arr[], int n)
{
    int sorted = 0, tmp;
    for( ; !sorted; n--) {
        sorted = 1;
        for(int i=1; i<n; i++) {
            if(arr[i] < arr[i-1])  {
                tmp = arr[i];
                arr[i] = arr[i-1];
                arr[i-1] = tmp;
                sorted = 0;
            }
        }
    }
}

int main()
{
    int len = 50000;
    int *arr = getRandArray(len, 1, len);
    // printArray(arr, len);

    // 排序
    bsort(arr, len);
    // printArray(arr, len);

    free(arr);
    return 0;
}
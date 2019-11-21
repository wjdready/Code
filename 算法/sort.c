#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>

int *getRandArray(int max, int min, int n)
{
    int *array = (int*)malloc(sizeof(int)*n);
    srand(time(NULL));
    for(int i=0; i<n; i++) 
        array[i] = rand() % max + min;
    return array;
}

void printArray(int *arr, int n)
{
    for(int i=0; i<n; i++) 
        printf("%d ", arr[i]);
    printf("\n");
}

void swap(int *x, int *y)
{   
    int t = *x;
    *x = *y;
    *y = t;
}

void bubbleSort(int *arr, int n)
{
    int sorted = 0;
    for( ; !sorted; n--) {
        sorted = 1;
        for(int i=1; i<n; i++) {
            if(arr[i] < arr[i-1]) {
                sorted = 0;
                swap(&arr[i], &arr[i-1]);
            }
        }
    }
}

void sectionSort(int *arr, int n)
{
    int min;
    for(int i=0; i<n; i++) {
        min = i;
        for(int j=i+1; j<n; j++) {
            if(arr[j] < arr[min])
                min = j;
        }
        swap(&arr[i], &arr[min]);
    }
}

void insertSort(int *arr, int n)
{
    int temp;
    for(int i=1; i<n; i++) {
        // 第 i 个掏空
        temp = arr[i];
        // 将数组左边右移
        int j;
        for(j=i; j>=1 && arr[j-1] > temp; j--)
            arr[j] = arr[j-1];
        arr[j] = temp;
    }
}

void shellSort(int *arr, int n)
{
    int temp;
    for(int gap=n/2; gap>0; gap /= 2) {
        
        for(int i=gap; i<n; i++) {
            temp = arr[i];

            int j;
            for(j=i; j>=gap && arr[j-gap] > temp; j-=gap)
                arr[j] = arr[j-gap];
            
            arr[j] = temp;
        }
    }
}

void quickSort(int *arr, int left, int right)
{
    if(left >= right)
        return;
    int last = left;
    for(int i=left+1; i<right; i++) {
        if(arr[i] < arr[left])
            swap(&arr[i], &arr[++last]);
    }
    swap(&arr[left], &arr[last]);
    quickSort(arr, left, last-1);
    quickSort(arr, last+1, right);
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

// 归并排序
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

// 桶排序
void bucketSort(int arr[], int n) 
{ 
    // 找到最大值
    int max = 0;
    for(int i=0; i<n; i++)
        max = arr[i] > max ? arr[i] : max;
    
    // 根据最大值获取辅助空间
    int *b = (int *)malloc(sizeof(int)*(max+1));
    memset(b, 0, (max + 1)*sizeof(int));

    // 对n个数进行统计
    for(int i=0; i<n; i++) {
        b[arr[i]]++;
    }

    // 将桶中的元素写道原数组
    for (int i = 0, j = 0; i <= max; ++i){
        while (b[i]--)
            arr[j++] = i;
    }

    free(b);
} 

int main()
{
    int len = 500000;
    int *arr = getRandArray(len, 1, len);
    // printArray(arr, len);
   
    // bubbleSort(arr, len);
    // quickSort(arr, 0, len);
    // sectionSort(arr, len);
    // insertSort(arr, len);
    // shellSort(arr,len);
    // mergeSort(arr, 0, len-1);
    bucketSort(arr, len);
    // printArray(arr, len);


    free(arr);
    return 0;
}
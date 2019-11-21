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



void shellSort(int arr[], int n) 
{ 
    // 从一个大的差距开始，然后缩小差距
    for (int gap = n / 2; gap > 0; gap /= 2) { 
        // 对此间隙大小进行间隙插入排序 
        // 第一个间隙元素arr[0...gap-1]已按间隙顺序 
        // 继续添加一个更多的元素，直到对整个数组进行间隙排序
        for (int i = gap; i < n; i++) { 
            // 将arr[i]添加到已进行间隙排序的元素
            // 将arr[i]保存在temp中，让 i 位置留空
            int temp = arr[i]; 
  
            // 将较早的间隙排序元素向上移动，直到找到arr[i]的正确位置   
            int j; 
            for (j = i; j >= gap && arr[j - gap] > temp; j -= gap) 
                arr[j] = arr[j - gap]; 
  
            // 将temp(原始arr[i])插入到其正确的位置
            arr[j] = temp; 
        } 
    } 
} 



void myshell(int *arr, int n)
{
    // 1、分组
    for(int gap=n/2; gap>0; gap /= 2) {

        // 轮流对每组排序
        for(int i=gap; i<n; i++) {
            // i 位置掏空
            int temp = arr[i];

            // 左边数组元素右移，腾出合适的位置
            int j;
            for(j=i; j>=gap && arr[j-gap] > temp; j -= gap)
                arr[j] = arr[j-gap];
            
            arr[j] = temp;
        }
    }
}


int main()
{
    int len = 20;
    int *arr = getRandArray(len, 1, len);
    printArray(arr, len);

    // 排序
    myshell(arr, len);
    printArray(arr, len);

    free(arr);
    return 0;
}
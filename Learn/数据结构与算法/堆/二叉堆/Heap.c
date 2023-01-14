#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef struct minHeap {
    int *harr;       // 指向数组的指针
    int capacity;   // 最小堆的最大容量
    int heapSize;  // 最小堆当前元素个数
}minHeap;


void swap(int *x, int *y)
{
    int t = *x;
    *x = *y;
    *y = t;
}

minHeap *newMinHeap(int cap)
{
    minHeap *heap = (minHeap*)malloc(sizeof(minHeap));
    
    heap->capacity = cap;
    heap->heapSize = 0;
    heap->harr = (int *)malloc(sizeof(int) * cap);

    return heap;
}

// 根据 i 值获取其他节点索引
int gteParent(int i) { return (i-1)/2; }
int getLeft(int i) { return (2*i+1); } 
int getRight(int i) { return (2*i+2); } 


// 插入一个新键值
void minHeapInsert(minHeap *heap, int key)
{
    if(heap->heapSize == heap->capacity) {
        printf("\nOverflow: Could not insert key\n");
        return;
    }

    // 首先将键值插入到最后面
    heap->harr[heap->heapSize++] = key;

    // 如果违反最小堆属性，则修复该属性
    int i = heap->heapSize - 1;
    while(i != 0 && heap->harr[gteParent(i)] > heap->harr[i]) {
        swap(&heap->harr[i], &heap->harr[gteParent(i)]); 
        i = gteParent(i); 
    }
}

// 检测如果不满足堆属性则递归修复
void minHeapify(minHeap *heap, int i) 
{ 
    int l = getLeft(i); 
    int r = getRight(i); 
    int smallest = i; 
    if (l < heap->heapSize && heap->harr[l] < heap->harr[i]) 
        smallest = l; 
    if (r < heap->heapSize && heap->harr[r] < heap->harr[smallest]) 
        smallest = r; 
    if (smallest != i) 
    { 
        swap(&heap->harr[i], &heap->harr[smallest]); 
        minHeapify(heap, smallest); 
    } 
} 

// 减小key值
void minHeapDecreaseKey(minHeap *heap, int i, int new_val) 
{ 
    heap->harr[i] = new_val; 
    while (i != 0 && heap->harr[gteParent(i)] > heap->harr[i]) { 
       swap(&heap->harr[i], &heap->harr[gteParent(i)]); 
       i = gteParent(i); 
    } 
} 


// 删除堆中的最小元素
int minHeapExtractMin(minHeap *heap)
{
    // 如果堆中元素为空或只有一个
    if(heap->heapSize < 0)
        return (unsigned int)(-1);
    if(heap->heapSize == 1) {
        heap->heapSize--;
        return heap->harr[0];
    }

    // 存放最小值
    int root = heap->harr[0];
    heap->harr[0] = heap->harr[heap->heapSize - 1];
    heap->heapSize--;
    // 删除后对堆进行修复
    minHeapify(heap, 0);

    return root;
}

// 删除堆中的第 i 个元素
void minHeapDelete(minHeap *heap, int i)
{
    // 先在 i 处替换为最小值
    minHeapDecreaseKey(heap, i, -(unsigned int)-1);
    // 再把最小值从堆中剔除
    minHeapExtractMin(heap);
}

// ===========打印部分=======================

typedef struct Trunk
{
    struct Trunk *prev;
    char *str;
}Trunk;


Trunk *newTrunk(Trunk *prev, char *str)
{
    Trunk *trunk = malloc(sizeof(Trunk));
    trunk->prev = prev;
    trunk->str = str;
}

void showTrunks(Trunk *p)
{
    if (p == NULL)
        return;
    showTrunks(p->prev);
    printf("%s", p->str);
}

// 递归的方式打印堆结构
void printHeap(minHeap *heap, int index, Trunk *prev, bool isLeft)
{
    if (index > heap->heapSize - 1)
        return;
    
    char *prev_str = "    ";
    Trunk *trunk = newTrunk(prev, prev_str);

    // 先打印右边
    printHeap(heap, getRight(index), trunk, true);

    // 判断树干类型
    if (!prev)
        trunk->str = "---";
    else if (isLeft)
    {
        trunk->str = ".---";
        prev_str = "   |";
    }
    else
    {
        trunk->str = "`---";
        prev->str = prev_str;
    }

    showTrunks(trunk);

    // 先当前值
    printf("%d\n",heap->harr[index]);
    if (prev)
        prev->str = prev_str;
    trunk->str = "   |";

    printHeap(heap, getLeft(index), trunk, false);
}

void printMinHeap(minHeap *heap)
{
    for(int i=0; i<heap->heapSize; i++) {
        printf("%d ",heap->harr[i]);
    }
    printf("\n");
}


minHeap *buildBST(minHeap *heap, int arr[], int left, int right, int index)
{
    if(right < left)
        return NULL;

    int mid = (left + right) / 2;
    // 插入中间值
    heap->harr[index] = arr[mid];
    heap->heapSize++;
    // 插入右边值
    buildBST(heap, arr, mid+1, right, getRight(index));
    // 先插入左边值
    buildBST(heap, arr, left, mid-1, getLeft(index));

    return heap;
}


int main()
{
    void heapTest();

    heapTest();
}

void heapTest()
{
    minHeap *heap = newMinHeap(20);

    minHeapInsert(heap, 12);
    minHeapInsert(heap, 20);
    printHeap(heap, 0, NULL, false);

    printf("\n插入10后\n");
    minHeapInsert(heap, 10);
    printHeap(heap, 0, NULL, false);

    printf("\n插入5后\n");
    minHeapInsert(heap, 5);
    printHeap(heap, 0, NULL, false);

    printf("\n插入90后\n");
    minHeapInsert(heap, 90);
    printHeap(heap, 0, NULL, false);

    printf("\n插入2后\n");
    minHeapInsert(heap, 2);
    printHeap(heap, 0, NULL, false);

    printf("\n删除最小值后\n");
    minHeapExtractMin(heap);
    printHeap(heap, 0, NULL, false);

    printf("\n删除第 3 个\n");
    minHeapDelete(heap, 2);
    printHeap(heap, 0, NULL, false);
}

int compare(const void *a, const void *b)
{
    return *(int*)a - *(int*)b;
}

void buildTest()
{
    int max = 100, min = 1;
    int num = 30;
    int v[30];

    minHeap *heap = newMinHeap(num);

    srand(time(NULL));
    for(int i=0; i < num; i++) {
        v[i] = rand() % max + min;
        printf("%d ",v[i]);
    }
    printf("\n");
    qsort(v, 30, sizeof(int), compare);
    for(int i=0; i < num; i++) {
        // heap->harr[heap->heapSize++] = v[i];
        printf("%d ",v[i]);
    }
    printf("\n");
    buildBST(heap, v, 0, num-1, 0);
    printHeap(heap, 0, NULL, false);
    printMinHeap(heap);
}

void randomTest()
{
    int max = 100, min = 1;
    int num = 30;

    minHeap *heap = newMinHeap(num);
    srand(time(NULL));
    for(int i=0; i < num; i++) {
        minHeapInsert(heap, rand() % max + min);
    }

    printHeap(heap, 0, NULL, false);
    printMinHeap(heap);

}
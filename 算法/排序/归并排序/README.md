归并排序
========

与快速排序一样，归并排序也是一种分而治之的算法。它将输入数组分成两半，为这两半调用自身，然后合并这两个排序的一半。函数 merge() 的作用是：合并两半。merge(arr, l, m, r)是一个关键过程，它假设arr[l..m]和arr[m+1..r]被排序，并将两个排序的子数组合并为一个。有关详细信息，请参见下面的C实现。

    MergeSort(arr[], l,  r)
    If r > l
        1. 找到中点将数组分成两半:  
                middle m = (l+r)/2
        2. 上半部分调用mergeSort:   
                Call mergeSort(arr, l, m)
        3. 下半部分调用mergeSort:
                Call mergeSort(arr, m+1, r)
        4. 合并步骤2和步骤3中排序好的两半:
                Call merge(arr, l, m, r)

维基百科的下图显示了示例数组{38，27，43，3，9，82，10}的完整合并排序过程。如果我们仔细看一下图表，我们可以看到数组被递归地分成两半，直到大小变为1。一旦大小变为1，合并过程就会起作用，并开始合并数组，直到合并完整的数组。

![归并排序](https://www.geeksforgeeks.org/wp-content/uploads/Merge-Sort-Tutorial.png)

**代码实现**

```C

```
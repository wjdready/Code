希尔排序
========

希尔排序是插入排序的变体。在插入排序中，我们只将元素向前移动一个位置。当一个元素必须向前移动很远时，就会涉及到许多动作。shellSort的想法是允许远距离项目的交换。在shellSort中，我们将数组h排序为一个较大的h值。我们不断减少h的值，直到它变为1。如果每h个元素的所有子列表都被排序，则数组称为h排序。


increment的取值较大，每个子序列中的元素较少，排序速度较快，到排序后期increment取值逐渐变小，子序列中元素个数逐渐增多，但由于前面工作的基础，大多数元素已经基本有序，所以排序速度仍然很快。
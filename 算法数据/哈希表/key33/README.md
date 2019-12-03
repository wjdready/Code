## key33方法

```C
// 根据键值获取对于数组的index
unsigned long hashIndex(hashTable *hash, const char *key)
{
    unsigned long index = 0;
    while(*key)
        index = index*33 + *key++;
    return index%hash->size;
}
```
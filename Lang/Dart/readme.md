
## String

1. split()

分割字符串

2. codeUnits 

获取字符串的 Unicode 码点（code unit）列表。

## Iterable

1. join()

将 Iterable 中的元素连接成一个字符串。

2. map()

遍历 Iterable 中的每个元素，并对每个元素应用指定的函数，然后返回一个新的 Iterable。

print(str.codeUnits.map((e) => "0x" + e.toRadixString(16)).join(' '));

## List 

implements Iterable

1. length

## Set

implements Iterable

## Uint8List


## ByteData


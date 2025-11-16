

## 安装

添加虚拟网卡: https://www.bilibili.com/opus/656616051915620369

修改单位为公制: https://blog.csdn.net/2401_83781013/article/details/136953296

##### 改单位

打开 `C:\Program Files\PTC\Creo 7.0.0.0\Common Files\text\config.pro`

inch 改为 mm
pound 改为 kilogram
所有 inlbs 改为 mmns

##### 改视角及工程图单位

打开 `C:\Program Files\PTC\Creo 7.0.0.0\Common Files\text\prodetail.dtl`

找到 projection_type, THIRD_ANGLE 改成 FIRST_ANGLE

找到 drawing_units, INCH 改为 MM

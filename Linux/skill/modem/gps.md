

## SIM7600X 模块定位功能
SIM7600X 常用命令: https://blog.csdn.net/qq_43231904/article/details/111954013

https://www.waveshare.net/wiki/SIM7600CE_4G_DONGLE#GNSS.E5.AE.9A.E4.BD.8D.E5.8A.9F.E8.83.BD
https://blog.csdn.net/qwdpoiguw/article/details/54292920

```sh
AT+CGPS=1	    # 打开GPS
AT+CGPSINFO     # 将GPS信息打印到串口
AT+CGPS=0       # 关闭GPS

# GPS信号较弱时返回
+CGPSINFO:,,,,,,,,

# 正常返回
+CGPSINFO: 3113.343286,N,12121.234064,E,131025,041754.0,11.8,0.0,

# +CGPSINFO: 3113.343286,  N,    12121.234064,      E,     131025,   041808.0,      12.2,         0.0,
# +CGPSINFO: [<lat>],   [<N/S>],   [<log>],     [<E/W>],  [<date>],  [<UTCtime>],   [<alt>],    [<speed>],  [<course>]
# [<lat>]：当前位置纬度,输出格式是ddmm.mmmm, 即 dd°mm.mmmm′
# [<N/S>]：N/S的指标，N=北或S=南
# [<log>]：当前位置经度,输出格式是dddmm.mmmm, 即ddd°mm.mmmm′
# [<E/W>]：E/W指标，E=东或W=西方
# [<date>]：日期,输出格式是日月年
# [<UTC time>]：UCT时间,输出格式是hhmmss.s
# [<alt>]：海平面高度,单位是米
# [<speed>]：地面速度,单位是节=(1852/3600)m/s≈0.5m/s
# [<course>]：方向,单位是度
```

### 定位误差修正

https://blog.csdn.net/drdr7/article/details/145869657

先将度分秒转换为小数点经纬度, 再将 GPS 使用的  WGS-84 坐标系转为, 中国地图服务（如高德、腾讯、百度） 使用 GCJ-02 或 BD-09 坐标系


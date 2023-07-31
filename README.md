# EC1515S 电子钟程序

## 1.复刻原版厂商自带程序

### 1.1.编写器件驱动

- [x]数码管、LED
- [x]按钮
- [x]光敏电阻、温敏电阻
  - 转换为光照强度和温度的算法
- DS1302 时钟芯片
- 蜂鸣器

### 1.2.复刻原版程序行为

- 显示时间、日期、年、温度
- 调整时间
  - 年月日
  - 时分
- 闹钟
  - 调整闹钟开关
  - 调整闹钟时间
- 低光线下显示变暗

## 2.自己设计新功能

- 休眠，省电模式
- 显示秒
- 亮度显示、精确温度显示
- 显示传感器源数据、调整修正值
- eeprom 保存设置
- iap下载歌曲，调整时间
- 整点报时

## 原版参数

```
Target model:
  Name: IAP15W413AS
  Magic: F562
  Code flash: 13.0 KB
  EEPROM flash: 0.0 KB
Target frequency: 11.045 MHz
Target BSL version: 7.2.5T
Target wakeup frequency: 37.175 KHz
Target options:
  reset_pin_enabled=False
  clock_source=internal
  clock_gain=high
  watchdog_por_enabled=False
  watchdog_stop_idle=True
  watchdog_prescale=256
  low_voltage_reset=True
  low_voltage_threshold=3
  eeprom_lvd_inhibit=True
  eeprom_erase_enabled=False
  bsl_pindetect_enabled=False
  por_reset_delay=long
  rstout_por_state=high
  uart2_passthrough=False
  uart2_pin_mode=normal
  cpu_core_voltage=unknown
Disconnected!
```

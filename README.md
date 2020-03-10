[English](./README.EN.md) | 简体中文

<!-- <p align="center"><img width="100" src="https://vuejs.org/images/logo.png"></p> -->
<h2 align="center">蓝牙的射频指纹嗅探器</h2>
<p align="center"><b>基于软件无线电(目前针对于bladeRF)的蓝牙射频指纹嗅探器</b></p>
(!项目正在移植到Qt平台，进度缓慢，代码已经Push到QtDev分支)

# 目录

- [必须收藏的资源](#必须收藏的资源)
- [快速入门](#快速入门)
  - [安装](#安装)
  - [用法](#用法)
- [贡献者们](#贡献者们)
- [维护者](#维护者)
- [感谢](#感谢)
- [License](#license)


### 特性

- 不仅可以嗅探周围的Ble广播包，还可以将其RawData存储下来！
- 支持 `bladeRF`，之后将会添加`USRP`、`harckRF`的支持！

## 必须收藏的资源

- [等待更新...](https://www.baidu.com)

## 快速入门

### 环境配置
1. 需要libBladeRF动态库，配置请参照 https://github.com/nuand/bladeRF/wiki#Getting_Started
2. 目前开发平台为Ubuntu18.04LTS，其他平台请自行解决环境问题，如果遇到问题欢迎提Issuse讨论。

### 安装
1. ``` git clone git@github.com:ZhangLei-cn/ble-sniffer.git ```将当前项目Clone到本地。
2. ``` cd ./ble-sniffer ``` ``` mkdir build``` ``` cd ./build ```进入到构建目录。
3. ``` cmake ../```构建可执行程序。
4. 如果顺利的话你现在可以找到构建好的可执行程序了。
![avatar](https://github.com/ZhangLei-cn/ble-sniffer/blob/master/doc/imag1.jpg)
### 目录说明
```
├─ 待更新
```

### 用法
关于Sdr的接收参数已经默认设置为：
```c++
config.channel = BLADERF_CHANNEL_RX(0);
config.frequency = 2402e6;
config.bandwidth = 2e6;
config.samplerate = 10e6;
config.gain = 30;
```

## 贡献者们

## 维护者


## 感谢
[JiaoXianjun](https://github.com/JiaoXianjun/BTLE)
[KunQian](https://www.kunqian.info/)
[backahasten](https://www.cnblogs.com/backahasten/)
## License

- [GPL](https://opensource.org/licenses/gpl-license)



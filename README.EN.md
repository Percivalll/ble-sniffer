English| [简体中文](./README.md) 

<!-- <p align="center"><img width="100" src="https://vuejs.org/images/logo.png"></p> -->
<h2 align="center">BleFpSniffer</h2>
<p align="center"><b>A Bltooth Low Energy Radio Frequency Fingerprint Sniffer based onSoftware Defined Radio (currently for bladeRF)</b></p>
(!The project is emigrating to Qt Platform,so it's upgrading slowly.Codes have been uploaded,please switch to QtDev branch.)

# Contents

- [MustKnow](#MustKnow)
- [QuickStart](#QuickStart)
  - [Environment](#Environment)
  - [Install](#Install)
  - [Usage](#Usage)
- [Contributora](#Contributors)
- [Maintainers](#Maintainers)
- [Acknowledgements](#Acknowledgements)
- [License](#license)


### Features

- It not only can be used to sniff surrounding Ble broadcast packets,but also to store their Raw Data!
- Support `bladeRF`，and then will support`USRP`、`harckRF`!

## Related Resources

- [BTLE sniffer](https://github.com/JiaoXianjun/BTLE)

## QuickStart

### Environment
1. Need libBladeRF API，if you haven't install it,please see https://github.com/nuand/bladeRF/wiki#Getting_Started
2. Now I developed this project on Ubuntu18.04LTS，if you use other platform ,please reslove the problems whitch are related to platform or create a new issuse.

### Install
1. ``` git clone git@github.com:ZhangLei-cn/ble-sniffer.git ```Clone the project to local.
2. ``` cd ./ble-sniffer ``` ``` mkdir build``` ``` cd ./build ```
3. ``` cmake ../```build the project.
4. If the procedure is successful, you can now find the executable program you've built.
![avatar](https://github.com/ZhangLei-cn/ble-sniffer/blob/master/doc/imag1.jpg)

Introduction of directories:

```
├─ Wait to update...
```

### Usage
Th default parameters of sdr have been set as:
```c++
config.channel = BLADERF_CHANNEL_RX(0);
config.frequency = 2402e6;
config.bandwidth = 2e6;
config.samplerate = 10e6;
config.gain = 30;
```
## Contributors

## Maintainers


## Acknowledgements
[JiaoXianjun](https://github.com/JiaoXianjun/BTLE)
[KunQian](https://www.kunqian.info/)
[backahasten](https://www.cnblogs.com/backahasten/)
## License

- [GPL](https://opensource.org/licenses/gpl-license)

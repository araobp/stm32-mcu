# Camera

<img src="./camera_board.jpg" width=400>

(Work in progress)


## Goal

- Capture QCIF images (176 x 144 pixels).
- Cut out 128 x 128 pixel or 64 x 64 pixel images from QCIF.

## Parts

- [HiLetgo OV7670 camera module](https://www.amazon.co.jp/HiLetgo%C2%AE-ov7670%E3%82%AB%E3%83%A1%E3%83%A9%E3%83%A2%E3%82%B8%E3%83%A5%E3%83%BC%E3%83%AB-%E3%82%B7%E3%83%B3%E3%82%B0%E3%83%AB%E3%83%81%E3%83%83%E3%83%97-%E9%9B%86%E9%8C%B2%E3%83%A2%E3%82%B8%E3%83%A5%E3%83%BC%E3%83%AB%E3%82%AB%E3%83%A1%E3%83%A9/dp/B071JHKQCZ/ref=sr_1_fkmr1_2?s=diy&ie=UTF8&qid=1549848789&sr=8-2-fkmr1&keywords=%E3%82%AB%E3%83%A1%E3%83%A9%E3%80%80%E3%82%AA%E3%83%A0%E3%83%8B%E3%83%93%E3%82%B8%E3%83%A7%E3%83%B3+HiLetgo)

## Test circuit

### Wirling among pins

|STM32F407VGTx|OV7670 module|Pull-up|
|-------------|-------------|-------|
|GND          | GND |    |
|3V           | 3V3 |    |
|GPIO PE1     |RESET|STM32-internal pull-up  |
|MCO PA8      |XCLK |    |
|SCL PB8      |SIOC(SCCB)|External 10k ohm pull-up|
|SDA PB9      |SIOO(SCCB)|External 10k ohm pull-up|

### Initialization test

As per the OV7670 datasheet, SCCB slave addresses are:
- 0x42 for write
- 0x43 for read

My HiLetgo OV7670 module returned the following ID over SCCB:
- Product ID: 0x76
- Version: 0x73

## TODO

- Make an OV7670 expansion board
- OV7670 config
- Test OV7670 parallel ports (DCMI) with DMA

## References

- [IT Text 画像工学](https://www.ohmsha.co.jp/book/9784274220074/)
- [Serial Camera Control Bus
Functional Specification ](http://www4.cs.umanitoba.ca/~jacky/Teaching/Courses/74.795-LocalVision/ReadingList/ov-sccb.pdf)
- [OV7670 datasheet](https://www.voti.nl/docs/OV7670.pdf)
- [Digital camera interface (DCMI) for STM32 MCUs](https://www.st.com/content/ccc/resource/technical/document/application_note/group0/c0/ef/15/38/d1/d6/49/88/DM00373474/files/DM00373474.pdf/jcr:content/translations/en.DM00373474.pdf)
- [STM32マイコンによるデジタルカメラの作成](https://qiita.com/take-iwiw/items/212ddb6faa05412c83b7)
- [STM32マイコンでOV7670などとSCCB通信する際の注意点](https://qiita.com/take-iwiw/items/cf10034890d2784676d0)

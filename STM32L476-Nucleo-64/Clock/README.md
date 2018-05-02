# Clock

## Parts

- [I2C character LCD](http://akizukidenshi.com/catalog/g/gK-08896/)

## Schematic

Note: PB8 and PB9 pins are 5V-tolerant as per [the datasheet](http://www.st.com/resource/en/datasheet/stm32l476je.pdf).

![schematic](./Clock.jpg)

## Clock setting

Use terminal emulator such as Teraterm to open the COM port.

If the current date and time is "May 2, 2018" on Tuesday, then the command is like this:

```
18 5 2 16 25 2
```

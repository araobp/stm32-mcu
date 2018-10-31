# PCM data resolution on STM32L4

Reference: [AN4990
Application note: 
Getting started with sigma-delta digital interface
on applicable STM32 microcontrollers](https://www.st.com/content/ccc/resource/technical/document/application_note/group0/b2/44/42/9d/46/b4/4d/34/DM00354333/files/DM00354333.pdf/jcr:content/translations/en.DM00354333.pdf)

## DFSDM output

The applicatio note explains that DFSDM handles data as 32bit internally, but PCM data output from DFSDM is actually 24-bit signed integer.

I examined the output from DFSDM on the debugger of TrueSTUDIO. The data was as follows: {24576, 27648, 17920, 35328, 16384...}.

All the data above is 2^9(9bit) * n:
- 24576 = 2^9 * 48
- 27648 = 2^9 * 54
-    :

So 9-bit right shift is required to get signed integer data, although I have not been able to find any documentation explaining why it is 9-bit.

## Resolution

The application note explains that the output data is 16bit resolution:

```
Resolution_out = Resolution_in * FOSR^FORD
```
- FOSR(decimation): 32
- FORD(sinc filter order): 3

```
2 * 32^3 = 65536 = 2^16
```

## DAC

DAC supports either 8bit or 12bit length data: 8bit unsigned integer or 12bit unsigned integer.

In case of 12bit, the following conversion is required for feeding the 24bit length PCM data into DAC:

```
(uint16_t)((PCM16bit integer data) >> 4 + 2048)
```

The wave form should follow the following:
- Highest level: 4095
- Zero level: 2048
- Lowest level: 0

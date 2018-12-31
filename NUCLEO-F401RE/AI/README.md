# X-CUBE-AI ver 3.3.0 evaluation

## X-CUBE-AI

- [STM32 solutions for Artificial Neural Networks](https://www.st.com/content/st_com/en/stm32-ann.html)
- [X-CUBE-AI](https://www.st.com/en/embedded-software/x-cube-ai.html)

## X-CUBE-AI guide

I just followed the instructions on [this video (YoueTube)](https://www.youtube.com/watch?v=grgNXdkmzzQ&list=PLnMKNibPkDnG9IC5Nl9vJg1CKMAO1kODW&t=141s&index=2).

## Loading Keras model into CubeMX

I loaded a Keras model in [this folder](../Thermography/tensorflow) into CubeMX. The model "rock-paper-scissors" was trained with normalized dataset of float type with its range -1.0 ~ +1.0.

## Validation result on CubeMX

It takes around 10msec to infer rock-paper-scissors on 32x32 image from the infrared array sensor.

```
Matching results...
Unable to compress to target: 4.000000 wanted, 3.080142 reached

ON-DEVICE STM32 execution ("network", auto-detect, 115200)..

<Stm32com id=0x279c8127080 - CONNECTED(COM6/115200) devid=0x433/STM32F401xD/E msg=1.0>
 0x433/STM32F401xD/E @84MHz/84MHz (FPU is present) lat=2 ART: PRFTen ICen DCen
 found network(s): ['network']
 description    : 'network' (32, 32, 1)-[4]->(1, 1, 3) macc=950717 rom=23.14KiB ram=23.13KiB
 tools versions : rt=(3, 3, 0) tool=(3, 3, 0)/(1, 1, 0) api=(1, 0, 0) "Mon Dec 31 08:42:10 2018"

Running with inputs=(10, 32, 32, 1)..
.... 1/10
.... 2/10
.... 3/10
.... 4/10
.... 5/10
.... 6/10
.... 7/10
.... 8/10
.... 9/10
.... 10/10
 RUN Stats    : batches=10 dur=7.062s tfx=6.575s 6.101KiB/s (wb=40.000KiB,rb=120B)

Results for 10 inference(s) @84/84MHz (macc:950717)
 duration    : 99.405 ms (average)
 CPU cycles  : 8350026 (average)
 cycles/MACC : 8.78 (average for all layers)

Inspector report (layer by layer)
 signature      : 761E9573
 n_nodes        : 4
 num_inferences : 10

Clayer  id  desc                          oshape            ms        
--------------------------------------------------------------------------------
0       0   10011/(Merged Conv2d / Pool)  (10, 15, 15, 16)  31.126    
1       2   10011/(Merged Conv2d / Pool)  (10, 6, 6, 32)    67.844    
2       6   10005/(Dense)                 (10, 1, 1, 3)     0.425     
3       6   10014/(Softmax)               (10, 1, 1, 3)     0.010     
                                                            99.405 (total)

  MACC / frame: 950717
  ROM size:     23.14 KBytes
  RAM size:     23.13 KBytes (Minimum: 23.13 KBytes)
  Comp. factor: 1.394


Matching criteria: L2 error < 0.01 on the output tensor

  Ref layer 6 matched with C layer 3, error: 0.0038189557

Validation: OK
 Validation OK
Python validation ended
```

## How to run the network

=> **[CODE_READING](CODE_READING.md)**

## Experiment next

I will try the following setup:

```

[PC]---32x32 image--->[STM32]---inference result--->[PC]

```

## Tips for TrueSTUDIO

I guess "Add necessary files as reference in the toolchain project config files" is the right option to select on Code Generator, but it did not work on TrueSTUDIO.

So I left the option to default, and I managed to build the code with extra steps as below:

[Step 1] Add the lib path

![](./truestudio_lib_path.jpg)

![](./truestudio_lib_path_edit.jpg)

[Step 2] Add the lib name

![](./truestudio_lib_name.jpg)

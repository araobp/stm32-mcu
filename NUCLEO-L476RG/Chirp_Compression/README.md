# Chirp compression

![](./oscilloscope/screenshots/chirp_compression.jpg)

## Background and motivation

- [This project "ultrasonic-communcations"](https://github.com/araobp/ultrasonic-communication) resulted in an unsuccessful output. But I recently experimented chirp compression with a matched filter. It turned out to be succesful with better SNR.
- I am working on AED (Acoustic Event Detection) in [this "acoustic-event-detection" project](https://github.com/araobp/acoustic-event-detection) right now. I wonder if the devic
e can employ chirp compression with a little cheap speaker (instead of BLE) to transmit data to the cloud.

## Chirp compression technique

It is very simple:

```
c(n)=IFFT[FFT{a(n)} * FFT{b(n)}]

a(n): raw wave of received chirp signal with zero padding
    [ raw wave of N samples       | zero padding of N samples   ]

b(n): inverse reference chirp signal with zero padding
    [ matched filter of N samples | zero padding of N samples   ]

First half of c(n) is added to the second half of previous c(n):
    [ A                           | B                           ]
                                  [ A                           | B                           ]
                                                  |
                                                  V
                                                output

```

All the noise is not compressed, that is the point.

## System components in development

- [Receiver (STM32L476RG with Knowles MEMS microphones](./stm32)
- [Oscilloscope GUI (Tkinter and matplotlib)](./oscilloscope)
- Transmitter to be developed (not a python-based one)

## Experiments (Dec 2, 2018)

```

Jupyter
Notebook                                                                          Chirp compression
 [PC]--[Mini speaker] ------ Chirp 15000Hz~18000Hz ------> [MEMS mic]--PDM-->[DFSDM][Arm Cortex-M4]
                                                                                          |
                                                                                         UART
                                                                                          V
                                                                                    [Oscilloscope GUI]

```

The screenshot above is captured by the oscilloscope GUI:
- Chirp transmitted by a very very cheap mini speaker at $1.
- Very small volume of the sound.
- 2 meters distance between the transmitter and the receiver.
- Background music (not background noise for me): Framenco guitar music by Paco de Lucia.

Parameters:
- Clock: 80_000_000/25(Divider)/42(FOSR) = 76.2kHz
- Frame length: 1024 samples, 13.4msec
- Sweep range: 15000Hz ~ 18000Hz

## Reference

- [Radar pulse compression](https://www.ittc.ku.edu/workshops/Summer2004Lectures/Radar_Pulse_Compression.pdf)
- [Chirp compression](https://en.wikipedia.org/wiki/Chirp_compression)

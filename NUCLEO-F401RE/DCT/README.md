# DCT Type-II Implementation on CMSIS-DSP

DCT Type-II is used for calculating MFCCs and image compression in my other projects.

Since CMSIS-DSP does not support DCT Type-II, I have developed it on my own.

I have also developed 2D-version of DCT Type-II for 2D image pre-processing.

- [header file](./stm32/Inc/dct.h)
- [source file](./stm32/Src/dct.c)

Note1: this implementation is based on CMSIS-DSP.
Note2: this implementation supports **norm="ortho"** only. Refer to [scipy.fftpack.dct](https://docs.scipy.org/doc/scipy-0.14.0/reference/generated/scipy.fftpack.dct.html).

## References

- [よくわかる信号処理](https://www.kinokuniya.co.jp/f/dsg-08-EK-0410603)
- [CMSIS-DSP](http://www.keil.com/pack/doc/CMSIS/DSP/html/index.html)

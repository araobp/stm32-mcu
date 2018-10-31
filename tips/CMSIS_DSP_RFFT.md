# CMSIS DSP RFFT pitfalls

## Input parameter is used as a work memory

I have not been able to use arm_rfft_fast_f32 API so far, because it seemed to output an incorrect result.

```
void arm_rfft_fast_f32(arm_rfft_fast_instance_f32 * S, float32_t * p, float32_t * pOut, uint8_t ifftFlag);
```

I have been using the API like this:

```
void arm_rfft_fast_f32(&S, p, p, 0);
```

Since the API seems to use p also as a work memory for calculating FFT, the output becomes incorrect.

Right way:

```
void arm_rfft_fast_f32(&S, p1, p2, 0);
```

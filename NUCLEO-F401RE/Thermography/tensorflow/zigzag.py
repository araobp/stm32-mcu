import math
import numpy as np

ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT = (1, 2, 3, 4, 5, 6, 7, 8)

def zigzag_scan(src):

    path = ONE
    i, j = 0, 0
    len_ = len(src)
    dst = np.zeros(len_)
    width = int(math.sqrt(len_))
    n = 0
    
    for _ in range(len_):
    
        dst[n] = src[j * width + i]
        n += 1
        
        if path == ONE:
            if i == width - 1:
                j += 1
                path = EIGHT
            else:
                i += 1
                path = TWO

        elif path == TWO:
            i -= 1
            j += 1
            if i == 0:
                if j == width - 1:
                    path = SEVEN
                else:
                    path = THREE

        elif path == THREE:
            if j == width - 1:
                i += 1
                path = EIGHT
            else:
                j += 1
                path = FOUR

        elif path == FOUR:
            i += 1
            j -= 1
            if j == 0:
                if i == width - 1:
                    path = FIVE
                else:
                    path = ONE

        elif path == FIVE:
            j += 1
            path = SIX

        elif path == SIX:
            i -= 1
            j += 1
            if j == width - 1:
                path = SEVEN

        elif path == SEVEN:
            i += 1
            path = EIGHT

        elif path == EIGHT:
            i += 1
            j -= 1
            if i == width - 1:
                path = FIVE
    return dst
# DSP library for audio
import matplotlib
FIGSIZE=[12, 4]
LINE_COLOR='darkcyan'
LINE_COLOR2='burlywood'
FS=44100
REAL_ONLY=False
GRID=False

import numpy as np
import scipy.signal as sg
import matplotlib.pyplot as plt

from scipy.fftpack import fft, ifft, fftfreq, fftshift
from scipy.io.wavfile import write
import peakutils

import scipy.io.wavfile as wv
from IPython.display import display, Audio

def setenv(figsize=None, fs=None, real_only=None, grid=None):
    if figsize:
        global FIGSIZE
        FIGSIZE = figsize
    if fs:
        global FS
        FS=fs
    if real_only:
        global REAL_ONLY
        REAL_ONLY=real_only
    if grid:
        global GRID
        GRID=grid
        
def plot_fft(wave, fs=None, xlim=None, ylim=None, thres=0.95, color=LINE_COLOR, mode='magnitude', figsize=None, shift=False, grid=None):
    if figsize is None:
        figsize = FIGSIZE
    if fs is None:
        fs = FS
    plt.figure(figsize=figsize)
    if grid is None:
        grid = GRID

    y = fft(wave)
    n = len(y)
    freq = fftfreq(n, 1/fs)

    if shift:
        y = fftshift(y)
        freq = fftshift(freq)
    else:
        n = int(n/2)
        y = y[:n]
        freq = freq[:n]

    magnitude = np.abs(y)
    
    if mode == 'magnitude':
        plt.plot(freq, magnitude, color=color)
        plt.ylabel("Magnitude")
        plt.title("Frequency domain (Magnitude)")
    elif mode == 'psd':
        psd = 20 * np.log10(magnitude)
        plt.plot(freq, psd, color=color)
        plt.ylabel("PSD (dB)")        
        plt.title("Frequency domain (PSD)")
    plt.xlabel("Frequency (Hz)")
    plt.grid(GRID)

    if xlim:
        plt.xlim(xlim)
    if ylim:
        plt.ylim(ylim)

    print("Frequencies at peaks: {} Hz".format(freq[peakutils.indexes(magnitude, thres=thres)]))

def plot_welch(wave, fs=None, thres=0.95, color=LINE_COLOR, xlim=None, ylim=None, nperseg=256, logscale=False, figsize=None, grid=None):
    
    if figsize is None:
        figsize = FIGSIZE
    if fs is None:
        fs = FS
    plt.figure(figsize=figsize)
    if grid is None:
        grid = GRID

    freq, y = sg.welch(wave, fs, nperseg=nperseg, scaling='density')
    n = len(y)
    
    if logscale:
        plt.plot(freq, 20*np.log10(y), color=color)
        plt.ylabel("PSD (dB)")
        plt.title("Frequency domain (PSD)")
    else:
        plt.plot(freq, y, color=color)
        plt.ylabel("PSD (V^2)/Hz")
        plt.title("Frequency domain (PSD)")
        
    plt.xlabel("Frequency (Hz)")
    plt.grid(GRID)

    if xlim:
        plt.xlim(xlim)
    if ylim:
        plt.ylim(ylim)

    print("Frequencies at peaks: {} Hz".format(freq[peakutils.indexes(y, thres=thres)]))

def plot_spectrogram(wave, fs=None, xlim=None, ylim=None, band=1, nperseg=256, mode='psd', cmap='summer', figsize=None):
    if figsize is None:
        figsize = FIGSIZE
    if fs is None:
        fs = FS
    plt.figure(figsize=figsize)
    f, t, Sxx = sg.spectrogram(np.real(wave), nperseg=nperseg, fs=fs, mode=mode)
    n = len(f)
    band = int(n * band)
    if mode == 'psd':
        plt.pcolormesh(t, f[:band], 20 * np.log10(Sxx[:band]), cmap=cmap)
    elif mode == 'magnitude':
        plt.pcolormesh(t, f[:band], Sxx[:band], cmap=cmap)
    plt.ylabel('Frequency [Hz]')
    plt.xlabel('Time [sec]')
    plt.title("Spectrogram")
    
    if xlim:
        plt.xlim(xlim)
    if ylim:
        plt.ylim(ylim)

def plot_wave(wave, fs=None, xlim=None, ylim=None, real_only=None, logscale=False, color=LINE_COLOR, color2=LINE_COLOR2, figsize=None, grid=None):
    if figsize is None:
        figsize = FIGSIZE
    if fs is None:
        fs = FS
    if real_only is None:
        real_only = REAL_ONLY
    if grid is None:
        grid = GRID
    plt.figure(figsize=figsize)
    t = np.linspace(0, len(wave)/fs, len(wave))
    re = np.real(wave)
    im = np.imag(wave)
    if logscale:
        re = 20 * np.log10(re)
        im = 20 * np.log10(im)
        plt.ylabel('Amplitude(dB)')
    else:
        plt.ylabel('Amplitude')

    if real_only:
        plt.plot(t, re, color=color)
    else:
        plt.plot(t, re, color=color)
        plt.plot(t, im, color=color2)

    plt.xlabel('Time [sec]')
    plt.title("Time domain")
    plt.grid(GRID)
    
    if xlim:
        plt.xlim(xlim)
    if ylim:
        plt.ylim(ylim)

def cut(wave, t0, t1, fs=None):
    if fs is None:
        fs = FS
    a = int(t0*fs)
    b = int(t1*fs)
    return(wave[a:b])

def buffer(length, wave, delay=0, fs=None):
    if fs is None:
        fs = FS
    d = int(fs * delay)
    l = int(fs * length)
    buf = np.zeros(l)
    buf[d:len(wave)+d] = wave
    return buf

def read(filename):
    rate, data = wv.read(filename)
    if len(data.shape) == 2:
        data = data[:,0]
    return (rate, data)

def play(rate=None, *args, **kwargs):
    if rate is None:
        rate = FS
    display(Audio(rate=rate, *args, **kwargs))

def lpf(wave, cutoff, fs=FS):
    WP = float(cutoff)/float(fs/2)
    WS = 1.3 * WP
    N, Wn = sg.buttord(wp=WP, ws=WS, gpass=2, gstop=30, analog=0)
    b, a = sg.butter(N, Wn, btype='low', analog=0, output='ba')
    g = sg.lfilter(b, a, wave)
    return g

def sin_wave(f, length=1.0, A=1.0, phase=0.0, fs=None):
    if fs is None:
        fs = FS
    t = np.linspace(0, length, int(length * fs))
    arg = (2.0 * np.pi * f * t) + phase
    return np.sin(arg)
    
def cos_wave(f, length=1.0, A=1.0, phase=0.0, fs=None):
    if fs is None:
        fs = FS
    t = np.linspace(0, length, int(length * fs))
    arg = (2.0 * np.pi * f * t) + phase
    return np.cos(arg)

def sinc_wave(length=1.0, offset=0.5, A=1.0, denseness=1000, fs=None):
    if fs is None:
        fs = FS
    x = np.linspace(0-offset, length-offset, int(length * fs)) * denseness
    return A*np.sinc(x)

class Chirp:

    def __init__(self, f0, f1, T=1.0, A=1.0, fs=None):
        if fs is None:
            self.fs = FS
        else:
            self.fs = fs
        self.f0 = f0
        self.f1 = f1
        self.T = T
        self.A = A

    def chirp(self, updown="up", phase=-np.pi/2.0):
        t = np.linspace(0, self.T, int(self.T * self.fs))
        k = float(self.f1 - self.f0)/float(self.T)
        if (updown == "up"):
            f = self.f0 + k * t / 2.0
        elif (updown == "down"):
            f = self.f1 - k * t / 2.0
        arg = (2.0 * np.pi * f * t) + phase
        return np.exp(1j * arg) * self.A

    def log_sweep(self, updown="up"):
        t = np.linspace(0, self.T, int(self.T * self.fs))
        k = float(self.f1 - self.f0)/float(self.T)
        o1 = 2.0 * np.pi * self.f0
        o2 = 2.0 * np.pi * self.f1
        o21 = o2 / o1
        arg = o1 * self.T / np.log(o21) * (np.exp(t/self.T*np.log(o21)) - 1.0)
        if updown == 'down':
            arg = arg[::-1]
        return np.sin(arg) * self.A
            
    def chirp_cos(self, updown="up", phase=-np.pi/2.0):
        return np.real(self.chirp(updown, phase))

    def chirp_sin(self, updown="up", phase=-np.pi/2.0):
        return np.imag(self.chirp(updown, phase))
    
    def silence(self):
        return np.zeros(int(self.T * self.fs))

    def white_noise(self, A):
        if A == None:
            A = self.A
        a = np.random.random(int(self.T * self.fs)) * 2 * A - A
        b = np.random.random(int(self.T * self.fs)) * 2 * A - A
        return a + 1j * b

    def constant_noise(self, f, A):
        if A == None:
            A = self.A
        t = np.linspace(0, self.T, int(self.T * self.fs))
        arg = 2 * np.pi * f * t
        return cos(arg) * A
    
    def carrier_IQ(self, iq, fc, phase=-np.pi/2.0):
        t = np.linspace(0, self.T, int(self.T * self.fs))
        if iq == 'I':
            return np.cos(2 * np.pi * fc * t + phase)
        elif iq == 'Q':
            return np.sin(2 * np.pi * fc * t + phase)

    def chirp_x_carrier(self, fc, updown="up"):
        t = np.linspace(0, self.T, int(self.T * self.fs))
        k = float(self.f1-self.f0)/float(self.T)
        if (updown == "up"):
            f = self.f0 + k * t / 2.0
        elif (updown == "down"):
            f = self.f1 - k * t / 2.0
        arg = 2 * np.pi * (fc - f) * t
        return np.cos(arg) * self.A

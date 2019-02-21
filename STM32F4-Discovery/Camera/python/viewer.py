import matplotlib
matplotlib.use('TkAgg')

import numpy as np
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import sys
import tkinter as Tk
from datetime import datetime
import time
import os

import matplotlib.pyplot as plt

import interface
import image

import h5py

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("port", help="serial port identifier")
parser.add_argument("-s", "--size", help="image sizes", default="m")
parser.add_argument("-e", "--effect", help="effect", default=None)
args = parser.parse_args()

def int2regvalue(value):
    if value >= 0:
        sign = 0b00000000
    else:
        sign = 0b10000000
    value = abs(value)
    if value > 127:
        value = 127
    return sign + value

if __name__ == '__main__':

    itfc = interface.Interface(port = args.port, size=args.size)
    gui = image.GUI(interface=itfc)

    PADX = 6
    PADX_GRID = 2
    PADY_GRID = 2

    root = Tk.Tk()
    root.wm_title("Image viewer")

    fig, ax = plt.subplots(1, 1, figsize=(4, 4))
    
    fig.subplots_adjust(bottom=0.15)

    frame = Tk.Frame(master=root)
    frame_row0 = Tk.Frame(master=frame)
    frame_row1 = Tk.Frame(master=frame)
    frame_row2 = Tk.Frame(master=frame)

    cnt = 0
    class_label_ = ''
    filename = None
    data = None
    shape = (8, 8)

    repeat_action = False

    canvas = FigureCanvasTkAgg(fig, master=frame_row0)
    canvas.draw()
    
    def pixels():
        global data, ax
        ax.clear()
        if args.effect is None:
            data = gui.plot(ax, interface.PIXELS)
        elif args.effect == 'd':
            data = gui.plot(ax, interface.DIFF)
        fig.tight_layout()
        canvas.draw()

    def pixels_continuous():
        global data, ax
        ax.clear()
        if args.effect is None:
            data = gui.plot(ax, interface.PIXELS)
        elif args.effect == 'd':
            data = gui.plot(ax, interface.DIFF)
        fig.tight_layout()
        canvas.draw()
        repeat(pixels_continuous)

    def brightness():
        value = int(entry_brightness.get())
        value = int2regvalue(value)
        itfc.write(interface.BRIGHTNESS, value)
    
    def contrast():
        value = int(entry_contrast.get())
        value += 0x40
        itfc.write(interface.CONTRAST, value)

    # Repeat an operation
    def repeat(func):
        if repeat_action:
            root.after(10, func)

    def repeat_toggle():
        global repeat_action
        if repeat_action == True:
            repeat_action = False
            button_continuous.configure(bg='lightblue')
        else:
            repeat_action = True
            button_continuous.configure(bg='red')
            pixels_continuous()

    def screenshot():
        fig.savefig('screen_shot.png')

    def _quit():
        itfc.close()
        root.quit()
        root.destroy() 

   
    label_class = Tk.Label(master=frame_row1, text='Class label:')
    entry = Tk.Entry(master=frame_row1, width=14)
    counter = Tk.Label(master=frame_row1)

    button_shutter = Tk.Button(master=frame_row1, text='Shutter', command=pixels, bg='lightblue', activebackground='grey', padx=PADX)
    button_continuous = Tk.Button(master=frame_row1, text='Continous', command=repeat_toggle, bg='lightblue', activebackground='grey', padx=PADX)
    button_screenshot = Tk.Button(master=frame_row1, text='Screenshot', command=screenshot, bg='lightblue', activebackground='grey', padx=PADX)
    button_quit = Tk.Button(master=frame_row1, text='Quit', command=_quit, bg='yellow', activebackground='grey', padx=PADX)

    entry_brightness = Tk.Entry(master=frame_row2, width=5)
    button_brightness = Tk.Button(master=frame_row2, text='Brightness', command=brightness, bg='lightblue', activebackground='grey', padx=PADX)
    entry_contrast = Tk.Entry(master=frame_row2, width=5)
    button_contrast = Tk.Button(master=frame_row2, text='Contrast', command=contrast, bg='lightblue', activebackground='grey', padx=PADX)

    ##### Place the parts on Tk #####

    frame.pack(expand=True, fill=Tk.BOTH)

    ### Row 0: main canvas
    canvas._tkcanvas.pack(expand=True, fill=Tk.BOTH)
    frame_row0.pack(expand=True, fill=Tk.BOTH)

    ### Row 1: operation ####
    label_class.grid(row=0, column=0, padx=PADX_GRID)
    entry.grid(row=0, column=1, padx=PADX_GRID)
    counter.grid(row=0, column=2, padx=PADX_GRID)
    counter.configure(text='({})'.format(str(cnt)))
    button_shutter.grid(row=0, column=3, padx=PADX_GRID)
    button_continuous.grid(row=0, column=4, padx=PADX_GRID)
    button_screenshot.grid(row=0, column=7, padx=PADX_GRID)
    button_quit.grid(row=0, column=8, padx=PADX_GRID)
    frame_row1.pack(pady=PADY_GRID)

    ### Row 2: calibration ####
    entry_brightness.grid(row=0, column=0, padx=PADX_GRID)
    button_brightness.grid(row=0, column=1, padx=PADX_GRID)
    entry_contrast.grid(row=0, column=2, padx=PADX_GRID)
    button_contrast.grid(row=0, column=3, padx=PADX_GRID)
    frame_row2.pack(pady=PADY_GRID)

    ##### loop forever #####
    Tk.mainloop()

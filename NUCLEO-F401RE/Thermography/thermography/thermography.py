#
# Thermography GUI
#

import matplotlib
import numpy as np
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import sys
import tkinter as Tk
import heatmap
from datetime import datetime
import time
import os

import matplotlib.pyplot as plt
plt.style.use('dark_background')

import argparse
parser = argparse.ArgumentParser()
parser.add_argument("port", help="serial port identifier")
parser.add_argument("-d", "--debug", help="serial port identifier", action="store_true")
args = parser.parse_args()

if __name__ == '__main__':

    gui = heatmap.GUI(port = args.port)

    matplotlib.use('TkAgg')

    PADX = 6
    PADX_GRID = 2
    PADY_GRID = 2

    root = Tk.Tk()
    root.wm_title("Viewer")
    
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(6, 5), gridspec_kw = {'width_ratios':[20, 1]})
    fig.subplots_adjust(bottom=0.15)

    frame = Tk.Frame(master=root)
    frame_row0 = Tk.Frame(master=frame)
    frame_row1 = Tk.Frame(master=frame)
    frame_row2 = Tk.Frame(master=frame)
    frame_row3 = Tk.Frame(master=frame)

    canvas = FigureCanvasTkAgg(fig, master=frame_row0)
    canvas.show()

    def pixels():
        ax1.clear()
        ax2.clear()
        data = gui.plot((ax1, ax2), heatmap.PIXELS, cmap='rainbow')
        fig.tight_layout()
        canvas.draw()
        thermistor()
        repeat(pixels)

    def thermistor():
        data = gui.plot((ax1, ax2), heatmap.THERMISTOR)
        label_thermistor.configure(text='Room temperature: {:.1f} degrees Celsius'.format(data[0]))

    repeat_action = False

    # Repeat an operation
    def repeat(func):
        if repeat_action:
            root.after(10, func)

    def repeat_toggle():
        global repeat_action
        if repeat_action == True:
            repeat_action = False
            button_repeat.configure(bg='lightblue')
        else:
            repeat_action = True
            button_repeat.configure(bg='red')

    def savefig():
        fig.savefig('screen_shot.png')

    def _quit():
        gui.close()
        root.quit()
        root.destroy() 

    label_thermistor = Tk.Label(master=frame_row1, padx=PADX)

    button_pixels = Tk.Button(master=frame_row2, text='Pixels', command=pixels, bg='lightblue', activebackground='grey', padx=PADX)
    button_repeat = Tk.Button(master=frame_row2, text='Repeat', command=repeat_toggle, bg='lightblue', activebackground='grey', padx=PADX)
    button_savefig = Tk.Button(master=frame_row2, text='Savefig', command=savefig, bg='lightblue', activebackground='grey', padx=PADX)
    button_quit = Tk.Button(master=frame_row2, text='Quit', command=_quit, bg='yellow', activebackground='grey', padx=PADX)
    

    ##### Place the parts on Tk #####

    frame.pack(expand=True, fill=Tk.BOTH)

    ### Row 0: main canvas
    canvas._tkcanvas.pack(expand=True, fill=Tk.BOTH)
    frame_row0.pack(expand=True, fill=Tk.BOTH)

    ### Row 1: information ####

    label_thermistor.grid(row=0, column=0, padx=PADX_GRID)
    frame_row1.pack(pady=PADY_GRID)

    ### Row 2: operation ####

    button_pixels.grid(row=0, column=0, padx=PADX_GRID)
    button_repeat.grid(row=0, column=2, padx=PADX_GRID)
    button_savefig.grid(row=0, column=3, padx=PADX_GRID)
    button_quit.grid(row=0, column=4, padx=PADX_GRID)
    frame_row2.pack(pady=PADY_GRID)
    
    ### Row 3 ####

    # DEBUG

    ##### loop forever #####
    thermistor()
    Tk.mainloop()

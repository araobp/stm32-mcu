# << Oscilloscope GUI >>
#
# This implementaion makes use of matplotlib on Tk for agile GUI development.
#
# Reference: https://matplotlib.org/2.1.0/gallery/user_interfaces/embedding_in_tk_sgskip.html
#

import matplotlib
import numpy as np
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import sys
import tkinter as Tk
import dsp
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

mode = dsp.ENDFIRE

if __name__ == '__main__':

    gui = dsp.GUI(port = args.port)

    ### Default settings to DSP ###
    ###############################

    matplotlib.use('TkAgg')

    PADX = 6
    PADX_GRID = 2
    PADY_GRID = 2

    ANGLE = ('L', 'l', 'c', 'r', 'R')

    root = Tk.Tk()
    root.wm_title("Oscilloscope")

    fig = Figure(figsize=(10, 4), dpi=100)
    ax = fig.add_subplot(111)
    fig.subplots_adjust(bottom=0.15)

    frame = Tk.Frame(master=root)
    frame_row0 = Tk.Frame(master=frame)
    frame_row1 = Tk.Frame(master=frame)
    frame_row2 = Tk.Frame(master=frame)
    frame_row3 = Tk.Frame(master=frame)

    canvas = FigureCanvasTkAgg(fig, master=frame_row0)
    canvas.show()

    ### Row 1 ####
    range_amplitude = Tk.Spinbox(master=frame_row1, width=6, values=[2**8, 2**9, 2**10, 2**11, 2**13, 2**15])

    ### Row 2 ####
    repeat_action = False

    # Repeat an operation
    def repeat(func):
        if repeat_action:
            root.after(50, func)

    def raw_wave():
        range_ = int(range_amplitude.get())
        mag = gui.plot_aed(ax, dsp.RAW_WAVE, range_=range_)
        fig.tight_layout()
        canvas.draw()
        repeat(raw_wave)

    def compression():
        range_ = int(range_amplitude.get())
        mag = gui.plot_aed(ax, dsp.COMPRESSION, range_=range_)
        fig.tight_layout()
        canvas.draw()
        repeat(compression)

    def beam_forming(angle):
        global mode
        angle = int(angle) + 2
        gui.set_beam_forming(mode, ANGLE[angle])

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
        root.quit()
        root.destroy()

    def broadside():
        global mode
        mode = dsp.BROADSIDE
        angle = range_beam_forming.get() + 2
        gui.set_beam_forming(mode, ANGLE[angle])

    def endfire():
        global mode
        mode = dsp.ENDFIRE
        angle = int(range_beam_forming.get()) + 2
        gui.set_beam_forming(mode, ANGLE[angle])

    def left_mic_only():
        gui.left_mic_only()

    def right_mic_only():
        gui.right_mic_only()

    ### Row 1 ####

    button_waveform = Tk.Button(master=frame_row1, text='Wave', command=raw_wave, bg='lightblue', activebackground='grey', padx=PADX)
    button_psd = Tk.Button(master=frame_row1, text='Compression', command=compression, bg='lightblue', activebackground='grey', padx=PADX)

    ### Row 2 ####

    button_repeat = Tk.Button(master=frame_row2, text='Repeat', command=repeat_toggle, bg='lightblue', activebackground='grey', padx=PADX)
    button_savefig = Tk.Button(master=frame_row2, text='Savefig', command=savefig, bg='lightblue', activebackground='grey', padx=PADX)
    button_quit = Tk.Button(master=frame_row2, text='Quit', command=_quit, bg='yellow', activebackground='grey', padx=PADX)

    label_beam_forming = Tk.Label(master=frame_row2, text='Beam forming:')
    label_left = Tk.Label(master=frame_row2, text='L')
    label_right = Tk.Label(master=frame_row2, text='R')
    range_beam_forming = Tk.Scale(master=frame_row2, orient=Tk.HORIZONTAL, length=70, from_=-1, to=1, showvalue=0, command=beam_forming)

    ### Row 3 ####
    
    button_broadside = Tk.Button(master=frame_row3, text='Broadside', command=broadside, bg='lightblue', activebackground='grey', padx=PADX)
    button_endfire = Tk.Button(master=frame_row3, text='Endfire', command=endfire, bg='lightblue', activebackground='grey', padx=PADX)
    button_left_mic_only = Tk.Button(master=frame_row3, text='Left mic only', command=left_mic_only, bg='lightblue', activebackground='grey', padx=PADX)
    button_right_mic_only = Tk.Button(master=frame_row3, text='Right mic only', command=right_mic_only, bg='lightblue', activebackground='grey', padx=PADX)

    ##### Place the parts on Tk #####

    frame.pack(expand=True, fill=Tk.BOTH)

    ### Row 0: main canvas
    frame_row0.pack(expand=True, fill=Tk.BOTH)
    canvas._tkcanvas.pack(expand=True, fill=Tk.BOTH)

    ### Row 1: operation ####

    frame_row1.pack(pady=PADY_GRID)

    # Waveform
    range_amplitude.grid(row=0, column=3, padx=PADX_GRID)
    button_waveform.grid(row=0, column=4, padx=PADX_GRID)

    # FFT (PSD)
    button_psd.grid(row=0, column=5, padx=PADX_GRID)

    ### Row 2 ####

    frame_row2.pack(pady=PADY_GRID)

    # Beam forming
    #label_beam_forming.grid(row=0, column=0, padx=PADX_GRID)
    #label_left.grid(row=0, column=1, padx=PADX_GRID)
    #range_beam_forming.grid(row=0, column=2, padx=PADX_GRID)
    #label_right.grid(row=0, column=3, padx=PADX_GRID)

    # Repeat, pre_emphasis, save fig and delete
    button_repeat.grid(row=0, column=4, padx=PADX_GRID)
    button_savefig.grid(row=0, column=6, padx=PADX_GRID)
        
    # Quit
    button_quit.grid(row=0, column=9, padx=PADX_GRID)

    ### Row 3 ####

    # DEBUG
    if args.debug:
        frame_row3.pack(pady=PADY_GRID)
        button_broadside.grid(row=0, column=2, padx=PADX_GRID)    
        button_endfire.grid(row=0, column=3, padx=PADX_GRID)            
        button_left_mic_only.grid(row=0, column=4, padx=PADX_GRID)    
        button_right_mic_only.grid(row=0, column=5, padx=PADX_GRID)    

    ##### loop forever #####
    Tk.mainloop()

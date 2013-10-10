#!/usr/bin/env python

import os, sys
import Tkinter
import Image, ImageTk

import Queue

import time
from threading import Thread


exit_gui = 0
root = Tkinter.Tk()
queue = Queue.Queue()

class InputReader(Thread):
    def run(self):
        line = ""
        while(line != "end\n"):
            line = sys.stdin.readline()
            queue.put(line)
            root.quit()

class GuiDrawer(Thread):
    def run(self):
        root.geometry('%dx%d' % (1024,256))
        dirlist = os.listdir('.')
        root.title('Simulacion RAM')
        image1 = Image.open('free_byte.bmp')

        blank_image = Image.new("RGB", (1024, 256))

        for y in range(0,10):
           for x in range(0, 512):
                blank_image.paste(image1,(x*2,y*17))
                
        tkpi = ImageTk.PhotoImage(blank_image)
        label_image = Tkinter.Label(root, image=tkpi)
        label_image.place(x=0,y=0,width=1024,height=256)

        line = ""
        while(line != "end\n"):
            root.mainloop()
            try:
                line = queue.get()
                #TODO: parse line :)
            except Queue.Empty:
                pass

InputReader().start()
GuiDrawer().start()




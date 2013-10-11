#!/usr/bin/env python

import os, sys
import Tkinter
import Image, ImageTk

import Queue

import time
from threading import Thread
from collections import namedtuple

Transform = namedtuple("Transform", "time method start end frag")

exit_gui = 0
root = Tkinter.Tk()
queue = Queue.Queue()

class InputReader(Thread):
    def run(self):
        line = sys.stdin.readline()
        while(line != "end\n"):
            data = line.split()
            transform = Transform(data[0],data[1],int(data[2]),int(data[3]),int(data[4]))
            queue.put(transform)
            root.quit()
            line = sys.stdin.readline()
        exit_gui = 1
        root.quit()


InputReader().start()

root.geometry('%dx%d' % (1024,256))
dirlist = os.listdir('.')
root.title('Simulacion RAM')
image_free = Image.open('free_byte.bmp')
image_alloc = Image.open('alloced_byte.bmp')

blank_image = Image.new("RGB", (1024, 1024))

for y in range(0,10):
   for x in range(0, 512):
        blank_image.paste(image_free,(x*2,y*17))
        
tkpi = ImageTk.PhotoImage(blank_image)
label_image = Tkinter.Label(root, image=tkpi)
label_image.place(x=0,y=0,width=1024,height=256)

while(not exit_gui):
    root.mainloop()
    try:
        transform = queue.get()
        image = image_free
        if transform and transform.method == "alloc":
            image = image_alloc
        for n in range(transform.start, transform.end):
                blank_image.paste(image,((n%512)*2,(n/512)*17))
                
        tkpi = ImageTk.PhotoImage(blank_image)
        label_image.destroy()
        label_image = Tkinter.Label(root, image=tkpi)
        label_image.place(x=0,y=0,width=1024,height=256)
    except Queue.Empty:
        pass




#!/usr/bin/env python

"""This is a small script to demonstrate using Tk to show PIL Image objects.
The advantage of this over using Image.show() is that it will reuse the
same window, so you can show multiple images without opening a new
window for each image.

This will simply go through each file in the current directory and
try to display it. If the file is not an image then it will be skipped.
Click on the image display window to go to the next image.

Noah Spurrier 2007
"""

import os, sys
import Tkinter
import Image, ImageTk

def button_click_exit_mainloop (event):
    event.widget.quit() # this will cause mainloop to unblock.

root = Tkinter.Tk()
root.bind("<Button>", button_click_exit_mainloop)
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
root.mainloop() # wait until user clicks the window

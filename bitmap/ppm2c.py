#!/usr/bin/env python
#
# ppm2c.py - transform a ppm image into a C-header file
#
# 1 = black
# 0 = white
#
# ppm values >=128 are black. Those less than 128 are white, only the
# first of the three color bytes per pixel is used.
#
# Every 8 bit is a y column, there are 128 y columns per row
# and 8 row in the display.
#
# TODO:
#  - get filename from argument or use stdin
#  - confirm file type is P3
#  - get image size from header
#  - get max color from header, divide by 2 to get B/W threshold
#
#########################################################
#
# Copyright 2015 Jerry Dunmire
# jedunmire+i2c_oled AT gmail
# This file is part of i2c_oled
# 
# i2c_oled is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 2 of the License, or
# (at your option) any later version.
# 
# i2c_oled is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with i2c_oled.  If not, see <http://www.gnu.org/licenses/>.
# 
# Files that are not part of i2c_oled are clearly identified at the top
# of each of the files. These files are distributed under terms noted in each
# of the files.
#



# Read PPM file.
# This is not very permissive and probably only works
# with GIMP PNM Filter Version 1.1 files, ASCII format
filename = 'bitmap.ppm'
pixRows = 64
pixPerRow = 128
maxColor = 255
linesPerPixel = 3

#numPixels = pixPerRow * pixPerCol;
threshold = maxColor / 2

bitmap = []
lineCntr = 0;
with open(filename) as f:
    data = f.readlines()[4:]
    for line in data:
        if lineCntr == 0:
            if int(line) < maxColor:
                bit = 0
            else:
                bit = 1
            bitmap.append(bit)
        lineCntr = (lineCntr + 1) % linesPerPixel;

# Print bitmap (for test and debug)
#for row in range(pixRows):
#    for pix in range(pixPerRow):
#        print '{:x}'.format(bitmap[(pix) + (row * pixPerRow)]),
#    print
#

# Print bytes
count = 0
pixPerPillar = 8
pillars = pixRows / pixPerPillar
print '#define BITMAP_ROWS {:d}'.format(pillars)
print '#define BITMAP_COLUMNS {:d}'.format(pixPerRow)
print 'unsigned char bitmap[] ='
print '{'
for pillar in range(pillars):
    for col in range(pixPerRow):
        pilValue = 0
        for pix in range(pixPerPillar):
            pilValue |= bitmap[
                    (col) + (pix * pixPerRow) + (pillar * pixPerPillar * pixPerRow)
                ] << pix
        print '0x{:02x},'.format(pilValue),
    print
print '};'


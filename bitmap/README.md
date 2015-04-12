ppm2c.py - generate bit map for 128x64 OLED display
===================================================
This is a Python script that will generate a `.h` file from a Portable
Pix Map (ppm) file. The script is not very robust, so your results may
vary.

The script was developed and tested on Ubuntu 14.0.4.

Usage
-----
  1. Create a greyscale image using Gimp. The max size is 128x64.
  2. Export as a PPM file in ASCII format
     The top two lines should look like this:

         P3
         # CREATOR: GIMP PNM Filter Version 1.1

     The 3d line should have two values that specify the dimension of
     you image. The reset of the file should have one value per line and
     the value should be in the range 0-255

  3. Change the following values in ppm2c.py to match your image (the
     values show are for the included face-smile.xcf image).

        filename = 'face-smile.ppm'
        pixRows = 64
        pixPerRow = 128
        maxColor = 255
        linesPerPixel = 3

  4. Run `ppm2c.py` and redirect the output to create the `.h` file:

        $ ppm2c.py > bitmap.h

  5. See `../app/testDisplay.c` for an example of how to use the bitmap
     header.


License
-------
Copyright 2015 Jerry Dunmire
jedunmire+i2c_oled AT gmail
This file, face-smile.xfc, and face-smile.ppm are part of i2c_oled

i2c_oled is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

i2c_oled is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with i2c_oled.  If not, see <http://www.gnu.org/licenses/>.

Files that are not part of i2c_oled are clearly identified at the top
of each of the files. These files are distributed under terms noted in each
of the files.



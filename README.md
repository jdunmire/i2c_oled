i2c_oled - 128x64 OLED display driver of ESP8622
=======================================================
i2c_oled is a driver and example app for an 128x64 OLED display.

This code improves on other versions of the i2c_oled driver by:

  * adding a compile option to rotation the display 180 degrees
  * cleaning up errors
  * improving bitmap display code
  * including bitmap generation tool

Prerequisits
------------
  * https://github.com/pfalcon/esp-open-sdk


Build and Install
----------------------
To build and install the test program:

  * Copy `paths.tmpl` to `paths.inv`.
  * Adjust the paths in `paths.inv` for your setup.
  * To rotate the image on the display 180 degrees, define
      `OLED_ROTATE_180` in `include/user_config.h`

  * Build:

        $ make

  * Install (put your target system in bootloader mode first):

        $ make flash

The test program (`user/testDisplay.h`) demonstrates each of the OLED
functions.

License
-------
Copyright 2015 Jerry Dunmire
jedunmire+i2c_oled AT gmail
This file is part of i2c_oled

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


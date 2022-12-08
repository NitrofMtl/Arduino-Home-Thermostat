# OBSELETE

# Arduino-Home-Thermostat Version 2.2
10 rooms thermostat


Sketch for Arduino DUE that use 10 analogs inputs to read temperature and 10 digitals outputs to control solid state relay (SSR) on heater.


User can control options through GUI web page with ethernet shield.

User can name room and control setpoints for each channels. 
It also have an program page for schedule temperature for each rooms.
Program haves 4 mode of operation:
Single day (sunday to saturday).
Week day (monday to friday).
Week-end.
All day.

Web page is store on SD card.

Configurations, setpoints and program alarms are store on SD card and restore when arduino reboot.


Version 2.0 upgrades:
Upgraded library perfomance for analong reading.
Adding analog sequencer.
Upgraded weeklyAlarm library.
Added library to handle timning event interval so save mcu operation.
Rap most of webpage file inside index.htm to have less request and faster load of graphical user interface.
Upgraded rest interface.
General sketch structure upgrade, bugs fix...

# When upgrading to weeklyAlarm librarie 4.0.2, you should delete backup file in your sd card to reset configuration.

Check "Library-dependency-list.md" for all link of out side code.

  Copyright (c) 03/03/2017

    By Nitrof

  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

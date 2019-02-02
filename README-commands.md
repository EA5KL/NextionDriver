[Return to main README](README.md "Return to main README")


Sending commands from the display
=================================

##### Execute a linux command:
```
    2A F0 (linux command) FF FF FF
```
The 'linux command' is executed.

##### Execute a linux command: with feedback:
```
   2A F1 (linux command) FF FF FF
```
The 'linux command' is executed and the __FIRST__ line of the result
is sent to the display variable 'msg'

There is an example HMI file included.
Press on the MMDVM logo on the main screen to go to the 'system' page
  
##### Refresh page
```
   2A FE (number) FF FF FF
```
Page (number) will be refreshed, that is : all __text__ fields for this page 
from the last time the page was shown, will be (re)sent to the display.  
Note: this will only be for fields which have a name in the range t0 ... t39
  
##### Last Heard
```
   2A FD (page) (number) FF FF FF
```
The (number) last heard fields of (page) are sent.  

Note : __EXPERIMENTAL !__ (works with some flaws)

For now : page __has to be 2__ and number <20  
The fiels will be sent and the fieldnames are prepended with 'LH(nr)'  
So: LH0t0 LH0t1 LH0t2 ... LH1t0 LH1t1 LH1t2 ... LH2t0 LH2t1 LH2t2 ...
  
##### Stop sending data to display
```
   2A FC (number) FF FF FF
```
The inhibit mode wil be set (number=1) / reset (number=0)  
When inhibit is set to active (1), the current RX buffer will be processed 
and then all data from MMDVMHost will be dropped.
This can be practical when changing the Nextion page to your settings page. 
Then no page changes or field data from MMDVMHost will be sent to the display.
When inhibit is reset (0) all data from then on will be processed.


##### Update Display
```
   2A FB FF FF FF
```
  
NextionDriver will search for a .tft file in the DataFilesPath directory 
where the filename matches the connected Nextion display model.
i.e. : when the a display NX4832K035_011R is connected, NextionDriver will 
search for a file NX4832K035.tft and try to load that into the display.
At this moment, baud rate switching is not yet supported. So the .tft file 
will be loaded at the active display baudrate (9600 for layout<4 and 115200 for 
layout 4)

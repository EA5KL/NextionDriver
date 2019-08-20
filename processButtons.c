/*
 *   Copyright (C) 2017,2018 by Lieven De Samblanx ON7LDS & Mariano Sanchez EA5KL
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <curl/curl.h>

#include "NextionDriver.h"

//============================================================================
//
//   Put your code in the subroutine below
//
//============================================================================

char lhreceived[780];

void function_pt(void *ptr, size_t size, size_t nmemb, void *stream){
    lhreceived = ptr;
}

void sendLHlist() {

    char buf[52], exit[52];
    int c, i, length = 52; // 52 is the length of the LHt(x) fields in nextion screen
	
    CURL *curl;
    curl = curl_easy_init();
    if(curl) {
      curl_easy_setopt(curl, CURLOPT_URL, "http://pi-star:raspberry@localhost/admin/mmdvmhost/lh_nextion.php");
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, function_pt);
      curl_easy_perform(curl);
      curl_easy_cleanup(curl);
    }

 // Sustituir la cadena constante por la llamada al modulo lh_nextion.php	
 // sprintf(text, "DMR Slot 2 |EB5EGK     |TG 214   |Net| 21.0| 0%|0.0%DMR Slot 1 |TA3NHI     |TG 91    |Net|  0.5| 0%|0.0%DMR Slot 2 |EA5GLZ     |TG 214   |Net| 41.5| 0%|0.0%DMR Slot 1 |9V1MH      |TG 91    |Net|  0.4| 0%|0.0%DMR Slot 1 |DA6RAM     |TG 91    |Net|  0.8| 0%|0.0%DMR Slot 2 |EA5HFB     |TG 214   |Net|  0.8| 0%|0.0%DMR Slot 2 |EA4LO      |TG 214   |Net|  0.5| 0%|0.0%DMR Slot 1 |DW1HBF     |TG 91    |Net|  0.5| 0%|0.0%DMR Slot 2 |EB4ERW     |TG 214   |Net|  0.5| 0%|0.0%DMR Slot 2 |EA9SV      |TG 214   |Net|  0.5| 0%|0.0%DMR Slot 1 |EA3GMP     |TG 91, 5 |Net|TX   |   |    POCSAG     |DAPNET     |DAPNET Us|Net|  0.0| 0%|0.0%DMR Slot 2 |EA5GK      |TG 214   |Net|  0.1| 0%|0.0%DMR Slot 2 |EB4AXR     |TG 214   |Net|  0.5| 0%|0.0%DMR Slot 1 |M0AUT      |TG 91    |Net|  0.8| 0%|0.0%");
	for (i=1; i<=15; i++) {
	c = 0;
	while (c < length) { 
            exit[c] = lhreceived[(length * (i - 1)) + c];
            c++;
        }
	sprintf(buf, "LHt%d.txt=\"%s\"", i, exit);
        sendCommand(buf);
    }
	
//	if (res != 0) {
//           sendCommand(text);
//           sendCommand("MMDVM.status.val=98");
//	} else {
//           sendCommand("LHt1.txt=\"No data in last heard list\"");
//           sendCommand("MMDVM.status.val=99");
//        } 
//}
}


void processButtons(unsigned char code) {

char buf[300];

    //See the README file for how to define a button on the Nextion Display
    if (code==0xA0) {
       sendLHlist();
     } else {
       sprintf(buf, "msg.txt=\"Button pressed %d (%s)\"", code, RXbuffer);
       sendCommand(buf);
     }
}


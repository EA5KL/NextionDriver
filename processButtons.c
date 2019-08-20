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


//int handle_data(char *ptr, int size, int nmemb, char *stream) { 
//    int numbytes = size*nmemb; 
//    char lastchar = *((char *) ptr + numbytes - 1); 
//    *((char *) ptr + numbytes - 1) = '\0'; 
//    contents.append((char *)ptr); 
//    contents.append(1,lastchar); 
//    *((char *) ptr + numbytes - 1) = lastchar; 
//      text = ((char *)ptr);
//      return size*nmemb; 
//    } 

void sendLHlist() {

    char text[1000], exit[70];
	
int c, i, length = 65; // 65 is the length of the LHt(x) fields in nextion screen

    // Sustituir la cadena constante por la llamada al modulo lh_nextion.php	
    sprintf(text, "LHt1.txt=\"DMR Slot 2 |EB5EGK     |TG 214   |Net| 21.0| 0%|0.0%|\"LHt2.txt=\"DMR Slot 1 |TA3NHI     |TG 91    |Net|  0.5| 0%|0.0%|\"LHt3.txt=\"DMR Slot 2 |EA5GLZ     |TG 214   |Net| 41.5| 0%|0.0%|\"LHt4.txt=\"DMR Slot 1 |9V1MH      |TG 91    |Net|  0.4| 0%|0.0%|\"LHt5.txt=\"DMR Slot 1 |DA6RAM     |TG 91    |Net|  0.8| 0%|0.0%|\"LHt6.txt=\"DMR Slot 2 |EA5HFB     |TG 214   |Net|  0.8| 0%|0.0%|\"LHt7.txt=\"DMR Slot 2 |EA4LO      |TG 214   |Net|  0.5| 0%|0.0%|\"LHt8.txt=\"DMR Slot 1 |DW1HBF     |TG 91    |Net|  0.5| 0%|0.0%|\"LHt9.txt=\"DMR Slot 2 |EB4ERW     |TG 214   |Net|  0.5| 0%|0.0%|\"LHt10.txt=\"DMR Slot 2 |EA9SV      |TG 214   |Net|  0.5| 0%|0.0%|\"LHt11.txt=\"DMR Slot 1 |EA3GMP     |TG 91, 5 |Net|TX   |   |    |\"LHt12.txt=\"POCSAG     |DAPNET     |DAPNET Us|Net|  0.0| 0%|0.0%|\"LHt13.txt=\"DMR Slot 2 |EA5GK      |TG 214   |Net|  0.1| 0%|0.0%|\"LHt14.txt=\"DMR Slot 2 |EB4AXR     |TG 214   |Net|  0.5| 0%|0.0%|\"LHt15.txt=\"DMR Slot 1 |M0AUT      |TG 91    |Net|  0.8| 0%|0.0%|\"");
    for (i=1; i<=15; i++) {
	c = 0;
	while (c < length) { 
            exit[c] = text[(length * (i - 1)) + c];
            c++;
        }
        sendCommand(exit);
    }
	
//    CURL* curl = curl_easy_init(); 
//    if(curl) 
//        { 
//        curl_easy_setopt(curl, CURLOPT_URL, "http://pi-star:raspberry@localhost/admin/mmdvmhost/lh_nextion.php"); 
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handle_data); 
//        CURLcode res = curl_easy_perform(curl); 
//        curl_easy_cleanup(curl); LHt1.txt="DMR Slot 2 |EB5EGK     |TG 214   |Net| 21.0| 0%|0.0%|"LHt2.txt="DMR Slot 1 |TA3NHI     |TG 91    |Net|  0.5| 0%|0.0%|"LHt3.txt="DMR Slot 2 |EA5GLZ     |TG 214   |Net| 41.5| 0%|0.0%|"LHt4.txt="DMR Slot 1 |9V1MH      |TG 91    |Net|  0.4| 0%|0.0%|"LHt5.txt="DMR Slot 1 |DA6RAM     |TG 91    |Net|  0.8| 0%|0.0%|"LHt6.txt="DMR Slot 2 |EA5HFB     |TG 214   |Net|  0.8| 0%|0.0%|"LHt7.txt="DMR Slot 2 |EA4LO      |TG 214   |Net|  0.5| 0%|0.0%|"LHt8.txt="DMR Slot 1 |DW1HBF     |TG 91    |Net|  0.5| 0%|0.0%|"LHt9.txt="DMR Slot 2 |EB4ERW     |TG 214   |Net|  0.5| 0%|0.0%|"LHt10.txt="DMR Slot 2 |EA9SV      |TG 214   |Net|  0.5| 0%|0.0%|"LHt11.txt="DMR Slot 1 |EA3GMP     |TG 91, 5 |Net|TX   |   |    |"LHt12.txt="POCSAG     |DAPNET     |DAPNET Us|Net|  0.0| 0%|0.0%|"LHt13.txt="DMR Slot 2 |EA5GK      |TG 214   |Net|  0.1| 0%|0.0%|"LHt14.txt="DMR Slot 2 |EB4AXR     |TG 214   |Net|  0.5| 0%|0.0%|"LHt15.txt="DMR Slot 1 |M0AUT      |TG 91    |Net|  0.8| 0%|0.0%|"

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


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

    char text[1800];
	
//    CURL* curl = curl_easy_init(); 
//    if(curl) 
//        { 
//        curl_easy_setopt(curl, CURLOPT_URL, "http://pi-star:raspberry@localhost/admin/mmdvmhost/lh_nextion.php"); 
//        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handle_data); 
//        CURLcode res = curl_easy_perform(curl); 
//        curl_easy_cleanup(curl); 
int res = 0;
	if (res !== 0) {
           sendCommand(text);
           sendCommand("MMDVM.status.val=98");
	} else {
           sendCommand("LHt1.txt=\"No data in last heard list\"");
           sendCommand("MMDVM.status.val=99");
        } 
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


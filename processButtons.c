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
#include "basicFunctions.h"

//============================================================================
//
//   Put your code in the subroutine below
//
//============================================================================

char *DataReceived;

void function_pt(void *ptr, size_t size, size_t nmemb, void *stream){
    DataReceived = ptr;
}

void sendLHlist(unsigned char param) { // Last heard list of stations

    char buf[800], exit[52], LHlistURL[80];
    int c, i, length = 52; // 52 is the length of the LHt(x) fields in nextion screen

    if (param == 0x01)
       sprintf(LHlistURL, "http://pi-star:raspberry@localhost/admin/mmdvmhost/lh_nextion.php?from=NET");
    else if(param == 0x02)
       sprintf(LHlistURL, "http://pi-star:raspberry@localhost/admin/mmdvmhost/lh_nextion.php?from=LOC");
	    
    CURL *curl;
    curl = curl_easy_init();
    if(curl) {
      curl_easy_setopt(curl, CURLOPT_URL, LHlistURL);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, function_pt);
      curl_easy_perform(curl);
      curl_easy_cleanup(curl);
    }
    for (i=1; i<=15; i++) {
	c = 0;
	while (c < length) { 
            exit[c] = DataReceived[(length * (i - 1)) + c];
            c++;
        }
	sprintf(buf, "LHt%d.txt=\"%s\"", i, exit);
        sendCommand(buf);
    }
	
//	if (res != 0) {
//           sendCommand("MMDVM.status.val=98");
//	} else {
//           sendCommand("LHt1.txt=\"No data in last heard list\"");
//           sendCommand("MMDVM.status.val=99");
//      } 
}


#define bcoEN	1472
#define bcoDIS	25356
#define pcoEN	0
#define pcoDIS	65535

void systemInformation() { // System information retrieval

    char buf[800], SysInfoURL[70];
    int i = 1, bco;
    float temperature;

    sprintf(SysInfoURL, "http://pi-star:raspberry@localhost/admin/mmdvmhost/sysInfo_nextion.php");
	
    CURL *curl;
    curl = curl_easy_init();
    if(curl) {
      curl_easy_setopt(curl, CURLOPT_URL, SysInfoURL);
      curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, function_pt);
      curl_easy_perform(curl);
      curl_easy_cleanup(curl);
    }
	
    char *token = strtok(DataReceived, "|");
    if (token != NULL) {
       while (token != NULL) {
          if (i == 1) { sprintf(buf, "SySt10.txt=\"%s\"", token); sendCommand(buf); }
          else if (i == 2) { sprintf(buf, "SySt11.txt=\"%s\"", token); sendCommand(buf); }
          else if (i == 3) { sprintf(buf, "SySt0.txt=\"%s\"", token); sendCommand(buf); }
          else if (i == 4) { sprintf(buf, "SySt4.txt=\"%s\"", token); sendCommand(buf); }
          else if (i == 5) { sprintf(buf, "SySt8.txt=\"%s\"", token); sendCommand(buf); }
          else if (i == 6) { sprintf(buf, "SySt1.txt=\"%s\"", token); sendCommand(buf); }
          else if (i == 7) { temperature = atof(token); }
	  else if (i == 8) {
             if (temperature < 50.0) bco = 5858;
	     else if ((temperature >= 50.0) && (temperature <= 69.99)) bco = 64832;
	     else if (temperature >= 70.00) bco = 63488;
             sprintf(buf, "SySt5.txt=\"%2.1f / %s\"", temperature, token); sendCommand(buf);
	     sprintf(buf, "SySt5.bco=%d", bco); sendCommand(buf);
	  } else if (i == 9) { sprintf(buf, "SySt3.txt=\"%s\"", token); sendCommand(buf); }
	  else if (i == 10) { sprintf(buf, "SySt6.txt=\"%s\"", token); sendCommand(buf); }
	  else if (i == 11) { sprintf(buf, "SySt7.txt=\"%s\"", token); sendCommand(buf); }
	  else if (i == 12) { sprintf(buf, "SySt9.txt=\"%s\"", token); sendCommand(buf); }
	  else if (i == 13) { sprintf(buf, "SySt2.txt=\"%s\"", token); sendCommand(buf); }
	  else if (i == 14) { sprintf(buf, "SySt2.bco=\%s", token); sendCommand(buf); }
	  else if (i == 15) { sprintf(buf, "SySt2.pco=\%s", token); sendCommand(buf); }
	  i++;
          token = strtok(NULL, "|");
       }
    }
    
    sprintf(buf, "A1.bco=%d", (proc_find("MMDVMHost")>0) ? bcoEN : bcoDIS); sendCommand(buf);
    sprintf(buf, "A1.pco=%d", (proc_find("MMDVMHost")>0) ? pcoEN : pcoDIS); sendCommand(buf);
    sprintf(buf, "A2.bco=%d", (proc_find("DMRGateway")>0) ? bcoEN : bcoDIS); sendCommand(buf);
    sprintf(buf, "A2.pco=%d", (proc_find("DMRGateway")>0) ? pcoEN : pcoDIS); sendCommand(buf);
    sprintf(buf, "A3.bco=%d", (proc_find("YSFGateway")>0) ? bcoEN : bcoDIS); sendCommand(buf);
    sprintf(buf, "A3.pco=%d", (proc_find("YSFGateway")>0) ? pcoEN : pcoDIS); sendCommand(buf);
    sprintf(buf, "A4.bco=%d", (proc_find("YSFParrot")>0) ? bcoEN : bcoDIS); sendCommand(buf);
    sprintf(buf, "A4.pco=%d", (proc_find("YSFParrot")>0) ? pcoEN : pcoDIS); sendCommand(buf);
    sprintf(buf, "A5.bco=%d", (proc_find("P25Gateway")>0) ? bcoEN : bcoDIS); sendCommand(buf);
    sprintf(buf, "A5.pco=%d", (proc_find("P25Gateway")>0) ? pcoEN : pcoDIS); sendCommand(buf);
    sprintf(buf, "A6.bco=%d", (proc_find("P25Parrot")>0) ? bcoEN : bcoDIS); sendCommand(buf);
    sprintf(buf, "A6.pco=%d", (proc_find("P25Parrot")>0) ? pcoEN : pcoDIS); sendCommand(buf);
    sprintf(buf, "A7.bco=%d", (proc_find("dstarrepeaterd")>0) ? bcoEN : bcoDIS); sendCommand(buf);
    sprintf(buf, "A7.pco=%d", (proc_find("dstarrepeaterd")>0) ? pcoEN : pcoDIS); sendCommand(buf);
    sprintf(buf, "A8.bco=%d", (proc_find("ircddbgatewayd")>0) ? bcoEN : bcoDIS); sendCommand(buf);
    sprintf(buf, "A8.pco=%d", (proc_find("ircddbgatewayd")>0) ? pcoEN : pcoDIS); sendCommand(buf);
    sprintf(buf, "A9.bco=%d", (proc_find("timeserverd")>0) ? bcoEN : bcoDIS); sendCommand(buf);
    sprintf(buf, "A9.pco=%d", (proc_find("timeserverd")>0) ? pcoEN : pcoDIS); sendCommand(buf);
    sprintf(buf, "A10.bco=%d", (proc_find("pistar-watchdog")>0) ? bcoEN : bcoDIS); sendCommand(buf);
    sprintf(buf, "A10.pco=%d", (proc_find("pistar-watchdog")>0) ? pcoEN : pcoDIS); sendCommand(buf);
    sprintf(buf, "A11.bco=%d", (proc_find("pistar-remote")>0) ? bcoEN : bcoDIS); sendCommand(buf);
    sprintf(buf, "A11.pco=%d", (proc_find("pistar-remote")>0) ? pcoEN : pcoDIS); sendCommand(buf);
    sprintf(buf, "A12.bco=%d", (proc_find("pistar-keeper")>0) ? bcoEN : bcoDIS); sendCommand(buf);
    sprintf(buf, "A12.pco=%d", (proc_find("pistar-keeper")>0) ? pcoEN : pcoDIS); sendCommand(buf);
    sprintf(buf, "A25.bco=%d", (proc_find("NXDNGateway")>0) ? bcoEN : bcoDIS); sendCommand(buf);
    sprintf(buf, "A25.pco=%d", (proc_find("NXDNGateway")>0) ? pcoEN : pcoDIS); sendCommand(buf);
    sprintf(buf, "A26.bco=%d", (proc_find("NXDNParrot")>0) ? bcoEN : bcoDIS); sendCommand(buf);
    sprintf(buf, "A26.pco=%d", (proc_find("NXDNParrot")>0) ? pcoEN : pcoDIS); sendCommand(buf);
    sprintf(buf, "A27.bco=%d", (proc_find("DAPNETGateway")>0) ? bcoEN : bcoDIS); sendCommand(buf);
    sprintf(buf, "A27.pco=%d", (proc_find("DAPNETGateway")>0) ? pcoEN : pcoDIS); sendCommand(buf);
}


void processButtons(unsigned char code, unsigned char param) {

char buf[300];

    //See the README file for how to define a button on the Nextion Display
    if (code==0xA0) { // A0 Show last heard list. Parameters: 01 for NET users, 02 for LOCal users
       sendLHlist(param);
    } else if (code==0xA1) { // A1 Refresh screens when changed in Nextion. Parameters: Number of screen to refresh 
       basicFunctions(param);
    } else if (code==0xA2) { // A2 System information sreen
       systemInformation();
    } else {
      sprintf(buf, "msg.txt=\"Button pressed %d (%s)\"", code, RXbuffer);
      sendCommand(buf);
    }
}


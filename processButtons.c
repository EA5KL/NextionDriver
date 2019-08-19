/*
 *   Copyright (C) 2017,2018 by Lieven De Samblanx ON7LDS
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
#include "NextionDriver.h"

//============================================================================
//
//   Put your code in the subroutine below
//
//============================================================================

void sendLHlist() {

    char text[1800];
	
       char pszResourcePath[]="/admin/mmdvmhost/lh_nextion.php";
       char pszHostAddress[]="pi-star:raspberry@localhost";

       sprintf(buf, "LHt1.txt=\"Button pressed (%s)\"", RXbuffer);
       sendCommand(text);
       sprintf(text, "GET /%s HTTP/1.1\r\nHost: %s\r\nContent-Type: text/plain\r\n\r\n", pszResourcePath, pszHostAddress);
       sendCommand(text);
       sendCommand("MMDVM.status.val=99");
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


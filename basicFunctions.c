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
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <syslog.h>

#include "NextionDriver.h"
#include "helpers.h"
#include "basicFunctions.h"

void basicFunctions() {

    char text[100];

    if (strlen(TXbuffer)==0) return;

    //---------------------------------------------------
    // the 'page' variable holds the last selected page
    //---------------------------------------------------
    if (strcmp(TXbuffer,"page MMDVM")==0) {
        page=0;
    }
    if (strcmp(TXbuffer,"page DStar")==0) {
        page=1;
    }
    if (strcmp(TXbuffer,"page DMR")==0) {
        page=2;
    }
    if (strcmp(TXbuffer,"page YSF")==0) {
        page=3;
    }
    if (strcmp(TXbuffer,"page P25")==0) {
        page=4;
    }
    if (strcmp(TXbuffer,"page NXDN")==0) {
        page=5;
    }
    if (strcmp(TXbuffer,"page POCSAG")==0) {
        page=6;
    }

    if ((strncmp(TXbuffer,"page ",5)==0)&&(changepages==1)) {
        strcat(TXbuffer,"0");
    }


    //--------------------------------------------------------------
    //                  Rest page
    //--------------------------------------------------------------
    //  * remove Freq and time when stopping
    //  * regularly check IP interface and send to display
    //  * get CPU temperature & frequency & load average 
    //       and send to display
    //  * send RX frequency and location (info from MMDVM.ini)
    //--------------------------------------------------------------

    //MMDVM is doing a clean shutdown.
    if ((page==0)&&(strstr(TXbuffer,"MMDVM STOPPED")>0)){
        sprintf(TXbuffer, "t30.txt=\"\"");
        sendCommand(text);
        sprintf(TXbuffer, "t2.txt=\"\"");
    }

    //remove dim if necessary
    if (removeDim) {
        if (strstr(TXbuffer,"dim=")!=NULL) {
            TXbuffer[0]=0;
            return;
        }
    }

    // if date/time is sent, check IP interface from time to time:
    //   and disk free in %
    if ((page==0)&&(strstr(TXbuffer,"t2.txt=")>0)&&(check++>100)) {
        getNetworkInterface(ipaddr);
        netIsActive[0]=getInternetStatus(check);
        sprintf(TXbuffer, "t3.txt=\"%s\"", ipaddr);
        check=0;
    }
    // check temp & CPU freq (also not too often)
    if ((page==0)&&(strstr(TXbuffer,"t2.txt=")>0)&&(check%8==0)) {
        FILE *deviceInfoFile;
        double val;
#ifdef XTRA
        //CPU temperature
        deviceInfoFile = fopen ("/sys/class/thermal/thermal_zone0/temp", "r");
        if (deviceInfoFile == NULL) {
            sprintf(text, "t20.txt=\"?\"");
        } else {
            fscanf (deviceInfoFile, "%lf", &val);
            val /= 1000;

            sprintf(text, "t20.txt=\"%2.2f %cC\"", val, 176);
            /*
            If you live in one of the 5 countries (Bahamas, Belize, Cayman Islands,
             Palau, US) where they use degrees F, you could comment the line above
             and uncomment the following line :                                      */
            //val=(val*1.8)+32;  sprintf(text, "t20.txt=\"%2.1f %cF\"", val, 176);

            fclose(deviceInfoFile);
        }
        sendCommand(text);
#endif

       //CPU frequency
        deviceInfoFile = fopen ("/sys/devices/system/cpu/cpu0/cpufreq/cpuinfo_cur_freq", "r");
        if (deviceInfoFile == NULL) {
            sprintf(text, "t21.txt=\"?\"");
        } else {
            fscanf (deviceInfoFile, "%lf", &val);
            val /= 1000;
            sprintf(text, "t21.txt=\"%0.0f MHz\"", val);
            fclose(deviceInfoFile);
        }
        sendCommand(text);
        //CPU loadavg 1 min
        deviceInfoFile = fopen ("/proc/loadavg", "r");
        if (deviceInfoFile == NULL) {
            sprintf(text, "t22.txt=\"?\"");
            sendCommand(text);
            sprintf(text, "cpuload.val=0");
        } else {
            fscanf (deviceInfoFile, "%lf", &val);
            sprintf(text, "t22.txt=\"%0.2f\"", val);
            sendCommand(text);
            sprintf(text, "cpuload.val=%d", (int)(val*100));
            fclose(deviceInfoFile);
        }
        sendCommand(text);

        //Disk free %
        sprintf(text, "t23.txt=\"%d\"",getDiskFree());
        sendCommand(text);

#ifdef XTRA
        //RXFrequency
        float fx;
        fx=RXfrequency;
        fx/=1000000;
        sprintf(text, "t30.txt=\"%3.4fMHz\"",fx);
        sendCommand(text);

        //TXFrequency
        fx=TXfrequency;
        fx/=1000000;
        sprintf(text, "t32.txt=\"%3.4fMHz\"",fx);
        sendCommand(text);

        //Location
        sprintf(text, "t31.txt=\"%s\"",location);
        sendCommand(text);
#endif

        //disable 25356 text 46486
        //enable  1472  text 0
        //error   55879

        #define bcoEN	1472
        #define bcoDIS	25356
        #define pcoEN	0
        #define pcoDIS	46486

        //Modes enabled/disabled
        sprintf(text, "A1.bco=%d",modeIsEnabled[C_DSTAR] ?  bcoEN : bcoDIS); sendCommand(text);
        sprintf(text, "A1.pco=%d",modeIsEnabled[C_DSTAR] ?  pcoEN : pcoDIS); sendCommand(text);
        sprintf(text, "A2.bco=%d",modeIsEnabled[C_DMR] ?  bcoEN : bcoDIS); sendCommand(text);
        sprintf(text, "A2.pco=%d",modeIsEnabled[C_DMR] ?  pcoEN : pcoDIS); sendCommand(text);
        sprintf(text, "A3.bco=%d",modeIsEnabled[C_YSF] ?  bcoEN : bcoDIS); sendCommand(text);
        sprintf(text, "A3.pco=%d",modeIsEnabled[C_YSF] ?  pcoEN : pcoDIS); sendCommand(text);
        sprintf(text, "A4.bco=%d",modeIsEnabled[C_P25] ?  bcoEN : bcoDIS); sendCommand(text);
        sprintf(text, "A4.pco=%d",modeIsEnabled[C_P25] ?  pcoEN : pcoDIS); sendCommand(text);
//        sprintf(text, "A5.bco=%d",modeIsEnabled[C_YSFDMR] ?  bcoEN : bcoDIS); sendCommand(text);
//        sprintf(text, "A5.pco=%d",modeIsEnabled[C_YSFDMR] ?  pcoEN : pcoDIS); sendCommand(text);
        sprintf(text, "A6.bco=%d",modeIsEnabled[C_NXDN] ?  bcoEN : bcoDIS); sendCommand(text);
        sprintf(text, "A6.pco=%d",modeIsEnabled[C_NXDN] ?  pcoEN : pcoDIS); sendCommand(text);

        //Internet
        sprintf(text, "N0.bco=%d",netIsActive[0] ?  bcoEN : bcoDIS); sendCommand(text);
        sprintf(text, "N0.pco=%d",netIsActive[0] ?  pcoEN : pcoDIS); sendCommand(text);

        //Network connections
        sprintf(text, "N1.bco=%d",(modeIsEnabled[C_DSTARNET]&&(proc_find("ircddbgatewayd")>0)) ?  bcoEN : bcoDIS); sendCommand(text);
        sprintf(text, "N1.pco=%d",(modeIsEnabled[C_DSTARNET]&&(proc_find("ircddbgatewayd")>0)) ?  pcoEN : pcoDIS); sendCommand(text);
        sprintf(text, "N2.bco=%d",(modeIsEnabled[C_DMRNET]&&(proc_find("MMDVMHost")>0)) ?  bcoEN : bcoDIS); sendCommand(text);
        sprintf(text, "N2.pco=%d",(modeIsEnabled[C_DMRNET]&&(proc_find("MMDVMHost")>0)) ?  pcoEN : pcoDIS); sendCommand(text);
//        sprintf(text, "N2.bco=%d",(modeIsEnabled[]&&(proc_find("DMRGateway")>0)) ?  bcoEN : bcoDIS); sendCommand(text);
//        sprintf(text, "N2.pco=%d",(modeIsEnabled[]&&(proc_find("DMRGateway")>0)) ?  pcoEN : pcoDIS); sendCommand(text);
        sprintf(text, "N3.bco=%d",(modeIsEnabled[C_YSFNET]&&(proc_find("YSFGateway")>0)) ?  bcoEN : bcoDIS); sendCommand(text);
        sprintf(text, "N3.pco=%d",(modeIsEnabled[C_YSFNET]&&(proc_find("YSFGateway")>0)) ?  pcoEN : pcoDIS); sendCommand(text);
        sprintf(text, "N4.bco=%d",(modeIsEnabled[C_P25NET]&&(proc_find("P25Gateway")>0)) ?  bcoEN : bcoDIS); sendCommand(text);
        sprintf(text, "N4.pco=%d",(modeIsEnabled[C_P25NET]&&(proc_find("P25Gateway")>0)) ?  pcoEN : pcoDIS); sendCommand(text);
//        sprintf(text, "N5.bco=%d",(modeIsEnabled[C_YSFDMRNET]&&(proc_find("")>0) ?  bcoEN : bcoDIS)); sendCommand(text);
//        sprintf(text, "N5.pco=%d",(modeIsEnabled[C_YSFDM_NET]&&(proc_find("")>0) ?  pcoEN : pcoDIS)); sendCommand(text);
        sprintf(text, "N6.bco=%d",(modeIsEnabled[C_NXDNNET]&&(proc_find("MMDVMHost")>0)) ?  bcoEN : bcoDIS); sendCommand(text);
        sprintf(text, "N6.pco=%d",(modeIsEnabled[C_NXDNNET]&&(proc_find("MMDVMHost")>0)) ?  pcoEN : pcoDIS); sendCommand(text);

        //Done
        sprintf(text, "MMDVM.status.val=20");
        sendCommand(text);
        sendCommand("click S0,1");

    }


    //send TG name if found
    if ((page==2)&&(strstr(TXbuffer,"t3.txt")!=NULL)) {
        char *TGname;
        int nr,TGindex;
        sendCommand(TXbuffer);
        if ((TXbuffer[8]>='0')&&(TXbuffer[8]<='9'))
            nr=atoi(&TXbuffer[8]);
        else
            nr=atoi(&TXbuffer[10]);
        TGindex=search_group(nr,groups,0,nmbr_groups-1);
        if (TGindex>=0) {
            TGname=groups[TGindex].name;
            sprintf(TXbuffer,"t8.txt=\"%s\"",TGname);
        } else if (TGindex<0) {
            //is it maybe a user private call ?
            TGindex=search_userID(nr,users,0,nmbr_users-1);
            if (TGindex>0) sprintf(TXbuffer,"t8.txt=\"Private %s\"",users[TGindex].data1);
        } else {
            sprintf(TXbuffer,"t8.txt=\"TG%d name not found\"",nr);
        }
        sendCommand(TXbuffer);
    }


    //send user data if found
    if ((page==2)&&(strstr(TXbuffer,"t2.txt")!=NULL)) {
        int nr,user;

        sendCommand(TXbuffer);

        nr=atoi(&TXbuffer[12]);
        if (nr>0) {
            user=search_userID(nr,users,0,nmbr_users-1);
        } else {
            TXbuffer[strlen(TXbuffer)-1]=' ';
            char* l=strchr(&TXbuffer[12], ' ');
            if (l!=NULL) l[0]=0;
            writelog(LOG_DEBUG,"Search for call [%s] \n",&TXbuffer[12]);
            user=search_userCALL(&TXbuffer[12],users,0,nmbr_users-1);
        }

        if (user>=0) {
            sprintf(TXbuffer,"t13.txt=\"%s\"",users[user].data1);
            sendCommand(TXbuffer);
            sprintf(TXbuffer,"t14.txt=\"%s\"",users[user].data2);
            sendCommand(TXbuffer);
            sprintf(TXbuffer,"t15.txt=\"%s\"",users[user].data3);
            sendCommand(TXbuffer);
            sprintf(TXbuffer,"t16.txt=\"%s\"",users[user].data4);
            sendCommand(TXbuffer);
            sprintf(TXbuffer,"t17.txt=\"%s\"",users[user].data5);
            sendCommand(TXbuffer);

        } else if (nr>0) {
            sprintf(TXbuffer,"t13.txt=\"DMRID %d\"",nr);
            sendCommand(TXbuffer);
            sprintf(TXbuffer,"t14.txt=\"Not found in\"");
            sendCommand(TXbuffer);
            sprintf(TXbuffer,"t15.txt=\"%s\"",usersFile);
            sendCommand(TXbuffer);
        }
        sprintf(text, "MMDVM.status.val=78");
        sendCommand(text);
        sendCommand("click S0,1");
    }


}
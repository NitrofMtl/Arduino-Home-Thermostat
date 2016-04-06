/*
  Copyright (c) 03/04/2016

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
*/

#include <SD.h>
#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <TimeLib.h>
#include <weeklyAlarm.h>
#include <DueTimer.h>
#include <RTD10k.h>
//#include <currentSwitch.h>  //not used in this controler
//#include <Bounce2.h> //not used in this controler
#include <uHTTP.h>
#include <ArduinoJson.h>
#include <snippets.h>
#include <IOctrl.h>
#define REQ_BUF_SZ   60   // size of buffer used to capture HTTP requests

#define vRef 3.3   //set reference voltage to 3.3 or 5.0V
#define Reso 12    //set resolution to 10 or 12 for arduino DUE
#define SDC_PIN 4 // pin for sd card
#define default_page "index.htm"  //set the default page to load on request

RTD10k RTDRead[10];  // start an instance of RTD10k
weeklyAlarm SPAlarm;
//currentSwitch workProof;
Backup bckup;
Snippets sn;

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF};
IPAddress ip(192, 168, 0, 110);
uHTTP *uHTTPserver = new uHTTP(80);
EthernetClient response;

EthernetUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets

char HTTP_req[REQ_BUF_SZ] = {0}; // buffered HTTP request stored as null terminated string
char req_index = 0;              // index into HTTP_req buffer

EthernetServer server(80);

//    Per channel declare section Struct array
const byte numChannel = 10;
RTDinChannels inChannelID[numChannel];
SSRoutput outChannelID[numChannel];

//global Regulator variables
byte K = 80; // set process gain 1 to 10
float vs = 0.5; // set sustain value in degres
float smm = 5; // treshold in pourcent
float calibOffset[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; //calibration of Shield rRef resistor

byte numAlarm = 10; // set the number of alarm
byte numSetpoint = 10; //set number of setpoint save in conjuction with outchannel

float alarmMem[10][10];  // vector 1 = numAlarm, vector 2 = setpoint


//-----------------------------------------------------------------

void setup() {
  Serial.begin(9600);
  setupSdCard();
  setupEthernet();
  setupTime();
  Timer3.attachInterrupt(regulator_inputs).setFrequency(0.1).start(); // read inputs every 10 sec
  Timer4.attachInterrupt(regulator_outputs).setFrequency(10).start(); //outputs regulator controler at 10 Hz
  RTDSetup();
  setupOutput();
  SPAlarm.weeklyAlarmInit();
  init_alarmMemory();
  restore(); //restoring data from sd card
  delay(1000);
}

//-----------------------------------------------------------
//-----------------------------------------------------------

void loop() {
//  printTime();
  webserv();
}

//-----------------------------------------------------------

void webserv() { //              uHTTP version
  char url[32];
  if ((response = uHTTPserver->available())) {

    if (uHTTPserver->uri("/") ) {
      if (uHTTPserver->method(uHTTP_METHOD_GET)) {
        strcpy(url, default_page); // if nothing requested, send default page
        uHTTPserver->send_headers(200, response);
        uHTTPserver->webFile_Post(url,  response); //send default page
      }
      else if (uHTTPserver->method(uHTTP_METHOD_OPTIONS)) uHTTPserver->send_headers(200, response);
    }
    else {
      if ( uHTTPserver->get_request("ajax_inputs", response)) writeJSONResponse(); // look up for a get request of key "ajax_inputs"
      else if ( uHTTPserver->get_request("ajax_alarms", response)) writeJSON_Alarm_Response();
      else if ( uHTTPserver->get_request("configs", response)) writeJSONConfigResponse();
      else if (uHTTPserver->put_request("channels", response)) parseJSONInputs(uHTTPserver->body());
      else if (uHTTPserver->put_request("alarms", response)) parseJSONalarms(uHTTPserver->body());
      else if (uHTTPserver->put_request("switch", response)) parseJSONswitch(uHTTPserver->body());
      else if (uHTTPserver->put_request("switchAlarms", response)) parseJSONswitchAlarms(uHTTPserver->body());
      else if (uHTTPserver->put_request("configs", response)) parseJSONConfigs(uHTTPserver->body());
      else
        strcpy(url, uHTTPserver->uri());
      uHTTPserver->webFile_Post(url,  response); //send other file if they exist on sd card

      // else uHTTPserver->send_headers(404, TEXT_HTML, response);
    }
    response.stop();
  }
}

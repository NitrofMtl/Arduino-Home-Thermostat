/*
  Copyright (c) 02/08/2018

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
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include "WebSocketsServer.h"
#include <TimeLib.h>
#include <weeklyAlarm.h>
//#include <DueTimer.h> //not used in this controler
#include <TimeOut.h>
//#include <currentSwitch.h>  //not used in this controler
//#include <Bounce2.h> //not used in this controler
#include <uHTTP.h>
#include <ArduinoJson.h>
#include <snippets.h>
#include <RTD10K.h>
#include <IOctrl.h>
#include <ADC_SEQR.h>
#include <Streaming.h>

#define REQ_BUF_SZ   60   // size of buffer used to capture HTTP requests

#define vRef 3.3   //set reference voltage to 3.3 or 5.0V
#define RESO 12    //set resolution to 10 or 12 for arduino DUE
#define SDC_PIN 4 // pin for sd card
#define default_page "index.htm"  //set the default page to load on request

//void watchdogSetup (void) {
  //watchdogEnable(4095); //enable watchdog to maximum time 0xFFF
//}


//currentSwitch workProof; not use in this controler
Backup bckup;
Snippets sn;

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF};
IPAddress ip(192, 168, 0, 110);
uHTTP *uHTTPserver = new uHTTP(80);
EthernetClient response;
EthernetUDP Udp;
unsigned int localPort = 8888;  // local port to listen for UDP packets
EthernetServer server(80);

const char* ntpHostName = "0.ca.pool.ntp.org";
#define syncInterval hr(24) //sync ntp time once a day
time_t runSince;
time_t lastNtpSync;

WebSocketsServer webSocket = WebSocketsServer(8181);
enum SubscriberPages { HOME, ALARMS, CONFIGS, SIZE_OF_ENUM };
bool subscribers   [WEBSOCKETS_SERVER_CLIENT_MAX][SIZE_OF_ENUM]  ;

Adc_Seqr adc;


RTD10k RTDRead(RESO); //library to read 10k RTD input
//    Per channel declare section
const byte numChannel = 10;
RTDinChannels inChannelID[numChannel]; //input channel obj
SSRoutput outChannelID[numChannel]; //ouput channel obj

//global Regulator variables
byte K = 80; // set process gain 1 to 10
float vs = 0.5; // set sustain value in degres
float smm = 5; // treshold in pourcent

//interval instance
Interval timerMainRegulator;
Interval timerWeeklyAlarm;
Interval timerSSROutput;
Interval timerWebSocket[WEBSOCKETS_SERVER_CLIENT_MAX];

//programable alarm section
const byte numAlarm = 10; // set the number of alarm
WeeklyAlarm SPAlarm(numAlarm);//initiane 10 alarm
float alarmMem[10][10];  // vector 1 = numAlarm, vector 2 = setpoint




//function prototype
void setupSdCard();
void setupEthernet();
void setupTime();
void setupWebSocket();
void regulator_inputs();
void regulator_outputs();
void checkWeeklyAlarm();
void RTDSetup();
void setupOutput();
void restore();

//-----------------------------------------------------------------
extern "C" char* sbrk(int incr);

int freeMemory() {
  char top;
  return &top - reinterpret_cast<char*>(sbrk(0));
}
//-----------------------------------------------------------------

void setup() {
  Serial.begin(115200);

  Serial.println("startup  !!!");
  setupSdCard();
  setupEthernet();
  setupWebSocket();
  delay(1000);//for stability, make time to internet sheild to set up
  //WDT_Restart (WDT);
  setupTime();
  //WDT_Restart (WDT);
  timerMainRegulator.interval(sc(10), regulator_inputs); // read inputs every 10 sec
  timerWeeklyAlarm.interval(mn(1), checkWeeklyAlarm); //check weekly alarm each minute
  RTDSetup();
  setupOutput();
  setupWeeklyAlarm();
  restore(); //restoring user data from sd card
  //WDT_Restart (WDT); //reset the watchdog timer
  regulator_inputs(); //read inputs a first time before loop start
  delay(200);//for stability
  //WDT_Restart (WDT); //reset the watchdog timer
  //Timer4.attachInterrupt(regulator_outputs).setFrequency(10).start(); //---->>> moved to interval methode: outputs regulator controler at 10 Hz
  timerSSROutput.interval(100, (regulator_outputs));  //outputs regulator controler at 10 Hz

  Serial.print ("Free memory is: ");
  Serial.println (freeMemory ());

}

//-----------------------------------------------------------
//-----------------------------------------------------------
void webServ();

void loop() {
  //printTime();
  webServ();
  webSocket.loop();
  Interval::handler();
  //WDT_Restart (WDT); //reset the watchdog timer
}

//-----------------------------------------------------------

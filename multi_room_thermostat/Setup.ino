//SETUP SD CARD
void setupSdCard() {
  Serial.print("Initializing SD card...");
  pinMode(SDC_PIN, OUTPUT);  //make sure that the default chip select pin is set to output
  digitalWrite(SDC_PIN, HIGH);
  delay(1);
  if (!SD.begin(SDC_PIN)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");
  delay(1);
  bckup.begin();
  delay(10);
}

//-----------------------------------------------------------
uHTTP_request getContainer[] = {    //resquest index : (ID, callback function)
  {"ajax_inputs", writeJSONResponse},
  {"ajax_alarms", writeJSON_Alarm_Response},
  {"configs", writeJSONConfigResponse}
};

uHTTP_request putContainer[] = {    //resquest index : (ID, callback function)
  {"channels", parseJSONInputs},
  {"alarms", parseJSONalarms},
  {"switch", parseJSONswitch},
  {"switchAlarms", parseJSONswitchAlarms},
  {"configs", parseJSONConfigs}
};


void setupEthernet() {
  Ethernet.begin(mac, ip);
  delay(500);
  Serial.print("Ip address; "); Serial.println(Ethernet.localIP());
  server.begin();
  uHTTPserver->begin();
  uHTTPserver->uHTTPclient(&response);   //link uHTTP EthernetClient with sketch EthernetClient
  uHTTPserver->addToContainer(GET, getContainer, SizeOfArray(getContainer)); //add get request to uhttp handler
  uHTTPserver->addToContainer(PUT, putContainer, SizeOfArray(putContainer)); //add put request to uhttp handler
  delay(1000);
}

//-----------------------------------------------------------

//IPAddress timeserver;
IPAddress hostByName(const char* hostName) {
  IPAddress aResult = static_cast<uint32_t>(0);
  if(aResult.fromString(hostName)) {
        // Host name is a IP address use it!
        return aResult;
    }
  DNSClient c;
  c.begin(Ethernet.dnsServerIP());
  c.getHostByName(hostName, aResult);
  return aResult;
}

bool setClock() {
  IPAddress ntpAddr = hostByName(ntpHostName);
  if (ntpAddr) {
    time_t getTime = getNtpTime(ntpAddr);
    if (getTime) {
      setTime(getTime);
      syncWithMinute();
      return true;
    }
  }
}

Interval clkSync;

void syncTime() {
  if (setClock) {
    lastNtpSync = now();
  }
}

void setupTime() {
  server.begin();
  Udp.begin(localPort);
  if (Serial.available() > 0); 
  Serial.println("waiting for sync");
  
  do {
    setClock();  
    //WDT_Restart (WDT);
  }while (!timeStatus());
  
  //printTime();
  runSince = now();
  lastNtpSync = now();
  digitalClockDisplay(now(), Serial);
  clkSync.interval(syncInterval, syncTime);
  //setSyncInterval(86400000);//sync clock once a day  
}

void syncWithMinute() {
  static int sec = -1;
  if ( -1 == sec ) sec = second(); //set sec in the first pass
  if ( second() == sec ) { //loop back function if second did not change
    TimeOut(1, syncWithMinute);
    return;
  } 
  int timeBeforeNewMinute = (60 - second()) * 1000; //millisecond before new minute
  TimeOut(timeBeforeNewMinute, starWeeklyAlarmHandler);
  sec = -1; //reset checker for the next sync
}

void starWeeklyAlarmHandler() {
  timerWeeklyAlarm.cancel();
  checkWeeklyAlarm();
  timerWeeklyAlarm.interval(mn(1), checkWeeklyAlarm); //check weekly alarm each minute
}



//-----------------------------------------------------------

void setupWebSocket() {
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  //initialize suscriber matrix
  for (int i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++) {
    for (int j = 0; j < SIZE_OF_ENUM; j++) {
      subscribers[i][j] = false;
    }
  }
}

//-----------------------------------------------------------
void RTDSetup() {
  //adc.begin(); //initiate sequencer, have to be before 'analogReadResolution' call to not reset it to 10
  analogReadResolution(RESO); //set arduino analog input to 12 bit reading
  /////         RTD inputs struct initializing
  //InChannels[id](name, pin, offset(optionnal))
  inChannelID[0].channelSet( "name0", A0 );
  inChannelID[1].channelSet( "name1", A1 );
  inChannelID[2].channelSet( "name2", A2 );
  inChannelID[3].channelSet( "name3", A3 );
  inChannelID[4].channelSet( "name4", A4 );
  inChannelID[5].channelSet( "name5", A5 );
  inChannelID[6].channelSet( "name6", A6 );
  inChannelID[7].channelSet( "name7", A7 );
  inChannelID[8].channelSet( "name8", A8 );
  inChannelID[9].channelSet( "name9", A9 );
  //start sampelr at 1Hz
  adc.begin(1,
    inChannelID[0].AinputPin,
    inChannelID[1].AinputPin,
    inChannelID[2].AinputPin,
    inChannelID[3].AinputPin,
    inChannelID[4].AinputPin,
    inChannelID[5].AinputPin,
    inChannelID[6].AinputPin,
    inChannelID[7].AinputPin,
    inChannelID[8].AinputPin,
    inChannelID[9].AinputPin
    );
  //adc.begin(1,0,1,2,3,4,5,6,7,8,9);
}

//-----------------------------------------------------------

void setupOutput() {

  /////        SSROutput initializing
  //  outChannels[id](ouputPin, Sp,treshold, ChSwitch)
  outChannelID[0].OutChannels(22, 5, smm, false);
  outChannelID[1].OutChannels(23, 5, smm, false);
  outChannelID[2].OutChannels(24, 5, smm, false);
  outChannelID[3].OutChannels(25, 5, smm, false);
  outChannelID[4].OutChannels(26, 5, smm, false);
  outChannelID[5].OutChannels(27, 5, smm, false);
  outChannelID[6].OutChannels(28, 5, smm, false);
  outChannelID[7].OutChannels(29, 5, smm, false);
  outChannelID[8].OutChannels(30, 5, smm, false);
  outChannelID[9].OutChannels(31, 5, smm, false);
}

//-----------------------------------------------------------

void setupWeeklyAlarm() {
  init_alarmMemory();
    //set alarm callback with int (callback, int)
    //set alarm: (type, almSwitch, Hour, Min)
    //AlarmType::SUNDAY, MONDAY, TUESDAY, WEDNESDAY, THURSDAY, FRIDAY, SATURSDAY, WEEK, WEEK_END, ALL_DAYS
  for (int i = 0; i < NUM_ALARM; i++) {
    SPAlarm[i].setCallback(setSp, i);
    SPAlarm[i].set(AlarmType::ALL_DAYS, OFF, 0, 0);
    weeklyAlarm.add(SPAlarm[i]);
  }
}

void init_alarmMemory() {   //initiane alarm memory matrix to 21C by default
  for (byte i = 0; i < NUM_ALARM; i++) {
    for (byte j = 0; j < numChannel; j++) {
      alarmMem[i][j] = 0;
    }
  }
}

void blink13() {
  static bool state = false;
  digitalWrite(13, state);
  state = !state;
}

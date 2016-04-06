//SETUP SD CARD
void setupSdCard() {
  Serial.println("Startup !!!");
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

void setupEthernet() {
  Ethernet.begin(mac, ip);
  Serial.print("Ip address; "); Serial.println(Ethernet.localIP());
  server.begin();
  uHTTPserver->begin();
}

//-----------------------------------------------------------

void setupTime() {
  server.begin();
  Udp.begin(localPort);
  if (Serial.available() > 0) ; {
    Serial.println("waiting for sync");
  }
//  setSyncProvider(getNtpTime);
    setClock(&Udp);  
  setSyncInterval(2592000);//sync clock once a mounth
}

//-----------------------------------------------------------

void RTDSetup() {
  analogReadResolution(Reso); //set arduino analog input to 12 bit reading
  for (int i = 0; i <= 9; i++) {
    RTDRead[i].init(3.3, 12); //initialize RTD library, set the reference input voltage 3.3V or 5V, arduino DUE only for 12 bytes resolution, 10 for other board
    RTDRead[i].calibrateRref(i, calibOffset[i]); //calibrate shield Reference resistor, (analog in, calibration value find with calibration sketch)
  }
  /////         RTD inputs struct initializing
  //InChannels[id](name, pin, switchCh, offset)
  inChannelID[0].channelSet( "name0", A0, true, 0);
  inChannelID[1].channelSet( "name1", A1, true, 0);
  inChannelID[2].channelSet( "name2", A2, false, 0);
  inChannelID[3].channelSet( "name3", A3, false, 0);
  inChannelID[4].channelSet( "name4", A4, true, 0);
  inChannelID[5].channelSet( "name5", A5, true, 0);
  inChannelID[6].channelSet( "name6", A6, true, 0);
  inChannelID[7].channelSet( "name7", A7, true, 0);
  inChannelID[8].channelSet( "name8", A8, true, 0);
  inChannelID[9].channelSet( "name9", A9, true, 0);
}

//-----------------------------------------------------------

void setupOutput() {

  /////        SSROutput initializing
  //  outChannels[id](ouputPin, Sp,treshold, ChSwitch)
  outChannelID[0].OutChannels(22, 5, smm, true);
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

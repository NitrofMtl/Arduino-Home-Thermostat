void webServ() { //run in void loop
  uHTTPserver->requestHandler();
}

//-----------------------------------------------------------
JsonObject& JSONInputs(JsonBuffer& jsonBuffer) {

  JsonObject& channels = jsonBuffer.createObject();
  JsonArray& chan = channels.createNestedArray("channels");

  for (uint8_t i = 0; i < 10; i++) {
    JsonObject& chan_in = jsonBuffer.createObject();
    chan_in["name"] = inChannelID[i].channelName;
    chan_in["canal"] = i;
    chan_in["status"] = onOffBool(outChannelID[i].channelSwitch);
    chan_in["temperature"] = roundToDigit(inChannelID[i].Ainput, 1);
    chan_in["setpoint"] = outChannelID[i].sp;
    chan_in["permission"] = outChannelID[i].permRun;
    chan_in["percentOut"] = outChannelID[i].Aoutput;
    chan.add(chan_in);
  }
  return channels;
}

//-----------------------------------------------------------

void writeJSONResponse() {
  StaticJsonBuffer<3000> jsonBuffer;
  JsonObject& json = JSONInputs(jsonBuffer);
  uHTTPserver->send_JSON_headers();
  json.printTo(response);
}

//-----------------------------------------------------------

void parseJSONInputs() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(uHTTPserver->body());

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  //root.prettyPrintTo(Serial); Serial.println("inputs");  // for debug
  byte  chanId = root["channels"]["canal"];
  strcpy(inChannelID[chanId].channelName, root["channels"]["name"]);
  outChannelID[chanId].sp = root["channels"]["setpoint"];
  backup();
}

void parseJSONswitch() {
  StaticJsonBuffer<255> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(uHTTPserver->body());
  //root.prettyPrintTo(Serial);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  //root.prettyPrintTo(Serial); Serial.println("switch");
  uint8_t  chanId = root["switchCh"]["canal"];
  outChannelID[chanId].channelSwitch = !outChannelID[chanId].channelSwitch;
  //if (outChannelID[chanId].channelSwitch) Serial.println("true"); else  Serial.println("false");

  backup();
}

//-----------------------------------------------------------
// -------------   JSON alarms -----------------------------

void parseJSONswitchAlarms() {
  StaticJsonBuffer<255> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(uHTTPserver->body());

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  //SPAlarm.toggle(root["switchAlarm"]["switchAlarm"]);    //toggle alarm switch
  int alm = root["switchAlarm"]["switchAlarm"];
  SPAlarm[alm].toggle(); //toggle alarm switch
  backup();
}

//-----------------------------------------------------------

JsonObject& JSONalarm(JsonBuffer& jsonBuffer) {
  JsonObject& alarm = jsonBuffer.createObject();
  JsonArray& chName = alarm.createNestedArray("chName");
  for ( byte i = 0; i < numChannel; i++) {
    chName.add(inChannelID[i].channelName);
  }
  JsonArray& alarmID = alarm.createNestedArray("alarms");


  for (byte i = 0; i < NUM_ALARM; i++) {
    JsonObject& alarm_in = SPAlarm[i].getJSON(jsonBuffer);
    JsonArray& setpoint = alarm_in.createNestedArray("setpoints");
    for ( byte j = 0; j < numChannel; j++) {
      float setPointAl = alarmMem[i][j];
      if ( 5 > setPointAl) {
        setpoint.add(RawJson("null"));
      }
      else setpoint.add(double_with_n_digits(setPointAl, 1));
    }
    alarmID.add(alarm_in);
  }
  return alarm;
}

void writeJSON_Alarm_Response() { //alarm json sender
  StaticJsonBuffer<5500> jsonBuffer;
  JsonObject& json = JSONalarm(jsonBuffer);
  uHTTPserver->send_JSON_headers();
  json.printTo(response);
}

//-----------------------------------------------------------

void parseJSONalarms() {
  //Serial.println("parse alarm request");
  StaticJsonBuffer<1023> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(uHTTPserver->body());

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  //root.prettyPrintTo(Serial);
  int8_t alarmID = root["alarms"]["index"];
  JsonObject& alarm = root["alarms"]["data"];
  SPAlarm[alarmID].parseJSON(alarm);
  JsonArray& setpoints = root["alarms"]["data"]["setpoints"];
  byte i = 0;
  for (JsonArray::iterator it = setpoints.begin(); it != setpoints.end(); ++it) {
    if (!*it) alarmMem[alarmID][i] = 0; //handle arduinoJson 'NaN'
    else alarmMem[alarmID][i] = *it;
    i++;
  }
  backup();
}

//-----------------------------------------------------------
//-----------------------------------------------------------

JsonObject& JSONconfigs__deprecated(JsonBuffer& jsonBuffer) { //<<<<<<--------------------to be deprecated
  JsonObject& configs = jsonBuffer.createObject();
  JsonArray& config = configs.createNestedArray("configs");
  JsonObject& config_in = jsonBuffer.createObject();
  config_in["K"] = K;
  config_in["vs"] = vs;
  config_in["smm"] = smm;
  config.add(config_in);
  JsonArray& chan = config_in.createNestedArray("inputs");

  for (byte i = 0; i < numChannel; i++) {
    JsonObject& chan_in = jsonBuffer.createObject();
    chan_in["offset"] = inChannelID[i].offset;
    chan_in["canal"] = String(i);
    chan_in["temperature"] = inChannelID[i].Ainput;
    chan.add(chan_in);
  }
  configs["systemTime"] = systemTime(jsonBuffer);
  //configs.prettyPrintTo(Serial);
  return configs;
}

JsonObject& JSONconfigs__Corrected(JsonBuffer& jsonBuffer) {
  JsonObject& configs = jsonBuffer.createObject();
  configs["K"] = K;
  configs["vs"] = vs;
  configs["smm"] = smm;

  JsonArray& chan = configs.createNestedArray("inputs");
  for (byte i = 0; i < numChannel; i++) {
    JsonObject& chan_in = jsonBuffer.createObject();
    chan_in["offset"] = inChannelID[i].offset;
    chan_in["canal"] = String(i);
    chan_in["temperature"] = roundToDigit(inChannelID[i].Ainput, 2);//inChannelID[i].Ainput;
    chan.add(chan_in);
  }
  configs["systemTime"] = systemTime(jsonBuffer);

  //configs.prettyPrintTo(Serial);
  return configs;
}

JsonObject& systemTime(JsonBuffer& jsonBuffer) {
  JsonObject& time = jsonBuffer.createObject();
  time["now"] = now();
  time["runSince"] = runSince;
  time["lastSync"] = lastNtpSync;
  return time;
}

void writeJSONConfigResponse() {
  inputRead(); // update all input reading more often for calibration
  StaticJsonBuffer<2000> jsonBuffer;
  JsonObject& json = JSONconfigs__deprecated(jsonBuffer); //<<<------ to be deprecated
  uHTTPserver->send_JSON_headers();
  json.printTo(response);
}

//-----------------------------------------------------------
//-----------------------------------------------------------

void parseJSONConfigs() {
  StaticJsonBuffer<2000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(uHTTPserver->body());

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  // root.prettyPrintTo(Serial);
  K = root["config"]["K"];
  vs = root["config"]["vs"];
  smm = root["config"]["smm"];
  byte id = 0;
  JsonArray& inputs = root["config"]["inputs"];
  for (JsonArray::iterator it = inputs.begin(); it != inputs.end(); ++it) {
    JsonObject& input = *it;
    id = input["canal"];
    inChannelID[id].offset = input["offset"];
  }

  backup();
}


double roundToDigit(double number, byte digit) {
  number = number * pow(10, digit);
  number = round(number);
  return number / pow(10, digit);
}

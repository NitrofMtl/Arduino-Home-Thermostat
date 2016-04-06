////--------------- JSON functions

JsonObject& JSONInputs(JsonBuffer& jsonBuffer) {

  JsonObject& channels = jsonBuffer.createObject();
  JsonArray& chan = channels.createNestedArray("channels");

  for (byte i = 0; i < 10; i++) {
    JsonObject& chan_in = jsonBuffer.createObject();
    chan_in["name"] = inChannelID[i].channelName;
    chan_in["canal"] = String(i);
    chan_in["status"] = onOffBool(outChannelID[i].channelSwitch);
    chan_in["temperature"] = inChannelID[i].Ainput;
    chan_in["setPoint"] = outChannelID[i].sp;
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
  uHTTPserver->send_JSON_headers(response);
  json.prettyPrintTo(response);
}

//-----------------------------------------------------------

void parseJSONInputs(const char json[]) {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(const_cast<char*>(json));

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  //root.prettyPrintTo(Serial); Serial.println("inputs");  // for debug
  byte  chanId = root["channels"]["canal"];
  strcpy(inChannelID[chanId].channelName, root["channels"]["name"]);
  outChannelID[chanId].sp = root["channels"]["setPoint"];
  backup();
}

void parseJSONswitch(const char json[]) {
  StaticJsonBuffer<255> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  root.prettyPrintTo(Serial); Serial.println("switch");
  byte  chanId = root["switchCh"]["canal"];
  outChannelID[chanId].channelSwitch = !outChannelID[chanId].channelSwitch;
  //Serial.println(chanId);
  if (outChannelID[chanId].channelSwitch) Serial.println("true"); else  Serial.println("false");
  backup();
}

//-----------------------------------------------------------
// -------------   JSON alarms -----------------------------

void parseJSONswitchAlarms(const char json[]) {
  StaticJsonBuffer<255> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(json);

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  SPAlarm.weeklyAlarmSwitch(root["switchAlarm"]["switchAlarm"]);//toggle alarm switch
}

//-----------------------------------------------------------
String weekType[11] {"null", "sunday", "monday", "tuesday", "wednesday", "thursday", "Friday", "saturday", "Week", "Week end", "All days" };

byte typeIdent(String input) {
  int output = 0;
  for (int i = 0; input != weekType[i]; i++) {
    output = i + 1;
  }
  return output;
}

JsonObject& JSONalarm(JsonBuffer& jsonBuffer) {
  JsonObject& alarm = jsonBuffer.createObject();
  JsonArray& alarmID = alarm.createNestedArray("alarms");


  for (byte i = 0; i < 10; i++) {
    String OnOff = intToOnOff(SPAlarm.getParam(i, 0)); //get alrm param,1= id, 2 = param 0-3[switch,type,hour,minute]
    String weekDay = weekType[SPAlarm.getParam(i, 1)];
    byte hourAl = SPAlarm.getParam(i, 2);
    byte minAl = SPAlarm.getParam(i, 3);
    JsonObject& alarm_in = jsonBuffer.createObject();
    alarm_in["switch"] = OnOff;
    alarm_in["type"] = weekDay;
    alarm_in["hour"] = hourAl;
    alarm_in["minute"] = minAl;
    JsonArray& setpoint = alarm_in.createNestedArray("setpoints");
    for ( byte j = 0; j < numSetpoint; j++) {
      float setPointAl = alarmMem[i][j];
      setpoint.add(double_with_n_digits(setPointAl, 1));
    }
    alarmID.add(alarm_in);
  }
  return alarm;
}

void writeJSON_Alarm_Response() { //alarm json sender
  StaticJsonBuffer<5500> jsonBuffer;
  JsonObject& json = JSONalarm(jsonBuffer);
  uHTTPserver->send_JSON_headers(response);
  json.prettyPrintTo(response);
}

//-----------------------------------------------------------

void parseJSONalarms(const char json[]) {
  StaticJsonBuffer<1023> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(const_cast<char*>(json));

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }

  byte alarmID = root["alarms"]["index"];
  byte weekType = typeIdent(toString(root["alarms"]["data"]["type"]));
  byte almrSwitch = onOffToInt(root["alarms"]["data"]["switch"]);
  byte alrmHour = root["alarms"]["data"]["hour"];
  byte alrmMin = root["alarms"]["data"]["minute"];
  SPAlarm.setWeeklyAlarm(alarmID, weekType, 1, alrmHour, alrmMin); //alarm tag, type alarm, alarm switch, hour, minute

  JsonArray& setpoints = root["alarms"]["data"]["setpoints"];
  byte i = 0;
  for (JsonArray::iterator it = setpoints.begin(); it != setpoints.end(); ++it) {
    alarmMem[alarmID][i] = *it;
    i++;
  }
backup();
}

//-----------------------------------------------------------
//-----------------------------------------------------------

JsonObject& JSONconfigs(JsonBuffer& jsonBuffer) {
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
    chan.add(chan_in);
  }

  // configs.prettyPrintTo(Serial);
  return configs;
}

void writeJSONConfigResponse() {
  StaticJsonBuffer<2000> jsonBuffer;
  JsonObject& json = JSONconfigs(jsonBuffer);
  uHTTPserver->send_JSON_headers(response);
  json.prettyPrintTo(response);
}

//-----------------------------------------------------------
//-----------------------------------------------------------

void parseJSONConfigs(const char json[]) {
  StaticJsonBuffer<2000> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(const_cast<char*>(json));

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
  RTDSetup(); //apply change to library obbject
  setupOutput();  //apply change to library obbject
  backup();
}

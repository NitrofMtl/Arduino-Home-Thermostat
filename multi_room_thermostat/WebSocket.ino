void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial << num << " Disconnected!" << endl;
      cleanupDisconnected(num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial << num << " Connected: " << ip << " " << (char*)payload << endl;
        // send message to client
        //##webSocket.sendTXT(num, "Connected");
      }
      break;
    case WStype_TEXT:
      Serial << num << " get Text: " << (char*)payload << endl;
      parseMessage((char*)payload, num);

      // send message to client
      //##webSocket.sendTXT(num, "message here");

      // send data to all connected clients
      //##webSocket.broadcastTXT("broadcast message here");
      break;
    case WStype_BIN:
      Serial << num << " get binary lenght: ";

      // send message to client
      webSocket.sendBIN(num, payload, lenght);
      break;
  }
}

void parseMessage(char* message, int num) {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.parseObject(message);

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return;
  }
  //root.prettyPrintTo(Serial);  // for debug
  wsRequestHandler(root, num);

}

struct WsContainer {
  char* id;
  SubscriberPages subscriberPages;
  void (*callback)();
  int intervalTime;
};

WsContainer wsGetContainer[] = {    //resquest index : (ID, callback function)
  {"ajax_inputs", HOME, wsSubscribeDataHome, 10000},
  {"ajax_alarms", ALARMS, writeJSON_Alarm_Response, 0},
  {"configs", CONFIGS, wsSendDataConfigs, 250}
};
#define SWSGC SizeOfArray(wsGetContainer) //Size of websocket get container

struct Object {
  const char* id;
  void (*parse)(JsonObject& data);
};

Object wsPutContainer[] = {
  { "setpointPut", wsParseHomePutSetpoint  },
  { "homeSwitch", wsParseHomeSwitch},
  { "offsetPut", parseConfigsPutRequest}
};
#define SWSPC SizeOfArray(wsPutContainer) //Size of websocket put container*/

bool noSubscriber(bool subscribed[]) {
  for (int i = 0; i < sizeof(subscribed); i++) {
    if (subscribed[i]) return false;
  }
}

enum class wsRequestType { GET, PUT, SUBSCRIBE, UNSUBSCRIBE, CONSOLE_LOG };

void wsRequestHandler(JsonObject& root, int num) {
  wsRequestType requestT;
  if (strncmp(root["request"], "get", 3) == 0 ) requestT = wsRequestType::GET;
  else if (strncmp(root["request"], "put", 3) == 0 ) requestT = wsRequestType::PUT;
  else if (strncmp(root["request"], "sub", 3) == 0 ) requestT = wsRequestType::SUBSCRIBE;
  else if (strncmp(root["request"], "uns", 3) == 0 ) requestT = wsRequestType::UNSUBSCRIBE;
  else if (strncmp(root["request"], "con", 3) == 0 ) requestT = wsRequestType::CONSOLE_LOG;

  switch (requestT) {
    case wsRequestType::GET:
      Serial.println("receive get request !!!");
      //root["data"] to do parse get data
      for (int i = 0; i < SWSGC; i++) { //i for request index
        if ( strcmp(root["id"], wsGetContainer[i].id) == 0 ) {
          wsGetContainer[i].callback();
        }
      }
      break;
    case wsRequestType::PUT:
      Serial.println("receive put request !!!");
      for (int i = 0; i < SWSPC; i++) { //i for request index
        if ( strcmp(root["id"], wsPutContainer[i].id) == 0 ) {
          Serial << "found put request: " << wsPutContainer[i].id << endl;
          root["data"].printTo(Serial); Serial << endl;
          wsPutContainer[i].parse(root["data"]);
          backup(); //store any change data
        }
      }
      break;
    case wsRequestType::SUBSCRIBE:
      for (int i = 0; i < SWSGC; i++) { //i for request index
        if ( strcmp(root["id"], wsGetContainer[i].id) == 0 ) {
          //start sending data to client
          Serial.println("get subscribtion");
          subscribers[i][num] = true;
          timerWebSocket[i].interval(wsGetContainer[i].intervalTime, wsGetContainer[i].callback);
          wsGetContainer[i].callback(); // do callback once to update data immediately
        }
      }
      break;
    case wsRequestType::UNSUBSCRIBE:
      for (int i = 0; i < SWSGC; i++) { //i for request index
        if (subscribers[i][num]) Serial << ("unSubscribtion: Socket #") << num << endl;
        subscribers[i][num] = false;
        if (noSubscriber(subscribers[i])) timerWebSocket[i].cancel();
      }
      break;
    case wsRequestType::CONSOLE_LOG:
      char id[20]  { root["id"] };
      char log[200] { root["data"] };
      Serial << "WS console message From socket #" << num << "] id:[" << id << "][  " << log << " ]" << endl;
      break;
  }
}

void cleanupDisconnected(int num) {
  Serial.println("clean disconnect");
  for (int i = 0; i < SWSGC; i++) { //i for request index
    if ( subscribers[i][num] ) {
      Serial.println("found subscribed");
      subscribers[i][num] = false;//unsuscribe to any get request for disconnected client
      if (noSubscriber(subscribers[i])) {
        timerWebSocket[i].cancel();
        Serial.println("clean interval");
      }
    }
  }
}

//-----------------------------------------------------------

char* sendDataHome() {
  StaticJsonBuffer<3000> jsonBuffer;
  JsonObject& json = JSONInputs(jsonBuffer);
  char message[3000];
  json.printTo(message);
  return message;
}

void wsSubscribeDataHome() {
  for (int i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++) {
    if (subscribers[HOME][i]) {
      webSocket.sendTXT(i, sendDataHome());
    }
  }
}

void wsParseHomeSwitch(JsonObject& data) {
  uint8_t  chanId = data["canal"];
  outChannelID[chanId].channelSwitch = !outChannelID[chanId].channelSwitch;
  //if (outChannelID[chanId].channelSwitch) Serial.println("true"); else  Serial.println("false");
  wsSubscribeDataHome();//update live data to devices
}

void wsParseHomePutSetpoint(JsonObject& data) {
  //data.prettyPrintTo(Serial);  // for debug
  byte  chanId = data["canal"];
  strcpy(inChannelID[chanId].channelName, data["name"]);
  outChannelID[chanId].sp = data["setpoint"];
  wsSubscribeDataHome();//update live data to devices
}

//-----------------------------------------------------------


void wsSendDataConfigs() {
  inputRead();//refresh analog readings
  StaticJsonBuffer<1000> jsonBuffer;
  JsonObject& json = jsonBuffer.createObject();
  json["id"] = "configs";

  //create data:
  StaticJsonBuffer<1000> dataBuffer;
  JsonObject& data = JSONconfigs__Corrected(dataBuffer);

  json["request"] = "response";
  json["data"] = data;
  char message[1000];
  json.printTo(message);
  //Serial << message << endl;

  for (int i = 0; i < WEBSOCKETS_SERVER_CLIENT_MAX; i++) {
    if (subscribers[CONFIGS][i]) {
      webSocket.sendTXT(i, message);// from test  webSocket.sendTXT(i,testMessage);
    }
  }
}

void parseConfigsPutRequest(JsonObject& data) {
  //data.printTo(Serial);
  int canal = data["canal"];
  int offset = data["offset"];
  inChannelID[canal].offset = offset;
}

//-----------------------------------------------------------

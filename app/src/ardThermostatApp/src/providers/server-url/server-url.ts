import { Injectable } from '@angular/core';
import { NativeStorage } from '@ionic-native/native-storage';
import { Platform } from 'ionic-angular';
import { ServerURL } from '../../pages/configs/server';

/*
  Generated class for the ServerUrlProvider provider.

  See https://angular.io/guide/dependency-injection for more info on providers
  and Angular DI.
*/
@Injectable()
export class ServerUrlProvider {
  server = <ServerURL>{
    URL: "Not set",
    localURL: "Not set",
    remoteURL: "Not set",
    remotePort: "0000",
    useRemote: false,
    websocketPort: "0000"
  }

  winStorage = window.localStorage;

  constructor(private nativeStorage: NativeStorage, private platform: Platform) {
    console.log('Hello ServerUrlProvider Provider');
  }

  url(): string {
    return this.server.URL;
    //return 'http://192.168.0.110';   ///keep for internal debugging

  } 

  webSocketUrl(): string {
    if (!this.server.useRemote) return "ws://" + this.server.localURL + ":" + this.server.websocketPort;
    else return "ws://" + this.server.remoteURL + ":" + this.server.websocketPort;
  }

  putServer(): void {
    if (!this.server.useRemote) {
      this.server.URL = 'http://' + this.server.localURL;
    }
    else {
      this.server.URL = ('http://' + this.server.remoteURL + ':' + this.server.remotePort);
    }
    if (this.platform.is('core')) {
      //console.log("id windows platform");
      this.winStorage.setItem('serverData', JSON.stringify(this.server));
    } else {
      this.nativeStorage.setItem('serverData', this.server);
    }
  }

  getServer() {
    return new Promise((res, err) => {
      setTimeout(() => {
        if (this.platform.is('core')  || this.platform.is('mobileweb')){
          //console.log("id windows platform");
          let data = JSON.parse(this.winStorage.getItem('serverData'));
            if (data) this.server = data;
        } else {
        this.nativeStorage.getItem('serverData')
          .then(
          data => {
            this.server = data;
            res();
          },
          err => console.error('get server data from native storage fail'+ err))
        }
      }, 0);
    });
  }

  reset(): void {
    this.server.URL = "Not set";
    this.server.localURL = "Not set";
    this.server.remoteURL = "Not set";
    this.server.remotePort = "0000";
    this.server.useRemote = false;
    this.server.websocketPort = "0000";
    this.putServer();
  }

}

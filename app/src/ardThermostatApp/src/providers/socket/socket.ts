import { Injectable, Inject } from '@angular/core';
import { Subject, Observable } from 'rxjs/Rx';
import { WebsocketProvider } from '../websocket/websocket';

export interface Message {
  id: string;
  request: string;
  data?: Object;
}

@Injectable()
export class SocketProvider {

  public message: Subject<Message>;

  constructor(private socket: WebsocketProvider, @Inject('server') private server) {
    //console.log('Hello SocketProvider Provider');
  }

  connect() {
    this.socket = new WebsocketProvider;
    let wsAddr = this.server.webSocketUrl();
    //let wsAddr = 'ws://192.168.0.110:8181';
    this.message = <Subject<Message>>this.socket
    //.connect('wss://echo.websocket.org')
    .connect(wsAddr)
    .catch((err: any)=>{
      console.error("unable to connect to websocket error: " + err.statusText);
      return Observable.throw(err.statusText);
    })
    .map((response: MessageEvent): Message => {
      try {
        let data = JSON.parse(response.data);
        return data;
      }catch(e) {
        alert(e);
        console.log("receive invalid JSON message: ");
        console.log(response.data);
      }
    })
  }

}

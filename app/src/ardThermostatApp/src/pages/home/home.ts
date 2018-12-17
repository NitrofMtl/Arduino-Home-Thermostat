import { Component, Inject } from '@angular/core';
import { IonicPage, NavController } from 'ionic-angular';

import 'rxjs/add/operator/map';
import { Subscription } from 'rxjs/Subscription';
import Rx from 'rxjs/Rx';
import { ScreenOrientation } from "@ionic-native/screen-orientation";
import { Platform } from 'ionic-angular';

import { Channel } from '../home/channel';
import { HomeDataProvider } from '../../providers/home-provider/home';

import { SocketProvider } from '../../providers/socket/socket';


@IonicPage()
@Component({
  selector: 'page-home',
  templateUrl: 'home.html',
  providers: [HomeDataProvider],
})
export class HomePage {
  channels: Channel[];
  thisPage: HomePage = this;
  interval: any;
  subscribtion: Subscription;
  activePage: boolean = false;

  private screenOrientation: ScreenOrientation;

  constructor(public nav: NavController, screenOrientation: ScreenOrientation, private socket: SocketProvider, private platform: Platform, @Inject('server') private server) {
    console.log("Hello home page");
    document.addEventListener('pause', () => {
      if(this.activePage) {
        let server = this.server.webSocketUrl();
        if (server !== "ws://Not set:0000") {
          this.socket.message.next(this.quitMessage);
          this.subscribtion.unsubscribe();
        }
      }      
    });

    this.platform.ready().then(() => {
      platform.resume.subscribe( ()=>{
        if(this.activePage) setTimeout( () => this.updateData(),250);
      });      
    });    

    this.screenOrientation = screenOrientation;
  }

  sliderOptions = { pager: true, autoHeight: true }

  ionViewDidEnter() {
    this.checkOrientation();
    this.screenOrientation.onChange().subscribe(
      value => this.checkOrientation(),
      error => alert('Changed error: ' + error)
    );
    this.activePage = true;
    this.platform.ready().then(() => {
      setTimeout( () => this.updateData(),250);
    });      
  }

  /*ionViewWillLeave() { ////version http
    this.subscription.unsubscribe();
  }*/

  ionViewWillLeave() { //version websocket
    let server = this.server.webSocketUrl();
    if (server !== "ws://Not set:0000") {
      this.socket.message.next(this.quitMessage);
      this.subscribtion.unsubscribe();
    }
    this.activePage = false;
  }

  screenVertical: boolean = false;

  checkOrientation(): void{
    if(this.screenOrientation.type === 'portrait-primary' ) {
      this.screenVertical = true;
    }
    else {
      this.screenVertical = false;
    }
  }

/*
  updateData() { //version http
    this.homedata.getData().subscribe(data => {
      this.channels = data["channels"];
    },
  (error:any) => {
    console.error("Server connection error, go to server configs page");
    this.nav.push(ConfigsServerPage);
  })
    const interval = Rx.Observable.interval(0).flatMap(() => this.homedata.getData());
    this.subscription = interval.subscribe(data => { this.channels = data["channels"]; });
  }*/



  updateData() { //version websocket   
      this.socket.connect();
        const interval = Rx.Observable.timer(200).flatMap(() => this.socket.message);
        this.subscribtion = interval.subscribe(data => {
          //console.log(data);
          this.channels = data["channels"]; 
        });
        this.socket.message.error
        setTimeout(() => {
          this.socket.message.next(this.enterMessage);
        },500);   
  }

  private enterMessage = { 
    id: 'ajax_inputs',
    request: 'subscribe'
  }

  private quitMessage = {
    id: 'ajax_inputs',
    request: 'unsubscribe'
  }

  /*switchChannel(cb: Channel) { //version http
    this.homedata.switchChannel(cb);
  }*/

  switchChannel(cb: Channel) { //version websocket
    let switchCh = {
      canal: cb.canal
    } 
  
    let postMessage = {
      id: "homeSwitch",
      request: "put",
      data: switchCh 
    }
  
    this.socket.message.next(postMessage);
  }

  putSetpoint(cb: Channel) { //version websocket
    let postMessage = {
      id: "setpointPut",
      request: "put",
      data: cb
    }
    this.socket.message.next(postMessage);
  }

}


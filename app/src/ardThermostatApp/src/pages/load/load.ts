import { Component, Inject } from '@angular/core';
import { IonicPage, NavController, NavParams } from 'ionic-angular';
import { TabsPage } from '../tabs/tabs';
import { ConfigsServerPage } from '../configs-server/configs-server';

/**
 * Generated class for the LoadPage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */

@IonicPage()
@Component({
  selector: 'page-load',
  templateUrl: 'load.html',
})
export class LoadPage {

  constructor(public navCtrl: NavController, public navParams: NavParams, @Inject('server') private server) {
  }
  statusMessage: String = "";

  ionViewDidEnter() {
    console.log('ionViewDidLoad LoadPage');
    this.statusMessage = "Getting adrresse from storage";
    //console.log(this.statusMessage);
    this.server.getServer().then(()=>{
      this.statusMessage = "Got data. Check for server response";
      this.server.checkArduinoAddr().then(()=>{
        this.statusMessage = "Got response: Now checking for websocket.";
        this.testWebsocket().then(() => {
          this.statusMessage = "Got response: Now go to Home page";
          setTimeout(()=> this.navCtrl.push(TabsPage),2000);
        }).catch(()=>{
          this.statusMessage = "Websocket connection fail: Now go to configs page";
          this.navCtrl.push(ConfigsServerPage);
        });
      }).catch(()=>{
        console.log("unable to connect: Now go to configs page ");
        this.navCtrl.push(ConfigsServerPage);
      })
    }).catch(()=>{
      console.log("unable to get adresse for storage: Now go to configs page");
      this.navCtrl.push(ConfigsServerPage);
    });
  }

  websocketStatus: boolean = false;

  testWebsocket() {
    return new Promise((res, err) => {
      setTimeout(() => {
        let ws = new WebSocket(this.server.webSocketUrl());
        ws.onopen = (evt) => {
          res();
          ws.close();
        }
        ws.onerror = (evt) => err();       
      },0);
    });
  }

}

import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams } from 'ionic-angular';
import { Subscription } from 'rxjs/Subscription';
import { Platform } from 'ionic-angular';

import { SocketProvider } from '../../providers/socket/socket';
import { Configs, InputConfigs } from '../../pages/configs-system/configsData';
import { Message } from '../../providers/socket/socket';

@IonicPage()
@Component({
  selector: 'page-configs-inputs',
  templateUrl: 'configs-inputs.html',
})
export class ConfigsInputsPage {
  constructor(public navCtrl: NavController, public navParams: NavParams, private socket: SocketProvider, private platform: Platform) { 

    document.addEventListener('pause', () => {
      if (this.activePage) {
        this.socket.message.next(this.quitMessage);
        this.subscribtion.unsubscribe();
      }        
    });

    this.platform.ready().then(() => {
      platform.resume.subscribe( ()=>{
        if (this.activePage) setTimeout( () => this.updateData(),250);
      });      
    });    

  }

  inputs: InputConfigs[];
  message: Message;
  subscribtion: Subscription;

  activePage: boolean = false;

  ionViewDidEnter() {
    this.updateData();
    this.activePage = true;
  }

  updateData(){
    this.socket.connect();
    this.subscribtion =  this.socket.message.subscribe(msg => {
      let configs = <Configs>msg.data;
      console.log(configs); 
      this.inputs = configs.inputs;
    });
    setTimeout(() => {
      this.socket.message.next(this.enterMessage);
  },500);
  }
 
  ionViewWillLeave() {
   this.socket.message.next(this.quitMessage);
   this.subscribtion.unsubscribe();
   this.activePage = false;
  }
  
  private enterMessage = { 
    id: 'configs',
    request: 'subscribe'
  }

  private quitMessage = {
    id: 'configs',
    request: 'unsubscribe'
  }

  changeOffset(chan: number, input: number): void {
    
    let configChannel = {
      offset: input,
      canal: chan,
    }

    let postMessage = {
      id: "offsetPut",
      request: 'put',
      data: configChannel
    }
    this.socket.message.next(postMessage);
  }

}


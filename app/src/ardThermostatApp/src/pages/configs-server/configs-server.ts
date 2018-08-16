import { Component, Inject } from '@angular/core';
import { IonicPage, NavController, NavParams, Platform } from 'ionic-angular';

/**
 * Generated class for the ConfigsServerPage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */

@IonicPage()
@Component({
  selector: 'page-configs-server',
  templateUrl: 'configs-server.html',
})
export class ConfigsServerPage {


  constructor(public navCtrl: NavController, public navParams: NavParams, platform: Platform, @Inject('server') private server) {
  }

  ionViewDidEnter() {
    console.log('ionViewDidLoad ConfigsServerPage');
  }


  cbPutServer(): void {
    this.server.putServer();
  }

  cbReset(): void {
    this.server.reset();
  }

  returnKeyboard() {
    alert('return keyboard')
  }
}

import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams } from 'ionic-angular';

import { HomeDataProvider } from '../../providers/home-provider/home';
import { Channel } from '../../pages/home/channel';
import  { Keyboard } from '@ionic-native/keyboard';

/**
 * Generated class for the ConfigNamePage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */

@IonicPage()
@Component({
  selector: 'page-config-name',
  templateUrl: 'config-name.html',
})
export class ConfigNamePage {
  channels: Channel[];

  constructor(public navCtrl: NavController, public navParams: NavParams, private keyboard: Keyboard, public homeData: HomeDataProvider) {
  }

  ionViewDidEnter() {
    console.log('ionViewDidLoad ConfigNamePage');
    this.updateData();
  }

  updateData() {
    this.homeData.getData().subscribe(data => {
      this.channels = data["channels"];
    });
  }

  clickOk(cb: Channel) {
    this.homeData.setChannel(cb);
    setTimeout(this.updateData(),200);
    this.keyboard.close();
  }

}

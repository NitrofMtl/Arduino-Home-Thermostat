import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams } from 'ionic-angular';

//import { HomeDataProvider } from '../../providers/home-provider/home';
import 'rxjs/add/operator/map';
//import { Subscription } from 'rxjs/Subscription';
//import Rx from 'rxjs/Rx';
//import { ModalController } from 'ionic-angular';
//import { ModalPage } from './modal-page';

import { ConfigsSystemPage } from '../configs-system/configs-system';
import { ConfigsInputsPage } from '../configs-inputs/configs-inputs';
import { ConfigsServerPage } from '../configs-server/configs-server';
import { ConfigNamePage } from '../config-name/config-name';

/**
 * Generated class for the ConfigsPage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */

@IonicPage()
@Component({
  selector: 'page-configs',
  templateUrl: 'configs.html',
})
export class ConfigsPage {


  constructor(public navCtrl: NavController, public navParams: NavParams) {
    //var serverURL: serverURL;
    //serverURL.localURL = 'http://192.168.0.110';
  }

  ionViewDidLoad() {
    console.log('ionViewDidLoad ConfigsPage');
  }

  goToName() {
    this.navCtrl.push(ConfigNamePage);
  }

  goToSystem() {
    this.navCtrl.push(ConfigsSystemPage);
  }

  goToInputs() {
    this.navCtrl.push(ConfigsInputsPage);
  }

  goToServer() {
    this.navCtrl.push(ConfigsServerPage);
  }

}

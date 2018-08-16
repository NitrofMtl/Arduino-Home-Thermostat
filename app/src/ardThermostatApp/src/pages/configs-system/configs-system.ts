import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams } from 'ionic-angular';
import { ConfigsProvider } from '../../providers/configs/configs';
import { Platform } from 'ionic-angular';

import { Configs, SystemTime } from '../configs-system/configsData';

/**
 * Generated class for the ConfigsSystemPage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */

@IonicPage()
@Component({
  selector: 'page-configs-system',
  templateUrl: 'configs-system.html',
})
export class ConfigsSystemPage {

  configs: Configs;
  systemTime: SystemTime;
  systemClock: Date;
  systemRunSince: Date;
  systemLastNtpSync: Date;

  constructor(public navCtrl: NavController, public navParams: NavParams, public configsData: ConfigsProvider, public plt: Platform) {
    
    document.addEventListener('resume', () => {
      this.updateData();
    });
  }


  updateData(){
    this.configsData.getConfigs().subscribe( data => {
      //console.log(data);
      this.configs = data["configs"][0];
      this.parseTimes(data["systemTime"]);
    })
  }


  ionViewDidLoad() {
        this.updateData();
  }

  
  cbPlusGain() {
    this.configs.K = this.configs.K + 1;
  }

  cbMinusGain() {
    this.configs.K = this.configs.K - 1;
  }

  cbPlusThresold() {
    this.configs.smm = this.configs.smm + 0.5;
  }

  cbMinusThresold() {
    this.configs.smm = this.configs.smm - 0.5;
  }

  cbPlusSV() {
    this.configs.vs = this.configs.vs + 0.5;
  }

  cbMinusSV() {
    this.configs.vs = this.configs.vs - 0.5;
  }

  cbOk()  {
    this.configsData.putConfig(this.configs);
  }

  private parseTimes(times: SystemTime) {
    //let test: Date = this.systemTime.now;
    this.systemClock = new Date(0);
    this.systemClock.setUTCSeconds(times.now);
    this.systemRunSince = new Date(0);
    this.systemRunSince.setUTCSeconds(times.runSince);
    this.systemLastNtpSync = new Date(0);
    this.systemLastNtpSync.setUTCSeconds(times.lastSync);
    //console.log(test);
    //console.log(test.toISOString());
  }

}



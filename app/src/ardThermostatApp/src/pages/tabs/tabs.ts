import { Component } from '@angular/core';
import { IonicPage, NavController } from 'ionic-angular';

import { HomePage } from '../home/home';
import { SchedulePage } from '../schedule/schedule';
import { ConfigsPage } from '../configs/configs';


/**
 * Generated class for the TabsPage tabs.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */

@IonicPage()
@Component({
  selector: 'page-tabs',
  templateUrl: 'tabs.html'
})
export class TabsPage {

  homeRoot = HomePage;
  sceduleRoot = SchedulePage;
  configsRoot = ConfigsPage;


  constructor(public navCtrl: NavController) {}

}

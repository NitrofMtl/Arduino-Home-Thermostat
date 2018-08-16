import { NgModule } from '@angular/core';
import { IonicPageModule } from 'ionic-angular';
import { ConfigsPage } from './configs';

@NgModule({
  declarations: [
    ConfigsPage,
  ],
  imports: [
    IonicPageModule.forChild(ConfigsPage),
  ],
})
export class ConfigsPageModule {}

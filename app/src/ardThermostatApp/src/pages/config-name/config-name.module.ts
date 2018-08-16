import { NgModule } from '@angular/core';
import { IonicPageModule } from 'ionic-angular';
import { ConfigNamePage } from './config-name';

@NgModule({
  declarations: [
    ConfigNamePage,
  ],
  imports: [
    IonicPageModule.forChild(ConfigNamePage),
  ],
})
export class ConfigNamePageModule {}

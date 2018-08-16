import { NgModule } from '@angular/core';
import { IonicPageModule } from 'ionic-angular';
import { ConfigsServerPage } from './configs-server';

@NgModule({
  declarations: [
    ConfigsServerPage,
  ],
  imports: [
    IonicPageModule.forChild(ConfigsServerPage),
  ],
})
export class ConfigsServerPageModule {}

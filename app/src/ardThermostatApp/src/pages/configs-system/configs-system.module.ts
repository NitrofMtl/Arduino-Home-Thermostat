import { NgModule } from '@angular/core';
import { IonicPageModule } from 'ionic-angular';
import { ConfigsSystemPage } from './configs-system';

@NgModule({
  declarations: [
    ConfigsSystemPage,
  ],
  imports: [
    IonicPageModule.forChild(ConfigsSystemPage),
  ],
})
export class ConfigsSystemPageModule {}

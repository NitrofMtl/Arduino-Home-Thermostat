import { NgModule } from '@angular/core';
import { IonicPageModule } from 'ionic-angular';
import { HomePage } from './home';
import { VerticalChannelComponent } from '../../components/vertical-channel/vertical-channel';
import { HorizontalChannelComponent } from '../../components/horizontal-channel/horizontal-channel';

@NgModule({
  declarations: [
    HomePage,
    VerticalChannelComponent,
    HorizontalChannelComponent,
  ],
  imports: [
    IonicPageModule.forChild(HomePage),
  ],
})
export class HomePageModule {}

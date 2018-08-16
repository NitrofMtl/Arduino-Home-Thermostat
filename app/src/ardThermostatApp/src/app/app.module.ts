import { NgModule, ErrorHandler } from '@angular/core';
import { BrowserModule } from '@angular/platform-browser';
import { IonicApp, IonicModule, IonicErrorHandler } from 'ionic-angular';
import { MyApp } from './app.component';

import { HttpClientModule } from '@angular//common/Http';
import { IonicStorageModule } from '@ionic/storage';
import { LongPressModule } from 'ionic-long-press';

//native
import { NativeStorage } from '@ionic-native/native-storage';
import { Keyboard } from '@ionic-native/keyboard';

import { HomePage } from '../pages/home/home';
import { SchedulePage } from '../pages/schedule/schedule';
import { ConfigsPage } from '../pages/configs/configs';
import { TabsPage } from '../pages/tabs/tabs';

import { ConfigNamePage } from '../pages/config-name/config-name'
import { ConfigsSystemPage } from '../pages/configs-system/configs-system';
import { ConfigsInputsPage } from '../pages/configs-inputs/configs-inputs';
import { ConfigsServerPage } from '../pages/configs-server/configs-server';

import { HomeDataProvider } from '../providers/home-provider/home';
import { ConfigsProvider } from '../providers/configs/configs';
import { ServerUrlProvider } from '../providers/server-url/server-url';
import { ScreenOrientation } from "@ionic-native/screen-orientation";

import { VerticalChannelComponent } from '../components/vertical-channel/vertical-channel';
import { HorizontalChannelComponent } from '../components/horizontal-channel/horizontal-channel';
import { ScheduleProvider } from '../providers/schedule/schedule';
import { WebsocketProvider } from '../providers/websocket/websocket';
import { SocketProvider } from '../providers/socket/socket';

import { StatusBar } from '@ionic-native/status-bar';
import { SplashScreen } from '@ionic-native/splash-screen';

@NgModule({
  declarations: [
    MyApp,
    HomePage,
    SchedulePage,
    ConfigsPage,
    TabsPage,
    ConfigNamePage,
    ConfigsSystemPage,
    ConfigsInputsPage,
    ConfigsServerPage,
    VerticalChannelComponent,
    HorizontalChannelComponent,
  ],
  imports: [
    BrowserModule,
    IonicModule.forRoot(MyApp),
    HttpClientModule,
    IonicStorageModule.forRoot(),
    LongPressModule,
  ],
  bootstrap: [IonicApp],
  entryComponents: [
    MyApp,
    HomePage,
    SchedulePage,
    ConfigsPage,
    TabsPage,
    ConfigNamePage,
    ConfigsSystemPage,
    ConfigsInputsPage,
    ConfigsServerPage,
  ],
  providers: [
    StatusBar,
    SplashScreen,
    {provide: ErrorHandler, useClass: IonicErrorHandler},
    NativeStorage,
    ScreenOrientation,
    Keyboard,
    HomeDataProvider,
    ConfigsProvider,
    {provide:'server', useClass:ServerUrlProvider},
    ScheduleProvider,
    WebsocketProvider,
    SocketProvider,
    WebsocketProvider,
    SocketProvider,
  ]
})
export class AppModule {}

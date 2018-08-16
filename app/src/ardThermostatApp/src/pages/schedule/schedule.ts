import { Component } from '@angular/core';
import { IonicPage, NavController, NavParams } from 'ionic-angular';
import { ScheduleProvider } from '../../providers/schedule/schedule';
import { AlarmData, Alarm } from '../schedule/alarm';

/**
 * Generated class for the ScedulePage page.
 *
 * See https://ionicframework.com/docs/components/#navigation for more info on
 * Ionic pages and navigation.
 */

@IonicPage()
@Component({
  selector: 'page-schedule',
  templateUrl: 'schedule.html',
})
export class SchedulePage {
  alarms: AlarmData; 
  channnelChecked = [
    false, false, false, false, false, false, false, false, false, false
  ];
  nowChecked: number;
  hours: number[];
  mins: number[];

  //clock: Date[];

  constructor(public navCtrl: NavController, public navParams: NavParams, public alarmData: ScheduleProvider) {
    this.hours = Array.from(Array(24).keys());
    this.mins = Array.from(Array(60).keys());
    document.addEventListener('resume', () => {
      this.updateData();
    });

  }

  ionViewDidEnter() {
    console.log('ionViewDidLoad ScedulePage');
    this.updateData();
    
  }

  updateData() {
    this.alarmData.getData().subscribe(data => {
      this.alarms = data;
    },(error: any) => {
      console.error('unable to get alarms data');
    })
/*    for ( let i:number of this.alarms.alarms ) {
      let j;
      this.clock[j].setHours()
    }*/
  }

  switch(i: number): void {
    this.alarmData.switchAlarm(i);
    setTimeout(this.updateData(),500);
  }

  digitalDisplay(input: number) {
    if ( 10 > input ) return '0' + input;
    else return input;
  }

  radioClk(j:number):void {
    this.channnelChecked = [
      false, false, false, false, false, false, false, false, false, false
    ];
    this.channnelChecked[j]= true;
    this.nowChecked = j;
  }

  tapOk(i: number): void {
    let alarm: Alarm = this.alarms.alarms[i];
    this.alarmData.putAlarm(alarm, i);
    setTimeout(this.updateData(),500);
  }

  spanCelcius(i:number, j:number): string {
    if ( this.alarms.alarms[i].setpoints[j] ) return '\xB0C';
    else return '';
  }

  tapPlus(i: number): void {
    if ( this.nowChecked != null ) {
      this.alarms.alarms[i].setpoints[this.nowChecked] += 0.5;
      if ( 5 > this.alarms.alarms[i].setpoints[this.nowChecked] ) this.alarms.alarms[i].setpoints[this.nowChecked] = 5;
    }
  }

  tapMinus(i: number): void {
    if ( this.nowChecked != null ) {
      this.alarms.alarms[i].setpoints[this.nowChecked] -= 0.5;
      if ( 5 > this.alarms.alarms[i].setpoints[this.nowChecked] ) this.alarms.alarms[i].setpoints[this.nowChecked] = null;
    }
  }

}

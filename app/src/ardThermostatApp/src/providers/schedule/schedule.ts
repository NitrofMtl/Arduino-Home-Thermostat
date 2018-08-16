import { HttpClient } from '@angular/common/http';
import { Injectable, Inject } from '@angular/core';
import { Observable } from 'rxjs/Observable';

import { AlarmData, Alarm } from '../../pages/schedule/alarm';
/*
  Generated class for the ScheduleProvider provider.

  See https://angular.io/guide/dependency-injection for more info on providers
  and Angular DI.
*/
@Injectable()
export class ScheduleProvider {

  constructor(public http: HttpClient, @Inject('server') private server) {
    console.log('Hello ScheduleProvider Provider');
  }

  getData(): Observable<AlarmData> {
    return this.http.get<AlarmData>(this.server.url() + '/ajax_alarms');
  }

  switchAlarm(cb: number) {
    var chanObj = {
      switchAlarm: {
        switchAlarm: cb
      }
    };
    this.http.put(this.server.url() + '/switchAlarms', chanObj,).subscribe(),{ responseType: "text" };
  }

  putAlarm(cb: Alarm, i:number) {
    var chanObj = {
      alarms : {
        index: i,
        data: cb
      }      
    };
    this.http.put(this.server.url() + '/alarms', chanObj,).subscribe(),{ responseType: "text" };
  }

}

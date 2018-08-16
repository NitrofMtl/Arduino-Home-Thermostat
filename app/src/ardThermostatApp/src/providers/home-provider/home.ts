import { HttpClient } from '@angular/common/http';
import { Injectable, Inject } from '@angular/core';
import { Observable } from 'rxjs/Observable';

import { Channel } from '../../pages/home/channel';

/*
  Generated class for the GetDataProvider provider.

  See https://angular.io/guide/dependency-injection for more info on providers
  and Angular DI.
*/
@Injectable()
export class HomeDataProvider {

  constructor(private http: HttpClient, @Inject('server') private server) {
    console.log('Hello GetDataProvider Provider');
  }

  getData(): Observable<Channel[]> {
    return this.http.get<Channel[]>(this.server.url() + '/ajax_inputs');
  }

  switchChannel(cb: Channel) {
    var chanObj = {
      switchCh: {
        canal: cb.canal,
        status: cb.status
      }
    };
    this.http.put(this.server.url() + '/switch', chanObj,).subscribe(),{ responseType: "text" };
  }

  setChannel(cb: Channel) {
    var chanObj = {
      channels: {
        canal: cb.canal,
        setpoint: cb.setpoint,
        name: cb.name,
      }
    };
    this.http.put(this.server.url() + '/channels', chanObj).subscribe();
  }

}
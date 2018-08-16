import { HttpClient } from '@angular/common/http';
import { Injectable, Inject } from '@angular/core';

import { Observable } from 'rxjs/Observable';
import { Platform } from 'ionic-angular';

import { Configs } from '../../pages/configs-system/configsData';

/*
  Generated class for the ConfigsProvider provider.

  See https://angular.io/guide/dependency-injection for more info on providers
  and Angular DI.
*/
@Injectable()
export class ConfigsProvider {

  constructor(public http: HttpClient, public plt: Platform, @Inject('server') private server) {
    console.log('Hello ConfigsProvider Provider');
  }

  getConfigs(): Observable<Configs> {
    return this.http.get<Configs>(this.server.url() + '/configs');
  }

  putConfig(configs: Configs) {
    let config = { "config": configs };
    console.log(config);
    this.http.put(this.server.url() + '/configs', config).subscribe();
  }

}
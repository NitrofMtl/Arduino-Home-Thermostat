import { Component, Input, Output, EventEmitter } from '@angular/core';

import { Channel } from '../../pages/home/channel';

/**
 * Generated class for the HorizontalChannelComponent component.
 *
 * See https://angular.io/api/core/Component for more info on Angular
 * Components.
 */
@Component({
  selector: 'horizontal-channel',
  templateUrl: 'horizontal-channel.html'
})
export class HorizontalChannelComponent {

  @Input() channel: Channel;
  @Output() switchClick: EventEmitter<Channel> = new EventEmitter<Channel>();
  @Output() sliderInput: EventEmitter<void> = new EventEmitter();
  @Output() sliderChange: EventEmitter<Channel> = new EventEmitter<Channel>();

  constructor() {
  }
  
  parentSwitchChannel(event, cb: Channel) {
    this.switchClick.emit(cb);
  }

  parentSliderInput(event) {
    this.sliderInput.emit();
  }

  parentPutSetpoint(event, cb: Channel, value: number) {
    cb.setpoint = value;
    this.sliderChange.emit(cb);
  }

}

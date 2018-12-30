import { Component, Input, OnChanges } from '@angular/core';

@Component({
  selector: 'heater-svg',
  templateUrl: 'heater-svg.html',
})
export class HeaterSvgComponent implements OnChanges {

  @Input() id: number = 0;
  @Input('stage') stagePercent: number = 0;

  constructor() {
  }

  ngOnChanges() {
    let stage = this.getStageLevel(this.stagePercent);

    let els = document.getElementsByClassName('heater')[this.id];
    for (let i = 1; i <= 4; i++) {
      let state = false;      
      if (stage >= i) {
        state = true;
      }
      
      if (els) {
        let el = els.getElementsByClassName('wave')[i-1];//wave 0 is stage 1
        if (state) {
          this.setColor(el, '#ff3300');
        } else {
          this.setColor(el, '#dadada');
        }
      }
    } 
  }

  getStageLevel(percent: number) : number {
    let stage: number = 0;
    if (percent > 99) {
      stage = 4;
    } 
    else if (percent > 70) {
      stage = 3;
    }
    else if (percent > 30) {
      stage = 2;
    }
    else if (percent > 0) {
      stage = 1;
    } 
    return stage;
  }

  setColor(item, color) {
    item.style["fill"] = color;
  }

}



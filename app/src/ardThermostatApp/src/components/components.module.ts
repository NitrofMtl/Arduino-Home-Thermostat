import { NgModule } from '@angular/core';
import { HorizontalChannelComponent } from './horizontal-channel/horizontal-channel';
import { VerticalChannelComponent } from './vertical-channel/vertical-channel';
import { HeaterSvgComponent } from './heater-svg/heater-svg';
@NgModule({
	declarations: [HorizontalChannelComponent,
    VerticalChannelComponent,
    HeaterSvgComponent,
    ],
	imports: [],
	exports: [HorizontalChannelComponent,
    VerticalChannelComponent,
    HeaterSvgComponent,
    ]
})
export class ComponentsModule {}

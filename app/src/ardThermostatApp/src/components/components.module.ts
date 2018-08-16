import { NgModule } from '@angular/core';
import { HorizontalChannelComponent } from './horizontal-channel/horizontal-channel';
import { VerticalChannelComponent } from './vertical-channel/vertical-channel';
@NgModule({
	declarations: [HorizontalChannelComponent,
    VerticalChannelComponent],
	imports: [],
	exports: [HorizontalChannelComponent,
    VerticalChannelComponent]
})
export class ComponentsModule {}

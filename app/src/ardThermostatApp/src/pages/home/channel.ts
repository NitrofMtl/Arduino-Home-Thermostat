export interface Channel {
    canal: string;
    name: number;
    status: string;
    temperature: number;
    setpoint: number;
    permission: boolean;
    percentOut: number;
  }

export interface ChannelArray {
  channels: Channel[];
}
  
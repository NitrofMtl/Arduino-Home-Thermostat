export interface InputConfigs {
    offset: number;
    canal: string;
    temperature: number;
}

export interface SystemTime {
    now: number;
    runSince: number;
    lastSync: number;
}

export interface Configs {
    K: number;
    vs: number;
    smm: number;
    inputs: InputConfigs[];
    systemTime: SystemTime;
}
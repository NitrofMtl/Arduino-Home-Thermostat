    export interface Alarm {
        switch: string;
        type: string;
        hour: number;
        minute: number;
        setpoints: number[];
    }

    export interface AlarmData {
        chName: string[];
        alarms: Alarm[];
    }
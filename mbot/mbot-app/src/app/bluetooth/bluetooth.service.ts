import { Injectable } from "@angular/core"
import { Observable, from } from "rxjs";

import bluetooth = require("nativescript-bluetooth");

@Injectable()
export class BluetoothService {
    public static connectedBot: bluetooth.Peripheral = null;

    public isEnabled(): Promise<boolean> {
        return bluetooth.isBluetoothEnabled();
    }

    public enable(): Promise<boolean> {
        return this.isEnabled()
        .then(enabled => {
            if (!enabled)
                return bluetooth.enable();
            return true;
        });
    }

    public availableBots(): Observable<bluetooth.Peripheral> {
        return new Observable<bluetooth.Peripheral>(observer => {
            this.enable()
            .then(enabled => {
                if (!enabled)
                    throw "Unable to enable";

                return bluetooth.startScanning({
                    seconds: 20,
                    skipPermissionCheck: false,
                    onDiscovered: (peripheral) => {
                        let name = peripheral.name || "";
                        name = name.toLowerCase();
                        if (name.indexOf("makeblock") >= 0)
                            observer.next(peripheral);
                    }
                });
            })
            .then(() => observer.complete());
        });
    }

    public connectToBot(uuid: string): Observable<bluetooth.Peripheral> {
        return new Observable<bluetooth.Peripheral>(observer => {
            this.availableBots()
            .subscribe(bot => {
                if (bot.UUID === uuid) {
                    bluetooth.connect({
                        UUID: uuid,
                        onConnected: (peripheral) => {
                            observer.next(peripheral);
                            BluetoothService.connectedBot = peripheral;
                        },
                        onDisconnected: (peripheral) => {
                            observer.error(peripheral);
                            BluetoothService.connectedBot = null;
                        }
                    });
                }
            })
            .add(() => observer.complete());
        });
    }

    public read(): Observable<bluetooth.ReadResult> {
        let bot = BluetoothService.connectedBot;
        return new Observable<bluetooth.ReadResult>(observer => {
            bluetooth.startNotifying({
                peripheralUUID: bot.UUID,
                serviceUUID: "ffe1",
                characteristicUUID: "ffe2",
                onNotify: (result) => {
                    observer.next(result);
                }
            });
        });
    }

    public write(...data: number[]): Promise<any> {
        let bot = BluetoothService.connectedBot;
        return bluetooth.writeWithoutResponse({
            peripheralUUID: bot.UUID,
            serviceUUID: "ffe1",
            characteristicUUID: "ffe3",
            value: data
        });
    }
}
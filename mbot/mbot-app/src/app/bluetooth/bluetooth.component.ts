import { Component, OnInit, ChangeDetectorRef, NgZone } from "@angular/core";
import { Router } from "@angular/router";
import { ObservableArray } from "tns-core-modules/data/observable-array";
import { Page } from "tns-core-modules/ui/page";
import { ListViewEventData } from "nativescript-ui-listview";

import { Peripheral } from "nativescript-bluetooth";
import { BluetoothService } from "./bluetooth.service";

@Component({
    moduleId: module.id,
    selector: "mb-bluetooth",
    templateUrl: "./bluetooth.component.html",
    styleUrls: ["./bluetooth.component.css"]
})
export class BluetoothComponent implements OnInit {
    devices: ObservableArray<Peripheral> = new ObservableArray();
    isScanning = false;

    constructor(
        private bts: BluetoothService,
        private router: Router,
        private page: Page,
        private cd: ChangeDetectorRef,
        private zone: NgZone
    ) { }

    ngOnInit() {
        this.page.addEventListener(
            Page.loadedEvent, 
            () => this.bluetooth()
        );
    }

    bluetooth() {
        this.isScanning = true;
        this.bts.availableBots().subscribe(bot => {
            let canAdd = this.devices
                .filter(d => d.UUID === bot.UUID)
                .length === 0;
                
            if (canAdd)
                this.devices.push(bot);
        })
        .add(() => {
            this.isScanning = false;
            // force detection
            this.cd.detectChanges();
        });
    }

    connect(uuid: string) {
        this.bts.connectToBot(uuid).subscribe(
            connect => {
                alert("Connected to " + connect.UUID);
                this.zone.run(() => {
                    this.router.navigate(["/home"], {
                        queryParams: { "connectedDevice": connect }
                    });
                });
            },
            disconnect => alert("Disconnected from " + disconnect.UUID)
        );
    }

    onPullToRefreshInitiated(e: ListViewEventData) {
        if (!this.isScanning)
            this.bluetooth();
        e.object.notifyPullToRefreshFinished();
    }
}
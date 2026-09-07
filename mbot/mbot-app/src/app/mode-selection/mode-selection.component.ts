import { Component, OnInit } from "@angular/core";
import { Router } from "@angular/router";

import { BluetoothService } from "../bluetooth/bluetooth.service";

@Component({
    moduleId: module.id,
    selector: "mb-mode-selection",
    templateUrl: "./mode-selection.component.html"
})
export class ModeSelectionComponent implements OnInit {
    constructor(private bts: BluetoothService, private router: Router) { }
    
    ngOnInit() {
        if (!BluetoothService.connectedBot) {
            this.router.navigate(["/bluetooth"]);
            return;
        }
    }

    onModeChange(mode: number) {
        if (!BluetoothService.connectedBot) {
            alert("Disconnected from bot!");
            this.router.navigate(["/bluetooth"]);
            return;
        }

        this.bts.write(mode)
        .then(() => {
            console.log("wrote mode");
        })
        .catch(err => console.log(err));
    }
}

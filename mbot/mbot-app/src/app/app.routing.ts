import { NgModule } from "@angular/core";
import { NativeScriptRouterModule } from "nativescript-angular/router";
import { Routes } from "@angular/router";

import { ModeSelectionComponent } from "./mode-selection/mode-selection.component";
import { BluetoothComponent } from "./bluetooth/bluetooth.component";

const routes: Routes = [
    { path: "", redirectTo: "/home", pathMatch: "full" },
    { path: "bluetooth", component: BluetoothComponent },
    { path: "home", component: ModeSelectionComponent }
];

@NgModule({
    imports: [NativeScriptRouterModule.forRoot(routes)],
    exports: [NativeScriptRouterModule]
})
export class AppRoutingModule { }
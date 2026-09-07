import { Component, Input } from "@angular/core";

@Component({
    selector: "mb-mode-button",
    templateUrl: "./mode-button.component.html"
})
export class ModeButtonComponent {
    @Input() type: "toggle" | "link" = "toggle";
}
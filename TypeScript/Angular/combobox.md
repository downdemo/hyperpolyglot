* option-component.component.html

```html
<select (change)="onChange($event)">
  <option *ngFor="let item of (enumType | enumKeyValues)" [value]="item.key" [selected]="currentValue == item.key">
    {{item.value}}</option>
</select>
```

* option-component.component.ts

```ts
import { Component, OnInit, Input, Output, EventEmitter } from '@angular/core';

@Component({
  selector: 'app-option-component',
  templateUrl: './option-component.component.html',
  styleUrls: ['./option-component.component.scss']
})
export class OptionComponentComponent implements OnInit {
  @Input() enumType: any;
  @Input() currentValue: number;
  @Output() valueChange = new EventEmitter<number>();

  constructor() { }

  ngOnInit() {
  }

  onChange(event: any) {
    this.currentValue = parseInt(event.target.value);
    this.valueChange.emit(this.currentValue);
  }

  setDisabled() { // 在父控件中通过 @ViewChild 调用
    const comboBoxObejct = this.thisElem.nativeElement.querySelector('select');
    comboBoxObejct.setAttribute('disabled', 'disabled');
    comboBoxObejct.setAttribute('style', 'background-color : #EEEEEE');
  }

  cancelDisabled() {
    const comboBoxObejct = this.thisElem.nativeElement.querySelector('select');
    comboBoxObejct.removeAttribute('disabled');
    comboBoxObejct.removeAttribute('style');
  }
}
```

## 使用

* .html

```html
<app-option-component [enumType]="enum1" [currentValue]="curIndex" (valueChange)="update($event);">
</app-option-component>
<ng-container *ngIf="show1()"> show 1 </ng-container>
<ng-container *ngIf="show2()"> show 2 </ng-container>
```

* .ts

```ts
enum options { 'option 1', 'option 2' }

enum1: any = options;
curIndex: number = 0;

update(event: any) {
  this.curIndex = event;
}

show1(): boolean {
  return options[this.curIndex] === 'option 1';
}

show2(): boolean {
  return options[this.curIndex] === 'option 2';
}
```

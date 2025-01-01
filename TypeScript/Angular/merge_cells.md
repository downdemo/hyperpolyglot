* .ts

```ts
rowContent: string[] = ['1', '2', '3', '4', '5', '6', '7', '8', '9'];

m: Map<string, string[]> = new Map([
  ['1', ['1-1', '1-2']],
  ['2', ['2-1', '2-2', '2-3']],
  ['3', ['3-1', '3-2']],
  ['4', ['4-1', '4-2']],
  ['5', ['5-1']],
  ['6', ['6-1', '6-2']],
  ['7', ['7-1']],
  ['8', ['8-1', '8-2', '8-3']],
  ['9', ['9-1', '9-2', '9-3']],
]);

update(key: string, index: number, event: any) {
  this.m.get(key)[index] = event.target.textContent;
}

rowToSpan(key: string) {
  return this.m.get(key).length;
}

isRowSpan1(key: string) {
  return this.rowToSpan(key) === 1;
}

isRowSpan2(key: string) {
  return this.rowToSpan(key) === 2;
}

isRowSpan3(key: string) {
  return this.rowToSpan(key) === 3;
}
```

* .html

```html
<div id="table" class="table-editable">
  <table class="table table-bordered table-responsive-md table-striped text-left">
    <tr>
      <th class="text-center">Col1</th>
      <th class="text-center">Col2</th>
    </tr>
    <ng-container *ngFor="let p of rowContent; let i = index;">

      <ng-container *ngIf="isRowSpan1(p)">
        <tr>
          <td>
            <div contenteditable="false" [innerHTML]="p"></div>
          </td>
          <td>
            <div (blur)="update(p, 0, $event)" contenteditable="true" [innerHTML]="m.get(p)[0]"></div>
          </td>
        </tr>
      </ng-container>

      <ng-container *ngIf="isRowSpan2(p)">
        <tr>
          <td rowspan="2">
            <div contenteditable="false" [innerHTML]="p"></div>
          </td>
          <td>
            <div (blur)="update(p, 0, $event)" contenteditable="true" [innerHTML]="m.get(p)[0]"></div>
          </td>
        </tr>
        <tr>
          <td>
            <div (blur)="update(p, 1, $event)" contenteditable="true" [innerHTML]="m.get(p)[1]"></div>
          </td>
        </tr>
      </ng-container>

      <ng-container *ngIf="isRowSpan3(p)">
        <tr>
          <td rowspan="3">
            <div contenteditable="false" [innerHTML]="p"></div>
          </td>
          <td>
            <div (blur)="update(p, 0, $event)" contenteditable="true" [innerHTML]="m.get(p)[0]"></div>
          </td>
        </tr>
        <tr>
          <td>
            <div (blur)="update(p, 1, $event)" contenteditable="true" [innerHTML]="m.get(p)[1]"></div>
          </td>
        </tr>
        <tr>
          <td>
            <div (blur)="update(p, 2, $event)" contenteditable="true" [innerHTML]="m.get(p)[2]"></div>
          </td>
        </tr>
      </ng-container>

    </ng-container>
  </table>
</div>
```

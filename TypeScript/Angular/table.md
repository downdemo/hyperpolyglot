```sh
ng generate component tables/table-xxx
```

* page1.component.html

```html
<app-table-xxx></app-table-xxx>
```

* table-xxx.component.html

```html
<!-- Editable table -->
<div class="card">
  <h3 class="card-header font-weight-bold text-uppercase py-4">{{'表名' | translate}}</h3>
  <div class="card-body">
  <div id="table" class="table-editable">
    <table class="table table-bordered table-responsive-md table-striped text-left">
    <tr>
      <th class="text-center">{{'水平表头1' | translate }}</th>
      <th class="text-center">{{'水平表头2' | translate}}</th>
    </tr>
    <tr *ngFor="let entry of entries; let id=index">
      <td>
      <div (blur)="updateList(id, 'prop1', $event)" contenteditable="true" [innerHTML]="entry.prop1"></div>
      </td>
      <td>
      <div (blur)="updateList(id, 'prop2', $event)" contenteditable="true" [innerHTML]="entry.prop2"></div>
      </td>
    </tr>
    </table>
  </div>
  </div>
</div>
<!-- Editable table -->
```

* 生成表头枚举

```
ng generate class models/xxx
```

* xxx.ts

```ts
export class A {
  prop1: string;
  prop2: string;
}
```

* table-xxx.component.ts

```ts
import { A } from 'src/app/models/xxx';  // 导入表头的枚举类型

export class TableXXXComponent implements OnInit {
  entries: A[] = [
    {
      prop1: '行1',
      prop2: '100',
    },
    {
      prop1: '行2',
      prop2: '200',
    },
    {
      prop1: '行3',
      prop2: '300',
    },
  ];

  updateList(id: number, property: string, event: any) {
    const editField = event.target.textContent;
    this.entries[id][property] = editField;
  }
}
```

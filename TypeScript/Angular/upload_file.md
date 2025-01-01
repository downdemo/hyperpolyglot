* 隐藏 input file 默认的选择框

```html
<input [hidden]="true" type="file" id="x" multiple="multiple" (change)="showFileName()">
<label> {{ selectFileName }} </label>
<button type="button" (click)="selectFile()">浏览</button>
<button type="button" (click)="uploadFile()">上传</button>
<label> {{ uploadTips }} </label>
```

* .ts

```ts
selectFileName: string = '未选择文件';
uploadTips: string = '';

showFileName() {
  const file: HTMLInputElement = this.thisElem.nativeElement.querySelector('#x') as HTMLInputElement;
  if (file.files.length === 0) return;
  let fileName: string = '';
  for (let i = 0; i < file.files.length; ++i) {
    fileName += `${file.files[i].name}、`;
  }
  fileName = fileName.substring(0, fileName.length - 1);
  this.selectFileName = fileName;
}

selectFile() {
  (this.thisElem.nativeElement.querySelector('#x') as HTMLInputElement).click();
}

uploadFile() {
  const file: HTMLInputElement = document.querySelector('#x') as HTMLInputElement;
  if (file.files.length === 0) {
    this.uploadTips = '请选择文件';
    return;
  }
  let data: FormData = new FormData();
  for (let i = 0; i < file.files.length; ++i) {
    data.append('fileName', file.files[i]);
  }
  data.append('userId', '123');
  const url: string = 'http://192.168.6.54:8080/calc/uploadFile';
  this.http.post(url, data).subscribe({
      next: (data) => {
        this.uploadTips = '正在上传';
      },
      error: (e) => {
        this.uploadTips = '上传失败';
      },
      complete: () => {
        this.uploadTips = '上传成功';
        setTimeout(() => { this.uploadTips = ''; }, 1000);
      }
    });
}
```

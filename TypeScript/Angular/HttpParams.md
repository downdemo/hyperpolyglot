```ts
export class testComponent implements OnInit {
  constructor(private http: HttpClient) {}
  testName: string = 'test';
  f(url: string) {
    const params: HttpParams = new HttpParams().set('userId', 'downdemo').set('fileName', this.testName);
    this.http.get(url + params).subscribe(
      data => { console.log(data); }
    );
  }
}
```

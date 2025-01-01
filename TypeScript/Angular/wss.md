```ts
export class AComponent implements OnInit {
  ws: WebSocket;
  f() {
    if (this.ws instanceof WebSocket) {
      this.ws.close();
    }
    const url: string = 'ws://localhost:3000/test';
    this.ws = new WebSocket(url);
    let observable = new Observable(observer => {
      this.ws.onmessage = (event) => observer.next(event.data);
      this.ws.onerror = (event) => observer.error(event);
      this.ws.onclose = (event) => observer.complete();
    });
    observable.subscribe({
      next: x => { console.log(x); },
      error: e => {},
      complete: () => {},
    });
  }
}
```

## 封装成 service

* 创建 `web-socket.service.ts`

```
ng g service services/webSocket

CREATE src/app/services/web-socket.service.spec.ts (349 bytes)
CREATE src/app/services/web-socket.service.ts (138 bytes)
```

* `web-socket.service.ts` 内容如下

```ts
import { Injectable } from '@angular/core';
import { Observable } from 'rxjs';

@Injectable({
  providedIn: 'root'
})
export class WebSocketService {
  constructor() { }
  ws: WebSocket;
  createObservable(url: string): Observable<any> {
    this.ws = new WebSocket(url);
    return new Observable(observer => {
      this.ws.onmessage = (event) => observer.next(event.data);
      this.ws.onerror = (event) => observer.error(event);
      this.ws.onclose = (event) => observer.complete();
    });
  }

  send(msg: any) {
    this.ws.send(JSON.stringify(msg));
  }

  close() {
    if (this.ws instanceof WebSocket) {
      this.ws.close();
    }
  }
}
```

* 使用该 service

```ts
import { WebSocketService } from 'src/app/services/web-socket.service';

export class AComponent implements OnInit {
  constructor(private wsService: WebSocketService) { }
  f() {
    const url: string = 'ws://localhost:3000/test';
    this.wsService.createObservable(url).subscribe({
      next: x => { console.log(x); },
      error: e => {},
      complete: () => {},
    });
  }
}
```

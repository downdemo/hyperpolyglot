## Creation Operators

```ts
const source$ = of(1, 2, 3);
const source$ = range(1.5, 100);
const source$ = defer(() => of(1, 2, 3));  // 在 subscribe 时才创建 Observable 对象
const source$ = from([1, 2, 3]);  // 把 Observerable-like 对象转为 Observerable
const source$ = fromEvent(document.querySelector('#clickButton'), 'click');
const source$ = generate(3, x => x < 10, x => x += 2, x => x * x);
const source$ = interval(1000);  // -012345---
const source$ = timer(2000);  // --(0|)
const source$ = timer(2000, 1000);  // --012345---
const source$ = empty();  // |
const source$ = throwError('boom');  // #
const source$ = never();  // -
source$.pipe(repeat(3));  // 重新 subscribe，上一次 complete 后才会 subscribe 下一次
source$.pipe(delay(2000));  // 2 秒后触发
source$.pipe(repeatWhen(() => interval(1000)));  // 每隔 1 秒触发
source$.pipe(delay(2000), repeatWhen(() => interval(1000)));  // 第二次触发前延迟 2 秒，相当于每隔 3 秒触发
```

## Join Creation Operators

```ts
of(1, 2, 3).pipe(startWith(4, 5, 6));  // 456123

const source1$ = of(1, 2, 3);
const source2$ = of(4, 5, 6);
concat(source1$, source2$);  // 123456
zip(source1$, source2$);  // [[1,4],[2,5],[3,6]]
forkJoin(source1$, source2$);  // [3,6]

const source1$ = timer(0, 2000);
const source2$ = timer(1000, 2000);
merge(source1$, source2$);
// 0-1-2-3-4-
// -0-1-2-3-4
// 0011223344
combineLatest(source1$, source2$);  // [0,0] [1,0] [1,1] [2,1] [2,2]---
combineLatest(source1$, source2$, (x, y) => `${x}${y}`);  // 00 10 11 21 22---
// 0-1-2-3-4-
// -0-1-2-3-4
// -011223344
// -001122334

const source1$ = of(1, 2, 3);
const source2$ = of(4, 5, 6);
combineLatest(source1$, source2$);  // [3,4] [3,5] [3,6]

const source1$ = interval(1000);
const source2$ = interval(2000);
source1$.pipe(withLatestFrom(source2$, (x, y) => `${x}${y}`));  // 10 20 31 41 52 62---
// -0123456
// --0-1-2-
// -0123456
// -0001122

const source1$ = interval(1000);
const source2$ = interval(2000);
const source3$ = timer(0);
race(source1$, source2$, source3$);  // 返回最早产生数据的上游，即 source3$
```

## Join Operators

```ts
const source$ = timer(0, 2000).pipe(
  take(2), // 生成0-1
  map(x => timer(1000, 2000).pipe( // 对于生成的0和1，各生成一个Observable
    map(y => `${x}${y}`), // y的是-0-1-2
    take(3), // 对于0：00-01-02，对于1：10-11-12
  )),
);
// source$ 是一个 Observerable，其中每个数据又是一个 Observerable
// source$ 类型为 Observerable<Observerable<string>>
// 每个 Observerable<string> 为
// -a-b-c    [a,b,c] = ['00','01','02']
// ---A-B-C  [A,B,C] = ['10','11','12']
source$.pipe(concatAll());  // -a-b-cA-B-C
source$.pipe(zipAll());  // ---[a,A]-[b,B]-[c,C]
source$.pipe(mergeAll());  // -a-(bA)-(cB)-C，括号表示同时发生
source$.pipe(combineAll());  // ---[a,A]-([b,A][b,B])-([c,B][c,C])
source$.pipe(switchAll());  // -a-A-B-C，生成 a 的下一秒切换到 2 秒时生成的第二个 Observable
source$.pipe(exhaust());  // -a-b-c，2 秒时第一个 Observable 未结束，生成的第二个 Observable 被丢弃
```

## Transformation Operators

```ts
of(1, 2, 3).pipe(map((value, index) => value + 1));  // 234
of(1, 2, 3).pipe(mapTo('A'));  // AAA
of({ id: '0', p: 'A' }, { id: '1', p: 'B' }).pipe(pluck('id'));  // 01
timer(0, 500).pipe(bufferTime(2000));  // [0,1,2,3] [4,5,6,7] [8,9,10,11]---
interval(100).pipe(bufferCount(3));  // [0,1,2] [3,4,5] [6,7,8]---
timer(0, 100).pipe(buffer(interval(300)));  // [0,1,2] [3,4,5] [6,7,8]---
interval(100).pipe(bufferWhen(() => timer(301)));  // [0,1,2] [3,4,5] [6,7,8]---
interval(100).pipe(bufferToggle(timer(0, 300), x => timer(200))); [0,1] [3,4] [6,7]---
interval(1000).pipe(concatMap(x => interval(1000).pipe(take(3))));  // 012012012---
interval(1000).pipe(concatMapTo(interval(1000).pipe(take(3))));
interval(1000).pipe(mergeMap(x => interval(1000)));  // 0(10)(210)(3210)(43210)---
interval(1000).pipe(mergeMapTo(interval(1000)));
// -0123456789
// |
// -01234---
// -0123---
// -012---
// -01---
// -0---
interval(1000).pipe(switchMap(x => interval(300)));  // 012012012--- 不断切换到新生成的 Observable 中
interval(1000).pipe(switchMapTo(interval(300)));
interval(1000).pipe(exhaustMap(x => interval(400).pipe(take(4))));  // 012301230123--- 上一个结束才切换到下一个
of(1).pipe(expand(x => of(x + 1)));  // 123456789---
const source$ = of({ id: 0, p: 'A' }, { id: 1, p: 'B' }, { id: 0, p: 'C' });
source$.pipe(groupBy(x => x.id), filter(x => x.key === 0), concatAll());  // { id: 0, p: 'A' } { id: 0, p: 'C' }
const [a$, b$] = partition(of(1, 2, 3, 4), x => x < 3);  // a$ 为 of (1,2) b$ 为 of (3,4)
of(1, 2, 3).pipe(scan((res, cur) => res + cur, 10));  // 11 13 16
of(1, 2, 3).pipe(mergeScan((res, cur) => of(res + cur), 10));  // 11 13 16
```

## Filtering Operators

```ts
interval(1000).pipe(filter(x => x % 2 === 0));  // 0246---
interval(1000).pipe(first(x => x % 2 === 0));  // 0
of(1, 2, 3).pipe(first(x => x < 0));  // 抛出 EmptyError
of(1, 2, 3).pipe(first(x => x < 0, 'hi'));  // hi
of(1, 2, 3).pipe(last(x => x > 0));  // 3
of(1, 2, 3).pipe(single(x => x < 2));  // 1，只有一个满足条件的值则返回这个值
of(1, 2, 3).pipe(single(x => x < 1));  // undefined
of(1, 2, 3).pipe(single(x => x < 3));  // 抛出 Sequence contains more than one element
of(1, 2, 3, 2, 3, 2, 1).pipe(distinct());  // 123
of({ id: '0', p: 'A' }, { id: '0', p: 'B' }).pipe(distinct(x => x.id));  // { id: '0', p: 'A' }
interval(1000).pipe(mapTo('A'), distinct(null, interval(1000)));  // AAA--- 内部 Observable 产生值时重新开始去重
of(1, 2, 2, 3, 3).pipe(distinctUntilChanged());  // 123
of({ id: '0' }, { id: '0' }).pipe(distinctUntilChanged((x, y) => x.id === y.id));  // { id: '0' }
of({ id: '0' }, { id: '0' }).pipe(distinctUntilKeyChanged('id'));  // { id: '0' }
of(1, 2, 3).pipe(elementAt(2));  // 3，获取指定索引对应的数据
of(1, 2, 3).pipe(elementAt(3));  // 找不到索引抛出 ArgumentOutOfRangeError
of(1, 2, 3).pipe(elementAt(3, 42));  // 42，找不到索引则使用默认值
interval(1000).pipe(take(2));  // 01
of(1, 2, 3).pipe(takeLast(2));  // 23
interval(1000).pipe(takeWhile(x => x < 3));  // 012
interval(1000).pipe(takeUntil(timer(2500)));  // 01
interval(1000).pipe(skip(2));  // 23456---
interval(1000).pipe(skipWhile(x => x < 3));  // 34567---
interval(1000).pipe(skipUntil(timer(2500)));  // 23456---
interval(1000).pipe(ignoreElements());  // 忽略所有数据
interval(1000).pipe(debounceTime(1500));  // 空，上游产生一个数的下 1.5 秒内不产生数，才能传递这个数
interval(1000).pipe(debounce(x => timer(1500)));  // 空，以内部 Observable 产生值的时刻作为时间
interval(1000).pipe(throttleTime(1500));  // 0246--- 传递一个数后开始计时，1.5 秒后才能传递下一个数
interval(1000).pipe(throttle(x => timer(1500)));  // 0246---
interval(1000).pipe(auditTime(1500));  // 1357--- 传递上游开始产生数的 1.5 秒内（提前完结则不传递）最后生成的数
interval(1000).pipe(audit(x => timer(1500)));  // 1357---
interval(1000).pipe(sampleTime(2000));  // 0246--- 直接开始计时，每隔 2 秒获取上游最后一个数
interval(1000).pipe(sample(interval(2000)));  // 1357---
```

## Multicasting Operators

```ts
const source$ = interval(1000).pipe(multicast(new Subject())) as ConnectableObservable<number>;
source$.subscribe(console.log);
setTimeout(() => source$.subscribe(console.log), 2000);
source$.connect();  // -01(22)(33)(44)---

const source$ = interval(1000).pipe(publish()) as ConnectableObservable<number>;
source$.subscribe(console.log);
setTimeout(() => source$.subscribe(console.log), 2000);
source$.connect();  // -01(22)(33)(44)---

const source$ = (interval(1000).pipe(take(2), multicast(() => new Subject())) as ConnectableObservable<number>).refCount();
source$.subscribe(console.log);
setTimeout(() => source$.subscribe(console.log), 3000);  // -01-01，超时则重新 subscribe

const source$ = interval(1000).pipe(take(2), share());
source$.subscribe(console.log);
setTimeout(() => source$.subscribe(console.log), 3000);  // -01-01

const source$ = (interval(1000).pipe(take(2), publishLast()) as ConnectableObservable<number>).refCount();
source$.subscribe(console.log);
setTimeout(() => source$.subscribe(console.log), 3000);  // -11，直接获取上游最后一个数据

const source$ = (interval(1000).pipe(publishReplay()) as ConnectableObservable<number>).refCount();
source$.subscribe(console.log);
setTimeout(() => source$.subscribe(console.log), 3000);  // -01(2012)(33)(44)---

const source$ = (interval(1000).pipe(publishReplay(2)) as ConnectableObservable<number>).refCount();
source$.subscribe(console.log);
setTimeout(() => source$.subscribe(console.log), 3000);  // -01 (212)(33)(44)--- 设置最多缓存 2 个数据

const source$ = (interval(1000).pipe(publishBehavior(8)) as ConnectableObservable<number>).refCount();
source$.subscribe(console.log);
setTimeout(() => source$.subscribe(console.log), 3000);  // 801 (22)(33)(44)--- 开始前先立刻使用上游最后的数据
```

## Error Handling Operators

```ts
throwError('boom').pipe(catchError((e, caught$) => of(`error is: ${e}`)));
concat(of(1, 2, 3), throwError('boom')).pipe(retry(2));  // 123123123#
concat(of(1, 2, 3), throwError('boom')).pipe(retryWhen(e$ => e$.pipe(delay(1000))));  // 123123123---
throwError('boom').pipe(finalize(() => console.log('over')));
```

## Conditional and Boolean Operators

```ts
of(1, 2, 3).pipe(every(x => x > 0));  // true
of(1, 2, 3).pipe(every(x => x > 1));  // false
of(1, 2, 3).pipe(find(x => x > 1));  // 2
of(1, 2, 3).pipe(find(x => x > 3));  // undefined
of(1, 2, 3).pipe(findIndex(x => x > 1));  // 1
of(1, 2, 3).pipe(findIndex(x => x > 3));  // -1
of(1, 2, 3).pipe(isEmpty());  // false
empty().pipe(isEmpty());  // true
never().pipe(isEmpty());  // 无结果，因为 never 不会 complete
empty().pipe(defaultIfEmpty('hi'));  // hi
of(1, 2, 3).pipe(defaultIfEmpty('hi'));  // 123
```

## Mathematical and Aggregate Operators

```ts
of(1, 2, 3).pipe(count());  // 3
of(1, 2, 3).pipe(max());  // 3
of(1, 2, 3).pipe(min());  // 1
of(1, 2, 3).pipe(reduce((res, cur) => res + cur, 10));  // 16
```

## node_modules

```ts
import { of, timer } from 'rxjs';
import { map, take } from 'rxjs/operators';
```

* rxjs/index.d.ts

```ts
export { Observable } from './internal/Observable';
export { ConnectableObservable } from './internal/observable/ConnectableObservable';
export { GroupedObservable } from './internal/operators/groupBy';
export { Operator } from './internal/Operator';
export { observable } from './internal/symbol/observable';
export { Subject } from './internal/Subject';
export { BehaviorSubject } from './internal/BehaviorSubject';
export { ReplaySubject } from './internal/ReplaySubject';
export { AsyncSubject } from './internal/AsyncSubject';
export { asap as asapScheduler } from './internal/scheduler/asap';
export { async as asyncScheduler } from './internal/scheduler/async';
export { queue as queueScheduler } from './internal/scheduler/queue';
export { animationFrame as animationFrameScheduler } from './internal/scheduler/animationFrame';
export { VirtualTimeScheduler, VirtualAction } from './internal/scheduler/VirtualTimeScheduler';
export { Scheduler } from './internal/Scheduler';
export { Subscription } from './internal/Subscription';
export { Subscriber } from './internal/Subscriber';
export { Notification, NotificationKind } from './internal/Notification';
export { pipe } from './internal/util/pipe';
export { noop } from './internal/util/noop';
export { identity } from './internal/util/identity';
export { isObservable } from './internal/util/isObservable';
export { ArgumentOutOfRangeError } from './internal/util/ArgumentOutOfRangeError';
export { EmptyError } from './internal/util/EmptyError';
export { ObjectUnsubscribedError } from './internal/util/ObjectUnsubscribedError';
export { UnsubscriptionError } from './internal/util/UnsubscriptionError';
export { TimeoutError } from './internal/util/TimeoutError';
export { bindCallback } from './internal/observable/bindCallback';
export { bindNodeCallback } from './internal/observable/bindNodeCallback';
export { combineLatest } from './internal/observable/combineLatest';
export { concat } from './internal/observable/concat';
export { defer } from './internal/observable/defer';
export { empty } from './internal/observable/empty';
export { forkJoin } from './internal/observable/forkJoin';
export { from } from './internal/observable/from';
export { fromEvent } from './internal/observable/fromEvent';
export { fromEventPattern } from './internal/observable/fromEventPattern';
export { generate } from './internal/observable/generate';
export { iif } from './internal/observable/iif';
export { interval } from './internal/observable/interval';
export { merge } from './internal/observable/merge';
export { never } from './internal/observable/never';
export { of } from './internal/observable/of';
export { onErrorResumeNext } from './internal/observable/onErrorResumeNext';
export { pairs } from './internal/observable/pairs';
export { partition } from './internal/observable/partition';
export { race } from './internal/observable/race';
export { range } from './internal/observable/range';
export { throwError } from './internal/observable/throwError';
export { timer } from './internal/observable/timer';
export { using } from './internal/observable/using';
export { zip } from './internal/observable/zip';
export { scheduled } from './internal/scheduled/scheduled';
export { EMPTY } from './internal/observable/empty';
export { NEVER } from './internal/observable/never';
export * from './internal/types';
export { config } from './internal/config';
```

* rxjs/operators/index.d.ts

```ts
export { audit } from '../internal/operators/audit';
export { auditTime } from '../internal/operators/auditTime';
export { buffer } from '../internal/operators/buffer';
export { bufferCount } from '../internal/operators/bufferCount';
export { bufferTime } from '../internal/operators/bufferTime';
export { bufferToggle } from '../internal/operators/bufferToggle';
export { bufferWhen } from '../internal/operators/bufferWhen';
export { catchError } from '../internal/operators/catchError';
export { combineAll } from '../internal/operators/combineAll';
export { combineLatest } from '../internal/operators/combineLatest';
export { concat } from '../internal/operators/concat';
export { concatAll } from '../internal/operators/concatAll';
export { concatMap } from '../internal/operators/concatMap';
export { concatMapTo } from '../internal/operators/concatMapTo';
export { count } from '../internal/operators/count';
export { debounce } from '../internal/operators/debounce';
export { debounceTime } from '../internal/operators/debounceTime';
export { defaultIfEmpty } from '../internal/operators/defaultIfEmpty';
export { delay } from '../internal/operators/delay';
export { delayWhen } from '../internal/operators/delayWhen';
export { dematerialize } from '../internal/operators/dematerialize';
export { distinct } from '../internal/operators/distinct';
export { distinctUntilChanged } from '../internal/operators/distinctUntilChanged';
export { distinctUntilKeyChanged } from '../internal/operators/distinctUntilKeyChanged';
export { elementAt } from '../internal/operators/elementAt';
export { endWith } from '../internal/operators/endWith';
export { every } from '../internal/operators/every';
export { exhaust } from '../internal/operators/exhaust';
export { exhaustMap } from '../internal/operators/exhaustMap';
export { expand } from '../internal/operators/expand';
export { filter } from '../internal/operators/filter';
export { finalize } from '../internal/operators/finalize';
export { find } from '../internal/operators/find';
export { findIndex } from '../internal/operators/findIndex';
export { first } from '../internal/operators/first';
export { groupBy } from '../internal/operators/groupBy';
export { ignoreElements } from '../internal/operators/ignoreElements';
export { isEmpty } from '../internal/operators/isEmpty';
export { last } from '../internal/operators/last';
export { map } from '../internal/operators/map';
export { mapTo } from '../internal/operators/mapTo';
export { materialize } from '../internal/operators/materialize';
export { max } from '../internal/operators/max';
export { merge } from '../internal/operators/merge';
export { mergeAll } from '../internal/operators/mergeAll';
export { mergeMap } from '../internal/operators/mergeMap';
export { mergeMap as flatMap } from '../internal/operators/mergeMap';
export { mergeMapTo } from '../internal/operators/mergeMapTo';
export { mergeScan } from '../internal/operators/mergeScan';
export { min } from '../internal/operators/min';
export { multicast } from '../internal/operators/multicast';
export { observeOn } from '../internal/operators/observeOn';
export { onErrorResumeNext } from '../internal/operators/onErrorResumeNext';
export { pairwise } from '../internal/operators/pairwise';
export { partition } from '../internal/operators/partition';
export { pluck } from '../internal/operators/pluck';
export { publish } from '../internal/operators/publish';
export { publishBehavior } from '../internal/operators/publishBehavior';
export { publishLast } from '../internal/operators/publishLast';
export { publishReplay } from '../internal/operators/publishReplay';
export { race } from '../internal/operators/race';
export { reduce } from '../internal/operators/reduce';
export { repeat } from '../internal/operators/repeat';
export { repeatWhen } from '../internal/operators/repeatWhen';
export { retry } from '../internal/operators/retry';
export { retryWhen } from '../internal/operators/retryWhen';
export { refCount } from '../internal/operators/refCount';
export { sample } from '../internal/operators/sample';
export { sampleTime } from '../internal/operators/sampleTime';
export { scan } from '../internal/operators/scan';
export { sequenceEqual } from '../internal/operators/sequenceEqual';
export { share } from '../internal/operators/share';
export { shareReplay } from '../internal/operators/shareReplay';
export { single } from '../internal/operators/single';
export { skip } from '../internal/operators/skip';
export { skipLast } from '../internal/operators/skipLast';
export { skipUntil } from '../internal/operators/skipUntil';
export { skipWhile } from '../internal/operators/skipWhile';
export { startWith } from '../internal/operators/startWith';
export { subscribeOn } from '../internal/operators/subscribeOn';
export { switchAll } from '../internal/operators/switchAll';
export { switchMap } from '../internal/operators/switchMap';
export { switchMapTo } from '../internal/operators/switchMapTo';
export { take } from '../internal/operators/take';
export { takeLast } from '../internal/operators/takeLast';
export { takeUntil } from '../internal/operators/takeUntil';
export { takeWhile } from '../internal/operators/takeWhile';
export { tap } from '../internal/operators/tap';
export { throttle } from '../internal/operators/throttle';
export { throttleTime } from '../internal/operators/throttleTime';
export { throwIfEmpty } from '../internal/operators/throwIfEmpty';
export { timeInterval } from '../internal/operators/timeInterval';
export { timeout } from '../internal/operators/timeout';
export { timeoutWith } from '../internal/operators/timeoutWith';
export { timestamp } from '../internal/operators/timestamp';
export { toArray } from '../internal/operators/toArray';
export { window } from '../internal/operators/window';
export { windowCount } from '../internal/operators/windowCount';
export { windowTime } from '../internal/operators/windowTime';
export { windowToggle } from '../internal/operators/windowToggle';
export { windowWhen } from '../internal/operators/windowWhen';
export { withLatestFrom } from '../internal/operators/withLatestFrom';
export { zip } from '../internal/operators/zip';
export { zipAll } from '../internal/operators/zipAll';
```

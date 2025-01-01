
* Client 与 Server 之间传递数据时，为了安全考虑，必须对数据进行加密。

## 对称加密

* 使用一个公开的对称加密算法 f，Client 和 Server 双方在发送数据前先约定使用一个密钥 Key，则对于要加密的数据 data 有如下公式：

```
f(Key, data) = x  // 加密
f(Key, x) = data  // 解密
// 即
f(Key, f(Key, data)) = data
```

* Client 和 Server 双方发送数据前，先加密数据，传输过程中的数据就是密文，并且双方都能根据密文解出数据。这种做法的问题在于，一旦双方约定的 Key 被其他人知道了，那么传输的密文对于有 Key 的人来说，和明文没有区别，窃取密文一样可以解出数据，对称加密就失去了安全性。为此，双方每次发送前，都要约定一个从未泄漏的密钥，这会使得双方需要管理大量密钥

## 非对称加密

* Server 管理公钥 PublicKey 和私钥 PrivateKey 两个钥匙，对于非对称加密算法有如下公式：

```
// 公钥加密，私钥解密
f(PublicKey, data) = x   // 公钥加密
f(PrivateKey, x) = data  // 私钥解密
// 私钥加密，公钥解密
f(PrivateKey, data) = x  // 私钥加密
f(PublicKey, x) = data   // 公钥解密
// 即
f(PrivateKey, f(PublicKey, data)) = data  // 公钥加密，私钥解密
f(PublicKey, f(PrivateKey, data)) = data  // 私钥加密，公钥解密
```

* Client 发送数据前，从 Server 请求 PublicKey，然后用公钥加密数据，传输给 Server，Server 收到后利用只有自己知道的 PrivateKey 即可解密。由于只有 Server 知道 PrivateKey，即使发送过程中，信息被窃取了，也无法被解密，因此 Client 发送数据的过程很安全。但当 Server 向 Client 返回数据时，就引入了一个问题：
  * 如果 Server 用 PublicKey 加密，Client 没有 PrivateKey，无法解密
  * 如果 Server 用 PrivateKey 加密，那么知道 PublicKey 就可以对密文解密，而其他人是可以窃取 PublicKey 的
* 因此，非对称加密在 Server 返回数据时，依然无法保证安全。

## 非对称加密再对称加密

* 比起非对称加密，对称加密的算法公开，计算量小，加密速度快，只要 Key 不泄漏就可以保证安全，而非对称加密可以保证 Client 发送的数据安全到达 Server，结合双方的优势即可实现一个安全的双向发送：
  * Client 发送数据前，从 Server 请求 PublicKey，然后用 PublicKey 加密一个随机生成的字符串，Server 收到密文后用 PrivateKey 解密出这个字符串，此时只有双方知道这个字符串
  * Server 向 Client 回复一条 Success 信息，表示确认收到了信息，此时双方就可以利用这个字符串作为对称加密的 Key 来加解密
* 但这样做还有一个中间人攻击（Man-in-the-MiddleAttack）的问题，假如有一个中间人在 Client 和 Server 之间转发数据，Client 和 Server 以为双方是直接通信，但实际上信息都被中间人窃取了：
  * 对于 Client：中间人伪装成 Server，Client 向中间人请求了一个公钥，Client 看不出这个公钥有什么异样，照样用它加密一个字符串，中间人收到字符串后用自己的私钥解密，然后回复一条 Success 信息，这样 Client 就把中间人当成了 Server，发送的数据都被中间人窃取了
  * 对于 Server：中间人和其他的 Client 没有什么区别，同样可以用非对称加密和对称加密与 Server 安全传输数据，于是中间人把窃取到的 Client 的数据全转发给 Server，Server 就会回复 Client 需要的返回结果，同样，这个返回结果也被中间人窃取了
* 因此结合非对称加密和对称加密，在中间人攻击下仍然无法保证安全

## CA（Certificate Authority）

* 其实解决中间人攻击的问题很简单，只要保证 Client 请求到的 PublicKey 是真正来自 Server 的，就能保证数据一旦加密，只有有 PrivateKey 的 Server 能解密，这就需要一个大家都认可的权威机构 CA，CA 用自己的私钥加密 Server 的 PublicKey，生成一个证书 License：

```
f(CAPrivateKey, ServerPublicKey) = License
```

* Client 不再请求 PublicKey，而是直接请求 Licence，然后用 CA 的公钥解密 License，就能得到 Server 的 PublicKey：

```
f(CAPublicKey, License) = ServerPublicKey
```

* 而 CA 的公钥是内置在本机操作系统和浏览器中的，不需要从网络上请求，一定是真实的（除非人为给本机安装不安全的根证书，或者安装的操作系统镜像里就注入了不安全的根证书，或者碰上了不可靠的 CA 故意签发不安全的 License），这样最终就保证了 Client 用上了 Server 的 PublicKey，Client 发送的数据一定是安全的，只有 Server 能解密
* SSL 3.1 版本开始就叫 TLS，SSL 的实际过程是：
  * Client 向 Server 发送信息，包括使用的 SSL 版本、非对称加密算法、随机数 A
  * Server 收到后回复确认信息，包括之后使用的对称加密算法、随机数 B、License
  * Client 验证 License，如果认证成功，则向 Server 发送随机数 C 用公钥加密的结果、Hash (A, B)
  * Server 验证 Hash (A, B)，确保是 Client 发送的信息，验证成功则将 Hash (A, B, C) 作为对称加密算法的 Key，并向 Client 返回 Hash (A, B, Hash (A, B))
  * Client 验证  Hash (A, B, Hash (A, B))，验证成功则将 Hash (A, B, C) 作为对称加密算法的 Key，此时双方都安全拥有了对称加密的 Key，用这个 Key 做对称加密即可

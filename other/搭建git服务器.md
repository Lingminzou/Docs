## 一、唠叨两句

通常情况下搭建 git 服务器的机会还是比较少的，而搭建 git 服务器的唯一用途就是多人协作。

github 即是一个典型的 git 服务器，但其免费托管的代码都是开源的，如果不想开源，则需要向 github 交保护费。

假设有一天你和你的小伙伴想搞个不开源的商业性项目，而且需要用到版本控制，这时我们便需要自建 git 服务器啦。

那在这之前我们得有一个和小伙伴之间都能访问的服务器才行，这让我想起了我去年为了搭梯子买的服务器了。

美帝的服务商 [VirMach](https://virmach.com/)，做活动的时候买的， 6 美刀一年，十分划算，为了买它我还特意去办了一张双币种信用。

![小水管勉强够用](http://wx2.sinaimg.cn/large/9e169b75gy1fqcglm9w5bj20wj0b1t92.jpg)

但现在貌似各服务商多支持支付宝付款了，也导致这种廉价服务器严重脱销，就是说，买不到啦！！！

现在 VirMach 推出的最便宜的 vps 要 1 美刀一月，包年 10 美刀一年，配置比我这还差些。

![](http://wx1.sinaimg.cn/large/9e169b75gy1fqch4w0jwrj20tc06f0tb.jpg)

而我的服务器今年 5 月 11 号就要到期了，突然想到续费的问题，在这种情况下我感觉肯定是续不了了，毕竟活动期买的。

但万万没想到前几天收到 VirMach 发来的续费账单，还是 6 美刀，还是 6 美刀！！！我毫不犹豫立马支付了账单。

真是美帝良心啊，要放国内奸商，我感觉肯定是续费不了。

## 二、进入正题

**第一步**，安装 git ，我用的是 ubuntu 服务器，所以使用 apt-get 命令即可，

```sh
sudo apt-get install git
```

**第二步**，为 git 服务专门创建一个 git 用户，

```sh
sudo adduser git
```

**第三步**，切换到 git 用户，做这步的原因是后面要创建和编辑一些文件最好是要属于 git 用户的，

```sh
su git
```

**第四步**，初始化一个空的 git 仓库，

假设我们将 git 仓库建立在 `/home/git/srv` 目录下，那我们 cd 进入这个目录执行如下命令，

```sh
git init --bare sample.git
```

注：服务器上 git 仓库通常都以 .git 结尾

**第五步**，创建证书登录，

git 服务可用通过 ssh 登录，所以需要收集小伙伴的 ssh 公钥，公钥通常位于用户目录的 `.ssh/id_rsa.pub` 文件中。

关于得到公钥的细节可以参考这篇文章 [windows下GitHub的SSH key配置](https://www.jianshu.com/p/9317a927e844)。

拿到小伙伴的公钥后我们将其添加到用户目录下的 `.ssh/authorized_keys` 文件中，一行一个。

如果系统里面没有这个文件则创建它，如下，

```sh
cd /home/git/
mkdir .ssh
touch .ssh/authorized_keys
```

到这里我们的 git 服务器就建立好啦，那我们在客户端使用 clone 命令来试试吧，如下：

```sh
git clone git@server:/home/git/srv/sample.git
```

注：其中的 server 即为你的服务器地址，可以直接为 IP 或域名。

在这个过程最容易遇到的问题就是文件权限的问题，如执行 push 提示权限不够。

如没有切到 git 下操作，创建的仓库将不属于 git 用户，我们用 chown 命令来改过来，如下：

```sh
chown -R git:git sample.git
```

另外就是读写权限，简单粗暴的就改为 777，保险点我们还是改为 764，如下：

```sh
chmod -R 764 sample.git
```

扫码关注我了解更多，周末愉快！ ^_^ 

![](http://wx1.sinaimg.cn/large/9e169b75gy1fqcisgsbd7j2076076q3e.jpg)

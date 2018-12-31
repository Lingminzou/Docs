## 一、问题描述

作为 git 初学者的我已经体验到了每次向 github push 时都要输入密码带来的烦恼，但没关系我会 google 啊，很快我找了到使用 SSH key 的方法来避免这个问题，并找到了一篇在 windows 平台上配置 github SSH key 的教程如下链接：

[windows下GitHub的SSH key配置](https://www.jianshu.com/p/9317a927e844)

很快我做好了一切，但我测试 push 时发现还是需要我输入密码，这是为什么呢？

## 二、柳暗花明

原来多是没认清协议惹的祸。我用于测试的仓库是用 https 协议克隆下来的，所以密码验证自然是使用 https 协议进行，所以并未使用到我们之前配置的 SSH key，解决方案即是更换为 SSH 协议，我们使用如下命令：

	git remote set-url origin git@github.com:youuserneme/yourrepositoryname

然后再次尝试提交，终于不会要求我输入密码了。

## 三、关于 git 协议

Git 可以使用四种主要的协议来传输数据：本地传输协议，SSH 协议，Git 协议和 HTTP 协议。

对我们来说会在 clone 过程中体现

如当使用使用本地协议时我们是这样做的：

	git clone /path/project.git

或者是这样的：

	git clone file:///path/project.git

使用 SSH 协议时：

	git clone user@server:project.git

如使用 SSH 协议 clone github 的仓库：

	git clone git@github.com:youuserneme/yourrepositoryname

使用 http 协议时：

	git clone http://example.com/gitproject.git

关于 git 协议更详细的介绍，请阅读以下链接的内容。

[服务器上的 Git - 协议](https://git-scm.com/book/zh/v1/%E6%9C%8D%E5%8A%A1%E5%99%A8%E4%B8%8A%E7%9A%84-Git-%E5%8D%8F%E8%AE%AE)

欢迎关注我的个人微信公众号了解更多。

![](https://mmbiz.qpic.cn/mmbiz_jpg/yhkQoyL4zW4g1wWs8nA54YpcTPFKjdmTofG0Ib3sHl2zYNibuKMqaN060Cj1iax3738eumYcfCv5bm5oqU9bhBrg/0?wx_fmt=jpeg)

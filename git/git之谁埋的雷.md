最近在代码里面发现了一些 bug ，但不知是谁所写，不过现在代码使用 git 进行管理的，由于之前一直使用 svn ，头次在 git 管理的仓库里面遇到这样的事情，一时还不知道咋弄。

换做以前是直接在 windows 下打开 svn 客户端，然后通过搜索该文件的日志，慢慢找到对应的点，看是谁提交的。

由于现在 git 都是命令行操作，按老套路好像不太好操作。

那有没有对应的命令能完成这个操作呢？

答案是肯定的，而且非常简单，看完后直接流泪，怎么这么简单，以前一直被 SVN 的 GUI 的客户端坑害，都没去想过命令行下怎么操作。

	git-blame - Show what revision and author last modified each line of a file

blame 中文意思是 责备，这名字取的特别的贴切，下面是它的具体用法：

	git blame file

git blame 后面接文件名则会把该文件每一行是谁的提交的展示出来。

如果想只查看某几行，可以使用 -L 参数，如下：

	git blame -L m,n file

以上将只展示这个文件 m 到 n 最后是谁提交的。

后面了解了下其实 svn 上也有一样的命令，也是叫这个名字 blame。

冤有头，债有主，找谁埋的雷，请记得使用 blame。

以上知道的太晚，做个笔记，下篇打算介绍下如何避免埋下些比较明显的雷。

欢迎关注我的个人微信公众号了解更多。

![](https://mmbiz.qpic.cn/mmbiz_jpg/yhkQoyL4zW4g1wWs8nA54YpcTPFKjdmTofG0Ib3sHl2zYNibuKMqaN060Cj1iax3738eumYcfCv5bm5oqU9bhBrg/0?wx_fmt=jpeg)
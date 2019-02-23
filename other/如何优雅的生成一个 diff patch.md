
某天如果你要把一条 git 或 svn 记录的修改发送给其他人时我们要怎么做呢？

在 git 下你可以这么做：

	git format-patch 【commit sha1 id】 -1

这样会生成单条记录的 patch 文件，当然也可以生成多条或者两条 commit 记录之间的修改。

例如我们在开源项目 nuklear 上执行如下命令生成一条记录的 patch

	git format-patch 181cfd86c47ae83eceabaf4e640587b844e613b6 -1

我们会在当前目录下得到一个 0001-Add-NK_KEYSTATE_BASED_INPUT-changes-to-src-files.patch 文件

那我们可以把这个文件发给其他人，其他人打入 patch 也很简单，使用如下命令

	git apply 【path/to/xxx.patch】

整个过程看起来很轻松，可一旦发生冲突就麻烦了，而且整个过程看起来黑漆漆的，不知道这个 patch 修改了些什么东西。

当然 .patch 我们是可以打开来看的，里面有一定的规则指明做了哪些改动，但不是特别直观，如下图：

![git patch 文件](http://wx1.sinaimg.cn/mw690/006kxAfbgy1g0ggotyul8j30rj0knmyu.jpg)

解决冲突的时候我们最好能很直观的知道这条记录到底改动了哪些东西。

相信大家肯定都用过 BCompare 这个神器，能很清楚的展示两个目录或文件间的差异，

如果能在提供 .patch 文件的同时提供这条记录修改文件的前后分别放在一个 old 一个 new 目录，然后用 BCompare 对比来看下，是不是更好？

这里我们以 svn 举例（因为我们的实际项目是使用 svn 管控代码的）来生成这样一个可以用 BCompare 对比着来看的 patch。

我们先找到如下这样一条 svn 记录：

![需要导出的 svn 记录](http://wx2.sinaimg.cn/mw690/006kxAfbgy1g0ggopfxvtj30wq08vjsf.jpg)

最终我们要导出这种效果：

![导出后最终的效果图](http://wx2.sinaimg.cn/mw690/006kxAfbgy1g0ggoxu7qej30v60ckgmm.jpg)

那我们应该怎么做呢？

如上，我们要在保证目录层级关系的情况下只导出这条记录修改过的文件。

对于得到这条记录修改哪些文件我们可以使用如下命令来得到，

	svn diff -r xxx0:xxx1 --summarize svn-link

然后我们可以先 checkout 下一个空的目录，然后根据得到修改过的文件一级级的 up 下来，主要使用如下两条命令：

	svn co --depth empty svn_repository       /* checkout 一个空的目录 */
	svn up --depth empty svn_repository_dir   /* up 一个空的目录，实际对文件也有效 */

有了大致思路后我们就用 python 来实现它吧，最后我的实现如下：

	#!/usr/bin/env python3
	# -*- codeing: utf-8 -*-
	
	import os,sys
	
	def usrage():
		print("./create-svn-diff.py diff -r xxx:xxx path")
	
	def svn_checkout_file(path, ver):
		items = path.split('/')
	
		tempDir = './temp/'
	
		for item in items:
			cmd = 'cd ' + tempDir + ';' + 'svn up --depth empty ' + item + ' -r ' + ver
			print(cmd) 
			if os.system(cmd) == 0:
				tempDir = tempDir + '/' + item
			else:
				cmd = 'cd ' + tempDir + ';' + 'svn up ' + item + ' -r ' + ver
				os.system(cmd)
	
	if __name__=='__main__':
	
		if len(sys.argv) != 5:
			usrage()
			sys.exit(0)
	
		link = sys.argv[4]
		cmd = 'svn co --depth empty ' + link + ' temp'
		os.system(cmd)
	
		cmd = 'svn --summarize ' + sys.argv[1] + ' ' + sys.argv[2] + ' ' + sys.argv[3] + ' ' + sys.argv[4]
		f = os.popen(cmd)
	
		lines = f.readlines()
	
		for line in lines:
			line = line.strip()
			if len(line) == 0 or line[0] == 'D':
				continue
	
			line = line[1:]
			line = line.strip()
			
			path = line.replace(link, '')
			path = path.lstrip('/')
	
			svn_checkout_file(path, sys.argv[3].split(':')[1])
	
		os.system('cp -rf temp old')
		
		cmd = 'cd ./old; svn up -r ' + sys.argv[3].split(':')[0]
		os.system(cmd)
	
		tempDir = 'svn-diff-' + sys.argv[3].split(':')[0] + '-' + sys.argv[3].split(':')[1]
		cmd = 'mv temp new;rm -rf ./new/.svn;rm -rf ./old/.svn;mkdir -p ' + tempDir + ';mv new ' + tempDir + ';mv old ' + tempDir
		os.system(cmd)

代码有些粗糙，但功能还是达成了，你也可以在我的 github 上下载到它，欢迎提建议和问题。

	https://github.com/Lingminzou/code/tree/master/python/create-svn-diff

扫码关注我了解更多

![](http://wx1.sinaimg.cn/large/9e169b75gy1fqcisgsbd7j2076076q3e.jpg)
	

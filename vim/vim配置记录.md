# vim 配置记录

工欲善其事必先利其器，不经配置的 vim 在实际使用过程中会有诸多不便之处，但只要稍加配置即十分称手。

## .vimrc 文件与 .vim 目录

那 .vimrc 即是 vim 的配置文件了，它位于 $HOME 目录，即 ~ 目录下。
vim 每次启动时多会去读取它，同时修改它并不能立即生效，需要重启生效。
如果你发现它没有的话可以手动的创建它。我的 vim 配置文件备份到了 vim_config 仓库，查看时选到 Vundle.vim 这个 Branch。
打开可以看到每一项配置均有注释，就不一一介绍了，这里说几个我遇到的问题。

往编辑中的文档粘贴代码时出现格式乱的问题。这是由于配置中开启了自动缩进的设置，所以粘贴进来的代码不能保持原来的格式，
解决办法即临时关闭自动缩进功能，粘贴完毕后再开启即可。
我们使用 <code>set past</code> 临时关闭自动缩进功能，粘贴完成后使用 <code>set nopaste</code> 打开。

想复制 vim 打开的文档内容，却受行号的影响。这个问题我们可以临时关闭行号显示来解决，
使用 <code>set nonumber</code> 临时关闭行号显示，完成后使用 <code>set number</code> 再打开。

.vim 目录则是存放 vim 插件的目录了，我们所有的插件安装好后都放在这个目录之下，关于插件的安装我们后面再介绍。
当配置好了之后我们只需要备份好 .vimrc 文件和 .vim 目录，再到新的机器上时把这两个东西拷贝过去立马就完成了 vim 的配置了，妈妈再也不用担心我天天瞎折腾了。

## ctags 与 cscope

通常来说 ctags 是必备的，它在代码跳转和代码自动补全上是离不开的。它能生成 tag 文件，其中包含函数、类、变量的定义信息。

我们使用如下命令来生成 tag 文件。

<code>ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .</code>

然后我们在 .vimrc 文件中把上述命令进行映射一个快捷键来方便的生成 tag 文件，如下：

<code>nnoremap <F5> :!ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .<CR></code>

这样我们按 F5 键的时候即开始执行生成 tag 文件。

需要注意的是我们安装的 ctags 是 Exuberant Ctags，这一点在 Ubuntu 系统中使用 apt-get 命令进行安装的时候需要注意。

另一个问题是当我们在服务器上的时候应该怎么办，因为在服务器上往系统上安装软件的时候是需要管理员权限的，而如果每次去麻烦管理员也是不太好的，所以当服务器上没有安装 ctags 的时候，我们可以尝试通过源码编译的方式来手动安装它。

首先下载源码解压后进入，执行如下命令来完成编译和安装。

<p>
<pre name="code" class="cpp">
mkdir install
./configure --prefix=./install
make
make install
</pre>
</p>

我们使用 prefix 参数指定了安装目录，因为默认它是安装到/usr/local目录的，那里我们是没有权限的，所以需要手工指定安装目录。
待完毕后另一件需要做的事情就是要让系统找到它，即加入系统的环境变量中，一个简单的办法即是写入到 .bashrc 文件中，如下我们在此文件
最后加入如下命令。

<code>export PATH=/home/zoulm/ctags/install/bin:$PATH</code>

这样就完成了在服务器上对 ctags 的手动安装。

关于 cscope 则看以被视为是 ctags 的加强版，它除了具备 ctags 的一些能力外，我们还可以通过它找到函数在哪些地方被调用了。

生成 cscope 数据库的方法比较简单，只需执行 <code>cscope -Rbq</code> 命令即可，而且还有一个好处是当更新代码再次生成数据库时，
它不像 ctags 那样会从新生成，它会只扫描改动过的文件从而加快这个过程。但需要注意的是默认的执行上述命令 cscope 只会解析 c 文件，虽然它本身是支持 c++、java 的，这时我们可以通过指定文件列表的方式使其解析我们需要的文件，默认的它会先查找名为 cscope.files 的文件，我们还可以通过 -i 参数来指定文件列表。如下是生成 cscope.files 文件的一个例子。

<p>
<pre name="code" class="cpp">
find . -regex '.*\.c\|.*\.cpp\|.*\.h' > cscope.files
</pre>
</p>

然后再执行 <code>cscope -bq</code> 命令即可，或者如果指定文件列表的话 <code>cscope -bq -i cscope.files</code>。

生成好数据库文件后，还需要在 vim 中把数据库加进来，这部分工作我们写在 .vimrc 文件中了，这样每次打开 vim 后就会去检查当前目录下的 cscope 数据库文件并加载进来，十分的方便，关于它的使用，根据官方的介绍我们在 .vimrc 为其加入了如下快捷键设置，详细内容请参见我的 .vimrc 文件。

这样例如我们需要查找一个函数在哪里被调用了，只需要光标移动到该函数名，然后同时按下 Ctrl+\ 然后再按下 c 即可，需要注意的是，结果是出现在一个临时窗口里，当选择完后它会关闭，所以我们可以再加入如下设置，这样 cscope 的结果就会放到 vim 的 quickfix 窗口保存起来，我们可以通过 cw 命令来开启它。

<code>set cscopequickfix=s-,c-,d-,i-,t-,e-</code>

同样的关于它的安装我们可以参考 ctags 的方式，在 Ubuntu 下选择 apt-get 的方式，或在没有 root 权限的系统上选择源码编译的方式安装到我们的用户目录下。

## 几个称手的插件

这里我使用了如下几个插件，基本能满足我需要了。

<ul>
<li>Vundle.vim 用于方便的管理插件</li>
<li>vim-colors-solarized 主题插件</li>
<li>ctrlp.vim 用于搜索和打开工程内的文件</li>
<li>taglist.vim 提供类似 Source Insight 中 Symbol 窗口的功能</li>
<li>AutoComplPop、OmniCppComplete 代码自动补全插件</li>
</ul>

关于插件的安装我们这里使用了 Vundle 这个插件来管理其它的 vim 插件，所以只需要装好 Vundle 就可以方便的安装其他插件了，
而且安装 Vundle的方法很简单，只需执行如下命令即可。

<code>git clone https://github.com/VundleVim/Vundle.vim.git ~/.vim/bundle/Vundle.vim</code>

待安装完成，我们再使用 Vundle 来安装其他插件，首先进入 vim，然后执行 <code>PluginInstall</code> 它既会开始安装 .vimrc 文件中指定的插件，直到显示 Done 后完成安装，使用 q 退出即可。

关于这几个插件的详细介绍和使用我这边就不再多说了，用名字很容易在网上查到。另外关于代码自动补全的插件还有个更厉害的叫 YouCompleteMe 但鉴于其安装麻烦，依赖多，最后我还是放弃了，想想我要在那些老服务器上用起来的话那真是太麻烦了，好在上述两个也基本够我使用的了，到此关于 vim 的配置就到就结束，希望日后不用再折腾它了，哈哈。

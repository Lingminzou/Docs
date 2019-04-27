 问题现象
------------
新项目开始，QA 报了个问题，蓝牙遥控器配对后按键就不响应了.......

好吧,很明显蓝牙遥控器没配对之前肯定是走红外过去的，配对以后就变成走蓝牙了，现在是按键走蓝牙有问题，那就看看吧。

开始 Debug
------------
首先想到用 getevent 试试，输入后发现一切正常，按键后有键值打印出来，那就再往上看看。

然后通过 logcat 直接 grep keycode 发现 key 经过 Android input 到 windowsManger 后 keycode 变成了 0x00，好吧，难怪会没有响应。

那从什么时候 keycode 从有值变成 0x00 呢？

然后有请 printf 大法，同时把 android input 相关模块的全部打印开出来，再加了几条打印。

编译 push 进去最后发现是在按键转换的时候进去的时候还正常出来就变 0x00 了。

到这里大概能猜到是 keymap 文件出了问题了。

分析解决
------------
在 android 里面按键的转换是由 KeyLayoutMap 文件完成的，我们可以为每个输入设备添加 KeyLayoutMap 文件， 也可以直接使用系统提供的默认 KeyLayoutMap 文件 Generic.kl。

当系统识别到一个新的输入设备后，便会去为其找到合适的 kl 文件，它的查找顺序如下：

	/odm/usr/keylayout/Vendor_XXXX_Product_XXXX_Version_XXXX.kl
	/vendor/usr/keylayout/Vendor_XXXX_Product_XXXX_Version_XXXX.kl
	/system/usr/keylayout/Vendor_XXXX_Product_XXXX_Version_XXXX.kl
	/data/system/devices/keylayout/Vendor_XXXX_Product_XXXX_Version_XXXX.kl
	/odm/usr/keylayout/Vendor_XXXX_Product_XXXX.kl
	/vendor/usr/keylayout/Vendor_XXXX_Product_XXXX.kl
	/system/usr/keylayout/Vendor_XXXX_Product_XXXX.kl
	/data/system/devices/keylayout/Vendor_XXXX_Product_XXXX.kl
	/odm/usr/keylayout/DEVICE_NAME.kl
	/vendor/usr/keylayout/DEVICE_NAME.kl
	/system/usr/keylayout/DEVICE_NAME.kl
	/data/system/devices/keylayout/DEVICE_NAME.kl
	/odm/usr/keylayout/Generic.kl
	/vendor/usr/keylayout/Generic.kl
	/system/usr/keylayout/Generic.kl
	/data/system/devices/keylayout/Generic.kl

当没有特别指定的 kl 文件后，便会最终匹配到 Generic.kl 文件。

了解了上面的机制后那我们来看下我们的蓝牙遥控器匹配后是使用了哪个 kl 文件呢？

这里可以使用 dumpsys input 来查看当前系统输入设备的情况，这里 dump 出来的结果有点奇怪 kl 文件为空，如下图：

![](https://github.com/Lingminzou/Docs/blob/master/pic/20190427/%E6%B2%A1%E5%8A%A0%E8%BD%BD%E5%88%B0%20Generic.kl.png?raw=true)

这就奇怪了，这里也说明了为什么 map 出来的 key 为 0x00 了，没有 kl 文件嘛。

那我们提供一个不就好了，但等等，按照前面的规则它不是应该找到 Generic.kl 文件吗，我们并没有为我们的蓝牙遥控器提供单独的 kl 文件。

而且蓝牙遥控器配对后应该是一个标准的 HID 输入设备，也应当是使用 Generic.kl 文件的，难道 Generic.kl 有问题？？？

查了下上传记录，果然有人修改过 Generic.kl 文件，那他是哪里搞错了呢？

这里我们在 source.android.com 的 input 介绍上看到系统是有提供一个按键映射检测工具 validatekeymaps。

它在完成 andorid 的全编译后就可以直接输入使用，参数指定 kl 文件。

那让我们来试试我们的 Generic.kl 文件，果然提示有问题，出现了重复的按键定义，如下图：

![](https://github.com/Lingminzou/Docs/blob/master/pic/20190427/%E9%AA%8C%E8%AF%81%E6%8C%89%E9%94%AE%E5%B8%83%E5%B1%80%E6%96%87%E4%BB%B6%E6%AD%A3%E7%A1%AE%E6%80%A7.png?raw=true)

修正所有错误提示后，再把 Generic.kl 文件 push 到机器里面，果然能用了，哈哈哈

然后 dumpsys input 看下 kl 文件也被正确加载到了，如下图：

![](https://github.com/Lingminzou/Docs/blob/master/pic/20190427/%E6%AD%A3%E5%B8%B8%E5%8A%A0%E8%BD%BD%E5%88%B0Generic.kl.png?raw=true)

到这问题就完美结局啦。

老司机有话说
------------
像这种按键无响应或按键响应不对的，如果没有对 android input 原生代码做过特别的客制化，那问题原因大多是出现在配置文件上，所以对于这类问题应该了解清楚配置文件的功效和副作用，便能有效应对此类问题。

推荐阅读：

	https://source.android.com/devices/input

扫码关注我了解更多

![](http://wx1.sinaimg.cn/large/9e169b75gy1fqcisgsbd7j2076076q3e.jpg)


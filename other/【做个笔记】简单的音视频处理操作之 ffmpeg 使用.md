## 一、FFmpeg 简介

FFmpeg 是一款开源的多媒体视频处理工具，它有非常强大的功能包括视频采集、视频格式转换、视频抓图、给视频加水印等。

许多视频播放处理软件多是基于 FFmpeg 开发，包括暴风影音、QQ影音、PotPlaye 等。

注：FFmpeg 是以命令行的方式运行的，所以我们要在终端操作它。

## 二、音视频基础知识

1、封装：封装即我们常说的文件格式 mp4、avi、mkv、rmvb。它像是一个容器，里面会包含视频、音频、字幕等信息。

2、编码：编码的目的是为了存储和压缩。视频和音频会采用不同的编码方式编码后放进容器中。

常见的视频编码方式有h.264(HVC)、h.265(HEVC)、MPEG4、VC-1等，常见的音频编码方式有AAC、AC-3、MP3等。

注：封装格式会限制编码方式，如 rmvb 在视频方面只支持 RealVideo 8, 9, 10 编码的视频，而 mkv 则支持所有编码方式的音视频。

## 三、FFmpeg 的安装

我们到 FFmpeg 官网下载它，如下链接，它提供了 Linux、Windows、Mac OS 常见的三个平台的安装包。

	https://www.ffmpeg.org/download.html

为了简单我们选择下载静态编译的包，这意味它可以放到任何位置多能正确的被执行。

但为了方便我们通常希望在任何位置打开终端都能运行 FFmpeg，这样我们可以把 FFmpeg 的存放路径添加至系统环境变量即可。

如在 Windows 平台，我们先将下载的静态编译包解压放置到 `C:\Program Files\FFmpeg` 目录，

然后将其可执行文件路径 `C:\Program Files\FFmpeg\bin` 添加至系统环境变量里，添加方法如下链接介绍。

	https://jingyan.baidu.com/article/47a29f24610740c0142399ea.html

之后我们在任何位置打开终端都可以执行 FFmepg 命令了，Linux 和 Mac OS 平台类似，先下载，然后添加环境变量，即可完成安装。

## 四、简单的音视频处理操作

### 4.1 视频剪辑

任务：截取 A 视频的 00:00 - 00:30 和 02:00 - 02:30 两个片段重新拼接成新的视频 B。

step1：截取 A 视频 00:00 - 00:30 片段输出为 A1

	/*	
	 * -ss 为指定起始时间
	 * -t  为指定持续的时长
	 * -i  为指定输入的视频文件
	 * -vcodec 为指定输出视频的编码格式，这里为 copy 即不改变原来的编码格式，可加快输出速度
	 * -acodec 为指定输出音频的编码格式，copy 意义同上
	 */	
	
	ffmpeg -ss 00:00:00 -t 00:00:30 -i A.mp4 -vcodec copy -acodec copy A1.mp4

step2：截取 A 视频 02:00 - 02:30 片段输出为 A2

	ffmpeg -ss 00:02:00 -t 00:00:30 -i A.mp4 -vcodec copy -acodec copy A2.mp4

setp3：拼接 A1 和 A2 输出为 B

	ffmpeg -f concat -i list.txt -c copy B.mp4

其中 list.txt 的内容如下：

	file ./A1.mp4
	file ./A2.mp4

### 4.2 提取或替换视频背景音

任务：提取视频 A 的背景音输出为 B.mp3

	ffmpeg -y -i A.mp4 -vn B.mp3

任务：用 B.mp3 替换视频 C 中的背景音

step1：提取 C 中的视频，去除背景音，输出为无声的视频 D

	ffmpeg -i C.mp4 -vcodec copy -an D.mp4

step2：将新的背景音 B 合成到视频 D 中生成新的视频 F

	ffmpeg -i D.mp4 -i B.mp3 -vcodec copy -acodec copy F.mp4

### 4.3 视频转码

这里有两个转换，一个封装的转换，一个编码的转换，我们常说的格式转换即为封装的转换，

但编码格式有时受限于封装格式，所以有时也需要发生编码格式的转换。

若不想关心音视频的编码方式只想换种封装格式，则可不指定编码方式，如下将 mp4 转为 mkv

	ffmpeg -i input.mp4 output.mkv

当然我们知道 mkv 是支持任意的音视频编码方式，所以我们可以指定以 copy 的方式来转换可以加快输出速度，如下：

	ffmepg -i input.mp4 -vcodec copy -acodec copy output.mkv

另外的要是我们的播放器受限只支持 h.264 编码方式的视频播放，那我们就可以指定输出的视频编码方式，如下：

	ffmpeg -i input.mp4 -vcodec h264 -acodec copy output.mkv

其他的我们不仅可以转换视频的封装格式，和编码方式，

我们也可以通过制定其他参数在改变视频的分辨率、码率、帧率等，了解更多可查看器帮助文档。

## 五、小技巧

1、限制 ffmpeg 的 cpu 使用率

视频处理是一件非常消耗计算力的事情，通常 ffmpeg 会消耗 100% 的 cpu 来做这些事情，

如果你使用的是笔记本，而视频又比较大，100% 的 cpu 使用持续不了几分钟，笔记本的风扇就要狂转，

所以我们情愿多花点时间也不愿意把电脑热坏了，这可以通过限制 ffmpeg 的 cpu 使用率来降低笔记本的负担。

我们通过加入 -threads 参数来实现这一点，如限制 cpu 使用率在 50%，则我们这样做：

	ffmpeg -i input.mp4 -threads 2 -vcodec h264 -acodec copy output.mkv

欢迎关注我的个人微信公众号了解更多。

![](https://mmbiz.qpic.cn/mmbiz_jpg/yhkQoyL4zW4g1wWs8nA54YpcTPFKjdmTofG0Ib3sHl2zYNibuKMqaN060Cj1iax3738eumYcfCv5bm5oqU9bhBrg/0?wx_fmt=jpeg)
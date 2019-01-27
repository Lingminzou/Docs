
## 为什么我打包的不行

前段时间在一个项目上，系统的升级包是用 tar 命令打包出来的，即把需要升级的分区等文件通过 tar 打包后通过特制的 bootload 升级系统，既然如此，有时候我只需要更新一个不需要编译的文件，就想直接修改升级包的内容然后执行升级，然后我通过 tar 命令解压出来，修改完文件再打包回去，结果升级不了，提示升级错误，让我点困惑。

好在能看到升级程序的源码，通过查看源码后发现，原来 tar 档可以做手脚的，可以得到一个特殊的 tar 档却又不影响正常的解压。

## tar 档的构成

tar 档是没有进行压缩的，其主要是将多个文件合并成一个文件，设计的最初目的是为了备份文件到磁带上（tape archive）。

合并时每个被合并的文件被附加上一个 512 字节的描述块（tar Header Block），其描述了该文件的名字，大小，所有人等信息，其组织方式如下

	|--- File1 Header Block ---|--- File1 ---|--- File2 Header Block ---|--- File2 ---|--- tar End Block ---|

tar Header Block 的结构可以通过查看 busybox 的源码找到，如下：

	/* POSIX tar Header Block, from POSIX 1003.1-1990  */
	#define TAR_BLOCK_SIZE 512
	#define NAME_SIZE      100
	#define NAME_SIZE_STR "100"
	typedef struct tar_header_t {     /* byte offset */
		char name[NAME_SIZE];     /*   0-99 */
		char mode[8];             /* 100-107 */
		char uid[8];              /* 108-115 */
		char gid[8];              /* 116-123 */
		char size[12];            /* 124-135 */
		char mtime[12];           /* 136-147 */
		char chksum[8];           /* 148-155 */
		char typeflag;            /* 156-156 */
		char linkname[NAME_SIZE]; /* 157-256 */
		/* POSIX:   "ustar" NUL "00" */
		/* GNU tar: "ustar  " NUL */
		/* Normally it's defined as magic[6] followed by
	 	* version[2], but we put them together to save code.
	 	*/
		char magic[8];            /* 257-264 */
		char uname[32];           /* 265-296 */
		char gname[32];           /* 297-328 */
		char devmajor[8];         /* 329-336 */
		char devminor[8];         /* 337-344 */
		char prefix[155];         /* 345-499 */
		char padding[12];         /* 500-512 (pad to exactly TAR_BLOCK_SIZE) */
	} tar_header_t;

关于它们更详细的解释可参考如下链接内容：

	https://www.ibm.com/support/knowledgecenter/sv/ssw_aix_71/com.ibm.aix.files/tar.h.htm

## 柳暗花明

其秘密就藏在 Header Block 的最后 12 字节中，这 12 字节并未真正使用到，实际用全零进行填充，这是一个可以客制化的地方。

知道了客制化的地方后还有问题需要解决，即 chksum 字段，如果只修改这里不修改 chksum 那么是无法正常解包的。

chksum 字段是对 Header Block 的一个校验，方式是将 512 字节累加起来，然后转成八进制的字串写入这个字段，在累加的时候 chksum 字段用 8 个空格字符替代。

如下我用 python 做了实现：

	#!/usr/bin/env python3
	# -*- codeing: utf-8 -*-
	
	import os,sys
	
	class tarHeader(object):
	
		def __init__(self, path, header):
			self.path = path
			self.header = bytearray(header)
			self.name = header[:100].decode('ascii')
			self.chksum = header[148:156]
			self.uname = header[265:297].decode('ascii')
			self.gname = header[297:329].decode('ascii')
	
		def print_info(self):
			print('file name: ' + self.name)
			print('tar header chksum: ' + self.chksum.decode())
			print('file owner: ' + self.uname)
			print('file group: ' + self.gname)
	
		def __refresh_chksum(self):                       # add '__' mean is private
			self.header[148:156] = b'        '
			chksum = 0
			for data in self.header:
				chksum += data
			chksum = str(oct(chksum)).replace('o', '')    # oct like: 0oXXXX
			temp = str.encode(chksum) + b'\x00'           # string to byte adn add string end byte 0x00
			self.header[148:(148 + len(temp))] = temp
			#print(self.header[148:156])
			with open(self.path, 'r+b') as f:             # user '+' update file
				f.write(self.header)
	
		def padding(self, data):                          # data: bytes
			if len(data) >= 12:
				print('Padding content needs to be less than 12 bytes...')
			else:
				self.header[500:(500 + len(data))] = data
				self.__refresh_chksum()
	
	if __name__=='__main__':
		if len(sys.argv) == 1:
			print('Plase input tar file...')
			sys.exit(0)
		else:
			tarFile = sys.argv[1]
			
		with open(tarFile, 'rb') as f:
			header = tarHeader(tarFile, f.read(512))
			header.padding(b'my magic\x00')
	

这样我们就能客制化一个特殊的 tar 档，比如在填充区填入独特的 magic 字段用于判断文件的有效性等。

扫码关注我了解更多

![](http://wx1.sinaimg.cn/large/9e169b75gy1fqcisgsbd7j2076076q3e.jpg)
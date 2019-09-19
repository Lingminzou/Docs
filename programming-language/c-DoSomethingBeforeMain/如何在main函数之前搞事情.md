很久没出来搞事情了，今天我们来在 main 函数之前搞搞事情。

写汇编的大神看到可能要笑了，我一直都在 main 函数前搞事情啊，要不是我搞好运行时环境，设置好栈，看你怎么运行哦。

那写 c 不同啦，想要在 main 前搞事情，那得借助 gcc 的黑魔法 **\_\_attribute\_\_**。

话不多说，直接上码：

	#include <stdio.h>
	#include <stdint.h>
	
	__attribute__((constructor)) void test01(void)
	{
	    printf("gao shi qing before main...\r\n");
	}
	
	int main(int argc, char *argv[])
	{
	    printf("hello world!!!\r\n");
	
	    return 0;
	}

使用 gcc 编译后运行会得到如下结果：

	gao shi qing before main...
	hello world!!!

没错，就是神奇的 **\_\_attribute\_\_((constructor))** 使得 test01 函数能在 main 函数之前被调用。

神奇是神奇，那有什么用处呢？

**该黑魔法在使用的时候任何文件任何地方都能生效，对于一个有初始化函数的模块来说，可以对其使用该黑魔法，这样就省去了自己显性的在 main 函数过程中调用它的初始化函数，让 gcc 来帮我们自动完成，对于使用该模块的人来说也省心省力。**

其他还有什么用处各位可以发挥自己的脑洞了。

有 constructor 那当然就有 destructor。聪明的你肯定能想到被 **\_\_attribute\_\_((destructor))** 加持的函数肯定是会自动的在 main 函数执行完毕后被自动调用。

有人可能会举手了，如果有很多有 **\_\_attribute\_\_((constructor))** 加成的函数它们谁先执行呢？

gcc 也为我们想好了，它提供了让我们设置优先级的功能，如下：

	__attribute__((constructor(101))) void test01(void)
	{
	    printf("test01 gao shi qing before main...\r\n");
	}
	
	__attribute__((constructor(102))) void test02(void)
	{
	    printf("test02 gao shi qing before main...\r\n");
	}

这样写清楚后，test01 就会优先于 test02 执行，使用 destructor 时同理，有点不同的是 destructor 时数值越大的越先得到执行，和 constructor 正好相反。

为什么是从 101 开始的呢，因为我填 1 的时候被 gcc 警告了，1 到 100 它要留着用。

说完 c 那我们再来说说 c++ 吧。

c++ 当然也可以使用 **\_\_attribute\_\_** 的黑魔法啦，但人家还有对象。

没错，就是对象，对象有构造和析构函数，我们定义了一个全局的对象后，**那它的构造函数和析构函数就会分别在 main 函数之前和之后被调用了（好像很厉害的样子，谁让人家有对象）**。

上码试试：

	#include <stdio.h>
	#include <stdint.h>
	
	__attribute__((constructor(101))) void test01(void)
	{
		printf("test01 gao shi qing before main...\r\n");
	}
	
	__attribute__((constructor(102))) void test02(void)
	{
		printf("test02 gao shi qing before main...\r\n");
	}
	
	class test{
	public:
		test()
		{
			printf("test object gao shi qing before main...\r\n");
		}
	
		~test()
		{
			printf("test object gao shi qing after main...\r\n");
		}
	};
	
	test test_instance;  // global object
	
	int main(int argc, char *argv[])
	{
		printf("hello world!!!\r\n");
	
		return 0;
	}
	
	__attribute__((destructor(102))) void test03(void)
	{
		printf("test03 gao shi qing after main...\r\n");
	}
	
	__attribute__((destructor(101))) void test04(void)
	{
		printf("test04 gao shi qing after main...\r\n");
	}

最终它的输出如下：

	test01 gao shi qing before main...
	test02 gao shi qing before main...
	test object gao shi qing before main...
	hello world!!!
	test object gao shi qing after main...
	test03 gao shi qing after main...
	test04 gao shi qing after main...

好了，今天就搞到这里了，搞的不好的地方欢迎留言指正，下次见。

扫码关注我了解更多

![](http://wx1.sinaimg.cn/large/9e169b75gy1fqcisgsbd7j2076076q3e.jpg)


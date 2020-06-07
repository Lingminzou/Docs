假设我们需要提供一个 so 给其他人用，并这个 so 需要执行初始化动作，你会怎么来设计？

### 方法1：简单粗暴

这种方法应该是最容易想到到的，直接简单粗暴，提供接口，让调用者来处理，示例如下：

	/****** method1.c ******/	
	void method1_init(void)
	{
	    printf("calling method1_init...\r\n");
	}
	
	void method1_dosomething(void)
	{
	    printf("calling method1_dosomething...\r\n");
	}


	/****** main.c ******/
	extern void method1_init(void);
	extern void method1_dosomething(void);
	
	int main(int argc, char *argv[])
	{
	    printf("test main run...\r\n");
	
	    method1_init();
	    method1_dosomething();
	
	    return 0x00;
	}


### 方法2：安能辨我是雄雌

直接上码再说吧。

	/***** method2.c *****/
	void __method2_init(void);
	void __method2_dosomething(void);
	
	static void (*fun)(void) = __method2_init;
	
	void __method2_init(void)
	{
	    printf("calling method2_init...\r\n");
	
	    fun = __method2_dosomething;
	
	    fun();
	}
	
	void __method2_dosomething(void)
	{
	    printf("calling method2_dosomething...\r\n");
	}
	
	void method2_dosomething(void)
	{
	    fun();
	}

	/****** main.c ******/
	extern void method2_dosomething(void);
	
	int main(int argc, char *argv[])
	{
	    printf("test main run...\r\n");
	
	    method2_dosomething();
	
	    return 0x00;
	}

这里的 method2_dosomething 接口既是提供服务的接口也是初始化的接口。

只会在第一次掉用它的时候执行 init 动作，这里利用了函数指针，对调用者来说非常的友好。

严格来做这里的静态函数指针变量相当于引入了状态，在多线程调用过程中需要加锁。

### 方法3：GCC 的黑魔法

这里用到了 gcc 的一个设置函数属性的功能，让其在加载过程自动的被调用，如下：

	/***** method3.c *****/
	__attribute__((constructor)) void __method3_init(void)
	{
	    printf("calling method3_init...\r\n");
	}
	
	void method3_dosomething(void)
	{
	    printf("calling method3_dosomething...\r\n");
	}

	/****** main.c ******/
	extern void method3_dosomething(void);
	
	int main(int argc, char *argv[])
	{
	    printf("test main run...\r\n");
	
	    method3_dosomething();
	
	    return 0x00;
	}

被 <code>\_\_attribute\_\_((constructor))</code> 修饰的函数会先于 main 函数执行，同时会自动的被调用。

这种方法得益于编译器的支持，对于调用者来说也是非常友好，同时不需要像方法2一样在多线程使用中需要用到锁的机制，此种方法实现更简单。

好了，这里主要介绍三种怎么处理一个模块初始化的方法，三种各有优缺点吧，适合才是最好的，项目中选择最适合自己实际情况的方法就好。

如果你有更好的方法或想法，欢迎评论留言，谢谢！


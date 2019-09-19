
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



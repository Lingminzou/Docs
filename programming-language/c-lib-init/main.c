
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

extern void method1_init(void);
extern void method1_dosomething(void);

extern void method2_dosomething(void);

extern void method3_dosomething(void);

int main(int argc, char *argv[])
{
    printf("test main run...\r\n");

    method1_init();
    method1_dosomething();

    method2_dosomething();

    method3_dosomething();

    return 0x00;
}



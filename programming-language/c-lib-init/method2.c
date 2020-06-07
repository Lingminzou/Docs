
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

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



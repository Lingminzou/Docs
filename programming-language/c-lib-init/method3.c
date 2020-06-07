
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

__attribute__((constructor)) void __method3_init(void)
{
    printf("calling method3_init...\r\n");
}

void method3_dosomething(void)
{
    printf("calling method3_dosomething...\r\n");
}


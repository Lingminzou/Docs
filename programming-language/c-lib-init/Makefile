
CC = gcc

.PHONY: all bin libmethod1 libmethod2 libmethod3 clean

all: bin libmethod1 libmethod2 libmethod3

bin: libmethod1 libmethod2 libmethod3
	$(CC) -g -rdynamic main.c -L. -lmethod1 -lmethod2 -lmethod3 -Wl,-rpath=. -o test

libmethod1:
	$(CC) -g -rdynamic method1.c -fPIC -shared -o libmethod1.so

libmethod2:
	$(CC) -g -rdynamic method2.c -fPIC -shared -o libmethod2.so

libmethod3:
	$(CC) -g -rdynamic method3.c -fPIC -shared -o libmethod3.so

clean:
	rm -rf test libmethod1.so libmethod2.so libmethod3.so



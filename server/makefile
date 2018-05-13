CC=g++
CFLAGS=-I. -I../common
DEPS = cmechatServer.h ../common/cmechatLogger.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cmechatServer: main.o cmechatServer.o ../common/cmechatLogger.o
	$(CC) -o cmechatServer main.o cmechatServer.o ../common/cmechatLogger.o -I. -I../common


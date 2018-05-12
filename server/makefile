CC=g++
CFLAGS=-I.
DEPS = cmechatServer.h cmechatLogger.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cmechatServer: cmechatServer.o ../common/cmechatLogger.o
	$(CC) -o cmechatServer cmechatServer.o ../common/cmechatLogger.o -I. -I../common


CC=g++
CFLAGS=-I. -I../common
DEPS = cmechatClient.h ../common/cmechatLogger.h

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

cmechatClient: main.o cmechatClient.o ../common/cmechatLogger.o ../common/cmechatCommandLineParser.C
	$(CC) -o cmechatClient main.o cmechatClient.o ../common/cmechatLogger.o ../common/cmechatCommandLineParser.C -I. -I../common


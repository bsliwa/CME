CC=g++
CFLAGS=-I. -I../common -std=c++11 -pthread
DEPS = cmechatServer.h cmechatUserManager.h cmechatUser.h ../common/cmechatLogger.h

%.o: %.c $(DEPS)
	$(CC) -std=c++11 -pthread -c -o $@ $< $(CFLAGS)

cmechatServer: main.o cmechatServer.o cmechatUser.o cmechatUserManager.o ../common/cmechatLogger.o ../common/cmechatCommandLineParser.C
	$(CC) -o cmechatServer main.o cmechatServer.o ../common/cmechatLogger.o ../common/cmechatCommandLineParser.C -I. -I../common


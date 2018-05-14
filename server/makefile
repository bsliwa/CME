CC=g++
CFLAGS=-I. -I../common -std=c++11 -pthread
DEPS = cmechatServer.h cmeChatUserManager.h cmechatUser.h ../common/cmechatLogger.h

cmechatServer : main.C cmechatUser.C cmechatServer.C cmechatUserManager.C ../common/cmechatCommandLineParser.C ../common/cmechatLogger.C
	$(CC) -o cmechatServer -std=c++11 -pthread main.C cmechatServer.C cmechatUser.C cmechatUserManager.C ../common/cmechatCommandLineParser.C ../common/cmechatLogger.C $(CFLAGS)


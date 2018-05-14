#include <iostream>

#include "cmechatServer.h"
#include "cmechatUserManager.h"

int main(int argc, char *argv[])
{
	cmechatServer server("./server.log", argc, argv);

	server.openServer();

	server.listen();

}


#include <iostream>

#include "cmechatServer.h"

int main(int argc, char *argv[])
{
	cmechatServer server("./server.log", argc, argv);

	server.openServer();

	server.listen();

}


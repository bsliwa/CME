#include "cmechatClient.h"

int main(int argc, char *argv[])
{
    cmechatClient client("./client.log", argc, argv);

    client.getUsername();

    client.connectToServer();
    
    client.registerUsername();

    client.runChat();
}
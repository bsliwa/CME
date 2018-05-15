#ifndef _CMECHAT_INTERFACE_
#define _CMECHAT_INTERFACE_

#define MAX_MSG_LEN 280
#define MAX_USERNAME_LEN 28


enum CMECHAT_INTERFACE_OPCODES
{
    CMECHAT_OPCODE_NEWUSER,
    CMECHAT_OPCODE_BROADCAST_MESSAGE,
    CMECHAT_OPCODE_UNICAST_MESSAGE
};

struct cmechatMessageNewuser
{
    unsigned char opcode;
    char username[MAX_USERNAME_LEN];
};

struct cmechatMessageBroadcastMessage
{
    unsigned char opcode;
    char sourceUsername[MAX_USERNAME_LEN];
    char body[MAX_MSG_LEN];
};

struct cmechatMessageUnicastMessage
{
    unsigned char opcode;
    char targetUser[MAX_USERNAME_LEN];
    char body[MAX_MSG_LEN];
};

#endif

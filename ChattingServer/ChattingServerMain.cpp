#include "pch.h"

int main(int argc, char* argv[])
{
    ChatServerService* chat_service = xnew ChatServerService();
    
    chat_service->init(1, 1);
    chat_service->open("127.0.0.1", 7777, []()
    {
        return xnew ChatClientSession;
    }, 1);


    while (true)
    {
        
    }
}

#include "pch.h"
#include <iostream>


int main()
{
    ChatClientService service;
    service.init(1);
    service.open("127.0.0.1", 7777, []() { return xnew chat_client::ChatServerSession; });

    while (true)
    {
    }
}

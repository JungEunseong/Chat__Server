#include "pch.h"
#include <iostream>


int main()
{
    ChatClientService service;
    service.open("127.0.0.1", 9999, []() { return xnew ChatServerSession; });

    while (true)
    {
    }
}

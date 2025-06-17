#include "pch.h"

#include <string>

int main(int argc, char* argv[])
{
    DummyChattingClientService service;
    service.init(1);

    std::wcout << L"seccion count: ";

    std::wstring session_count;
    std::wcin >> session_count;
    
    service.open("127.0.0.1", 7777, []() { return xnew dummy_client::ChatServerSession; }
        , stoi(session_count));

    while (true)
    {
    }
}

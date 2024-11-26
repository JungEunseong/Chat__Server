#pragma once
#include <iostream>
#include <WinSock2.h>
#include <MSWSock.h>
#include <ws2tcpip.h>
#include <mswsock.h>
#pragma comment(lib,"mswsock.lib")
#pragma comment(lib, "ws2_32.lib")

#include <vector>
#include <concurrent_queue.h>
#include <thread>
#include <functional>

#include "Network/iTask.h"
#include "Network/NetworkIO.h"
#include "Network/NetworkCore.h"
#include "Network/RecvBuffer.h"

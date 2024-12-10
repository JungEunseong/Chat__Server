#pragma once
#include <iostream>
#include <WinSock2.h>
#include <MSWSock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <vector>
#include <concurrent_queue.h>
#include <thread>
#include <functional>

#include "iTask.h"
#include "NetworkIO.h"
#include "NetworkCore.h"

#pragma once

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "winmm.lib")

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <vector>
#include <unordered_map>
#include <iostream>
#include <chrono>
#include <thread>
#include <queue>
#include <stack>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <memory>
#include <string_view>
#include <exception>

#include "Util/Logger/LoggerLib.h"

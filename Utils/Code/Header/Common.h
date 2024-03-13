#pragma once

#if defined(_WIN32) || defined(_WIN64)

#include <winSock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define MAXLINE MAX_PATH

#elif defined(__linux__)

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

define MAXLINE PATH_MAX

#else //VxWorks

#include <vxWorks.h>
#include <sockLib.h>
#include <inetLib.h>

#endif

#include <iostream>
#include <string>
#include <malloc.h>

#define MAX_NUM_CLIENTS 10
#define EXIT_CODE 999999
#define OK_OP 0
#define READ_DONE 1



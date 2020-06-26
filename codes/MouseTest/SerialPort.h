#pragma once

#include <Windows.h>

HANDLE SerialPortOpen(const char* port, int baudrate);
int SerialPortSend(HANDLE hcom, const void* buf, int len);
int SerialPortReceive(HANDLE hcom, void* buf, int len);

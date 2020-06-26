/*
 * Copyright (C) 2020 The AXiG Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.

 * File: SerialPort.cpp
 * Version: 1.00
 * Author: Herbert H. <bgameapp@gmail.com>
 * Date: 2020/6/26
 */

#include <Stdio.h>
#include "SerialPort.h"

// Open and configure the specified serial port.
// Return a handle on success, or NULL on failure.
HANDLE SerialPortOpen(const char* port, int baudrate)
{
	HANDLE hcom = NULL;
	
	// open serial port
	hcom = CreateFileA(port, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hcom == INVALID_HANDLE_VALUE) {
		printf("Failed to open [%s]\n", port);
		return NULL;
	}
	else {
		printf("Connected to %s\n", port);
	}
	
	// setup input/output buffer size
	if (SetupComm(hcom, 1024, 1024) == FALSE) {
		printf("SetupComm() failed\n");
		CloseHandle(hcom);
		return NULL;
	}

	// setup read/write timeout(ms)
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout = 1000;
	timeouts.ReadTotalTimeoutConstant = 1000;
	timeouts.ReadTotalTimeoutMultiplier = 100;
	timeouts.WriteTotalTimeoutConstant = 1000;
	timeouts.WriteTotalTimeoutMultiplier = 100;
	if (SetCommTimeouts(hcom, &timeouts) == FALSE) {
		printf("SetupCommTimeouts() failed\n");
		CloseHandle(hcom);
		return NULL;
	}

	// configure serial port parameters
	DCB state;
	memset(&state, 0, sizeof(state));
	state.DCBlength = sizeof(state);
	state.BaudRate = baudrate;
	state.ByteSize = 8;
	state.StopBits = ONESTOPBIT;
	state.Parity = NOPARITY;
	if (SetCommState(hcom, &state) == FALSE) {
		printf("SetCommState() failed\n");
		CloseHandle(hcom);
		return NULL;
	}

	// clear input/output buffer
	if (PurgeComm(hcom, PURGE_TXCLEAR | PURGE_RXCLEAR) == FALSE) {
		printf("PurgeComm() failed\n");
		CloseHandle(hcom);
		return NULL;
	}
	return hcom;
}

// Send characters to the serial port.
// Return number of bytes sent on success, or -1/-2 on failure.
int SerialPortSend(HANDLE hcom, const void* buf, int len)
{
	DWORD num;

	if (WriteFile(hcom, buf, len, &num, NULL) == FALSE) {
		printf("SerialPortSend() failed\n");
		return -1;
	}

	if (num != len) {
		printf("Failed to send all bytes\n");
		return -2;
	}

	return num;
}

// Receive characters from the serial port.
// Return number of bytes received, or -1 on failure/timeout.
int SerialPortReceive(HANDLE hcom, void* buf, int len)
{
	DWORD num;

	if (ReadFile(hcom, buf, len, &num, NULL) == FALSE) {
		printf("Failed to receive\n");
		return -1;

	}
	
	return num;
}


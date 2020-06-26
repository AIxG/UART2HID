#pragma once

#include <Windows.h>

int Keyboard(HANDLE hcom, unsigned char* key, unsigned char len, char control, char press);
int MouseRelative(HANDLE hcom, char x, char y, char press, char z);
int MouseAbsolute(HANDLE hcom, int x, int y, char press, char z);

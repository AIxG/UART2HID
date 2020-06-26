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

 * File: CH9329.h
 * Version: 1.00
 * Author: Herbert H. <bgameapp@gmail.com>
 * Date: 2020/6/26
 */

#pragma once

#include <Windows.h>

int Keyboard(HANDLE hcom, unsigned char* key, unsigned char len, char control, char press);
int MouseRelative(HANDLE hcom, char x, char y, char press, char z);
int MouseAbsolute(HANDLE hcom, int x, int y, char press, char z);

#include <stdio.h>
#include "CH9329.h"
#include "SerialPort.h"

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

// ASCII to Keycode
unsigned char ASCII_Keycode[256] = {
	//0     1     2     3     4     5     6     7       8     9     A     B     C     D     E     F
	0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,   0X2A, 0X2B, 0X00, 0X00, 0X00, 0X38, 0X00, 0X00, // 0
	0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00, 0X00,   0X00, 0X00, 0X00, 0X29, 0X00, 0X00, 0X00, 0X00, // 1
	0X2C, 0X1E, 0X34, 0X20, 0X21, 0X22, 0X24, 0X34,   0X26, 0X27, 0X25, 0X2E, 0X36, 0X2D, 0X37, 0X38, // 2
	0X27, 0X1E, 0X1F, 0X20, 0X21, 0X22, 0X23, 0X24,   0X25, 0X26, 0X33, 0X33, 0X36, 0X2E, 0X37, 0X38, // 3
	0X00, 0X04, 0X05, 0X06, 0X07, 0X08, 0X09, 0X0A,   0X0B, 0X0C, 0X0D, 0X0E, 0X0F, 0X10, 0X11, 0X12, // 4
	0X13, 0X14, 0X15, 0X16, 0X17, 0X18, 0X19, 0X1A,   0X1B, 0X1C, 0X1D, 0X2F, 0X31, 0X30, 0X00, 0X2D, // 5
	0X35, 0X04, 0X05, 0X06, 0X07, 0X08, 0X09, 0X0A,   0X0B, 0X0C, 0X0D, 0X0E, 0X0F, 0X10, 0X11, 0X12, // 6
	0X13, 0X14, 0X15, 0X16, 0X17, 0X18, 0X19, 0X1A,   0X1B, 0X1C, 0X1D, 0X2F, 0X31, 0X30, 0X35, 0X00, // 7

	0X80, 0X81, 0X82, 0X83, 0X84, 0X85, 0X86, 0X87,   0X88, 0X89, 0X8A, 0X8B, 0X8C, 0X8D, 0X8E, 0X8F, // 8
	0X90, 0X91, 0X92, 0X93, 0X94, 0X95, 0X96, 0X97,   0X98, 0X99, 0X9A, 0X9B, 0X9C, 0X9D, 0X9E, 0X9F, // 9
	0XA0, 0XA1, 0XA2, 0XA3, 0XA4, 0XA5, 0XA6, 0XA7,   0XA8, 0XA9, 0XAA, 0XAB, 0XAC, 0XAD, 0XAE, 0XAF, // A
	0XB0, 0XB1, 0XB2, 0XB3, 0XB4, 0XB5, 0XB6, 0XB7,   0XB8, 0XB9, 0XBA, 0XBB, 0XBC, 0XBD, 0XBE, 0XBF, // B
	0XC0, 0XC1, 0XC2, 0XC3, 0XC4, 0XC5, 0XC6, 0XC7,   0XC8, 0XC9, 0XCA, 0XCB, 0XCC, 0XCD, 0XCE, 0X4F, // C
	0X50, 0X51, 0X52, 0XD3, 0XD4, 0XD5, 0XD6, 0XD7,   0XD8, 0XD9, 0XDA, 0XDB, 0XDC, 0XDD, 0XDE, 0XDF, // D
	0XE0, 0XE1, 0XE2, 0XE3, 0XE4, 0XE5, 0XE6, 0XE7,   0XE8, 0XE9, 0XEA, 0XEB, 0XEC, 0XED, 0XEE, 0XEF, // E
	0XF0, 0XF1, 0XF2, 0XF3, 0XF4, 0XF5, 0XF6, 0XF7,   0XF8, 0XF9, 0XFA, 0XFB, 0XFC, 0XFD, 0XFE, 0XFF  // F
};

int Keyboard(HANDLE hcom, unsigned char* key, unsigned char len, char control, char press)
{
	// data[0] = WIN_R | ALT_R | SHIFT_R | CTRL_R | WIN_L | ALT_L | SHIFT_L | CTRL_L
	// data[1] = 0x00
	// data[2~7] = 6 pressed keys
	unsigned char buf[14] = { 0x57, 0xAB, 0x00, 0x02, 0x08, 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, 0x0C };
	int i, ret, sum;

	if (press == 1) {
		buf[5] = control;
		sum = 0x10C + control;
		for (i = 0; i < len; i++) {
			buf[i + 7] = ASCII_Keycode[key[i]];
			sum += buf[i + 7];
		}
		buf[13] = (unsigned char)(sum & 0xFF);
	}

	ret = SerialPortSend(hcom, buf, 14);
	if (ret != 14) {
		printf("Keyboard send error(%d)\n", ret);
		return ret;
	}
	ret = SerialPortReceive(hcom, buf, 7);
	if (ret != 7) {
		printf("Keyboard receive error(%d): ", ret);
		for (i = 0; i < ret; i++)
			printf("0x%02X ", buf[i]);
		printf("\n");
		return ret;
	}

	return -1;  // success
}

int MouseRelative(HANDLE hcom, char x, char y, char press, char z)
{
	// data[0] = 0x01
	// data[1] = 0 | 0 | 0 | 0 | 0 | M | R | L
	// data[2] = x
	// data[3] = y
	// data[4] = z
	unsigned char buf[11] = { 0x57, 0xAB, 0x00, 0x05, 0x05, 0x01,0x00,0x00,0x00,0x00, 0x0D };
	int i, ret, sum;

	if (x != 0 || y != 0 || press != 0) {
		buf[6] = press;
		sum = 0x10D + press;
		buf[7] = x;
		sum += x;
		buf[8] = y;
		sum += y;
		buf[9] = z;
		sum += z;
		buf[10] = (unsigned char)(sum & 0xFF);
	}

	ret = SerialPortSend(hcom, buf, 11);
	if (ret != 11) {
		printf("Relative mouse send error(%d)\n", ret);
		return ret;
	}
	ret = SerialPortReceive(hcom, buf, 7);
	if (ret != 7) {
		printf("relative mouse receive error(%d): ", ret);
		for (i = 0; i < ret; i++)
			printf("0x%02X ", buf[i]);
		printf("\n");
		return ret;
	}

	return -1;  // success
}

int MouseAbsolute(HANDLE hcom, int x, int y, char press, char z)
{
	// data[0] = 0x02
	// data[1] = 0 | 0 | 0 | 0 | 0 | M | R | L
	// data[2~3] = x
	// data[4~5] = y
	// data[6] = z
	unsigned char buf[13] = { 0x57, 0xAB, 0x00, 0x04, 0x07, 0x02,0x00,0x00,0x00,0x00,0x00,0x00, 0x0F };
	int i, ret, sum;
	int mx, my;

	mx = 4096 * x / SCREEN_WIDTH;
	my = 4096 * y / SCREEN_HEIGHT;

	buf[6] = press;
	sum = 0x10F + press;
	buf[7] = (unsigned char)(mx & 0xff);
	sum += buf[7];
	buf[8] = (unsigned char)((mx >> 8) & 0xff);
	sum += buf[8];
	buf[9] = (unsigned char)(my & 0xff);
	sum += buf[9];
	buf[10] = (unsigned char)((my >> 8) & 0xff);
	sum += buf[10];
	buf[11] = z;
	sum += z;
	buf[12] = (unsigned char)(sum & 0xFF);

	ret = SerialPortSend(hcom, buf, 13);
	if (ret != 13) {
		printf("Absolute mouse send error(%d)\n", ret);
		return ret;
	}
	ret = SerialPortReceive(hcom, buf, 7);
	if (ret != 7) {
		printf("absolute mouse receive error(%d): ", ret);
		for (i = 0; i < ret; i++)
			printf("0x%02X ", buf[i]);
		printf("\n");
		return ret;
	}

	return -1;  // success
}

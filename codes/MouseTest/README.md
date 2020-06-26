# MouseTest

This application uses keyboard to control the mouse emulated by the AIxG UART2HID.

![MouseTest demo](https://raw.githubusercontent.com/AIxG/UART2HID/master/images/MouseTest_demo.jpg)



## Instructions

1. Plug the HID and UART connectors into a computer.

2. Create a new *console* project `MouseTest` in Visual Studio.

3. Copy all the source files(*.cpp and *.h) to the project folder.

4. Add the source files to the project.

![MouseTest Solution Explorer](https://raw.githubusercontent.com/AIxG/UART2HID/master/images/MouseTest_solution.jpg)

5. Specify your serial port number in MouseTest.cpp, eg:
```C++
#define DEFAULTSERIALPORT	"COM4"
```

6. Compile and run!

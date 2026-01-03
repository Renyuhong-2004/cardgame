// proj.win32/main.h
#ifndef __MAIN_H__
#define __MAIN_H__

#include "platform/CCPlatformConfig.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32

// 声明 main 函数（控制台程序入口点）
int main(int argc, char* argv[]);

#endif // CC_PLATFORM_WIN32

#endif // __MAIN_H__
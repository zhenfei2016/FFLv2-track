/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  main.cpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/07/26 
*  https://github.com/zhenfei2016/FFL-v2.git
*
*
*/
#include <stdio.h>
#include <utils/FFL_MemoryWatch.hpp>

extern int serverMain();

extern "C" int main(int argc ,const char* argv[]) {	
	FFL::startMemoryWatch(); 	
    FFL_LogSetLevel(FFL_LOG_LEVEL_ALL);
	FFL_LOG_INFO("start track server");
	serverMain();
	FFL_LOG_INFO("quit track server");
	FFL_dumpMemoryLeak();	
	return 0;
}

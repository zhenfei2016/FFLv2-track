/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  trackServer.cpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/07/26 
*  https://github.com/zhenfei2016/FFL-v2.git
*  启动track http服务器
*
*/
#include <FFL.h>
#include <FFL_CommandHelper.h>
#include <net/FFL_NetServer.hpp>
#include <net/http/FFL_Http.hpp>

#include <utils/FFL_StringHelper.hpp>
#include "trackReader.hpp"


TrackFileReader  gTrackFile;
class HttpApiGetTrackListHandelr : public FFL::HttpApiHandler{
public:
    virtual void onHttpQuery(FFL::HttpConnect* conn, FFL::String& path, FFL::String& query){
        FFL::String json;     

		do {
			if (!gTrackFile.readJson(json, 1000)) {
				gTrackFile.reset();
				gTrackFile.readJson(json, 1000);
			}
		} while (0);
		

		if (json.size() > 0) {
			conn->createResponse()->writeJson(json);
		}
		conn->realese();
    }
    
};


class HttpTrackFileHandelr : public FFL::HttpFileHandler {
	//
	//  返回是否处理这个请求
	//
	virtual bool onHttpFile(FFL::HttpConnect* conn, const FFL::String& path) {
		FFL::sp<FFL::HttpFileResponse> res = new FFL::HttpFileResponse(conn);
		char processdir[1024] = { 0 };
		char processname[1024] = { 0 };
		FFL_getCurrentProcessPath(processdir, 1023, processname);
		strcat(processdir, path.c_str());
		res->response(processdir);
		conn->realese();
		return true;
	}
};

static int gExitFlag = 0;
void trackExit(const char* args, void* userdata) {	
	gExitFlag = 1;	
	printf("exit sys. \n");
}
static int quitFlag(void* userdata) {
	return gExitFlag;
}
void startService(const char* args, void* userdata) {	
	printf("startService. \n");
}
void restartService(const char* args, void* userdata) {	
	printf("restartService. \n");
}
void stopService(const char* args, void* userdata) {	
	printf("stopService. \n");
}

void openTool(const char* args, void* userdata) {
	printf("failed to openTool. \n");
	ShellExecuteA(NULL,"open", "http://127.0.0.1:5000/timeline.html", NULL, NULL, SW_SHOW);
}


static CmdOption  gCmdOption[] = {
	{ "exit",0,trackExit,"exit sys." },	
	//{ "start",0,startService,"start service" },
	//{ "restart",0,restartService,"re start service" },
	//{ "stop",0,stopService,"stop service" },
	{ "open",0,openTool,"open timeline tool" },	
	{ 0,0,0,0 },
};

int serverMain() {
	FFL::HttpConnectMgr mgr;    
    FFL::sp<FFL::HttpApiHandler> handler=new HttpApiGetTrackListHandelr();
    mgr.registerApi("/FFLv2?getTrackList", handler);		
	mgr.setFileHandler(new HttpTrackFileHandelr());

	FFL::TcpServer server(NULL,5000);
	server.setConnectManager(&mgr);
	server.start();
	//
	//  打印一下帮助函数
	//
	FFL_cmdUsage(gCmdOption);

	FFL_inputLooper(gCmdOption, 0, quitFlag);
	return 0;
}

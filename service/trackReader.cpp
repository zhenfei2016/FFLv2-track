/*
*  This file is part of FFL project.
*
*  The MIT License (MIT)
*  Copyright (C) 2017-2018 zhufeifei All rights reserved.
*
*  trackReader.cpp   
*  Created by zhufeifei(34008081@qq.com) on 2018/07/26 
*  https://github.com/zhenfei2016/FFL-v2.git
*
*  读取本地的track文件
*/
#include "trackReader.hpp"
#include <utils/FFL_StringHelper.hpp>
#include <utils/FFL_File.hpp>

TrackFileReader::TrackFileReader(){
	reset();
}
TrackFileReader::~TrackFileReader(){
	mFile.close();
}


static FFL::String toJson(const FFL::String& info) {
	FFL::String json;
	FFL::Vector<FFL::String> arr = FFL::StringSplit(info, ",");
	for (uint32_t i = 0; i < arr.size(); i++) {
		FFL::Vector<FFL::String> keyValue = FFL::StringSplit(arr[i], ":");
		if (keyValue.size() != 2) {
			continue;
		}

		FFL::String value;
		FFL::formatString(value, "\"%s\":\"%s\"", keyValue[0].c_str(), keyValue[1].c_str());

		if (!json.empty()) {
			json += "," + value;
		}
		else {
			json = value;
		}
	}
	return "{" + json + "}";
}
void TrackFileReader::reset() {
	char processdir[1024] = {0};
	char processname[1024] = { 0 }; 
	FFL_getCurrentProcessPath(processdir,1023, processname);

	strcat(processdir, "track.json");
	open(processdir);
}
//
//  返回是否成功， false表示失败了
//
bool TrackFileReader::readJson(FFL::String& json, int32_t maxLine) {
	bool bQuit = false;	

	FFL::Vector< FFL::String >  lineArray;
	while (!bQuit) {
		uint8_t buf[1024] = {};
		size_t readed = 0;
		if (FFL_OK != mFile.read(buf, 1023, &readed)) {
			bQuit = true;
		}
		if (readed <= 0) {
			bQuit = true;
		}

		uint8_t* start = buf;
		uint8_t* cur = buf;
		uint8_t* end = cur + readed;
		while (cur != end) {
			if (*cur == '\r') {
				mStream.append((const char*)start, cur - start);
				lineArray.push_back(toJson(mStream));
				mStream = "";
				start = cur + 1;
				if ((int32_t)lineArray.size() >=maxLine) {
					bQuit = true;
				}
			}
			cur++;
		}

		if (start != end) {
			mStream.append((const char*)start, end - start);
		}
	}
	
	if(lineArray.size()>0){
		for (uint32_t i = 0; i < lineArray.size(); i++) {
			if (json.empty()) {
				json += lineArray[i];
			}
			else {
				json += "," + lineArray[i];
			}
		}
		json = "{\"tarck\":[" + json + "]}";
		return true;
	}
	
	return false;
}

bool TrackFileReader::open(const char*path) {
	if (FFL::fileIsExist(path)) {
		mFile.close();
		if (FFL_OK == mFile.open(path)) {
			return true;
		}
	}
	
	printf("failed to open file: %s", path);
	return false;
}
void TrackFileReader::close() {
	mFile.close();
}



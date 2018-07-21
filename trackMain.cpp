#include <FFL.h>
#include <FFL_CommandHelper.h>
#include <net/FFL_NetServer.hpp>
#include <net/http/FFL_Http.hpp>

#include <utils/FFL_StringHelper.hpp>
#include <utils/FFL_File.hpp>

class TrackFileReader {
public:
	TrackFileReader() {
#ifdef WIN32
        const char* path="e:\\pipeline_track.txt";
#else
        const char* path="/Users/zhufeifei/work/movic/track-pipelinemsg.log";
#endif
		open(path);
	}

	~TrackFileReader() {
		mFile.close();
	}

	void open(const char* path) {
		if (!FFL::fileIsExist(path)) {
			mFile.close();
			mFile.open(path);
		}else{
			printf("file not exist: %s", path);
		}
	}

	void readJson(uint32_t lineNum, FFL::Vector< FFL::String >&  arr) {
		bool bQuit=false;
		FFL::String line;
		while (!bQuit) {
			uint8_t buf[1024] = {};
			size_t readed = 0;
			if (FFL_OK != mFile.read(buf, 1023, &readed)) {
				bQuit=true;
			}

			uint8_t* start = buf;
			uint8_t* cur= buf;
			uint8_t* end= cur +readed;
			while (cur != end){
				if (*cur == '\r') {
					line.append((const char*)start, cur - start);
					arr.push_back(toJson(line));
					line = "";
					start = cur + 1;

					if (arr.size() >= lineNum) {
						bQuit = true;
					}
				}				
				cur++;
			}

			if (start != end) {
				line.append((const char*)start, end - start);
			}
		}
	}

	FFL::String toJson(const FFL::String& info) {
		FFL::String json;
		FFL::Vector<FFL::String> arr = FFL::StringSplit(info, ",");
		for (uint32_t i = 0; i < arr.size(); i++) {
			FFL::Vector<FFL::String> keyValue = FFL::StringSplit(arr[i],":");
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

	FFL::File  mFile;
};

TrackFileReader  gTrackFile;
class HttpClient : public FFL::HttpConnectHandler {
public:
	HttpClient() {

	}

	virtual void onReceiveRequest(FFL::HttpConnect* conn, FFL::HttpRequest* request) {
		
		FFL::String json;
		FFL::Vector< FFL::String >  arr;
		gTrackFile.readJson(10,arr);

		for (uint32_t i = 0; i < arr.size(); i++) {
			if (json.empty()) {
				json += arr[i];
			}
			else {
				json += "," + arr[i];
			}
		}		

		FFL::sp<FFL::HttpResponse> res = request->createResponse();
		res->writeJson("["+ json + "]");

		conn->close();
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

void setTrackfile(const char* args, void* userdata) {	
	if (FFL::fileIsExist(args)) {
		gTrackFile.open(args);
		printf("setTrackfile: %s", args);
	}
	else {
		printf("file not exist: %s", args);
	}
}

static CmdOption  gCmdOption[] = {
	{ "exit",0,trackExit,"exit sys." },
	{ "setTrackfile",0,setTrackfile,"set track file path" },
	{ "start",0,startService,"start service" },
	{ "restart",0,restartService,"re start service" },
	{ "stop",0,stopService,"stop service" },
	{ 0,0,0,0 },
};

int serverMain() {
	FFL::HttpConnectMgr mgr;
	mgr.setHandler(new HttpClient());

	char buf[1024] = {};
	FFL_socketLocalAddr(buf, 1023);

	FFL::TcpServer server(NULL,5000);
	server.setConnectManager(&mgr);
	server.start();

	FFL_inputLooper(gCmdOption, 0, quitFlag);

	return 0;
}

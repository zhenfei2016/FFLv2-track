#include <FFL.h>
#include <utils/FFL_StringHelper.hpp>
#include <utils/FFL_File.hpp>

class TrackFileReader {
public:
	TrackFileReader();
	~TrackFileReader();

	void reset();
	//
	//  返回是否成功， false表示失败了
	//  maxLine:最多读取多小行
	//
	bool readJson(FFL::String& json, int32_t maxLine);
private:
    bool open(const char*path);
	void close();
private:
	FFL::File mFile;
	FFL::String mStream;
};


#include <FFL.h>
#include <utils/FFL_StringHelper.hpp>
#include <utils/FFL_File.hpp>

class TrackFileReader {
public:
	TrackFileReader();
	~TrackFileReader();

	void reset();
	//
	//  �����Ƿ�ɹ��� false��ʾʧ����
	//  maxLine:����ȡ��С��
	//
	bool readJson(FFL::String& json, int32_t maxLine);
private:
    bool open(const char*path);
	void close();
private:
	FFL::File mFile;
	FFL::String mStream;
};


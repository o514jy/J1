#include "pch.h"
#include "FileUtils.h"
#include <filesystem>
#include <fstream>

/*---------------
	FileUtils
-----------------*/

namespace fs = std::filesystem;

vector<BYTE> FileUtils::ReadFile(const WCHAR* path)
{
	vector<BYTE> ret;

	// 디렉터리를 가져오는 등 편한 기능이 많다.
	// 여기서 하는 일은 경로를 받아와서
	fs::path filePath{ path };

	const uint32 fileSize = static_cast<uint32>(fs::file_size(filePath));
	ret.resize(fileSize);

	// ret에 저장을 해서 반환한다.
	basic_ifstream<BYTE> inputStream{ filePath };
	inputStream.read(&ret[0], fileSize);

	return ret;
}

wstring FileUtils::Convert(string str)
{
	const int32 srcLen = static_cast<int32>(str.size());

	wstring ret;
	if (srcLen == 0)
		return ret;

	const int32 retLen = ::MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<char*>(&str[0]), srcLen, NULL, 0);
	ret.resize(retLen);
	::MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<char*>(&str[0]), srcLen, &ret[0], retLen);
	
	return ret;
}

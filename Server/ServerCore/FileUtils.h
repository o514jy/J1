#pragma once
#include <vector>
#include "Types.h"

/*---------------
	FileUtils
-----------------*/

class FileUtils
{
public:
	// path로부터 파일을 받아와서 ret에 저장하고 반환한다.
	static vector<BYTE>		ReadFile(const WCHAR* path);
	// 받아온 파일을 UTF-16으로 변환해준다.
	static wstring			Convert(string str);
};


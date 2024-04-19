#pragma once
#include <vector>
#include "Types.h"

/*---------------
	FileUtils
-----------------*/

class FileUtils
{
public:
	// path�κ��� ������ �޾ƿͼ� ret�� �����ϰ� ��ȯ�Ѵ�.
	static vector<BYTE>		ReadFile(const WCHAR* path);
	// �޾ƿ� ������ UTF-16���� ��ȯ���ش�.
	static wstring			Convert(string str);
};


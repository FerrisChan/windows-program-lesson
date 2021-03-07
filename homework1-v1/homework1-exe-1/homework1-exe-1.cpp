#include <iostream>
// #include "..\homework1-interface-1\FileOperationLib.h"
#include "..\..\homework1-v2\homework1-interface-2\FileOperationLib.h"  // WARNING 必须先build dll-2
#include <string>

using namespace std;

#ifdef UNICODE
#define stdLog std::wcout
#else
#define stdLog std::cout
#endif // UNICODE

void FixFilePathEndWithSeparators(CString& strFilePath) {
	CString strEndChar = strFilePath.Right(1);
	if (strFilePath.Right(1).CompareNoCase(L"\\") != 0)
	{
		strFilePath.AppendFormat(L"\\");
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	if (argc < 3)
	{
		stdLog << "Version 1 usage: [Source Path]  [Destination Path]" << endl;
		stdLog << "Source Path :" << "源目录路径 " << endl;
		stdLog << "Destination Path: " << "目标路径 " << endl;
	}

	CString strSrcPath = argv[1];
	CString strDstPath = argv[2];
	FixFilePathEndWithSeparators(strSrcPath);
	FixFilePathEndWithSeparators(strDstPath);

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	LARGE_INTEGER filesize;

	CString strFirstFile = strSrcPath;
	strFirstFile.AppendFormat(L"\\*");
	hFind = FindFirstFile(strFirstFile, &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		stdLog << "copying file path  [" << strSrcPath.GetString() << "] not existing"  << endl;
		return Err_File_Not_Exist;
	}

	do
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// _tprintf(TEXT("  %s   <DIR>\n"), FindFileData.cFileName);
			continue;
		}
		else
		{
			CString strSrcFile = strSrcPath;
			strSrcFile.AppendFormat(L"%s", FindFileData.cFileName);
			CString strDstFile = strDstPath;
			strDstFile.AppendFormat(L"%s", FindFileData.cFileName);

			if (copy_file(strSrcFile, strDstFile) != Err_Ok)
			{
				stdLog << "calling  copy_file  [" << strSrcFile.GetString() << "] wrong" << endl;
			}
			int nSize = 0;
			int ret = get_file_size(strSrcFile, nSize) / 1024; // k为单位
			if (ret != Err_Ok)
			{
				stdLog << "copying file [" << strSrcFile.GetString() << "] to [" << strDstFile.GetString() << "] wrong, error code is [" << ret << "]" << endl;
			}
			stdLog << "is copying file [" << strSrcFile.GetString() <<  "] to [" << strDstFile.GetString() << "], file size is " << nSize <<"k" << endl;
		}
	} while (FindNextFile(hFind, &FindFileData) != 0);
	return Err_Ok;
}
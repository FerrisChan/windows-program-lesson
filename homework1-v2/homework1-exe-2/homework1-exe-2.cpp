#include <iostream>
//#include "..\..\homework1-v1\homework1-interface-1\FileOperationLib.h"  // 
#include "..\homework1-interface-2\FileOperationLib.h"
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
		stdLog << "Version 1 usage: [Source Path]  [Destination Path] [Filter Format]" << endl;
		stdLog << "Source Path :" << "源目录路径 " << endl;
		stdLog << "Destination Path: " << "目标路径 " << endl;
		stdLog << "Filter Format: " << "过滤格式，可选" << endl;
		return Err_Fail;
	}

	CString strSrcPath = argv[1];
	CString strDstPath = argv[2];
	CString strFilterFormat = argv[3];
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
		stdLog << "copying file path  [" << strSrcPath.GetString() << "] not existing" << endl;
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

#if DLL_VERSION <=1
			if (copy_file(strSrcFile, strDstFile) != Err_Ok)
			{
				stdLog << "calling  copy_file  [" << strSrcFile.GetString() << "] wrong" << endl;
				continue;
			}
			
#else
			int errCode = copy_file_with_fileter(strSrcFile, strDstFile, strFilterFormat);
			if (errCode != Err_Ok)
			{
				if (Err_Filter_File == errCode)
				{
					stdLog << "calling  copy_file  [" << strSrcFile.GetString() << "] is filting" << endl;
				}
				else
				{
					stdLog << "calling  copy_file  [" << strSrcFile.GetString() << "] wrong" << endl;
				}
				continue;
			}
#endif
			
			int nSize = 0;
			int ret = get_file_size(strSrcFile, nSize) ; 
			if (ret != Err_Ok)
			{
				stdLog << "copying file [" << strSrcFile.GetString() << "] to [" << strDstFile.GetString() << "] wrong, error code is [" << ret << "]" << endl;
			}
			else
			{ 
				stdLog << "is copying file [" << strSrcFile.GetString() << "] to [" << strDstFile.GetString() << "], file size is " << nSize / 1024 << "k" << endl;
			}
		}
	} while (FindNextFile(hFind, &FindFileData) != 0);
	return Err_Ok;
}
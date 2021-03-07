#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include "FileOperationLib.h"
#include <iostream>

using namespace std;


#ifdef UNICODE
// #define stdLog std::wcout
#else
#define stdLog std::cout
#endif // UNICODE

// 获取文件大小
unsigned int  get_file_size(in_parm const CString strFilePath,  out_parm int& nFileSize) {
	// stdLog << "call  " << __FUNCTION__ << strFilePath << "\n";
	WIN32_FIND_DATAW file_data;
	HANDLE file = FindFirstFileW(strFilePath, &file_data);

	if (file == INVALID_HANDLE_VALUE)
		return Err_File_Not_Exist;
	LARGE_INTEGER li = { file_data.nFileSizeLow, (LONG)file_data.nFileSizeHigh };
	nFileSize = li.QuadPart;
	FindClose(file);
	return Err_Ok;
}

// 复制文件路径以及相应的文件
unsigned int  copy_file(in_parm const CString strSrcName, in_parm const CString strDstName) {
	// stdLog << "call  " << __FUNCTION__ << strSrcName << "  " << strDstName << "\n";
	if (!FilePathIsExist(strSrcName, FALSE))
	{
		//stdLog<< "File " << strSrcName << "  not exits " << std::endl;
		return Err_File_Not_Exist;
	}
	//stdLog << "File size is  " << get_file_size(strSrcName) / 1024  << "k" << "\n";
	int retCode = CopyFile(strSrcName, strDstName, FALSE);
	return retCode != 0 ? Err_Ok : Err_Fail;
}

bool FilePathIsExist(const CString filepath_in, bool is_directory) {
	const DWORD file_attr = ::GetFileAttributesW(filepath_in);
	if (file_attr != INVALID_FILE_ATTRIBUTES) {
		if (is_directory)
			return (file_attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
		else
			return true;
	}
	return false;
}
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

unsigned int  copy_file(in_parm const CString strSrcName, in_parm const CString strDstName) {
	// stdLog << "call  " << __FUNCTION__ << strSrcName << "  " << strDstName << "\n";
	return copy_file_with_fileter(strSrcName, strDstName, L"");
}

unsigned int  copy_file_with_fileter(in_parm const CString strSrcName, in_parm const CString strDstName, in_parm CString strFilterPostfix) {
	// stdLog << "call  " << __FUNCTION__ << strSrcName << "  " << strDstName << "\n";
	_ASSERT( (DLL_VERSION >= 2));
	if (DLL_VERSION <2)
	{
		return Err_Dll_Version_Not_Match;
	}
	if (!IsFilePathExist(strSrcName, FALSE))
	{
		//stdLog<< "File " << strSrcName << "  not exits " << std::endl;
		return Err_File_Not_Exist;
	}
	//stdLog << "File size is  " << get_file_size(strSrcName) / 1024  << "k" << "\n";
	if (IsFileNeedFilte(strSrcName, strFilterPostfix))
	{
		return Err_Filter_File;
	}
	int retCode = CopyFile(strSrcName, strDstName, FALSE);
	return retCode != 0 ? Err_Ok : Err_Fail;
}

bool IsFilePathExist(const CString strFilePath, bool is_directory) {
	const DWORD file_attr = ::GetFileAttributesW(strFilePath);
	if (file_attr != INVALID_FILE_ATTRIBUTES) {
		if (is_directory)
			return (file_attr & FILE_ATTRIBUTE_DIRECTORY) != 0;
		else
			return true;
	}
	return false;
}

bool IsFileNeedFilte(in_parm const CString strFileName, in_parm CString strFilterPostfix) {
	int nPos = strFileName.ReverseFind('.');
	CString  strFileExtName = strFileName.Right(strFileName.GetLength() - nPos); 
	auto ret = strFileExtName.CompareNoCase(strFilterPostfix);
	if (strFileExtName.CompareNoCase(strFilterPostfix) == 0)
	{
		return TRUE;
	}
	return FALSE;
}

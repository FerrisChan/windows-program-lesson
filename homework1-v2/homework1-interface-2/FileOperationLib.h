#pragma once
#include <string>
#include <atlstr.h>

#define DLL_VERSION 2

#define in_parm 
#define out_parm

enum ErrorCode
{
	Err_Ok = 0,
	Err_Fail,
	Err_File_Not_Exist,
	Err_Filter_File,
	Err_Dll_Version_Not_Match,
};

#ifdef HOMEWORK1DLL_EXPORTS
#define LIBRARY_API __declspec(dllexport)
#else
#define LIBRARY_API __declspec(dllimport)
#endif

extern "C"{
	LIBRARY_API unsigned int  get_file_size(in_parm const CString  strFilename, out_parm int& nFileSize);
	LIBRARY_API unsigned int  copy_file(in_parm const CString strFilename, out_parm const CString strDstName);
	LIBRARY_API unsigned int  copy_file_with_fileter(in_parm const CString strSrcName, in_parm const CString strDstName, in_parm CString strFilterPostfix = L"");
	LIBRARY_API bool IsFileNeedFilte(in_parm const CString strFileName, in_parm CString strFilterPostfix);  // For gtest
}

bool IsFilePathExist(const CString strFilename, bool is_directory);

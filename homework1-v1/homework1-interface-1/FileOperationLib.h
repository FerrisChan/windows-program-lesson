#pragma once
#include <string>
#include <atlstr.h>

#define DLL_VERSION 1

#define in_parm 
#define out_parm

enum ErrorCode
{
	Err_Ok = 0,
	Err_Fail,
	Err_File_Not_Exist,
};

#ifdef HOMEWORK1DLL_EXPORTS
#define LIBRARY_API __declspec(dllexport)
#else
#define LIBRARY_API __declspec(dllimport)
#endif



extern "C" {
	LIBRARY_API unsigned int  get_file_size(in_parm const CString  strFilename, out_parm int & nFileSize);
	LIBRARY_API unsigned int  copy_file(in_parm const CString strFilename, out_parm const CString strDstName);
}

bool FilePathIsExist(const CString filepath_in, bool is_directory);
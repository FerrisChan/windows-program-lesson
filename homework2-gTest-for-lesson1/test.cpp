#include "pch.h"
#include "../homework1-v2/homework1-interface-2/FileOperationLib.h" // WARNING： 必须先build dll-2，不能build dll-1
// TEST(TestCaseName, TestName) {
//   EXPECT_EQ(1, 1);
//   EXPECT_TRUE(true);
// }


// 在Visual Studio 2019环境用gtest框架对上节课作业“拷贝文件（夹）dll”增加单元测试工程:
// 测试case 至少5个
// 说明每个case的测试点

TEST(FileOperationLib, FileOperation) {

	EXPECT_EQ(IsFileNeedFilte(L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\src\\test.cpp", L".cpp"), TRUE);   // 根据文件后缀，cpp 文件需要过滤

	EXPECT_EQ(IsFileNeedFilte(L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\src\\test.cpp", L".txt"), FALSE);  // txt格式， cpp文件不应该过滤

	EXPECT_EQ(IsFileNeedFilte(L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\src\\test.txt", L".cpp"), FALSE);  // txt 文件，cpp格式， 不应该过滤
	
	EXPECT_EQ(copy_file(L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\src\\test.cpp", L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\src\\test.cpp"), Err_Fail);  // 拷贝同一个文件夹下的文件，失败
	
	EXPECT_EQ(copy_file(L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\src\\test.cpp", L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\dst\\test.cpp"), Err_Ok);    // 把一个存在的文件，拷贝到另一个文件夹, 默认不过滤，成功

	EXPECT_NE(copy_file_with_fileter(L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\src\\test.cpp", L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\dst\\test.cpp", L".txt"), Err_Dll_Version_Not_Match);  // 调用函数，检测dll 版本是否符合

	EXPECT_EQ(copy_file_with_fileter(L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\src\\test.cpp", L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\dst\\test.cpp", L".txt"), Err_Ok);  // 拷贝cpp文件，不被txt格式过滤，成功

	EXPECT_EQ(copy_file_with_fileter(L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\src\\test.cpp", L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\dst\\test.cpp", L".cpp"), Err_Filter_File); // 拷贝cpp文件，被cpp格式过滤，返回被过滤的结果
}
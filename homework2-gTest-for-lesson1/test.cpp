#include "pch.h"
#include "../homework1-v2/homework1-interface-2/FileOperationLib.h" // WARNING�� ������build dll-2������build dll-1
// TEST(TestCaseName, TestName) {
//   EXPECT_EQ(1, 1);
//   EXPECT_TRUE(true);
// }


// ��Visual Studio 2019������gtest��ܶ��Ͻڿ���ҵ�������ļ����У�dll�����ӵ�Ԫ���Թ���:
// ����case ����5��
// ˵��ÿ��case�Ĳ��Ե�

TEST(FileOperationLib, FileOperation) {

	EXPECT_EQ(IsFileNeedFilte(L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\src\\test.cpp", L".cpp"), TRUE);   // �����ļ���׺��cpp �ļ���Ҫ����

	EXPECT_EQ(IsFileNeedFilte(L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\src\\test.cpp", L".txt"), FALSE);  // txt��ʽ�� cpp�ļ���Ӧ�ù���

	EXPECT_EQ(IsFileNeedFilte(L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\src\\test.txt", L".cpp"), FALSE);  // txt �ļ���cpp��ʽ�� ��Ӧ�ù���
	
	EXPECT_EQ(copy_file(L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\src\\test.cpp", L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\src\\test.cpp"), Err_Fail);  // ����ͬһ���ļ����µ��ļ���ʧ��
	
	EXPECT_EQ(copy_file(L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\src\\test.cpp", L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\dst\\test.cpp"), Err_Ok);    // ��һ�����ڵ��ļ�����������һ���ļ���, Ĭ�ϲ����ˣ��ɹ�

	EXPECT_NE(copy_file_with_fileter(L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\src\\test.cpp", L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\dst\\test.cpp", L".txt"), Err_Dll_Version_Not_Match);  // ���ú��������dll �汾�Ƿ����

	EXPECT_EQ(copy_file_with_fileter(L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\src\\test.cpp", L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\dst\\test.cpp", L".txt"), Err_Ok);  // ����cpp�ļ�������txt��ʽ���ˣ��ɹ�

	EXPECT_EQ(copy_file_with_fileter(L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\src\\test.cpp", L"C:\\Users\\CF\\Desktop\\windows-program-lesson\\Release\\dst\\test.cpp", L".cpp"), Err_Filter_File); // ����cpp�ļ�����cpp��ʽ���ˣ����ر����˵Ľ��
}
// homework3-diff-stdcall-cdecl.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdarg.h>
#include <vector>

using namespace std;



	int __stdcall homework_stdcall(int a = 0, char* str = NULL, int c = 0) {
		int a1 = a + c;
		return a1;
	}

	int __cdecl homework_cdeclcall(int a = 0, char* str = NULL, int c = 0) {
		int a1 = a + c;
		//std::cout << __FUNCTION__ << endl;
		// 		int b = 0;
// 		b = b + a;
// 		va_list args;
// 		va_start(args, c);
// 		vector<int> vec;
// 		for (int i = 0; i < c; ++i)
// 		{
// 			auto v = va_arg(args, int);
// 			vec.push_back(v);
// 		}
// 		va_end(args);
		return 0;
	}





int main()
{
	int ret1 = homework_stdcall(1,NULL,3);
	// homework_stdcall(1, NULL, 3, 4, 5, 6, 7, 8, "1243", 9);
	int ret2 = homework_cdeclcall(1, NULL, 3);
	// homework_cdeclcall(1, NULL, 3, 4, 5, 6, 7, 8, "1243", 9);
    // std::cout << "Hello World!\n"; 

	int ret3 = ret1 + ret2;
	return  ret3;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

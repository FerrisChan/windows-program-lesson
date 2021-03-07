// homework8-single-process.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>

#pragma data_seg("Shared")
volatile long g_lInstances = 0;
#pragma data_seg()
#pragma comment(linker, "/Section:Shared,RWS")

void KeepTyping()
{
	while (1)
	{
		string str;
		std::cin >> str;
		cout << str;
	}
}

using namespace std;
int main(int argc, char* argv[])
{
	// using memeory 
	if (g_lInstances != 0)
	{
		std::cout << "Already has anther process, this would terminate automatically in 5s \n";
		Sleep(5 * 1000);
		return -1;
	}

	InterlockedExchangeAdd(&g_lInstances, 1);

	cout << "this is the only one process, keep typing" << endl;
	KeepTyping();

	InterlockedExchangeAdd(&g_lInstances, -1);

	// using gobal mutex
// 	HANDLE hMutex = NULL;
// 	do
// 	{
// 		hMutex = CreateMutex(NULL, FALSE, L"Global\\1E8496BB-5425-44EB-BA28-CFA996300B24");
// 
// 		DWORD dwError = GetLastError();
// 
// 		if (ERROR_ALREADY_EXISTS == dwError || ERROR_ACCESS_DENIED == dwError)
// 		{
// 			std::cout << "Already has anther process, this would terminate automatically in 5s \n";
// 			Sleep(5 * 1000);
// 			break;
// 		}
// 		cout << "this is the only one process, keep typing" << endl;
//		KeepTyping();
// 
// 	} while (false);
// 
// 	if (NULL != hMutex)
// 	{
// 		CloseHandle(hMutex);
// 	}

	return 0;

}

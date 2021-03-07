// homework3-2-deadlock-analyze.cpp : This file contains the 'main' function. Program execution begins and ends there.


#include <iostream>
#include <windows.h>
#include <thread>

using namespace std;

CRITICAL_SECTION g_cs1;
CRITICAL_SECTION g_cs2;


void work1() {
	for (int i = 0; i < 10000; i++) {
		EnterCriticalSection(&g_cs1);
		EnterCriticalSection(&g_cs2);
		std::cout << "work1 : " << i << std::endl;
		::LeaveCriticalSection(&g_cs2);
		::LeaveCriticalSection(&g_cs1);
	}
}

void work2() {
	for (int i = 0; i < 10000; i++) {
		EnterCriticalSection(&g_cs2);
		EnterCriticalSection(&g_cs1);
		std::cout << "work2 : " << i << std::endl;
		::LeaveCriticalSection(&g_cs1);
		::LeaveCriticalSection(&g_cs2);
	}
}

homework4 - change - other - windows

int main()
{
	InitializeCriticalSection(&g_cs1);
	InitializeCriticalSection(&g_cs2);

	std::thread t1(work1);
	std::thread t2(work2);
	t1.join();
	t2.join();

	::DeleteCriticalSection(&g_cs1);
	::DeleteCriticalSection(&g_cs2);
}


// class KCriticalSesion
// {
// public:
// 
// 	KCriticalSesion(CRITICAL_SECTION sesion)
// 		: m_sesion(sesion)
// 	{
// 		::InitializeCriticalSection(&m_sesion);
// 	}
// 
// 	KCriticalSesion()
// 	{
// 		::InitializeCriticalSection(&m_sesion);
// 	}
// 
// 	~KCriticalSesion() {
// 		::DeleteCriticalSection(&m_sesion);
// 	}
// 
// 	void Lock(void) { ::EnterCriticalSection(&m_sesion); }
// 	void Unlock(void) { ::LeaveCriticalSection(&m_sesion); };
// 
// private:
// 	CRITICAL_SECTION m_sesion;
// };

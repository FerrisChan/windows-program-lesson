#include "hash_cache.h"
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

#define  SUM  20
static class HashCache cache;
static int order = 0;
mutex g_mutex;  // for  order ¼ÆÊý

void putWork1() {
	for (int i = 0; i < SUM /2 ; i++)
	{
		int j = 0;
		{
			std::lock_guard<std::mutex>  lock(g_mutex);
			j = order++;
		}
		cache.put(to_string(j), string(__FUNCTION__).append(" ").append(to_string(i).c_str()));
	}
}


void putWork2() {
	for (int i = 0; i < SUM /2; i++)
	{
		int j = 0;
		{
			std::lock_guard<std::mutex>  lock(g_mutex);
			j = order++;
		}
		
		cache.put(to_string(j), string(__FUNCTION__).append(" ").append(to_string(i).c_str()));
	}
}

void getWork() {
	for (int i = 0; i < SUM; i++)
	{
		cout <<i << "   "  <<  cache.get(to_string(i)) << endl;
	}
}

int main(int argc, char* argv[])
{
	std::thread t1(putWork1);
	std::thread t2(putWork2);
	cache.save_cache();
	t1.join();
	t2.join();
	getWork();
	return 0;
}

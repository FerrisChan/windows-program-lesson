#include "hash_cache.h"
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

#define  SUM  40
static class HashCache cache;
static int order = 0;
mutex g_mutex;  // order ¼ÆÊý

string printType(TYPE type);

void putWork1();
void putWork2();
void getWork();

int main(int argc, char* argv[])
{
	std::thread t1(putWork1);
	std::thread t2(putWork2);
	t1.join();
	t2.join();
	getWork();
	cache.save_cache();
	return 0;
}

string printType(TYPE type)
{
	string strType;
	switch (type)
	{
	case TYPE_BOOL:
		strType = "[bool]";
		break;
	case TYPE_INT:
		strType = "[int]";
		break;
	case TYPE_FLOAT:
		strType = "[float]";
		break;
	case TYPE_STRING:
		strType = "[string]";
		break;
	default:
		break;
	}
	return strType;
}

void putWork1()
{
	for (int i = 0; i < SUM / 2; i++)
	{
		int j = 0;
		{
			std::lock_guard<std::mutex>  lock(g_mutex);
			j = order++;
		}
		Value val;
		val.strValue = string(__FUNCTION__).append("").append(to_string(i).c_str());
		val.itype = TYPE_STRING;
		val.ilength = val.strValue.length();
		cache.put(to_string(j), val);
	}
}


void putWork2()
{
	for (int i = 0; i < SUM / 2; i++)
	{
		int j = 0;
		{
			std::lock_guard<std::mutex>  lock(g_mutex);
			j = order++;
		}

		Value val;
		val.strValue = string(__FUNCTION__).append("").append(to_string(i).c_str());
		val.itype = TYPE_STRING;
		val.ilength = val.strValue.length();
		cache.put(to_string(j), val);
	}
}

void getWork()
{
	for (int i = 0; i < SUM; i++)
	{
		Value val = cache.get(to_string(i));
		cout << "get " << i << ": type:" << printType(val.itype) << ", ilength:[" << val.ilength << "] ,Value:[" << val.strValue << "]" << endl;
	}
}
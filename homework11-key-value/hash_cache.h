#ifndef HASH_CACHE_H
#define HASH_CACHE_H

#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>
#include <map>

#define PAGE_SIZE 10
#define DISK_SIZE 20
#define CACHE_SIZE 2

#define FAILED -1
using namespace std;

typedef enum {
	TYPE_BOOL = 0,
	TYPE_INT,
	TYPE_FLOAT,
	TYPE_STRING,
} TYPE;

using Value = struct  _sValue
{
	bool operator == (const _sValue& t1) {
		return strValue == t1.strValue;
	}

	bool operator != (const _sValue& t1) {
		return !(strValue == t1.strValue);
	}

	_sValue() {
		itype = TYPE_STRING;
		ilength = 0;
	};
	TYPE itype;
	int ilength;
	string strValue;
};

class HashCache {
	class Page;
	class Node;

public:
	HashCache(int capacity = CACHE_SIZE);
	~HashCache();
	void save_cache();
	Value get(string key);
	void put(string key, Value value);

private:

	Node* search(string key);
	int load_file_to_page(const int& file_num, Page*& page, map<string, Node* >& mapNode);
	int save_page_to_file(Page*& page);
	void erase_page_of_cache(Page*& page, map<string, Node* >& hash_map);
	int load_new_file_index(const char* save_file_index = "file_index.txt");
	void save_new_file_index(const char* save_file_index = "file_index.txt");
	void load_key_file_map(const char* key_filename = "key_file.txt");
	void save_hotdata_to_file();
	void push_key_to_keymap(Page*& page, const char* key_filename = "key_file.txt");
	Page* get_new_page();
	void detach(Page* page);
	void attach(Page* page);

private:

	Page* entries_;
	vector<Page*> free_entries_;
	map<string, Node* > m_map_hot;
	map<string, int> key_file_map;
	Page* put_page;
	bool put_page_existed;
	int new_file_index_;
	Page* head_, * tail_;

	mutex m_mutex_put;
	mutex m_mutex_get;
};
#endif

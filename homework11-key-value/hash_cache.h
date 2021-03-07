#ifndef HASH_CACHE_H
#define HASH_CACHE_H

#include <vector>
#include <unordered_map>
#include <fstream>
#include <string>
#include <thread>
#include <mutex>

#define PAGE_SIZE 10
#define DISK_SIZE 20
#define CACHE_SIZE 2

#define FAILED -1
using namespace std;

class HashCache {
	class Page;
	class Node;

public:
	HashCache(int capacity = CACHE_SIZE);
	~HashCache();
	void save_cache();  //将cache中所有dirty（已修改）内容保存到文件（可以定期调用或者最后结束程序时调用）
	string get(string key);
	void put(string key, string value);

private:

	Node* search(string key);  // 搜索key，成功返回 key 代表的节点，失败返回NULL
	int load_file_to_page(const int& file_num, Page*& page, unordered_map<string, Node* >& hash_map); // 将第file_num个文件加载到page中，并推入hash_map的表中
	int save_page_to_file(Page*& page);  // 将page保存到文件
	void erase_page_of_cache(Page*& page, unordered_map<string, Node* >& hash_map);  // 将该页的内容从hash_map中释放
	int load_new_file_index(const char* save_file_index = "file_index.txt"); // 在该文件中存储着存储文件已经使用到第几个；加载失败返回-1
	void save_new_file_index(const char* save_file_index = "file_index.txt");  // 程序结束时，保存现在的文件序号到文件
	void load_key_file_map(const char* key_filename = "key_file.txt"); // 加载 key 和 key存储文件的映射
	void push_key_to_keymap(Page*& page, const char* key_filename = "key_file.txt"); // 将page中的所有（key, file_num_）推入key_dile_map,并存入文件
	Page* get_new_page(); // 得到新的一页，可处理没有空页的情况
	void detach(Page* page);  // 卸载该页
	void attach(Page* page);  // 将该页链接到头部

private:

    Page* entries_;
    vector<Page*> free_entries_;  // 存储空闲page
    unordered_map<string, Node* > hash_map; // 存储节点的散列表，以供快速查询
    unordered_map<string, int> key_file_map; // 存储key和其对应文件间的映射
    Page* put_page;  // 用以标记写入的page
    bool put_page_existed;  // 用于put_page被顶出之后的判断
    const char**file_list_; // 已弃用。文件列表，后来采用直接用文件序号直接构造文件名。
    int new_file_index_; // 新建文件序号，初始为 -1
    Page *head_, *tail_; // 双向列表的头尾，以实现LRU Cache
   
	mutex put_mutex;
	mutex get_mutex;
};
#endif
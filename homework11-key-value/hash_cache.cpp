#include "hash_cache.h"
#include <iostream>
#include <algorithm>
using namespace std;
bool file_accessed[DISK_SIZE] = { false };

class HashCache::Node {
public:
    std::string key_;
	Value value_;  	// std::string value_;
    class Page* page_;  
};


class HashCache::Page {
public:
    int file_num_; 
    bool lock_;
    bool dirty_;  
    bool is_putpape;
    class Node data_[PAGE_SIZE];  
    class Page* next;
    class Page* prev;
    Page() {
        lock_ = false;
        dirty_ = false;
        is_putpape = false;
    }
};

HashCache::HashCache(int capacity) {
    new_file_index_ = 0 /*load_new_file_index()*/; 
   //  new_file_index_ = load_new_file_index(); // the next new file num.
    load_key_file_map(); // load the map of key and its file from keyfile.
    put_page = NULL;
    put_page_existed = false;
    // 为cache分配内存
    entries_ = new Page[capacity];
    for (int i = 0; i < capacity; i++) {
        for (int j = 0; j < PAGE_SIZE; j++)  
            (entries_ + i)->data_[j].page_ = entries_ + i;
        free_entries_.push_back(entries_ + i); 
    }
    head_ = new Page;
    tail_ = new Page;
    head_->prev = NULL;
    head_->next = tail_;
    tail_->prev = head_;
    tail_->next = NULL;
}
HashCache::~HashCache() {
    // 保存cache的数据
	save_hotdata_to_file();
    save_cache();
    //delete entries_;
    delete head_;
    delete tail_;
}


HashCache::Node* HashCache::search(string key) {
    Node* node = m_map_hot[key];
    Page* page = NULL;
    if (node) {
        page = node->page_;
        detach(page);
        attach(page);
    }
    else {
    	int file_num = key_file_map[key];
    	if (file_num == 0)
    		return node;
        page = get_new_page(); 
        if (load_file_to_page(file_num, page, m_map_hot) == FAILED) {
            free_entries_.push_back(page); 
        }
        if (node = m_map_hot[key]) {
            attach(page);
        }
    }
    return node;
}

Value HashCache::get(string key) {
	std::lock_guard<std::mutex>  lock(m_mutex_get);
	Node* node = search(key);
    if (node)
        return node->value_;
    return Value(); 
}

void HashCache::put(string key, Value value) {
	std::lock(m_mutex_get, m_mutex_put);
	std::lock_guard<std::mutex>  lock1(m_mutex_get, std::adopt_lock);
	std::lock_guard<std::mutex>  lock2(m_mutex_put, std::adopt_lock);
    Node* node = NULL;
    node = search(key);
    if (node && node->page_) {  
        if (value != node->value_ && node) {
            node->value_ = value;
            node->page_->dirty_ = true;
        }
        return;
    }
    else {
       
        static int data_index = 0;
        if (data_index == PAGE_SIZE || put_page_existed == false) {
        	push_key_to_keymap(put_page); 
            data_index = 0;
        }
        if (data_index == 0 || put_page_existed == false) { 
                                                          
            put_page = get_new_page();
            put_page_existed = true;
            put_page->file_num_ = ++new_file_index_; 
            put_page->dirty_ = true;
            for (int i = 0; i < PAGE_SIZE; i++) {
                put_page->data_[i].key_ = "";
                put_page->data_[i].value_ = Value();
            }
        }
        put_page->data_[data_index].key_ = key;
        put_page->data_[data_index].value_ = value;
        m_map_hot[key] = put_page->data_ + data_index; 
        data_index++;
    }
}

int HashCache::load_file_to_page(const int &file_num, Page* &page, map<string, Node* > &mapNode) {
    ifstream input;
    string file_name = to_string(file_num) + ".txt"; 
    input.open(file_name, ios::in|ios::binary);
    if (!input || page == NULL)
        return FAILED; 
    file_accessed[file_num] = true;  
    page->file_num_ = file_num;
    Node* data = page->data_;
    for (int i = 0; i < PAGE_SIZE; i++) {
// 		string key;
// 		string strValue;
// 		if (!(input >> key >> strValue))
// 		{
// 			Value* newValue = new Value;
// 			data[i].key_ = "";
// 			data[i].value_ = *newValue; // 输入为空
// 		}
// 		else
// 		{
// 
// 			Value* newValue = new Value;
// 			data[i].key_ = key;
// 			newValue->strValue = strValue;
// 			data[i].value_ = *newValue; // 
// 		}

		char szBuf[LEN_VALUE * 2] = {0};
		if (!input.getline(szBuf, sizeof(szBuf)))
		{
		 	Value* newValue = new Value;
		 	data[i].key_ = "";
		 	data[i].value_ = *newValue; // 输入为空
		}
		else
		{
			Value* newValue = new Value;
			int k;
			int t;
			int l;
			char v[LEN_VALUE] = { 0 };
			scanf_s(szBuf, LEN_VALUE * 2, "%d %d%d%s\n", &k, &t, &l, v);
			newValue->itype = (TYPE)t;
			newValue->ilength = l;
			strcpy_s(newValue->strValue, v);
			data[i].key_ = to_string(k);
			data[i].value_ = *newValue;
		}

           
        mapNode[data[i].key_] = data + i;
    }
    input.close();
    return 0;
}

int HashCache::save_page_to_file(Page* &page) {
    page->dirty_ = false;
    if (page == NULL)
        return FAILED;
    ofstream output;
    string file_name = to_string(page->file_num_) + ".txt"; // 用文件编号构造文件名
    output.open(file_name, ofstream::binary);
    Node* data = page->data_;
    for (int i = 0; i < PAGE_SIZE; i++) 
	{

		string key = data[i].key_;
		if (key.length() > 0) {
			char szBuf[LEN_VALUE * 2] = { 0 };
			sprintf_s(szBuf, LEN_VALUE * 2, "%d %d%d%s\n", atoi(key.c_str()), data[i].value_.itype, data[i].value_.ilength, data[i].value_.strValue);
			output.write(szBuf, strlen(szBuf));
// 			string buff = key;
// 			buff.append(key).append(" ");
// 			buff.append(to_string(data[i].value_.itype));
//			output << data[i].key_ << " " << data[i].value_.itype << data[i].value_.ilength << data[i].value_.strValue << endl;
		}
		else
		{
			output <<"emptykey" << endl; 
		}
    }
    output.close();
    return 0;
}


int HashCache::load_new_file_index(const char* save_file_index) {
    ifstream input;
    input.open(save_file_index, ios::in);
    if (!input) {
        return 0;// 不存在该文件，返回初始序号0
    }
    int index;
    input >> index;
    input.close();
    return index;
}
void HashCache::save_new_file_index(const char* save_file_index) {
    ofstream output;
    output.open(save_file_index, ios::out);
    if (!(output << new_file_index_)) {
        cout << "save_new_file_index fai：write <" << save_file_index << "> failed." << endl;
        exit(-1);
    }
    output.close();
}

void HashCache::erase_page_of_cache(Page* &page, map<string, Node* > &hash_map)
{
	if (page->file_num_ < DISK_SIZE)
	{
		file_accessed[page->file_num_] = false; // 表示该文件已经不再hash_map中
	}
    for (int i = 0; i < PAGE_SIZE; i++) {
        hash_map.erase(page->data_[i].key_);
    }
}

void HashCache::save_cache() {
    // 保存搜索用hash_map
    Page* page = head_->next;
    while (page != tail_) {
        if (page->dirty_ == true) {
            save_page_to_file(page);
        }
        page = page->next;
    }
    save_new_file_index();
    push_key_to_keymap(put_page);
}

void HashCache::detach(Page* page) {
	try
	{
		if (!page || !page->prev || !page->next)
			return;
		page->prev->next = page->next;
		page->next->prev = page->prev;
	}
	catch (...)
	{
		
	}

}

void HashCache::attach(Page* page) {
	try
	{
		if (!page || !page->prev || !page->next)
			return;
		page->prev = head_;
		page->next = head_->next;
		head_->next->prev = page;
		head_->next = page;
	}
	catch (...)
	{

	}
}
    
HashCache::Page* HashCache::get_new_page() {
    Page* page;
    if (free_entries_.empty() == true) {
        // cache 已满，从尾部取下一个page
        page = tail_->prev;
        if (page == put_page) {
            put_page_existed = false;
        	push_key_to_keymap(page); // 该页将被弹出，保存key，file映射
        } // put_page 被顶出
        if (page->dirty_ == true)
        // if (page->dirty_ == true)
            save_page_to_file(page); // 保存该页，并从hash_map中删除该页
        erase_page_of_cache(page, m_map_hot);
        detach(page);
        free_entries_.push_back(page); //之所以入栈又出栈，是为了应对读取失败的情况
    }
    page = free_entries_.back();
    free_entries_.pop_back();
    return page;
}
void HashCache::push_key_to_keymap(Page* &page, const char* key_filename) {
	if (page == NULL)
		return;
	ofstream output;
		// save to file (append)
	output.open(key_filename, ios::app);

	Node* data = page->data_;
	int file_num = page->file_num_;
	for (int i = 0; i < PAGE_SIZE; i++) {
		key_file_map[data[i].key_] = file_num;
		output << data[i].key_ << " " << file_num << endl;
	}
	output.close();
}
void HashCache::load_key_file_map(const char* key_filename) {
	ifstream input;
	input.open(key_filename, ios::in);
	if (!input)
		return;
	string key;
	int file_num;
	while (input >> key >> file_num) {
		key_file_map[key] = file_num;
	}
	input.close();
}

void HashCache::save_hotdata_to_file() {
	
	ofstream output;
	string file_name = "hotdata.txt"; 
	output.open(file_name, std::ofstream::binary);

	for (auto iter = m_map_hot.begin(); iter!=m_map_hot.end(); ++iter )
	{
		Node* n = iter->second;
		string key = iter->first;
		if (key.length() > 0 && n) {
			char szBuf[LEN_VALUE * 2] = { 0 };
			sprintf_s(szBuf, LEN_VALUE * 2, "%d %d%d%s\n", atoi(key.c_str()), n->value_.itype, n->value_.ilength, n->value_.strValue);
			output.write(szBuf, strlen(szBuf));
		}
// 		if (key.length() > 0 && n)
// 		{
// 			output << key << " " << n->value_.itype << n->value_.ilength << string(n->value_.strValue) << endl; 
// 		}
	}
	output.close();
	return ;
}
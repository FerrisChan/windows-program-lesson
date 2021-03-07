
#include <iostream>
#include "Alloc.h"
#include <vector>

int locate = 0;
class TestMemoryPoolOneByte
{
private:
	char onebyte;
public:
	TestMemoryPoolOneByte() {}

	static void* operator new(size_t)
	{
		locate++;
		return MyMemory::alloc::allocate(sizeof(TestMemoryPoolOneByte));  // 使用内存池分配
	}
	static void operator delete(void* p) {
		MyMemory::alloc::deallocate(p, sizeof(TestMemoryPoolOneByte));
	}

	void print()
	{
		std::cout << this << ": ";
		std::cout << __FUNCTION__ << " the " << locate  << " object" << std::endl;
	}
};

class TestMemoryPoolOnek
{
public:
	char strOneK[1024];

public:
	TestMemoryPoolOnek() {}
	static void* operator new(size_t)
	{
		locate++;
		return MyMemory::alloc::allocate(sizeof(TestMemoryPoolOnek));  // 使用malloc 分配
	}
	static void operator delete(void* p) {
		MyMemory::alloc::deallocate(p, sizeof(TestMemoryPoolOnek));
	}
	void print()
	{
		std::cout << this << ": ";
		std::cout << __FUNCTION__ <<  " the " << locate << " object" << std::endl;
	}

};

class TestMemoryPoolOneM
{
private:
	char strOneM[1024 * 1024];

public:
	TestMemoryPoolOneM() {}
	static void* operator new(size_t)
	{
		locate++;
		return MyMemory::alloc::allocate(sizeof(TestMemoryPoolOneM));
	}
	static void operator delete(void* p) {
		MyMemory::alloc::deallocate(p, sizeof(TestMemoryPoolOneM));
	}

	void print()
	{
		std::cout << this << ": ";
		std::cout << __FUNCTION__ << " the " << locate << " object" << std::endl;
	}
};

/************************************************************************************************************* */

int main()
{
	printf("testing is the same address\n");
	//申请第一个内存
	TestMemoryPoolOneByte* Small1 = new TestMemoryPoolOneByte;
	Small1->print();

	//申请第二个内存并释放
	TestMemoryPoolOneByte* Small2 = new TestMemoryPoolOneByte;
	Small2->print();
	delete Small2;

	//申请第三个内存并释放，因前一个内存被释放，地址打印应为第二个内存地址
	TestMemoryPoolOneByte* Small3 = new TestMemoryPoolOneByte;
	Small3->print();
	delete(Small3);

	printf("testing onek  looping, using memory pool \n");
	TestMemoryPoolOneByte* small = nullptr;
	for (int i = 0; i < 9; i++)
	{
		small = new TestMemoryPoolOneByte();    // 每次申请的都是相同的内存地址，使用内存池分配
		small->print();
		delete small;
	}

	printf("testing onek looping, using malloc \n");
	TestMemoryPoolOnek* onek = nullptr;
	for (int i = 0; i < 9; i++)
	{
		onek = new TestMemoryPoolOnek();
		memset(onek->strOneK, i, 1024);
		onek->print();
		delete onek;
	}

	printf("testing OneM looping, using malloc  \n");
	std::vector<TestMemoryPoolOneM> arrOneM;
	for (int i = 0; i < 9; i++)
	{
		TestMemoryPoolOneM* onem = nullptr;
		onem = new TestMemoryPoolOneM();
		arrOneM.push_back(*onem);
		onem->print();
	}

// 	printf("testing Bad alloc \n");
// 	std::vector<TestMemoryPoolOneM> arrBadAlloc;
// 	for (int i = 0; i < 20; i++)  // i 大于 2056， bad alloc
// 	{
// 		TestMemoryPoolOneM* onem = nullptr;
// 		onem = new TestMemoryPoolOneM();
// 		arrBadAlloc.push_back(*onem);
// 		onem->OutputInformation();
// 	}

	printf("testing end\n");
	system("pause");
}



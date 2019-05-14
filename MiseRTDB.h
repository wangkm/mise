#pragma once
#include <string>
#include <hash_map>

using namespace std;
using namespace stdext;

class MiseRTDB
{
private:
	hash_map<string, double> hm;

public:
	MiseRTDB(void);
	~MiseRTDB(void);

	void Clear();							//清空当前的所有数据

	void AddElement(string key);			//创建一个关键字为key的hash元素
	void DelElement(string key);			//删除一个关键字为key的hash元素

	void SetData(string key, double value);	//将关键字为key的元素值置为value
	double GetData(string key);				//读出关键字为key的元素值

	//提供一种遍历数据库的方式
	size_t Size() { return hm.size(); }
	std::pair<string, double> GetDBData(int index);
	
};

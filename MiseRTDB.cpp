#include "StdAfx.h"
#include "MiseRTDB.h"

MiseRTDB::MiseRTDB(void)
{
}

MiseRTDB::~MiseRTDB(void)
{
	hm.clear();
}

void MiseRTDB::Clear()
{
	hm.clear();
}

void MiseRTDB::AddElement(string key)
{
	try{
		hm.insert(std::pair<string, double>(key, 0.0));
	}
	catch(...){
		string strTmp;
		strTmp = "创建数据库元素 ";
		strTmp += key;
		strTmp += " 出错";
		AfxMessageBox(_T(strTmp.c_str()));
	}

}
void MiseRTDB::DelElement(string key)
{
	try{
		hm.erase(key);
	}
	catch(...){
		string strTmp;
		strTmp = "删除数据库元素 ";
		strTmp += key;
		strTmp += " 出错";
		AfxMessageBox(_T(strTmp.c_str()));
	}
}

void MiseRTDB::SetData(string key, double value)
{
	try{
		hash_map<string, double> :: iterator hm_Iter;
		hm_Iter = hm.find(key);
		hm_Iter->second = value;
	}
	catch(...){
		string strTmp;
		strTmp = "设定数据库元素 ";
		strTmp += key;
		strTmp += " 的值出错";
		AfxMessageBox(_T(strTmp.c_str()));
	}
}
double MiseRTDB::GetData(string key)
{
	try{
		hash_map<string, double> :: iterator hm_Iter;
		hm_Iter = hm.find(key);
		return hm_Iter->second;
	}
	catch(...){
		string strTmp;
		strTmp = "获取数据库元素 ";
		strTmp += key;
		strTmp += " 的值出错";
		AfxMessageBox(_T(strTmp.c_str()));
		return 0.0;
	}
}

std::pair<string, double> MiseRTDB::GetDBData(int index)
{
	ASSERT(index < hm.size());
	hash_map<string, double> :: iterator hm_Iter = hm.begin();
	for ( int i = 0; i < index; i++)
	{
		hm_Iter++;
	}
	return std::pair<string, double>(hm_Iter->first, hm_Iter->second);
}
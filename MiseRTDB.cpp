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
		strTmp = "�������ݿ�Ԫ�� ";
		strTmp += key;
		strTmp += " ����";
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
		strTmp = "ɾ�����ݿ�Ԫ�� ";
		strTmp += key;
		strTmp += " ����";
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
		strTmp = "�趨���ݿ�Ԫ�� ";
		strTmp += key;
		strTmp += " ��ֵ����";
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
		strTmp = "��ȡ���ݿ�Ԫ�� ";
		strTmp += key;
		strTmp += " ��ֵ����";
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
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

	void Clear();							//��յ�ǰ����������

	void AddElement(string key);			//����һ���ؼ���Ϊkey��hashԪ��
	void DelElement(string key);			//ɾ��һ���ؼ���Ϊkey��hashԪ��

	void SetData(string key, double value);	//���ؼ���Ϊkey��Ԫ��ֵ��Ϊvalue
	double GetData(string key);				//�����ؼ���Ϊkey��Ԫ��ֵ

	//�ṩһ�ֱ������ݿ�ķ�ʽ
	size_t Size() { return hm.size(); }
	std::pair<string, double> GetDBData(int index);
	
};

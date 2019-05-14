#include "stdafx.h" 
#include "MyDataDef.h"

// �Զ��庯��------------------------------------------------------
// �׳�
int jiecheng(int n)
{
	ASSERT ( n >= 0);

	int result = 1; 
	for (int i = 1; i <= n; i++) 
	{ 
		result = result * i;
	} 

	return result; 
}

//�����������У���[1 2 3]
bool ParseParameters(CString strList, std::vector<double>& parList)
{
	strList.Trim();
	if ( strList.Left(1) != "[" || strList.Right(1) != "]" )
	{
		AfxMessageBox("ϵ����Ҫ���ڷ�������");
		return false;
	}
	CString strTmp;
	bool newNum = true;
	for ( int i = 1; i < strList.GetLength(); i++)
	{
		if ( strList.GetAt( i ) != ' ' && strList.GetAt( i ) != ']')
		{
			if ( newNum )
			{
				strTmp = strList.GetAt( i );
				newNum = false;
			}
			else
			{
				strTmp += strList.GetAt( i );
			}
		}
		else
		{
			if ( !newNum )
			{
				//��strTmp��ֵת��double����
				char *buffer, *stopstring;
				buffer = strTmp.GetBuffer(0);
				double value = strtod(buffer, &stopstring);
				CString sss = _T(stopstring);
				if ( sss == "" )
				{
					parList.push_back(value);
				}
				else
				{
					AfxMessageBox("������ʽ���󡣴������" + strTmp);
					return false;
				}
				newNum = true;
			}
			else
			{
				// nothing to do
			}
		}
	}
	return true;

}

//ȥ����ֵ��������ʽת��string������0
void DelEndZeroFromStr(CString& numStr)
{
	if (numStr.Find(".") == -1 )
	{
		return;
	}
	for(int i = numStr.GetLength() - 1; i > 0; i--){	//ȥ�������������0
		if(numStr.GetAt(i) == '0')
		{
			numStr.Delete( i, 1 );
		}
		else if(numStr.GetAt(i) == '.')
		{
			numStr.Delete( i, 1 );
			break;
		}
		else
		{
			break;
		}
	}
}

// ȱʡ��ʱ���ƽ��㷨���������ƽ� -------------------------------------------------------------
// current_time Ϊ��ǰ����ʱ��
// step_time Ϊ�趨�ķ��沽��
// dbΪʵʱ���ݿ�ָ�룬����������ѯ��������ڵ�ı��η�����
// ����ֵΪ��һ������ʱ��
//
// ����db������size()������ȡԪ�ظ�������GetDBData(index)������ȡԪ�ص�ǰֵ���û������Լ�����洢�ṹ��
// ����ǰ�����������ı仯����ȷ����һ���ķ���ʱ�䣬ʵ�ֱ䲽������
double TimeAdvance_Default(double current_time, double step_time, MiseRTDB *db)
{
	return current_time + step_time;
}
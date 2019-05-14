#include "stdafx.h" 
#include "MyDataDef.h"

// 自定义函数------------------------------------------------------
// 阶乘
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

//解析参数序列，如[1 2 3]
bool ParseParameters(CString strList, std::vector<double>& parList)
{
	strList.Trim();
	if ( strList.Left(1) != "[" || strList.Right(1) != "]" )
	{
		AfxMessageBox("系数需要放在方括号内");
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
				//将strTmp的值转成double变量
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
					AfxMessageBox("参数格式错误。错误对象：" + strTmp);
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

//去掉数值按浮点形式转成string后多余的0
void DelEndZeroFromStr(CString& numStr)
{
	if (numStr.Find(".") == -1 )
	{
		return;
	}
	for(int i = numStr.GetLength() - 1; i > 0; i--){	//去掉结果后面多余的0
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

// 缺省的时间推进算法，按步长推进 -------------------------------------------------------------
// current_time 为当前仿真时刻
// step_time 为设定的仿真步长
// db为实时数据库指针，可以用来查询各个输出节点的本次仿真结果
// 返回值为下一个仿真时刻
//
// 其中db可以用size()函数获取元素个数，用GetDBData(index)函数获取元素当前值，用户可以自己定义存储结构来
// 根据前几步仿真结果的变化幅度确定下一步的仿真时间，实现变步长仿真
double TimeAdvance_Default(double current_time, double step_time, MiseRTDB *db)
{
	return current_time + step_time;
}
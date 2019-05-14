#include "stdafx.h"
#include <math.h>
#include <vector>
#include "MiseRTDB.h"

#pragma once

#define g_pApp		((CmiseApp*)AfxGetApp())

#define COMPONENT_CFG_FILE	"Components.xml"	//存储组件信息的xml文件

#define COM_MAX_PER_MODEL	100		//每个模型允许的最多模块数目
#define OUTPUT_MAX_SIZE		100		//每个模块允许最多的输出接口数目
#define INPUT_MAX_SIZE		100		//每个模块允许最多的输出接口数目

#define COM_TYPE_MAX_SIZE	100		//组件树中最多支持多少个组件

//MISE支持的数据类型
#define MISE_DOUBLE			1001
#define MISE_UNKNOWN		1010

//仿真类型
#define STEP_SIMULATION		1		//定步长仿真


#define COM_WIDTH	80		//模块的宽度
#define COM_HEIGHT	100		//模块的高度
#define COM_MINSIZE 30		//模块最小的高度或宽度

#define LINE_CLR		RGB(0, 0, 255)	//选中的连线的颜色
#define COMP_CLR		RGB(0, 0, 255)	//选中的组件的颜色
#define SLCT_LINE_CLR	RGB(255, 0, 0)	//选中的连线的颜色
#define SLCT_COMP_CLR	RGB(255, 0, 0)	//选中的组件的颜色

#define COM_BKCLR	RGB(255, 255, 255)	//模块的背景颜色

#define DRAG_DELAY	50

//输入输出箭头的长度
#define IO_ARROW_SIZE	5

//标识连线自动拐点的长度
#define BREAK_SPACE		40

// 标识当前操作
#define NONE_ACTION 0	//没有任何操作
#define SLCT_COMP	1	//选择组件（和连线）
#define CONN_COMP	2	//在组件间连线
#define SLCT_LINE	3	//选择连线
#define RESZ_COMP_LEFT		4	//改变组件大小
#define RESZ_COMP_RIGHT		5	//改变组件大小
#define RESZ_COMP_TOP		6	//改变组件大小
#define RESZ_COMP_BOTTOM	7	//改变组件大小

#define SEL_SENSITIVITY		4	//选择连线时的灵敏度，单位为像素

//------------- 函数定义列表 -----------------------------------
#define UDF_COMPONENT	0		//用户自定义的模块，可以修改

#define SRC_CLOCK		100		//仿真时钟
#define SRC_SIN			101		//sin源
#define SRC_COS			102		//cos源
#define SRC_CONST		103		//常数
#define SRC_STEP		104		//阶跃函数

#define MATH_ADD		201		//加法器
#define MATH_MUL		202		//乘法器
#define MATH_RECIPROCAL	203		//取倒数
#define MATH_POW		204		//幂运算

#define CONT_DIFF		211		//取微分
#define CONT_INTEGRAL	212		//取积分
#define CONT_TRANSFUN	213		//传递函数
#define CONT_ZEROPOLE	214		//零-极点分析

#define OUT_FILER		301		//结果输出到文件
#define OUT_MONITOR		302		//结果输出为图形


//特殊定义--------------------------------------------------------
#define MONITOR_MAX		5		//曲线输出窗口可以输出的最多曲线数目



//自定义函数
int jiecheng(int n);
bool ParseParameters(CString strList, std::vector<double>& parList);
void DelEndZeroFromStr(CString& numStr);
double TimeAdvance_Default(double current_time, double step_time, MiseRTDB *db);
#pragma once
#include <vector>
using namespace std;

class MiseModel;

//存储参数值的联合
union IOValue
{
	double	l;
	int		i;
};
//输入输出接口类
class IOPort
{
public:
	CString name;			//端口名，在一个组件内部是唯一的
	int	type;				//参数类型，目前只支持double
	CString	source;			//输入接口的数据来源，即实时数据库中的key值，用于在运行时快速查询，对输出接口无效
	IOValue	default_value;	//缺省值，对输出端口无效
	IOValue	current_value;	//当前值
	CPoint	pos;
};

//Mise基本模块
class MiseComponent
{
public:
	MiseModel* parent;		//指向所属的MiseModel对象
	CString name;			//name of current component
	int		type;			//该模块的类型，目前可以分为CONST_COMPONENT和UDF_COMPONENT两种
	int		m_width; 
	int		m_height;
	CRect	rect;			//该组件的位置，暂时仅支持矩形
	BOOL	isSelected;		//whether this component is selected
	vector<IOPort> inputs;	//输入接口集合
	vector<IOPort> outputs;	//输出接口集合
	bool	m_displayPortName;	//标识是否显示接口名称

	CString	dllPath;		//对应动态链接库的位置
	CString functionName;	//对应函数入口名称，缺省跟模块名相同

	CString comment;		//模块对应的说明

	unsigned int m_inputs_min, m_inputs_max, m_outputs_min, m_outputs_max;	//用来记录输入输出端口的个数范围

protected:
	HINSTANCE handle;		//对应的dll句柄

	// 有可能需要让用户知道输入端口的数目和名称
	// 返回值是0说明运行正常，否则出错。用户可以自行设定错误代码意义
	int (*funEntry)(double current_time, double step_time, double *inputs, char *inputs_list, double *outputs, char *outputs_list);		//功能函数入口

public:
	MiseComponent();
	~MiseComponent();
	CString GetName(){return name;}
	CRect GetRect(){return rect;}

	void Move(int x, int y);
	void MoveEx(CRect newRect);

	void AddInput(CString name, int type, IOValue default_value);
	void AddOutput(CString name, int type);
	void DelInput(int index);
	void DelOutput(int index);
	void AltInput(int index, CString name, int type, IOValue default_value);
	void AltOutput(int index, CString name, int type);

	void AltName(CString newName);		//修改模块的名称

	BOOL TestExist(CString IOName);		//检测名称为name的端口是否存在

	virtual BOOL Init();
	virtual BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	virtual BOOL PostRun();				//运行之后的后处理
	virtual BOOL Clear();
	virtual BOOL Display();				//显示
};

//Mise模块间的数据连线
class ConnectLine
{
public:
	MiseModel* parent;	//指向所属的MiseModel对象
	CString outport;	//输出的端口，形式为 组件名.端口名
	CString inport;		//输入的端口，形式为 组件名.端口名

	CPoint startPoint;	//起点坐标
	CPoint endPoint;	//终点坐标
	int break1x;		//第一个拐点的x坐标
	int break2y;		//第二个拐点的y坐标，如果为-1，则无第三个和第四个拐点
	int break3x;		//第三个拐点的x坐标

	BOOL m_isSelected;	//whether this line is selected
	UINT selectedPart;	//标记被选中的折线，用来进行拖动 

	ConnectLine();
	~ConnectLine();
	BOOL NeedBeSelected(CPoint point);	//检测当鼠标点击point点的时候是否选中该连线

};

//Mise模型，由模块和连线组成
class MiseModel
{
public:

	vector<MiseComponent*>	components;
	vector<ConnectLine>		connlines;
public:
	MiseModel();
	~MiseModel();
	void AppendComponent(MiseComponent* pComponent);
	void DelComponent(unsigned int index);

	void AddConnLine(ConnectLine* pConnLine);


	void Clear();	//清除所有元素
	BOOL IsEmpty();	//当前模型是否为空
	BOOL Load(CString fileName);	//从文件加载模型信息
	BOOL Save(CString fileName);	//将模型信息存储到磁盘文件
};

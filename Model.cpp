#pragma once
#include "stdafx.h"
#include "mise.h"
#include "Model.h"
#include <assert.h>
#include "MyDataDef.h"
#include "ComDefDialog.h"
#include "MiseLibs.h"


#import <msxml3.dll>
using namespace MSXML2;


//==================== class MiseComponent ================================
MiseComponent::MiseComponent()
{
	name = "";	//指定的模块名称
	dllPath = "";		//初始时候未指定dll文件位置
	functionName = "";
	parent = NULL;
	isSelected = false;	//初始处于未被选中状态

	m_width = COM_WIDTH;
	m_height = COM_HEIGHT;
	m_displayPortName = true;
	//rect.left = 0;
	//rect.top = 0;
	//rect.right = COM_WIDTH;
	//rect.bottom = COM_HEIGHT;

	//输入输出端口的数目范围
	m_inputs_min = 0;
	m_inputs_max = INPUT_MAX_SIZE;
	m_outputs_min = 0;
	m_outputs_max = OUTPUT_MAX_SIZE;
}

MiseComponent::~MiseComponent()
{
	this->inputs.clear();
	this->outputs.clear();
}

//测试该变量是否存在
BOOL MiseComponent::TestExist(CString IOName)
{
	ASSERT(IOName.Trim() != "");

	unsigned int i;
	//检查输入端口
	for (i = 0; i < this->inputs.size(); i++)
	{
		if ( this->inputs[i].name == IOName )
		{
			break;
		}
	}
	if ( i < this->inputs.size() )
	{
		return TRUE;
	}

	//检查输出端口
	for (i = 0; i < this->outputs.size(); i++)
	{
		if ( this->outputs[i].name == IOName )
		{
			break;
		}
	}
	if ( i < this->outputs.size() )
	{
		return TRUE;
	}

	//未发现重名变量，返回FALSE
	return FALSE;

}
//add a new input
void MiseComponent::AddInput(CString name, int type, IOValue default_value)
{
	//首先确定是否可以添加
	if ( this->inputs.size() >= this->m_inputs_max )
	{
		CString msg;
		msg.Format("添加失败。本模块最多允许%d个输入接口", this->m_inputs_max);
		AfxMessageBox(msg);
		return;
	}


	//查找name是否重复，如果重复，提示重新输入
	if ( TestExist(name) )
	{
		AfxMessageBox("已存在同名变量，请重新命名");
		return;
	}

	//插入输入端口vector中
	IOPort *pInput = new IOPort;
	pInput->name = name;
	pInput->type = type;
	pInput->default_value = default_value;
	pInput->pos.x = rect.left;
	inputs.push_back(*pInput);

	//修改其他输入端口的纵坐标位置及相应连线
	for ( unsigned int i = 0; i < this->inputs.size(); i++ )
	{
		//modify y position of other ports
		this->inputs[i].pos.y = rect.top + (rect.bottom - rect.top) / this->inputs.size() * ((double)i + 0.5);
		if ( this->parent )	//如果该组件已经加入到模型中
		{
			//modify related connectlines
			CString strTmp;
			strTmp.Format("%s.%s", this->name, this->inputs[i].name);
			for (unsigned int j = 0; j < this->parent->connlines.size(); j++)
			{
				if ( this->parent->connlines[j].inport == strTmp )
				{
					this->parent->connlines[j].endPoint = this->inputs[i].pos;
				}
			}
		}
	}
}

//add a new output
void MiseComponent::AddOutput(CString name, int type)
{
	//首先确定是否可以添加
	if ( this->outputs.size() >= this->m_outputs_max )
	{
		CString msg;
		msg.Format("添加失败。本模块最多允许%d个输出接口", this->m_outputs_max);
		AfxMessageBox(msg);
		return;
	}

	//查找name是否重复，如果重复，提示重新输入
	if ( TestExist(name) )
	{
		AfxMessageBox("已存在同名变量，请重新命名");
		return;
	}

	//插入输入端口vector中
	IOPort *pOutput = new IOPort;
	pOutput->name = name;
	pOutput->type = type;
	pOutput->pos.x = rect.right;
	outputs.push_back(*pOutput);

	//修改其他输出端口的纵坐标位置及相应连线
	for ( unsigned int i = 0; i < this->outputs.size(); i++ )
	{
		//modify y position of other ports
		this->outputs[i].pos.y = rect.top + (rect.bottom - rect.top) / this->outputs.size() * ((double)i + 0.5);

		//modify related connectlines
		if ( this->parent )	//如果该组件已经加入到模型中
		{
			CString strTmp;
			strTmp.Format("%s.%s", this->name, this->outputs[i].name);
			for (unsigned int j = 0; j < this->parent->connlines.size(); j++)
			{
				if ( this->parent->connlines[j].outport == strTmp )
				{
					this->parent->connlines[j].startPoint.x = this->outputs[i].pos.x + IO_ARROW_SIZE;
					this->parent->connlines[j].startPoint.y = this->outputs[i].pos.y;
				}
			}
		}
	}
}

void MiseComponent::DelInput(int index)
{
	ASSERT(index >= 0 && index < this->inputs.size());
	
	//首先确定是否可以删除
	if ( this->inputs.size() <= this->m_inputs_min )
	{
		CString msg;
		msg.Format("删除失败。本模块至少需要%d个输入接口", this->m_inputs_min);
		AfxMessageBox(msg);
		return;
	}

	//删除输入端口为待删除端口的连线
	CString strTmp;
	strTmp.Format("%s.%s", this->name, this->inputs[index].name);
	for(unsigned int i = 0; i < this->parent->connlines.size(); i++)
	{
		if ( this->parent->connlines[i].inport == strTmp )
		{
			this->parent->connlines.erase(this->parent->connlines.begin() + i);
			i--;
		}
	}

	//删除该输入端口
	this->inputs.erase(this->inputs.begin() + index);

	//修改其他输入端口的纵坐标位置及相应连线
	for ( unsigned int i = 0; i < this->inputs.size(); i++ )
	{
		//modify y position of other ports
		this->inputs[i].pos.y = rect.top + (rect.bottom - rect.top) / this->inputs.size() * ((double)i + 0.5);
		//modify related connectlines
		CString strTmp;
		strTmp.Format("%s.%s", this->name, this->inputs[i].name);
		for (unsigned int j = 0; j < this->parent->connlines.size(); j++)
		{
			if ( this->parent->connlines[j].inport == strTmp )
			{
				this->parent->connlines[j].endPoint = this->inputs[i].pos;
			}
		}
	}
	
}

void MiseComponent::DelOutput(int index)
{
	ASSERT(index >= 0 && index < this->outputs.size());
	
	//首先确定是否可以删除
	if ( this->outputs.size() <= this->m_outputs_min )
	{
		CString msg;
		msg.Format("删除失败。本模块至少需要%d个输出接口", this->m_outputs_min);
		AfxMessageBox(msg);
		return;
	}

	//删除输入端口为待删除端口的连线
	CString strTmp;
	strTmp.Format("%s.%s", this->name, this->outputs[index].name);
	for(unsigned int i = 0; i < this->parent->connlines.size(); i++)
	{
		if ( this->parent->connlines[i].outport == strTmp )
		{
			this->parent->connlines.erase(this->parent->connlines.begin() + i);
			i--;
		}
	}

	//删除该输出端口
	this->outputs.erase(this->outputs.begin() + index);

	//修改其他输出端口的纵坐标位置及相应连线
	for ( unsigned int i = 0; i < this->outputs.size(); i++ )
	{
		//modify y position of other ports
		this->outputs[i].pos.y = rect.top + (rect.bottom - rect.top) / this->outputs.size() * ((double)i + 0.5);
		//modify related connectlines
		CString strTmp;
		strTmp.Format("%s.%s", this->name, this->outputs[i].name);
		for (unsigned int j = 0; j < this->parent->connlines.size(); j++)
		{
			if ( this->parent->connlines[j].outport == strTmp )
			{
				this->parent->connlines[j].startPoint.x = this->outputs[i].pos.x + IO_ARROW_SIZE;
				this->parent->connlines[j].startPoint.y = this->outputs[i].pos.y;
			}
		}
	}

}

//修改输入变量
void MiseComponent::AltInput(int index, CString name, int type, IOValue default_value)
{
	ASSERT(index >= 0 && index < this->inputs.size());

	//首先查找name是否重复，如果重复，提示重新输入
	if ( TestExist(name) )
	{
		AfxMessageBox("已存在同名变量，请重新命名");
		return;
	}

	//修改同该输入端口相关的连线
	CString oldPortName, newPortName;
	oldPortName.Format("%s.%s", this->name, this->inputs[index].name);
	newPortName.Format("%s.%s", this->name, name);
	for (unsigned int i = 0; i < this->parent->connlines.size(); i++)
	{
		if ( this->parent->connlines[i].inport == oldPortName )
		{
			this->parent->connlines[i].inport = newPortName;
		}
	}

	//修改该输入端口的信息
	this->inputs[index].name = name;
	this->inputs[index].type = type;
	this->inputs[index].default_value = default_value;
}

//修改输出变量
void MiseComponent::AltOutput(int index, CString name, int type)
{
	ASSERT(index >= 0 && index < this->outputs.size());

	//首先查找name是否重复，如果重复，提示重新输入
	if ( TestExist(name) )
	{
		AfxMessageBox("已存在同名变量，请重新命名");
		return;
	}

	//修改同该输入端口相关的连线
	CString oldPortName, newPortName;
	oldPortName.Format("%s.%s", this->name, this->outputs[index].name);
	newPortName.Format("%s.%s", this->name, name);
	for (unsigned int i = 0; i < this->parent->connlines.size(); i++)
	{
		if ( this->parent->connlines[i].outport == oldPortName )
		{
			this->parent->connlines[i].outport = newPortName;
		}
	}

	//修改该输入端口的信息
	this->outputs[index].name = name;
	this->outputs[index].type = type;
}

//移动组件到某位置
void MiseComponent::MoveEx(CRect newRect)
{
	rect = newRect;

	//修改输入端口及相应连线
	for ( unsigned int j = 0; j < this->inputs.size(); j++ )
	{
		//modify y position of other ports
		this->inputs[j].pos.x = rect.left;
		this->inputs[j].pos.y = rect.top + (rect.bottom - rect.top) / this->inputs.size() * ((double)j + 0.5);
		//modify related connectlines
		CString strTmp;
		strTmp.Format("%s.%s", this->name, this->inputs[j].name);
		for (unsigned int i = 0; i < this->parent->connlines.size(); i++)
		{
			if ( this->parent->connlines[i].inport == strTmp )
			{
				this->parent->connlines[i].endPoint = this->inputs[j].pos;
				//调整拐点位置
				if ( this->parent->connlines[i].endPoint.x - this->parent->connlines[i].startPoint.x > BREAK_SPACE )
				{
					this->parent->connlines[i].break1x = (this->parent->connlines[i].startPoint.x + this->parent->connlines[i].endPoint.x) / 2;
					this->parent->connlines[i].break2y = -1;
				}
				else
				{
					this->parent->connlines[i].break1x = this->parent->connlines[i].startPoint.x + BREAK_SPACE / 2;
					this->parent->connlines[i].break2y = (this->parent->connlines[i].startPoint.y + this->parent->connlines[i].endPoint.y) / 2;
					this->parent->connlines[i].break3x = this->parent->connlines[i].endPoint.x - BREAK_SPACE / 2;
				}
			}
		}
	}

	//修改输出端口的纵坐标位置及相应连线
	for ( unsigned int j = 0; j < this->outputs.size(); j++ )
	{
		//modify y position of other ports
		this->outputs[j].pos.x = rect.right;
		this->outputs[j].pos.y = rect.top + (rect.bottom - rect.top) / this->outputs.size() * ((double)j + 0.5);
		//modify related connectlines
		CString strTmp;
		strTmp.Format("%s.%s", this->name, this->outputs[j].name);
		for (UINT i = 0; i < this->parent->connlines.size(); i++)
		{
			if ( this->parent->connlines[i].outport == strTmp )
			{
				this->parent->connlines[i].startPoint.x = this->outputs[j].pos.x + IO_ARROW_SIZE;
				this->parent->connlines[i].startPoint.y = this->outputs[j].pos.y;
				//调整拐点位置
				if ( this->parent->connlines[i].endPoint.x - this->parent->connlines[i].startPoint.x > BREAK_SPACE )
				{
					this->parent->connlines[i].break1x = (this->parent->connlines[i].startPoint.x + this->parent->connlines[i].endPoint.x) / 2;
					this->parent->connlines[i].break2y = -1;
				}
				else
				{
					this->parent->connlines[i].break1x = this->parent->connlines[i].startPoint.x + BREAK_SPACE / 2;
					this->parent->connlines[i].break2y = (this->parent->connlines[i].startPoint.y + this->parent->connlines[i].endPoint.y) / 2;
					this->parent->connlines[i].break3x = this->parent->connlines[i].endPoint.x - BREAK_SPACE / 2;
				}
			}
		}
	}

}

//平移组件到某位置
void MiseComponent::Move(int x, int y)
{
	rect.left	+= x;
	rect.right	+= x;
	rect.top	+= y;
	rect.bottom += y;
	for (unsigned int j = 0; j < this->inputs.size(); j++)
	{
		this->inputs[j].pos.x += x;
		this->inputs[j].pos.y += y;
		//修改相应连线的坐标
		CString strTmp;
		strTmp.Format("%s.%s", this->name, this->inputs[j].name);
		for (unsigned int i = 0; i < this->parent->connlines.size(); i++)
		{
			if ( this->parent->connlines[i].inport == strTmp )
			{
				this->parent->connlines[i].endPoint = this->inputs[j].pos;
				//调整拐点位置
				if ( this->parent->connlines[i].endPoint.x - this->parent->connlines[i].startPoint.x > BREAK_SPACE )
				{
					this->parent->connlines[i].break1x = (this->parent->connlines[i].startPoint.x + this->parent->connlines[i].endPoint.x) / 2;
					this->parent->connlines[i].break2y = -1;
				}
				else
				{
					this->parent->connlines[i].break1x = this->parent->connlines[i].startPoint.x + BREAK_SPACE / 2;
					this->parent->connlines[i].break2y = (this->parent->connlines[i].startPoint.y + this->parent->connlines[i].endPoint.y) / 2;
					this->parent->connlines[i].break3x = this->parent->connlines[i].endPoint.x - BREAK_SPACE / 2;
				}
			}
		}
	}
	for (UINT j = 0; j < this->outputs.size(); j++)
	{
		this->outputs[j].pos.x += x;
		this->outputs[j].pos.y += y;
		//修改相应连线的坐标
		CString strTmp;
		strTmp.Format("%s.%s", this->name, this->outputs[j].name);
		for (UINT i = 0; i < this->parent->connlines.size(); i++)
		{
			if ( this->parent->connlines[i].outport == strTmp )
			{
				this->parent->connlines[i].startPoint.x = this->outputs[j].pos.x + IO_ARROW_SIZE;
				this->parent->connlines[i].startPoint.y = this->outputs[j].pos.y;
				//调整拐点位置
				if ( this->parent->connlines[i].endPoint.x - this->parent->connlines[i].startPoint.x > BREAK_SPACE )
				{
					this->parent->connlines[i].break1x = (this->parent->connlines[i].startPoint.x + this->parent->connlines[i].endPoint.x) / 2;
					this->parent->connlines[i].break2y = -1;
				}
				else
				{
					this->parent->connlines[i].break1x = this->parent->connlines[i].startPoint.x + BREAK_SPACE / 2;
					this->parent->connlines[i].break2y = (this->parent->connlines[i].startPoint.y + this->parent->connlines[i].endPoint.y) / 2;
					this->parent->connlines[i].break3x = this->parent->connlines[i].endPoint.x - BREAK_SPACE / 2;
				}
			}
		}
	}
}

void MiseComponent::AltName(CString newName)
{
	if ( this->name == newName )
	{
		return;
	}

	//首先确保不会重名
	for(unsigned int i = 0; i < this->parent->components.size(); i++)
	{
		if ( this->parent->components[i]->name == newName )
		{
			AfxMessageBox("存在同名组件，请重新命名");
			return;
		}
	}

	//修改相关输入输出的端口名称
	CString oldPortName, newPortName;
	for(unsigned index = 0; index < this->inputs.size(); index++)
	{
		oldPortName.Format("%s.%s", this->name, this->inputs[index].name);
		newPortName.Format("%s.%s", newName, this->inputs[index].name);
		for (unsigned int i = 0; i < this->parent->connlines.size(); i++)
		{
			if ( this->parent->connlines[i].inport == oldPortName )
			{
				this->parent->connlines[i].inport = newPortName;
			}
		}
	}
	for(unsigned index = 0; index < this->outputs.size(); index++)
	{
		oldPortName.Format("%s.%s", this->name, this->outputs[index].name);
		newPortName.Format("%s.%s", newName, this->outputs[index].name);
		for (unsigned int i = 0; i < this->parent->connlines.size(); i++)
		{
			if ( this->parent->connlines[i].outport == oldPortName )
			{
				this->parent->connlines[i].outport = newPortName;
			}
		}
	}

	//修改模块名称
	this->name = newName;
}

//仿真初始化
BOOL MiseComponent::Init()
{
	//载入对应的动态链接库
	CString strDllPath;

	//用户指定的dll，用全路径
	if ( this->type == UDF_COMPONENT )
	{
		strDllPath = this->dllPath;
	}
	//系统dll，到系统路径下去找
	else
	{
		strDllPath.Format("%s\\components\\%s", g_pApp->m_appPath, this->dllPath);
	}

	handle = LoadLibrary(strDllPath);

	if(!handle)
	{
		CString strTmp;
		strTmp.Format("加载模块%s对应的动态链接库%s出错，请检查路径设置是否正确", name, dllPath);
		AfxMessageBox(strTmp);
		return FALSE;
	}
	//获取函数入口
	(FARPROC &)(funEntry) = GetProcAddress(handle, this->functionName);
	if(!funEntry)
	{
		CString strTmp;
		strTmp.Format("加载模块%s对应的动态链接库%s的%s函数出错，请确认dll文件是否符合要求", name, dllPath, functionName);
		AfxMessageBox(strTmp);
		FreeLibrary(handle);
		return FALSE;
	}

	return TRUE;
}

//仿真运行
BOOL MiseComponent::Run(double current_time, double step_time, double *inputs, double *outputs)
{
	CString strInputsList = "";
	for (unsigned int i = 0; i < this->inputs.size(); i++)
	{
		strInputsList += this->inputs[i].name;
	}
	CString strOutputsList = "";
	for (unsigned int i = 0; i < this->outputs.size(); i++)
	{
		strOutputsList += this->outputs[i].name;
	}

	//调用dll函数
	int ok = (funEntry)(current_time, step_time, inputs, strInputsList.GetBuffer(0), outputs, strOutputsList.GetBuffer(0));
	if ( ok != 0 )
	{
		CString strTmp;
		strTmp.Format("模块%s调用的函数%s执行失败，返回代码为%d，请查询相应的错误代码表确定原因", this->name, this->functionName, ok);
		AfxMessageBox(strTmp);
		return FALSE;
	}

	return TRUE;
}

//释放资源
BOOL MiseComponent::Clear()
{
	FreeLibrary(handle);
	return TRUE;
}

//运行之后的后处理
BOOL MiseComponent::PostRun()
{
	return true;
}

//显示模型信息，缺省为模型定义
BOOL MiseComponent::Display()
{
	CComDefDialog dlgComDef(this);
	if (dlgComDef.DoModal() == IDOK)
	{
		//修改模块名称
		this->AltName(dlgComDef.m_name);

		//修改模块对应的dll位置
		this->dllPath = dlgComDef.m_dllPath;

		//修改模块的函数入口
		this->functionName = dlgComDef.m_funcName;

		//修改模块注释
		this->comment = dlgComDef.m_comment;
	}

	return TRUE;
}

//========================= end of class MiseComponent ===========================================


//============================ class MiseModel ===================================================
MiseModel::MiseModel()
{
}

MiseModel::~MiseModel()
{
	components.clear();
	connlines.clear();
}

void MiseModel::AppendComponent(MiseComponent* pMiseComponent)
{
	//首先确保不会重名
	UINT i;
	for( i = 0; i < this->components.size(); i++)
	{
		if ( this->components[i]->name == pMiseComponent->name )
			break;
	}
	//如果有重名组件，则在新组件后顺序加数字避免重名
	if ( i < this->components.size() )
	{
		UINT j;
		CString strTmp;
		for( j = 0; ; j++)
		{
			strTmp.Format("%s%d",pMiseComponent->name, j);
			UINT k;
			for(k = 0; k < components.size(); k++)
			{
				if ( strTmp == components[k]->GetName() )
					break;
			}
			if ( k == components.size() )
				break;
		}
		pMiseComponent->name = strTmp;
	}

	//加入组件列表中
	pMiseComponent->parent = this;
	components.push_back(pMiseComponent);

}
//delete a component based on its index
void MiseModel::DelComponent(unsigned int index)
{
	ASSERT(index >= 0 && index < this->components.size());

	//delete related connect lines, input
	for(unsigned int i = 0; i < this->components[index]->inputs.size(); i++)
	{
		CString strTmp;
		strTmp.Format("%s.%s", this->components[index]->name, this->components[index]->inputs[i].name);
		for (unsigned int j = 0; j < this->connlines.size(); j++)
		{
			if ( this->connlines[j].inport == strTmp )
			{
				this->connlines.erase(this->connlines.begin() + j);
				j--;
			}
		}
	}

	//delete related connect lines, outpput
	for(unsigned int i = 0; i < this->components[index]->outputs.size(); i++)
	{
		CString strTmp;
		strTmp.Format("%s.%s", this->components[index]->name, this->components[index]->outputs[i].name);
		for (unsigned int j = 0; j < this->connlines.size(); j++)
		{
			if ( this->connlines[j].outport == strTmp )
			{
				this->connlines.erase(this->connlines.begin() + j);
				j--;
			}
		}
	}

	//delete component
	delete(this->components[index]);
	this->components.erase(this->components.begin() + index);

}

//清空当前模型数据
void MiseModel::Clear()
{
	this->components.clear();
	this->connlines.clear();
}

BOOL MiseModel::IsEmpty()
{
	if ( this->components.size() == 0 && this->connlines.size() == 0 )
		return TRUE;

	return FALSE;
}
BOOL MiseModel::Save(CString fileName)
{
	MSXML2::IXMLDOMDocumentPtr pXMLDom;
	HRESULT hr;

	hr = pXMLDom.CreateInstance(__uuidof(DOMDocument30));
	if (FAILED(hr)) 
	{
		printf("Failed to CreateInstance on an XML DOM");
		return NULL;
	}
	pXMLDom->preserveWhiteSpace = VARIANT_TRUE;
	   
	// Create a processing instruction targeted for xml.
	MSXML2::IXMLDOMProcessingInstructionPtr pi;
	pi = pXMLDom->createProcessingInstruction("xml", "version='1.0'");
	if (pi != NULL) {
		pXMLDom->appendChild(pi);
		pi.Release();
	}

	// Create a processing instruction targeted for xml-stylesheet.
	pi = pXMLDom->createProcessingInstruction("xml-stylesheet", "type='text/xml' href='dom.xsl'");
	if (pi != NULL) {
		pXMLDom->appendChild(pi);
		pi.Release();
	}

	// Create a comment for the document.
	MSXML2::IXMLDOMCommentPtr pc;
	pc = pXMLDom->createComment("Multidisciplinary simulation model, version='1.0'");
	if (pc != NULL) {
		pXMLDom->appendChild(pc);
		pc.Release();
	}

	// Create the root element (i.e., the documentElement).
	MSXML2::IXMLDOMElementPtr pe;
	pe = pXMLDom->createElement("root");

	MSXML2::IXMLDOMAttributePtr pa;
	pa = pXMLDom->createAttribute("CreateTime");
	if (pa != NULL)
	{
		CTime t = CTime::GetCurrentTime();
		CString s = t.Format( "%Y, %B %d, %H:%M:%S" );

		pa->value = s.GetBuffer();
		
		pe->setAttributeNode(pa);
		pa.Release();
	}

	// Add the root element to the DOM instance.
	pXMLDom->appendChild(pe);
	pe.Release();

	// 存储仿真参数 =============================================================================
	pe = pXMLDom->createElement("sim_parameters");
	if ( pe != NULL )
	{
		pe->setAttribute("start_time", g_pApp->m_start_time);
		pe->setAttribute("end_time", g_pApp->m_end_time);
		pe->setAttribute("step_time", g_pApp->m_step_time);
		pXMLDom->documentElement->appendChild(pe);
		pe.Release();
	}
	// 仿真参数存储完毕 =========================================================================

	//=========================== 存储Component =================================================
	pe = pXMLDom->createElement("components");
	if (pe != NULL)
	{
		//Attention: IXMLDOMElementPtr是智能指针，不需要release，除非在一个定义段里循环利用
		for ( UINT i = 0; i < this->components.size(); i++)
		{
			//创建component节点
			MSXML2::IXMLDOMElementPtr pe_component = pXMLDom->createElement("component");

			if ( pe_component != NULL )
			{
				//写入component的name属性
				pe_component->setAttribute("name", components[i]->name.GetBuffer(0));

				//写入component的type属性
				pe_component->setAttribute("type", components[i]->type);

				//写入component的dllpath属性
				pe_component->setAttribute("dll_path", components[i]->dllPath.GetBuffer(0));

				//写入component的function_name属性
				pe_component->setAttribute("function_name", components[i]->functionName.GetBuffer(0));

				//加入component的comment子节点
				MSXML2::IXMLDOMElementPtr pe_comment = pXMLDom->createElement("comment");
				if ( pe_comment != NULL )
				{
					pe_comment->text = components[i]->comment.GetBuffer(0);
					pe_component->appendChild(pe_comment);
				}

				//------------------- 加入component的rect子节点 ----------------------
				MSXML2::IXMLDOMElementPtr pe_rect = pXMLDom->createElement("rect");
				if ( pe_rect != NULL )
				{
					pe_rect->setAttribute("left", components[i]->rect.left);
					pe_rect->setAttribute("right", components[i]->rect.right);
					pe_rect->setAttribute("top", components[i]->rect.top);
					pe_rect->setAttribute("bottom", components[i]->rect.bottom);
					pe_component->appendChild(pe_rect);
					
				}
				//------------------- end of component的rect子节点 ----------------------

				//------------------- 加入component的input子节点 ------------------------
				MSXML2::IXMLDOMElementPtr pe_inputs = pXMLDom->createElement("inputs");
				if ( pe_inputs != NULL )
				{
					//创建input子节点
					for ( UINT j = 0; j < components[i]->inputs.size(); j++ )
					{
						MSXML2::IXMLDOMElementPtr pe_input = pXMLDom->createElement("input");
						if ( pe_input != NULL )
						{
							//设定input属性
							pe_input->setAttribute("name", components[i]->inputs[j].name.GetBuffer(0));

							switch ( components[i]->inputs[j].type )
							{
							case(MISE_DOUBLE):
								pe_input->setAttribute("type", "double");
								break;
							default:
								break;
							}

							pe_input->setAttribute("default_value", components[i]->inputs[j].default_value.l);

							//加入input的position子节点
							MSXML2::IXMLDOMElementPtr pe_pos = pXMLDom->createElement("position");
							if ( pe_rect != NULL )
							{
								pe_pos->setAttribute("x", components[i]->inputs[j].pos.x);
								pe_pos->setAttribute("y", components[i]->inputs[j].pos.y);
								pe_input->appendChild(pe_pos);
							}

							//将该input加入到inputs中
							pe_inputs->appendChild(pe_input);
						}
					}
					pe_component->appendChild(pe_inputs);
				}
				//----------------------------- end of input ------------------------------

				//------------------------- 加入component的output子节点 -------------------
				MSXML2::IXMLDOMElementPtr pe_outputs = pXMLDom->createElement("outputs");
				if ( pe_outputs != NULL )
				{
					//创建input子节点
					for ( UINT j = 0; j < components[i]->outputs.size(); j++ )
					{
						MSXML2::IXMLDOMElementPtr pe_output = pXMLDom->createElement("output");
						if ( pe_output != NULL )
						{
							//设定input属性
							pe_output->setAttribute("name", components[i]->outputs[j].name.GetBuffer(0));

							switch ( components[i]->outputs[j].type )
							{
							case(MISE_DOUBLE):
								pe_output->setAttribute("type", "double");
								break;
							default:
								break;
							}


							//加入output的position子节点
							MSXML2::IXMLDOMElementPtr pe_pos = pXMLDom->createElement("position");
							if ( pe_rect != NULL )
							{
								pe_pos->setAttribute("x", components[i]->outputs[j].pos.x);
								pe_pos->setAttribute("y", components[i]->outputs[j].pos.y);
								pe_output->appendChild(pe_pos);
							}

							//将该input加入到inputs中
							pe_outputs->appendChild(pe_output);
						}
					}
					pe_component->appendChild(pe_outputs);
				}
				//------------------------- end of output ----------------------------------
	
				//------------------------- 加入component的special子节点 -------------------
				MSXML2::IXMLDOMElementPtr pe_special = pXMLDom->createElement("special");
				if ( pe_special != NULL )
				{
					//根据不同的组件类型填写不同的参数
					switch( components[i]->type )
					{

						case(SRC_CLOCK):	//sin 源
						{
							break;
						}

						case(SRC_SIN):	//sin 源
						{
							Mise_source_sin *pSin = (Mise_source_sin*)components[i];
							pe_special->setAttribute("A", pSin->m_A);
							pe_special->setAttribute("B", pSin->m_B);
							pe_special->setAttribute("C", pSin->m_C);
							break;
						}
						
						case(SRC_COS):	//cos 源
						{
							Mise_source_cos *pCos = (Mise_source_cos*)components[i];
							pe_special->setAttribute("A", pCos->m_A);
							pe_special->setAttribute("B", pCos->m_B);
							pe_special->setAttribute("C", pCos->m_C);
							break;
						}
						case(SRC_CONST):	//const 源
						{
							Mise_source_const *pConst = (Mise_source_const*)components[i];
							pe_special->setAttribute("value", pConst->m_value);
							break;
						}

						case(SRC_STEP):		//step 源
						{
							Mise_source_step *pStep = (Mise_source_step*)components[i];
							pe_special->setAttribute("A", pStep->m_A);
							pe_special->setAttribute("B", pStep->m_B);
							pe_special->setAttribute("T", pStep->m_T);
							break;
						}

						case(MATH_ADD):	//加法器
						{
							break;
						}

						case(MATH_MUL):	//乘法器
						{
							break;
						}

						case(MATH_RECIPROCAL):	//取倒数
						{
							break;
						}

						case(MATH_POW):		//幂运算
						{
							break;
						}

						case(CONT_DIFF):		//取微分
						{
							break;
						}

						case(CONT_INTEGRAL):	//取积分
						{
							break;
						}

						case(OUT_FILER):	//输出到文件
						{
							Mise_output_filer *pFiler = (Mise_output_filer*)components[i];
							pe_special->setAttribute("file_name", pFiler->m_fileName.GetBuffer(0));
							pe_special->setAttribute("with_title", pFiler->m_withTitle);
							pe_special->setAttribute("with_time", pFiler->m_withTime);
							break;
						}
						
						case(OUT_MONITOR):	//结果输出为曲线
						{
							break;
						}

						case(CONT_TRANSFUN):		//一般传递函数
						{
							Mise_cont_transfunction *pTranFun = (Mise_cont_transfunction*)components[i];

							CString upParameters = "[";	//分子系数
							for ( unsigned int i = 0; i < pTranFun->m_upList.size(); i++)
							{
								CString strTmp;
								strTmp.Format(" %f", pTranFun->m_upList[i]);
								DelEndZeroFromStr(strTmp);
								upParameters += strTmp;
							}
							upParameters += "]";
							pe_special->setAttribute("UP", upParameters.GetBuffer(0));

							CString downParameters = "[";	//分母系数
							for ( unsigned int i = 0; i < pTranFun->m_downList.size(); i++)
							{
								CString strTmp;
								strTmp.Format(" %f", pTranFun->m_downList[i]);
								DelEndZeroFromStr(strTmp);
								downParameters += strTmp;
							}
							downParameters += "]";
							pe_special->setAttribute("DP", downParameters.GetBuffer(0));
							break;
						}
						
						case(CONT_ZEROPOLE):		//零－极点分析
						{
							Mise_cont_ZeroPole *pZeroPole = (Mise_cont_ZeroPole*)components[i];

							// 增益
							pe_special->setAttribute("K", pZeroPole->m_plus);

							CString upParameters = "[";	//分子系数
							for ( unsigned int i = 0; i < pZeroPole->m_upList.size(); i++)
							{
								CString strTmp;
								strTmp.Format(" %f", pZeroPole->m_upList[i]);
								DelEndZeroFromStr(strTmp);
								upParameters += strTmp;
							}
							upParameters += "]";
							pe_special->setAttribute("UP", upParameters.GetBuffer(0));

							CString downParameters = "[";	//分母系数
							for ( unsigned int i = 0; i < pZeroPole->m_downList.size(); i++)
							{
								CString strTmp;
								strTmp.Format(" %f", pZeroPole->m_downList[i]);
								DelEndZeroFromStr(strTmp);
								downParameters += strTmp;
							}
							downParameters += "]";
							pe_special->setAttribute("DP", downParameters.GetBuffer(0));
							break;
						}

						default:
						{
							break;
						}
					}

					pe_component->appendChild(pe_special);
				}


				//------------------------- end of special ---------------------------------

				//将该component加入到模型中
				pe->appendChild(pe_component);
			}
		}
		pXMLDom->documentElement->appendChild(pe);
		pe.Release();
	}
	//========================== end of 存储component ============================
	
	//========================== 存储连线 ========================================
	pe = pXMLDom->createElement("connLines");
	if (pe != NULL)
	{
		for(UINT i = 0; i < this->connlines.size(); i++)
		{

			//创建connLine节点
			MSXML2::IXMLDOMElementPtr pe_connLine = pXMLDom->createElement("connLine");
			if ( pe_connLine != NULL )
			{
				//写入connLine的相关属性
				pe_connLine->setAttribute("break1x", connlines[i].break1x);
				pe_connLine->setAttribute("break2y", connlines[i].break2y);
				pe_connLine->setAttribute("break3x", connlines[i].break3x);

				pe_connLine->setAttribute("inport", connlines[i].inport.GetBuffer(0));
				pe_connLine->setAttribute("outport", connlines[i].outport.GetBuffer(0));

				//写入起点位置
				MSXML2::IXMLDOMElementPtr pe_startPt = pXMLDom->createElement("start_point");
				if ( pe_startPt != NULL )
				{
					pe_startPt->setAttribute("x", connlines[i].startPoint.x);
					pe_startPt->setAttribute("y", connlines[i].startPoint.y);

					pe_connLine->appendChild(pe_startPt);
				}

				//写入终点位置
				MSXML2::IXMLDOMElementPtr pe_endPt = pXMLDom->createElement("end_point");
				if ( pe_endPt != NULL )
				{
					pe_endPt->setAttribute("x", connlines[i].endPoint.x);
					pe_endPt->setAttribute("y", connlines[i].endPoint.y);

					pe_connLine->appendChild(pe_endPt);
				}

				pe->appendChild(pe_connLine);

			}
		}
		pXMLDom->documentElement->appendChild(pe);

	}
	//========================== end of 存储连线 =================================

	hr = pXMLDom->save(fileName.GetBuffer(0));

	if (pXMLDom)
		pXMLDom.Release();

	if (FAILED(hr)) 
	{
		return FALSE;
	}

	return TRUE;
}

BOOL MiseModel::Load(CString fileName)
{
	//确定当前模型为空
	ASSERT(this->IsEmpty());

	MSXML2::IXMLDOMDocumentPtr pXMLDom;
	HRESULT hr;

	hr = pXMLDom.CreateInstance(__uuidof(DOMDocument30));
	if (FAILED(hr)) 
	{
		AfxMessageBox("初始化DOMDocument30类失败\n");
		return FALSE;
	}

	pXMLDom->async = VARIANT_FALSE;
	if ( pXMLDom->load(fileName.GetBuffer(0)) != VARIANT_TRUE)
	{
		CString strTmp = "加载模型文件失败\n";
		strTmp += (LPCSTR)pXMLDom->parseError->Getreason();
		AfxMessageBox(strTmp);
		return FALSE;
	}

	// 加载仿真参数 ==================================================================
	MSXML2::IXMLDOMNodePtr pNodeSimParameters = pXMLDom->selectSingleNode("//sim_parameters[0]");
	g_pApp->m_start_time = ((MSXML2::IXMLDOMElementPtr)pNodeSimParameters)->getAttribute("start_time");
	g_pApp->m_end_time = ((MSXML2::IXMLDOMElementPtr)pNodeSimParameters)->getAttribute("end_time");
	g_pApp->m_step_time = ((MSXML2::IXMLDOMElementPtr)pNodeSimParameters)->getAttribute("step_time");
	// 仿真参数加载完毕 ==============================================================


	UINT i;
	//=========================== 加载component ======================================
	MiseComponent* pComponent;
	CString strQryComponent;
	MSXML2::IXMLDOMNodePtr pNodeComponent;

	i = 0;
	strQryComponent.Format("//components/component[%d]", i++);
	pNodeComponent = pXMLDom->selectSingleNode(strQryComponent.GetBuffer(0));
	while ( pNodeComponent != NULL )
	{
		//获取type
		int comType = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("type");

		//根据不同的type生成不同的组件，以后还要添加特殊属性
		switch(comType)
		{
			case(SRC_CLOCK):	//sin 源
			{
				//创建组件
				Mise_source_clock *pSourceClock = new Mise_source_clock();

				//向上传递指针
				pComponent = pSourceClock;
				break;
			}

			case(SRC_SIN):	//sin 源
			{
				//创建组件
				Mise_source_sin *pSourceSin = new Mise_source_sin();

				//获取附加属性
				CString strQrySpecial;
				strQrySpecial = strQryComponent + "/" + "special";
				MSXML2::IXMLDOMNodePtr pNodeSpecial = pXMLDom->selectSingleNode(strQrySpecial.GetBuffer(0));
				pSourceSin->m_A = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("A");
				pSourceSin->m_B = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("B");
				pSourceSin->m_C = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("C");
				

				//向上传递指针
				pComponent = pSourceSin;
				break;
			}
			
			case(SRC_COS):	//cos 源
			{
				//创建组件
				Mise_source_cos *pSourceCos = new Mise_source_cos();

				//获取附加属性
				CString strQrySpecial;
				strQrySpecial = strQryComponent + "/" + "special";
				MSXML2::IXMLDOMNodePtr pNodeSpecial = pXMLDom->selectSingleNode(strQrySpecial.GetBuffer(0));
				pSourceCos->m_A = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("A");
				pSourceCos->m_B = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("B");
				pSourceCos->m_C = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("C");

				//向上传递指针
				pComponent = pSourceCos;
				break;
			}
			case(SRC_CONST):	//const 源
			{
				//创建组件
				Mise_source_const *pSourceConst = new Mise_source_const();

				//获取附加属性
				CString strQrySpecial;
				strQrySpecial = strQryComponent + "/" + "special";
				MSXML2::IXMLDOMNodePtr pNodeSpecial = pXMLDom->selectSingleNode(strQrySpecial.GetBuffer(0));
				pSourceConst->m_value = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("value");

				//向上传递指针
				pComponent = pSourceConst;
				break;
			}

			case(SRC_STEP):		//step 源
			{
				//创建组件
				Mise_source_step *pSourceStep = new Mise_source_step();

				//获取附加属性
				CString strQrySpecial;
				strQrySpecial = strQryComponent + "/" + "special";
				MSXML2::IXMLDOMNodePtr pNodeSpecial = pXMLDom->selectSingleNode(strQrySpecial.GetBuffer(0));
				pSourceStep->m_A = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("A");
				pSourceStep->m_B = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("B");
				pSourceStep->m_T = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("T");

				//向上传递指针
				pComponent = pSourceStep;
				break;
			}

			case(MATH_ADD):	//加法器
			{
				//创建组件
				Mise_math_add *pMathAdd = new Mise_math_add();
				//向上传递指针
				pComponent = pMathAdd;
				break;
			}

			case(MATH_MUL):	//乘法器
			{
				//创建组件
				Mise_math_multiply *pMathMul = new Mise_math_multiply();
				//向上传递指针
				pComponent = pMathMul;
				break;
			}

			case(MATH_RECIPROCAL):	//取倒数
			{
				//创建组件
				Mise_math_reciprocal *pMathRec = new Mise_math_reciprocal();
				//向上传递指针
				pComponent = pMathRec;
				break;
			}

			case(MATH_POW):		//幂运算
			{
				//创建组件
				Mise_math_pow *pMathPow = new Mise_math_pow();
				//向上传递指针
				pComponent = pMathPow;
				break;
			}

			case(CONT_DIFF):	//取微分
			{
				//创建组件
				Mise_cont_differential *pContDiff = new Mise_cont_differential();
				//向上传递指针
				pComponent = pContDiff;
				break;
			}

			case(CONT_INTEGRAL):	//取积分
			{
				//创建组件
				Mise_cont_integral *pContIntegral = new Mise_cont_integral();
				//向上传递指针
				pComponent = pContIntegral;
				break;
			}

			case(CONT_TRANSFUN):	//传递函数
			{
				//创建组件
				Mise_cont_transfunction *pTranFun = new Mise_cont_transfunction();
				//获取附加属性
				CString strQrySpecial;
				strQrySpecial = strQryComponent + "/" + "special";
				MSXML2::IXMLDOMNodePtr pNodeSpecial = pXMLDom->selectSingleNode(strQrySpecial.GetBuffer(0));
				CString upParameters = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("UP");
				pTranFun->m_upList.clear();
				if(!ParseParameters(upParameters, pTranFun->m_upList) || pTranFun->m_upList.size() == 0)
				{
					AfxMessageBox("获取传递函数的分子系数失败");
					return false;
				}

				CString downParameters = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("DP");
				pTranFun->m_downList.clear();
				if(!ParseParameters(downParameters, pTranFun->m_downList) || pTranFun->m_downList.size() == 0)
				{
					AfxMessageBox("获取传递函数的分母系数失败");
					return false;
				}

				//向上传递指针
				pComponent = pTranFun;
				break;
			}

			case(CONT_ZEROPOLE):	//零－极点形式的传递函数
			{
				//创建组件
				Mise_cont_ZeroPole *pZeroPole = new Mise_cont_ZeroPole();
				//获取附加属性
				CString strQrySpecial;
				strQrySpecial = strQryComponent + "/" + "special";
				MSXML2::IXMLDOMNodePtr pNodeSpecial = pXMLDom->selectSingleNode(strQrySpecial.GetBuffer(0));

				pZeroPole->m_plus = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("K");

				CString upParameters = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("UP");
				pZeroPole->m_upList.clear();
				if(!ParseParameters(upParameters, pZeroPole->m_upList))
				{
					AfxMessageBox("获取传递函数的分子系数失败");
					return false;
				}

				CString downParameters = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("DP");
				pZeroPole->m_downList.clear();
				if(!ParseParameters(downParameters, pZeroPole->m_downList))
				{
					AfxMessageBox("获取传递函数的分母系数失败");
					return false;
				}

				//向上传递指针
				pComponent = pZeroPole;
				break;
			}

			case(OUT_FILER):	//输出到文件
			{
				//创建组件
				Mise_output_filer *pOutputFiler = new Mise_output_filer();

				//获取附加属性
				CString strQrySpecial;
				strQrySpecial = strQryComponent + "/" + "special";
				MSXML2::IXMLDOMNodePtr pNodeSpecial = pXMLDom->selectSingleNode(strQrySpecial.GetBuffer(0));
				pOutputFiler->m_fileName = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("file_name");
				pOutputFiler->m_withTime = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("with_time");
				pOutputFiler->m_withTitle = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("with_title");

				//向上传递指针
				pComponent = pOutputFiler;
				break;
			}
			
			case(OUT_MONITOR):	//结果输出为曲线
			{
				//创建组件
				Mise_output_monitor *pOutputMonitor = new Mise_output_monitor();
				//向上传递指针
				pComponent = pOutputMonitor;
				break;
			}

			default:
			{
				//创建组件
				MiseComponent *pUDFComponent = new MiseComponent();
				//向上传递指针
				pComponent = pUDFComponent;
				break;
			}
		}


		//获取name
		pComponent->name = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("name");
		//获取type
		pComponent->type = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("type");
		//获取dllPath
		pComponent->dllPath = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("dll_path");
		//获取functionName
		pComponent->functionName = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("function_name");

		//获取comment
		CString strQryComment;
		strQryComment = strQryComponent + "/" + "comment";
		MSXML2::IXMLDOMNodePtr pNodeComment = pXMLDom->selectSingleNode(strQryComment.GetBuffer(0));
		pComponent->comment = ((MSXML2::IXMLDOMElementPtr)pNodeComment)->text.GetBSTR();

		//获取rect
		CString strQryRect;
		strQryRect = strQryComponent + "/" + "rect";
		MSXML2::IXMLDOMNodePtr pNodeRect = pXMLDom->selectSingleNode(strQryRect.GetBuffer(0));
		int left = ((MSXML2::IXMLDOMElementPtr)pNodeRect)->getAttribute("left");
		int right = ((MSXML2::IXMLDOMElementPtr)pNodeRect)->getAttribute("right");
		int top = ((MSXML2::IXMLDOMElementPtr)pNodeRect)->getAttribute("top");
		int bottom = ((MSXML2::IXMLDOMElementPtr)pNodeRect)->getAttribute("bottom");
		pComponent->rect = CRect(left, top, right, bottom);


		//获取inputs
		UINT j = 0;
		CString strQryInput;
		MSXML2::IXMLDOMNodePtr pNodeInput;
		strQryInput.Format("%s/inputs/input[%d]", strQryComponent, j++);
		pNodeInput = pXMLDom->selectSingleNode(strQryInput.GetBuffer(0));

		while ( pNodeInput != NULL )
		{
			CString strInputName = ((MSXML2::IXMLDOMElementPtr)pNodeInput)->getAttribute("name");
			CString strInputType = ((MSXML2::IXMLDOMElementPtr)pNodeInput)->getAttribute("type");
			double dblInputDefValue = ((MSXML2::IXMLDOMElementPtr)pNodeInput)->getAttribute("default_value");

			//获取position
			CString strQryInputPos;
			strQryInputPos = strQryInput + "/" + "position";
			MSXML2::IXMLDOMNodePtr pNodePosition = pXMLDom->selectSingleNode(strQryInputPos.GetBuffer(0));
			int x = ((MSXML2::IXMLDOMElementPtr)pNodePosition)->getAttribute("x");
			int y = ((MSXML2::IXMLDOMElementPtr)pNodePosition)->getAttribute("y");

			int type;
			if ( strInputType == "double" )
				type = MISE_DOUBLE;
			else
				type = MISE_UNKNOWN;

			IOValue default_value;
			default_value.l = dblInputDefValue;

			pComponent->AddInput(strInputName, type, default_value);

			strQryInput.Format("%s/inputs/input[%d]", strQryComponent, j++);
			pNodeInput = pXMLDom->selectSingleNode(strQryInput.GetBuffer(0));
		}

		//获取outputs
		UINT k = 0;
		CString strQryOutput;
		MSXML2::IXMLDOMNodePtr pNodeOutput;
		strQryOutput.Format("%s/outputs/output[%d]", strQryComponent, k++);
		pNodeOutput = pXMLDom->selectSingleNode(strQryOutput.GetBuffer(0));

		while ( pNodeOutput != NULL )
		{
			CString strOutputName = ((MSXML2::IXMLDOMElementPtr)pNodeOutput)->getAttribute("name");
			CString strOutputType = ((MSXML2::IXMLDOMElementPtr)pNodeOutput)->getAttribute("type");

			//获取position
			CString strQryOutputPos;
			strQryOutputPos = strQryOutput + "/" + "position";
			MSXML2::IXMLDOMNodePtr pNodePosition = pXMLDom->selectSingleNode(strQryOutputPos.GetBuffer(0));
			int x = ((MSXML2::IXMLDOMElementPtr)pNodePosition)->getAttribute("x");
			int y = ((MSXML2::IXMLDOMElementPtr)pNodePosition)->getAttribute("y");

			int type;
			if ( strOutputType == "double" )
				type = MISE_DOUBLE;
			else
				type = MISE_UNKNOWN;


			pComponent->AddOutput(strOutputName, type);

			strQryOutput.Format("%s/outputs/output[%d]", strQryComponent, k++);
			pNodeOutput = pXMLDom->selectSingleNode(strQryOutput.GetBuffer(0));
		}

		//将该组件加入到模型中
		this->AppendComponent(pComponent);

		//继续查询下一个节点
		strQryComponent.Format("//components/component[%d]", i++);
		pNodeComponent = pXMLDom->selectSingleNode(strQryComponent.GetBuffer(0));

	}
	//=========================== end of 加载component ======================================

	//=========================== 加载连线 ==================================================
	CString strQryConnLine;
	MSXML2::IXMLDOMNodePtr pNodeConnLine;

	i = 0;
	strQryConnLine.Format("//connLines/connLine[%d]", i++);
	pNodeConnLine = pXMLDom->selectSingleNode(strQryConnLine.GetBuffer(0));
	while ( pNodeConnLine != NULL )
	{
		//创建连线对象
		ConnectLine* pConnLine = new ConnectLine;

		//获取相关属性
		pConnLine->inport = ((MSXML2::IXMLDOMElementPtr)pNodeConnLine)->getAttribute("inport");
		pConnLine->outport = ((MSXML2::IXMLDOMElementPtr)pNodeConnLine)->getAttribute("outport");
		pConnLine->break1x = ((MSXML2::IXMLDOMElementPtr)pNodeConnLine)->getAttribute("break1x");
		pConnLine->break2y = ((MSXML2::IXMLDOMElementPtr)pNodeConnLine)->getAttribute("break2y");
		pConnLine->break3x = ((MSXML2::IXMLDOMElementPtr)pNodeConnLine)->getAttribute("break3x");

		//获取输入点位置
		CString strQryStartPt = strQryConnLine + "/" + "start_point";
		MSXML2::IXMLDOMNodePtr pNodeStartPt = pXMLDom->selectSingleNode(strQryStartPt.GetBuffer(0));
		if ( pNodeStartPt != NULL)
		{
			pConnLine->startPoint.x = ((MSXML2::IXMLDOMElementPtr)pNodeStartPt)->getAttribute("x");
			pConnLine->startPoint.y = ((MSXML2::IXMLDOMElementPtr)pNodeStartPt)->getAttribute("y");
		}

		//获取输出点位置
		CString strQryEndPt = strQryConnLine + "/" + "end_point";
		MSXML2::IXMLDOMNodePtr pNodeEndPt = pXMLDom->selectSingleNode(strQryEndPt.GetBuffer(0));
		if ( pNodeEndPt != NULL )
		{
			pConnLine->endPoint.x = ((MSXML2::IXMLDOMElementPtr)pNodeEndPt)->getAttribute("x");
			pConnLine->endPoint.y = ((MSXML2::IXMLDOMElementPtr)pNodeEndPt)->getAttribute("y");
		}

		//连线加入模型中
		this->AddConnLine(pConnLine);

		//下一个查询
		strQryConnLine.Format("//connLines/connLine[%d]", i++);
		pNodeConnLine = pXMLDom->selectSingleNode(strQryConnLine.GetBuffer(0));
	}

	//=========================== end of 加载连线 ===========================================



	pXMLDom.Release();

	return TRUE;
}

void MiseModel::AddConnLine(ConnectLine* pConnLine)
{
	this->connlines.push_back(*pConnLine);
}

//=========================== end of class MiseModel ========================================

//=========================== class ConnectLine =============================================
ConnectLine::ConnectLine()
{
	m_isSelected = FALSE;
}
ConnectLine::~ConnectLine()
{
}

//检测当鼠标点击point点的时候是否选中该连线
BOOL ConnectLine::NeedBeSelected(CPoint point)
{
	CRect rect;
	//检测第一段折线
	rect.left = this->startPoint.x;
	rect.right = this->break1x;
	rect.top = this->startPoint.y - SEL_SENSITIVITY;
	rect.bottom = this->startPoint.y + SEL_SENSITIVITY;
	if(rect.PtInRect(point)){
		selectedPart = 1;
		return TRUE;
	}

	if (this->break2y == -1 )	//有两个拐点
	{
		//检测第二段折线
		rect.left = this->break1x - SEL_SENSITIVITY;
		rect.right = this->break1x + SEL_SENSITIVITY;
		rect.top = this->startPoint.y < this->endPoint.y ? this->startPoint.y : this->endPoint.y;
		rect.bottom = this->startPoint.y < this->endPoint.y ? this->endPoint.y : this->startPoint.y;
		if(rect.PtInRect(point)){
			selectedPart = 2;
			return TRUE;
		}
		//检测第三段折线
		rect.left = this->break1x;
		rect.right = this->endPoint.x;
		rect.top = this->endPoint.y - SEL_SENSITIVITY;;
		rect.bottom = this->endPoint.y + SEL_SENSITIVITY;;
		if(rect.PtInRect(point)){
			selectedPart = 3;
			return TRUE;
		}
	}
	else	//有四个拐点
	{
		//检测第二段折线
		rect.left = this->break1x - SEL_SENSITIVITY;
		rect.right = this->break1x + SEL_SENSITIVITY;
		rect.top = this->startPoint.y < this->break2y ? this->startPoint.y : this->break2y;
		rect.bottom = this->startPoint.y < this->break2y ? this->break2y : this->startPoint.y;
		if(rect.PtInRect(point)){
			selectedPart = 2;
			return TRUE;
		}

		//检测第三段折线
		rect.left = this->break3x;
		rect.right = this->break1x;
		rect.top = this->break2y - SEL_SENSITIVITY;;
		rect.bottom = this->break2y + SEL_SENSITIVITY;;
		if(rect.PtInRect(point)){
			selectedPart = 3;
			return TRUE;
		}

		//检测第四段折线
		rect.left = this->break3x - SEL_SENSITIVITY;
		rect.right = this->break3x + SEL_SENSITIVITY;
		rect.top = this->break2y < this->endPoint.y ? this->break2y : this->endPoint.y;
		rect.bottom = this->break2y < this->endPoint.y ? this->endPoint.y : this->break2y;
		if(rect.PtInRect(point)){
			selectedPart = 4;
			return TRUE;
		}

		//检测第五段折线
		rect.left = this->break3x;
		rect.right = this->endPoint.x;
		rect.top = this->endPoint.y - SEL_SENSITIVITY;;
		rect.bottom = this->endPoint.y + SEL_SENSITIVITY;;
		if(rect.PtInRect(point)){
			selectedPart = 5;
			return TRUE;
		}

	}

	//都不在范围内，返回FALSE
	return FALSE;

}

//=========================== end of class ConnectLine ======================================
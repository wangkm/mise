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
	name = "";	//ָ����ģ������
	dllPath = "";		//��ʼʱ��δָ��dll�ļ�λ��
	functionName = "";
	parent = NULL;
	isSelected = false;	//��ʼ����δ��ѡ��״̬

	m_width = COM_WIDTH;
	m_height = COM_HEIGHT;
	m_displayPortName = true;
	//rect.left = 0;
	//rect.top = 0;
	//rect.right = COM_WIDTH;
	//rect.bottom = COM_HEIGHT;

	//��������˿ڵ���Ŀ��Χ
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

//���Ըñ����Ƿ����
BOOL MiseComponent::TestExist(CString IOName)
{
	ASSERT(IOName.Trim() != "");

	unsigned int i;
	//�������˿�
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

	//�������˿�
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

	//δ������������������FALSE
	return FALSE;

}
//add a new input
void MiseComponent::AddInput(CString name, int type, IOValue default_value)
{
	//����ȷ���Ƿ�������
	if ( this->inputs.size() >= this->m_inputs_max )
	{
		CString msg;
		msg.Format("���ʧ�ܡ���ģ���������%d������ӿ�", this->m_inputs_max);
		AfxMessageBox(msg);
		return;
	}


	//����name�Ƿ��ظ�������ظ�����ʾ��������
	if ( TestExist(name) )
	{
		AfxMessageBox("�Ѵ���ͬ������������������");
		return;
	}

	//��������˿�vector��
	IOPort *pInput = new IOPort;
	pInput->name = name;
	pInput->type = type;
	pInput->default_value = default_value;
	pInput->pos.x = rect.left;
	inputs.push_back(*pInput);

	//�޸���������˿ڵ�������λ�ü���Ӧ����
	for ( unsigned int i = 0; i < this->inputs.size(); i++ )
	{
		//modify y position of other ports
		this->inputs[i].pos.y = rect.top + (rect.bottom - rect.top) / this->inputs.size() * ((double)i + 0.5);
		if ( this->parent )	//���������Ѿ����뵽ģ����
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
	//����ȷ���Ƿ�������
	if ( this->outputs.size() >= this->m_outputs_max )
	{
		CString msg;
		msg.Format("���ʧ�ܡ���ģ���������%d������ӿ�", this->m_outputs_max);
		AfxMessageBox(msg);
		return;
	}

	//����name�Ƿ��ظ�������ظ�����ʾ��������
	if ( TestExist(name) )
	{
		AfxMessageBox("�Ѵ���ͬ������������������");
		return;
	}

	//��������˿�vector��
	IOPort *pOutput = new IOPort;
	pOutput->name = name;
	pOutput->type = type;
	pOutput->pos.x = rect.right;
	outputs.push_back(*pOutput);

	//�޸���������˿ڵ�������λ�ü���Ӧ����
	for ( unsigned int i = 0; i < this->outputs.size(); i++ )
	{
		//modify y position of other ports
		this->outputs[i].pos.y = rect.top + (rect.bottom - rect.top) / this->outputs.size() * ((double)i + 0.5);

		//modify related connectlines
		if ( this->parent )	//���������Ѿ����뵽ģ����
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
	
	//����ȷ���Ƿ����ɾ��
	if ( this->inputs.size() <= this->m_inputs_min )
	{
		CString msg;
		msg.Format("ɾ��ʧ�ܡ���ģ��������Ҫ%d������ӿ�", this->m_inputs_min);
		AfxMessageBox(msg);
		return;
	}

	//ɾ������˿�Ϊ��ɾ���˿ڵ�����
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

	//ɾ��������˿�
	this->inputs.erase(this->inputs.begin() + index);

	//�޸���������˿ڵ�������λ�ü���Ӧ����
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
	
	//����ȷ���Ƿ����ɾ��
	if ( this->outputs.size() <= this->m_outputs_min )
	{
		CString msg;
		msg.Format("ɾ��ʧ�ܡ���ģ��������Ҫ%d������ӿ�", this->m_outputs_min);
		AfxMessageBox(msg);
		return;
	}

	//ɾ������˿�Ϊ��ɾ���˿ڵ�����
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

	//ɾ��������˿�
	this->outputs.erase(this->outputs.begin() + index);

	//�޸���������˿ڵ�������λ�ü���Ӧ����
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

//�޸��������
void MiseComponent::AltInput(int index, CString name, int type, IOValue default_value)
{
	ASSERT(index >= 0 && index < this->inputs.size());

	//���Ȳ���name�Ƿ��ظ�������ظ�����ʾ��������
	if ( TestExist(name) )
	{
		AfxMessageBox("�Ѵ���ͬ������������������");
		return;
	}

	//�޸�ͬ������˿���ص�����
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

	//�޸ĸ�����˿ڵ���Ϣ
	this->inputs[index].name = name;
	this->inputs[index].type = type;
	this->inputs[index].default_value = default_value;
}

//�޸��������
void MiseComponent::AltOutput(int index, CString name, int type)
{
	ASSERT(index >= 0 && index < this->outputs.size());

	//���Ȳ���name�Ƿ��ظ�������ظ�����ʾ��������
	if ( TestExist(name) )
	{
		AfxMessageBox("�Ѵ���ͬ������������������");
		return;
	}

	//�޸�ͬ������˿���ص�����
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

	//�޸ĸ�����˿ڵ���Ϣ
	this->outputs[index].name = name;
	this->outputs[index].type = type;
}

//�ƶ������ĳλ��
void MiseComponent::MoveEx(CRect newRect)
{
	rect = newRect;

	//�޸�����˿ڼ���Ӧ����
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
				//�����յ�λ��
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

	//�޸�����˿ڵ�������λ�ü���Ӧ����
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
				//�����յ�λ��
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

//ƽ�������ĳλ��
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
		//�޸���Ӧ���ߵ�����
		CString strTmp;
		strTmp.Format("%s.%s", this->name, this->inputs[j].name);
		for (unsigned int i = 0; i < this->parent->connlines.size(); i++)
		{
			if ( this->parent->connlines[i].inport == strTmp )
			{
				this->parent->connlines[i].endPoint = this->inputs[j].pos;
				//�����յ�λ��
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
		//�޸���Ӧ���ߵ�����
		CString strTmp;
		strTmp.Format("%s.%s", this->name, this->outputs[j].name);
		for (UINT i = 0; i < this->parent->connlines.size(); i++)
		{
			if ( this->parent->connlines[i].outport == strTmp )
			{
				this->parent->connlines[i].startPoint.x = this->outputs[j].pos.x + IO_ARROW_SIZE;
				this->parent->connlines[i].startPoint.y = this->outputs[j].pos.y;
				//�����յ�λ��
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

	//����ȷ����������
	for(unsigned int i = 0; i < this->parent->components.size(); i++)
	{
		if ( this->parent->components[i]->name == newName )
		{
			AfxMessageBox("����ͬ�����������������");
			return;
		}
	}

	//�޸������������Ķ˿�����
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

	//�޸�ģ������
	this->name = newName;
}

//�����ʼ��
BOOL MiseComponent::Init()
{
	//�����Ӧ�Ķ�̬���ӿ�
	CString strDllPath;

	//�û�ָ����dll����ȫ·��
	if ( this->type == UDF_COMPONENT )
	{
		strDllPath = this->dllPath;
	}
	//ϵͳdll����ϵͳ·����ȥ��
	else
	{
		strDllPath.Format("%s\\components\\%s", g_pApp->m_appPath, this->dllPath);
	}

	handle = LoadLibrary(strDllPath);

	if(!handle)
	{
		CString strTmp;
		strTmp.Format("����ģ��%s��Ӧ�Ķ�̬���ӿ�%s��������·�������Ƿ���ȷ", name, dllPath);
		AfxMessageBox(strTmp);
		return FALSE;
	}
	//��ȡ�������
	(FARPROC &)(funEntry) = GetProcAddress(handle, this->functionName);
	if(!funEntry)
	{
		CString strTmp;
		strTmp.Format("����ģ��%s��Ӧ�Ķ�̬���ӿ�%s��%s����������ȷ��dll�ļ��Ƿ����Ҫ��", name, dllPath, functionName);
		AfxMessageBox(strTmp);
		FreeLibrary(handle);
		return FALSE;
	}

	return TRUE;
}

//��������
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

	//����dll����
	int ok = (funEntry)(current_time, step_time, inputs, strInputsList.GetBuffer(0), outputs, strOutputsList.GetBuffer(0));
	if ( ok != 0 )
	{
		CString strTmp;
		strTmp.Format("ģ��%s���õĺ���%sִ��ʧ�ܣ����ش���Ϊ%d�����ѯ��Ӧ�Ĵ�������ȷ��ԭ��", this->name, this->functionName, ok);
		AfxMessageBox(strTmp);
		return FALSE;
	}

	return TRUE;
}

//�ͷ���Դ
BOOL MiseComponent::Clear()
{
	FreeLibrary(handle);
	return TRUE;
}

//����֮��ĺ���
BOOL MiseComponent::PostRun()
{
	return true;
}

//��ʾģ����Ϣ��ȱʡΪģ�Ͷ���
BOOL MiseComponent::Display()
{
	CComDefDialog dlgComDef(this);
	if (dlgComDef.DoModal() == IDOK)
	{
		//�޸�ģ������
		this->AltName(dlgComDef.m_name);

		//�޸�ģ���Ӧ��dllλ��
		this->dllPath = dlgComDef.m_dllPath;

		//�޸�ģ��ĺ������
		this->functionName = dlgComDef.m_funcName;

		//�޸�ģ��ע��
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
	//����ȷ����������
	UINT i;
	for( i = 0; i < this->components.size(); i++)
	{
		if ( this->components[i]->name == pMiseComponent->name )
			break;
	}
	//�������������������������˳������ֱ�������
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

	//��������б���
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

//��յ�ǰģ������
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

	// �洢������� =============================================================================
	pe = pXMLDom->createElement("sim_parameters");
	if ( pe != NULL )
	{
		pe->setAttribute("start_time", g_pApp->m_start_time);
		pe->setAttribute("end_time", g_pApp->m_end_time);
		pe->setAttribute("step_time", g_pApp->m_step_time);
		pXMLDom->documentElement->appendChild(pe);
		pe.Release();
	}
	// ��������洢��� =========================================================================

	//=========================== �洢Component =================================================
	pe = pXMLDom->createElement("components");
	if (pe != NULL)
	{
		//Attention: IXMLDOMElementPtr������ָ�룬����Ҫrelease��������һ���������ѭ������
		for ( UINT i = 0; i < this->components.size(); i++)
		{
			//����component�ڵ�
			MSXML2::IXMLDOMElementPtr pe_component = pXMLDom->createElement("component");

			if ( pe_component != NULL )
			{
				//д��component��name����
				pe_component->setAttribute("name", components[i]->name.GetBuffer(0));

				//д��component��type����
				pe_component->setAttribute("type", components[i]->type);

				//д��component��dllpath����
				pe_component->setAttribute("dll_path", components[i]->dllPath.GetBuffer(0));

				//д��component��function_name����
				pe_component->setAttribute("function_name", components[i]->functionName.GetBuffer(0));

				//����component��comment�ӽڵ�
				MSXML2::IXMLDOMElementPtr pe_comment = pXMLDom->createElement("comment");
				if ( pe_comment != NULL )
				{
					pe_comment->text = components[i]->comment.GetBuffer(0);
					pe_component->appendChild(pe_comment);
				}

				//------------------- ����component��rect�ӽڵ� ----------------------
				MSXML2::IXMLDOMElementPtr pe_rect = pXMLDom->createElement("rect");
				if ( pe_rect != NULL )
				{
					pe_rect->setAttribute("left", components[i]->rect.left);
					pe_rect->setAttribute("right", components[i]->rect.right);
					pe_rect->setAttribute("top", components[i]->rect.top);
					pe_rect->setAttribute("bottom", components[i]->rect.bottom);
					pe_component->appendChild(pe_rect);
					
				}
				//------------------- end of component��rect�ӽڵ� ----------------------

				//------------------- ����component��input�ӽڵ� ------------------------
				MSXML2::IXMLDOMElementPtr pe_inputs = pXMLDom->createElement("inputs");
				if ( pe_inputs != NULL )
				{
					//����input�ӽڵ�
					for ( UINT j = 0; j < components[i]->inputs.size(); j++ )
					{
						MSXML2::IXMLDOMElementPtr pe_input = pXMLDom->createElement("input");
						if ( pe_input != NULL )
						{
							//�趨input����
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

							//����input��position�ӽڵ�
							MSXML2::IXMLDOMElementPtr pe_pos = pXMLDom->createElement("position");
							if ( pe_rect != NULL )
							{
								pe_pos->setAttribute("x", components[i]->inputs[j].pos.x);
								pe_pos->setAttribute("y", components[i]->inputs[j].pos.y);
								pe_input->appendChild(pe_pos);
							}

							//����input���뵽inputs��
							pe_inputs->appendChild(pe_input);
						}
					}
					pe_component->appendChild(pe_inputs);
				}
				//----------------------------- end of input ------------------------------

				//------------------------- ����component��output�ӽڵ� -------------------
				MSXML2::IXMLDOMElementPtr pe_outputs = pXMLDom->createElement("outputs");
				if ( pe_outputs != NULL )
				{
					//����input�ӽڵ�
					for ( UINT j = 0; j < components[i]->outputs.size(); j++ )
					{
						MSXML2::IXMLDOMElementPtr pe_output = pXMLDom->createElement("output");
						if ( pe_output != NULL )
						{
							//�趨input����
							pe_output->setAttribute("name", components[i]->outputs[j].name.GetBuffer(0));

							switch ( components[i]->outputs[j].type )
							{
							case(MISE_DOUBLE):
								pe_output->setAttribute("type", "double");
								break;
							default:
								break;
							}


							//����output��position�ӽڵ�
							MSXML2::IXMLDOMElementPtr pe_pos = pXMLDom->createElement("position");
							if ( pe_rect != NULL )
							{
								pe_pos->setAttribute("x", components[i]->outputs[j].pos.x);
								pe_pos->setAttribute("y", components[i]->outputs[j].pos.y);
								pe_output->appendChild(pe_pos);
							}

							//����input���뵽inputs��
							pe_outputs->appendChild(pe_output);
						}
					}
					pe_component->appendChild(pe_outputs);
				}
				//------------------------- end of output ----------------------------------
	
				//------------------------- ����component��special�ӽڵ� -------------------
				MSXML2::IXMLDOMElementPtr pe_special = pXMLDom->createElement("special");
				if ( pe_special != NULL )
				{
					//���ݲ�ͬ�����������д��ͬ�Ĳ���
					switch( components[i]->type )
					{

						case(SRC_CLOCK):	//sin Դ
						{
							break;
						}

						case(SRC_SIN):	//sin Դ
						{
							Mise_source_sin *pSin = (Mise_source_sin*)components[i];
							pe_special->setAttribute("A", pSin->m_A);
							pe_special->setAttribute("B", pSin->m_B);
							pe_special->setAttribute("C", pSin->m_C);
							break;
						}
						
						case(SRC_COS):	//cos Դ
						{
							Mise_source_cos *pCos = (Mise_source_cos*)components[i];
							pe_special->setAttribute("A", pCos->m_A);
							pe_special->setAttribute("B", pCos->m_B);
							pe_special->setAttribute("C", pCos->m_C);
							break;
						}
						case(SRC_CONST):	//const Դ
						{
							Mise_source_const *pConst = (Mise_source_const*)components[i];
							pe_special->setAttribute("value", pConst->m_value);
							break;
						}

						case(SRC_STEP):		//step Դ
						{
							Mise_source_step *pStep = (Mise_source_step*)components[i];
							pe_special->setAttribute("A", pStep->m_A);
							pe_special->setAttribute("B", pStep->m_B);
							pe_special->setAttribute("T", pStep->m_T);
							break;
						}

						case(MATH_ADD):	//�ӷ���
						{
							break;
						}

						case(MATH_MUL):	//�˷���
						{
							break;
						}

						case(MATH_RECIPROCAL):	//ȡ����
						{
							break;
						}

						case(MATH_POW):		//������
						{
							break;
						}

						case(CONT_DIFF):		//ȡ΢��
						{
							break;
						}

						case(CONT_INTEGRAL):	//ȡ����
						{
							break;
						}

						case(OUT_FILER):	//������ļ�
						{
							Mise_output_filer *pFiler = (Mise_output_filer*)components[i];
							pe_special->setAttribute("file_name", pFiler->m_fileName.GetBuffer(0));
							pe_special->setAttribute("with_title", pFiler->m_withTitle);
							pe_special->setAttribute("with_time", pFiler->m_withTime);
							break;
						}
						
						case(OUT_MONITOR):	//������Ϊ����
						{
							break;
						}

						case(CONT_TRANSFUN):		//һ�㴫�ݺ���
						{
							Mise_cont_transfunction *pTranFun = (Mise_cont_transfunction*)components[i];

							CString upParameters = "[";	//����ϵ��
							for ( unsigned int i = 0; i < pTranFun->m_upList.size(); i++)
							{
								CString strTmp;
								strTmp.Format(" %f", pTranFun->m_upList[i]);
								DelEndZeroFromStr(strTmp);
								upParameters += strTmp;
							}
							upParameters += "]";
							pe_special->setAttribute("UP", upParameters.GetBuffer(0));

							CString downParameters = "[";	//��ĸϵ��
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
						
						case(CONT_ZEROPOLE):		//�㣭�������
						{
							Mise_cont_ZeroPole *pZeroPole = (Mise_cont_ZeroPole*)components[i];

							// ����
							pe_special->setAttribute("K", pZeroPole->m_plus);

							CString upParameters = "[";	//����ϵ��
							for ( unsigned int i = 0; i < pZeroPole->m_upList.size(); i++)
							{
								CString strTmp;
								strTmp.Format(" %f", pZeroPole->m_upList[i]);
								DelEndZeroFromStr(strTmp);
								upParameters += strTmp;
							}
							upParameters += "]";
							pe_special->setAttribute("UP", upParameters.GetBuffer(0));

							CString downParameters = "[";	//��ĸϵ��
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

				//����component���뵽ģ����
				pe->appendChild(pe_component);
			}
		}
		pXMLDom->documentElement->appendChild(pe);
		pe.Release();
	}
	//========================== end of �洢component ============================
	
	//========================== �洢���� ========================================
	pe = pXMLDom->createElement("connLines");
	if (pe != NULL)
	{
		for(UINT i = 0; i < this->connlines.size(); i++)
		{

			//����connLine�ڵ�
			MSXML2::IXMLDOMElementPtr pe_connLine = pXMLDom->createElement("connLine");
			if ( pe_connLine != NULL )
			{
				//д��connLine���������
				pe_connLine->setAttribute("break1x", connlines[i].break1x);
				pe_connLine->setAttribute("break2y", connlines[i].break2y);
				pe_connLine->setAttribute("break3x", connlines[i].break3x);

				pe_connLine->setAttribute("inport", connlines[i].inport.GetBuffer(0));
				pe_connLine->setAttribute("outport", connlines[i].outport.GetBuffer(0));

				//д�����λ��
				MSXML2::IXMLDOMElementPtr pe_startPt = pXMLDom->createElement("start_point");
				if ( pe_startPt != NULL )
				{
					pe_startPt->setAttribute("x", connlines[i].startPoint.x);
					pe_startPt->setAttribute("y", connlines[i].startPoint.y);

					pe_connLine->appendChild(pe_startPt);
				}

				//д���յ�λ��
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
	//========================== end of �洢���� =================================

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
	//ȷ����ǰģ��Ϊ��
	ASSERT(this->IsEmpty());

	MSXML2::IXMLDOMDocumentPtr pXMLDom;
	HRESULT hr;

	hr = pXMLDom.CreateInstance(__uuidof(DOMDocument30));
	if (FAILED(hr)) 
	{
		AfxMessageBox("��ʼ��DOMDocument30��ʧ��\n");
		return FALSE;
	}

	pXMLDom->async = VARIANT_FALSE;
	if ( pXMLDom->load(fileName.GetBuffer(0)) != VARIANT_TRUE)
	{
		CString strTmp = "����ģ���ļ�ʧ��\n";
		strTmp += (LPCSTR)pXMLDom->parseError->Getreason();
		AfxMessageBox(strTmp);
		return FALSE;
	}

	// ���ط������ ==================================================================
	MSXML2::IXMLDOMNodePtr pNodeSimParameters = pXMLDom->selectSingleNode("//sim_parameters[0]");
	g_pApp->m_start_time = ((MSXML2::IXMLDOMElementPtr)pNodeSimParameters)->getAttribute("start_time");
	g_pApp->m_end_time = ((MSXML2::IXMLDOMElementPtr)pNodeSimParameters)->getAttribute("end_time");
	g_pApp->m_step_time = ((MSXML2::IXMLDOMElementPtr)pNodeSimParameters)->getAttribute("step_time");
	// �������������� ==============================================================


	UINT i;
	//=========================== ����component ======================================
	MiseComponent* pComponent;
	CString strQryComponent;
	MSXML2::IXMLDOMNodePtr pNodeComponent;

	i = 0;
	strQryComponent.Format("//components/component[%d]", i++);
	pNodeComponent = pXMLDom->selectSingleNode(strQryComponent.GetBuffer(0));
	while ( pNodeComponent != NULL )
	{
		//��ȡtype
		int comType = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("type");

		//���ݲ�ͬ��type���ɲ�ͬ��������Ժ�Ҫ�����������
		switch(comType)
		{
			case(SRC_CLOCK):	//sin Դ
			{
				//�������
				Mise_source_clock *pSourceClock = new Mise_source_clock();

				//���ϴ���ָ��
				pComponent = pSourceClock;
				break;
			}

			case(SRC_SIN):	//sin Դ
			{
				//�������
				Mise_source_sin *pSourceSin = new Mise_source_sin();

				//��ȡ��������
				CString strQrySpecial;
				strQrySpecial = strQryComponent + "/" + "special";
				MSXML2::IXMLDOMNodePtr pNodeSpecial = pXMLDom->selectSingleNode(strQrySpecial.GetBuffer(0));
				pSourceSin->m_A = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("A");
				pSourceSin->m_B = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("B");
				pSourceSin->m_C = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("C");
				

				//���ϴ���ָ��
				pComponent = pSourceSin;
				break;
			}
			
			case(SRC_COS):	//cos Դ
			{
				//�������
				Mise_source_cos *pSourceCos = new Mise_source_cos();

				//��ȡ��������
				CString strQrySpecial;
				strQrySpecial = strQryComponent + "/" + "special";
				MSXML2::IXMLDOMNodePtr pNodeSpecial = pXMLDom->selectSingleNode(strQrySpecial.GetBuffer(0));
				pSourceCos->m_A = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("A");
				pSourceCos->m_B = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("B");
				pSourceCos->m_C = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("C");

				//���ϴ���ָ��
				pComponent = pSourceCos;
				break;
			}
			case(SRC_CONST):	//const Դ
			{
				//�������
				Mise_source_const *pSourceConst = new Mise_source_const();

				//��ȡ��������
				CString strQrySpecial;
				strQrySpecial = strQryComponent + "/" + "special";
				MSXML2::IXMLDOMNodePtr pNodeSpecial = pXMLDom->selectSingleNode(strQrySpecial.GetBuffer(0));
				pSourceConst->m_value = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("value");

				//���ϴ���ָ��
				pComponent = pSourceConst;
				break;
			}

			case(SRC_STEP):		//step Դ
			{
				//�������
				Mise_source_step *pSourceStep = new Mise_source_step();

				//��ȡ��������
				CString strQrySpecial;
				strQrySpecial = strQryComponent + "/" + "special";
				MSXML2::IXMLDOMNodePtr pNodeSpecial = pXMLDom->selectSingleNode(strQrySpecial.GetBuffer(0));
				pSourceStep->m_A = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("A");
				pSourceStep->m_B = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("B");
				pSourceStep->m_T = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("T");

				//���ϴ���ָ��
				pComponent = pSourceStep;
				break;
			}

			case(MATH_ADD):	//�ӷ���
			{
				//�������
				Mise_math_add *pMathAdd = new Mise_math_add();
				//���ϴ���ָ��
				pComponent = pMathAdd;
				break;
			}

			case(MATH_MUL):	//�˷���
			{
				//�������
				Mise_math_multiply *pMathMul = new Mise_math_multiply();
				//���ϴ���ָ��
				pComponent = pMathMul;
				break;
			}

			case(MATH_RECIPROCAL):	//ȡ����
			{
				//�������
				Mise_math_reciprocal *pMathRec = new Mise_math_reciprocal();
				//���ϴ���ָ��
				pComponent = pMathRec;
				break;
			}

			case(MATH_POW):		//������
			{
				//�������
				Mise_math_pow *pMathPow = new Mise_math_pow();
				//���ϴ���ָ��
				pComponent = pMathPow;
				break;
			}

			case(CONT_DIFF):	//ȡ΢��
			{
				//�������
				Mise_cont_differential *pContDiff = new Mise_cont_differential();
				//���ϴ���ָ��
				pComponent = pContDiff;
				break;
			}

			case(CONT_INTEGRAL):	//ȡ����
			{
				//�������
				Mise_cont_integral *pContIntegral = new Mise_cont_integral();
				//���ϴ���ָ��
				pComponent = pContIntegral;
				break;
			}

			case(CONT_TRANSFUN):	//���ݺ���
			{
				//�������
				Mise_cont_transfunction *pTranFun = new Mise_cont_transfunction();
				//��ȡ��������
				CString strQrySpecial;
				strQrySpecial = strQryComponent + "/" + "special";
				MSXML2::IXMLDOMNodePtr pNodeSpecial = pXMLDom->selectSingleNode(strQrySpecial.GetBuffer(0));
				CString upParameters = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("UP");
				pTranFun->m_upList.clear();
				if(!ParseParameters(upParameters, pTranFun->m_upList) || pTranFun->m_upList.size() == 0)
				{
					AfxMessageBox("��ȡ���ݺ����ķ���ϵ��ʧ��");
					return false;
				}

				CString downParameters = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("DP");
				pTranFun->m_downList.clear();
				if(!ParseParameters(downParameters, pTranFun->m_downList) || pTranFun->m_downList.size() == 0)
				{
					AfxMessageBox("��ȡ���ݺ����ķ�ĸϵ��ʧ��");
					return false;
				}

				//���ϴ���ָ��
				pComponent = pTranFun;
				break;
			}

			case(CONT_ZEROPOLE):	//�㣭������ʽ�Ĵ��ݺ���
			{
				//�������
				Mise_cont_ZeroPole *pZeroPole = new Mise_cont_ZeroPole();
				//��ȡ��������
				CString strQrySpecial;
				strQrySpecial = strQryComponent + "/" + "special";
				MSXML2::IXMLDOMNodePtr pNodeSpecial = pXMLDom->selectSingleNode(strQrySpecial.GetBuffer(0));

				pZeroPole->m_plus = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("K");

				CString upParameters = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("UP");
				pZeroPole->m_upList.clear();
				if(!ParseParameters(upParameters, pZeroPole->m_upList))
				{
					AfxMessageBox("��ȡ���ݺ����ķ���ϵ��ʧ��");
					return false;
				}

				CString downParameters = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("DP");
				pZeroPole->m_downList.clear();
				if(!ParseParameters(downParameters, pZeroPole->m_downList))
				{
					AfxMessageBox("��ȡ���ݺ����ķ�ĸϵ��ʧ��");
					return false;
				}

				//���ϴ���ָ��
				pComponent = pZeroPole;
				break;
			}

			case(OUT_FILER):	//������ļ�
			{
				//�������
				Mise_output_filer *pOutputFiler = new Mise_output_filer();

				//��ȡ��������
				CString strQrySpecial;
				strQrySpecial = strQryComponent + "/" + "special";
				MSXML2::IXMLDOMNodePtr pNodeSpecial = pXMLDom->selectSingleNode(strQrySpecial.GetBuffer(0));
				pOutputFiler->m_fileName = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("file_name");
				pOutputFiler->m_withTime = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("with_time");
				pOutputFiler->m_withTitle = ((MSXML2::IXMLDOMElementPtr)pNodeSpecial)->getAttribute("with_title");

				//���ϴ���ָ��
				pComponent = pOutputFiler;
				break;
			}
			
			case(OUT_MONITOR):	//������Ϊ����
			{
				//�������
				Mise_output_monitor *pOutputMonitor = new Mise_output_monitor();
				//���ϴ���ָ��
				pComponent = pOutputMonitor;
				break;
			}

			default:
			{
				//�������
				MiseComponent *pUDFComponent = new MiseComponent();
				//���ϴ���ָ��
				pComponent = pUDFComponent;
				break;
			}
		}


		//��ȡname
		pComponent->name = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("name");
		//��ȡtype
		pComponent->type = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("type");
		//��ȡdllPath
		pComponent->dllPath = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("dll_path");
		//��ȡfunctionName
		pComponent->functionName = ((MSXML2::IXMLDOMElementPtr)pNodeComponent)->getAttribute("function_name");

		//��ȡcomment
		CString strQryComment;
		strQryComment = strQryComponent + "/" + "comment";
		MSXML2::IXMLDOMNodePtr pNodeComment = pXMLDom->selectSingleNode(strQryComment.GetBuffer(0));
		pComponent->comment = ((MSXML2::IXMLDOMElementPtr)pNodeComment)->text.GetBSTR();

		//��ȡrect
		CString strQryRect;
		strQryRect = strQryComponent + "/" + "rect";
		MSXML2::IXMLDOMNodePtr pNodeRect = pXMLDom->selectSingleNode(strQryRect.GetBuffer(0));
		int left = ((MSXML2::IXMLDOMElementPtr)pNodeRect)->getAttribute("left");
		int right = ((MSXML2::IXMLDOMElementPtr)pNodeRect)->getAttribute("right");
		int top = ((MSXML2::IXMLDOMElementPtr)pNodeRect)->getAttribute("top");
		int bottom = ((MSXML2::IXMLDOMElementPtr)pNodeRect)->getAttribute("bottom");
		pComponent->rect = CRect(left, top, right, bottom);


		//��ȡinputs
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

			//��ȡposition
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

		//��ȡoutputs
		UINT k = 0;
		CString strQryOutput;
		MSXML2::IXMLDOMNodePtr pNodeOutput;
		strQryOutput.Format("%s/outputs/output[%d]", strQryComponent, k++);
		pNodeOutput = pXMLDom->selectSingleNode(strQryOutput.GetBuffer(0));

		while ( pNodeOutput != NULL )
		{
			CString strOutputName = ((MSXML2::IXMLDOMElementPtr)pNodeOutput)->getAttribute("name");
			CString strOutputType = ((MSXML2::IXMLDOMElementPtr)pNodeOutput)->getAttribute("type");

			//��ȡposition
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

		//����������뵽ģ����
		this->AppendComponent(pComponent);

		//������ѯ��һ���ڵ�
		strQryComponent.Format("//components/component[%d]", i++);
		pNodeComponent = pXMLDom->selectSingleNode(strQryComponent.GetBuffer(0));

	}
	//=========================== end of ����component ======================================

	//=========================== �������� ==================================================
	CString strQryConnLine;
	MSXML2::IXMLDOMNodePtr pNodeConnLine;

	i = 0;
	strQryConnLine.Format("//connLines/connLine[%d]", i++);
	pNodeConnLine = pXMLDom->selectSingleNode(strQryConnLine.GetBuffer(0));
	while ( pNodeConnLine != NULL )
	{
		//�������߶���
		ConnectLine* pConnLine = new ConnectLine;

		//��ȡ�������
		pConnLine->inport = ((MSXML2::IXMLDOMElementPtr)pNodeConnLine)->getAttribute("inport");
		pConnLine->outport = ((MSXML2::IXMLDOMElementPtr)pNodeConnLine)->getAttribute("outport");
		pConnLine->break1x = ((MSXML2::IXMLDOMElementPtr)pNodeConnLine)->getAttribute("break1x");
		pConnLine->break2y = ((MSXML2::IXMLDOMElementPtr)pNodeConnLine)->getAttribute("break2y");
		pConnLine->break3x = ((MSXML2::IXMLDOMElementPtr)pNodeConnLine)->getAttribute("break3x");

		//��ȡ�����λ��
		CString strQryStartPt = strQryConnLine + "/" + "start_point";
		MSXML2::IXMLDOMNodePtr pNodeStartPt = pXMLDom->selectSingleNode(strQryStartPt.GetBuffer(0));
		if ( pNodeStartPt != NULL)
		{
			pConnLine->startPoint.x = ((MSXML2::IXMLDOMElementPtr)pNodeStartPt)->getAttribute("x");
			pConnLine->startPoint.y = ((MSXML2::IXMLDOMElementPtr)pNodeStartPt)->getAttribute("y");
		}

		//��ȡ�����λ��
		CString strQryEndPt = strQryConnLine + "/" + "end_point";
		MSXML2::IXMLDOMNodePtr pNodeEndPt = pXMLDom->selectSingleNode(strQryEndPt.GetBuffer(0));
		if ( pNodeEndPt != NULL )
		{
			pConnLine->endPoint.x = ((MSXML2::IXMLDOMElementPtr)pNodeEndPt)->getAttribute("x");
			pConnLine->endPoint.y = ((MSXML2::IXMLDOMElementPtr)pNodeEndPt)->getAttribute("y");
		}

		//���߼���ģ����
		this->AddConnLine(pConnLine);

		//��һ����ѯ
		strQryConnLine.Format("//connLines/connLine[%d]", i++);
		pNodeConnLine = pXMLDom->selectSingleNode(strQryConnLine.GetBuffer(0));
	}

	//=========================== end of �������� ===========================================



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

//��⵱�����point���ʱ���Ƿ�ѡ�и�����
BOOL ConnectLine::NeedBeSelected(CPoint point)
{
	CRect rect;
	//����һ������
	rect.left = this->startPoint.x;
	rect.right = this->break1x;
	rect.top = this->startPoint.y - SEL_SENSITIVITY;
	rect.bottom = this->startPoint.y + SEL_SENSITIVITY;
	if(rect.PtInRect(point)){
		selectedPart = 1;
		return TRUE;
	}

	if (this->break2y == -1 )	//�������յ�
	{
		//���ڶ�������
		rect.left = this->break1x - SEL_SENSITIVITY;
		rect.right = this->break1x + SEL_SENSITIVITY;
		rect.top = this->startPoint.y < this->endPoint.y ? this->startPoint.y : this->endPoint.y;
		rect.bottom = this->startPoint.y < this->endPoint.y ? this->endPoint.y : this->startPoint.y;
		if(rect.PtInRect(point)){
			selectedPart = 2;
			return TRUE;
		}
		//������������
		rect.left = this->break1x;
		rect.right = this->endPoint.x;
		rect.top = this->endPoint.y - SEL_SENSITIVITY;;
		rect.bottom = this->endPoint.y + SEL_SENSITIVITY;;
		if(rect.PtInRect(point)){
			selectedPart = 3;
			return TRUE;
		}
	}
	else	//���ĸ��յ�
	{
		//���ڶ�������
		rect.left = this->break1x - SEL_SENSITIVITY;
		rect.right = this->break1x + SEL_SENSITIVITY;
		rect.top = this->startPoint.y < this->break2y ? this->startPoint.y : this->break2y;
		rect.bottom = this->startPoint.y < this->break2y ? this->break2y : this->startPoint.y;
		if(rect.PtInRect(point)){
			selectedPart = 2;
			return TRUE;
		}

		//������������
		rect.left = this->break3x;
		rect.right = this->break1x;
		rect.top = this->break2y - SEL_SENSITIVITY;;
		rect.bottom = this->break2y + SEL_SENSITIVITY;;
		if(rect.PtInRect(point)){
			selectedPart = 3;
			return TRUE;
		}

		//�����Ķ�����
		rect.left = this->break3x - SEL_SENSITIVITY;
		rect.right = this->break3x + SEL_SENSITIVITY;
		rect.top = this->break2y < this->endPoint.y ? this->break2y : this->endPoint.y;
		rect.bottom = this->break2y < this->endPoint.y ? this->endPoint.y : this->break2y;
		if(rect.PtInRect(point)){
			selectedPart = 4;
			return TRUE;
		}

		//�����������
		rect.left = this->break3x;
		rect.right = this->endPoint.x;
		rect.top = this->endPoint.y - SEL_SENSITIVITY;;
		rect.bottom = this->endPoint.y + SEL_SENSITIVITY;;
		if(rect.PtInRect(point)){
			selectedPart = 5;
			return TRUE;
		}

	}

	//�����ڷ�Χ�ڣ�����FALSE
	return FALSE;

}

//=========================== end of class ConnectLine ======================================
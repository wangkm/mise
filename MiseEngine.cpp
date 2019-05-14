#include "StdAfx.h"
#include "MiseEngine.h"
#include "MyDataDef.h"
#include "mise.h"
#include "MiseLibs.h"
#include "MainFrm.h"

MiseEngine::MiseEngine(void)
{
}

MiseEngine::~MiseEngine(void)
{
}

BOOL MiseEngine::Init(double start_time, double end_time, double step_time)
{
	//=========================== model checking ====================================================
	//���ģ������
	if ( g_pApp->m_miseModel.components.size() > COM_MAX_PER_MODEL )
	{
		CString msg;
		msg.Format("��Ǹ����ǰ�汾��һ��ģ�������ֻ֧��%d�����", COM_MAX_PER_MODEL);
		AfxMessageBox(msg);
		return FALSE;
	}
	//���ģ����������ӿ���Ŀ
	for ( UINT count = 0; count < g_pApp->m_miseModel.components.size(); count++)
	{
		if ( g_pApp->m_miseModel.components[count]->inputs.size() > INPUT_MAX_SIZE )
		{
			CString msg;
			msg.Format("��Ǹ�����%s������ӿ�Ϊ%d����������ǰ�汾���������ֵ%d��", g_pApp->m_miseModel.components[count]->name, g_pApp->m_miseModel.components[count]->inputs.size(), INPUT_MAX_SIZE);
			AfxMessageBox(msg);
			return FALSE;
		}
		if ( g_pApp->m_miseModel.components[count]->outputs.size() > INPUT_MAX_SIZE )
		{
			CString msg;
			msg.Format("��Ǹ�����%s������ӿ�Ϊ%d����������ǰ�汾���������ֵ%d��", g_pApp->m_miseModel.components[count]->name, g_pApp->m_miseModel.components[count]->outputs.size(), OUTPUT_MAX_SIZE);
			AfxMessageBox(msg);
			return FALSE;
		}
	}

	//������ʱ����趨
	if ( ( start_time < end_time && step_time < 0 ) || ( start_time > end_time && step_time > 0 ) )
	{
		AfxMessageBox("����ʱ���趨����");
		return FALSE;
	}
	//==================================== end of model checking =====================================

	//ģ���ʼ��
	unsigned int i;
	for(i = 0; i < g_pApp->m_miseModel.components.size(); i++)
	{
		//�趨����˿ڵ�source�����ڿ��ټ���
		for (unsigned int j = 0; j < g_pApp->m_miseModel.components[i]->inputs.size(); j++)
		{
			//������յ�ǰsource
			g_pApp->m_miseModel.components[i]->inputs[j].source = "";

			CString strTmp;
			strTmp.Format("%s.%s", g_pApp->m_miseModel.components[i]->name, g_pApp->m_miseModel.components[i]->inputs[j].name);
			for (unsigned int k = 0; k < g_pApp->m_miseModel.connlines.size(); k++)
			{
				if ( g_pApp->m_miseModel.connlines[k].inport == strTmp )
				{
					g_pApp->m_miseModel.components[i]->inputs[j].source = g_pApp->m_miseModel.connlines[k].outport;
					break;
				}
			}
		}

		//ÿ��ģ���ʼ��
		if ( !g_pApp->m_miseModel.components[i]->Init() )
		{
			CString strTmp;
			strTmp.Format("%s��ʼ��ʧ��", g_pApp->m_miseModel.components[i]->name);
			AfxMessageBox(strTmp);
			return FALSE;
		}

	}
	//��̬���ӿ���سɹ���׼��ʵʱ���ݿ⣬���÷���ʱ�������׼����ʼ����
	//������ݿ��е���������
	g_pApp->m_RTDB.Clear();

	//Ϊ��������˿���ʵʱ���ݿ��д����ڵ�
	for(i = 0; i < g_pApp->m_miseModel.components.size(); i++)
	{
		for ( unsigned int j = 0; j < g_pApp->m_miseModel.components[i]->outputs.size(); j++ )
		{
			CString strTmp;
			strTmp.Format("%s.%s", g_pApp->m_miseModel.components[i]->name, g_pApp->m_miseModel.components[i]->outputs[j].name);

			string strTmp2(strTmp.GetBuffer(0));
			g_pApp->m_RTDB.AddElement(strTmp2);
		}
	}

	//�趨����ʱ�����
	m_start_time	= start_time;
	m_end_time		= end_time;
	m_step_time		= step_time;

	return TRUE;
}

BOOL MiseEngine::Run()
{

	if ( g_pApp->m_miseModel.components.size() == 0 )
	{
		return TRUE;
	}

	MiseComponent *pGenericComponent = NULL;
	m_sim_time = m_start_time;

	while ( ( m_step_time > 0 && m_sim_time <= m_end_time ) || ( m_step_time < 0 && m_sim_time >= m_end_time ) )
	{
		//��״̬������ʾ����ʱ��
		CString strTmp;
		strTmp.Format("time=%f", m_sim_time);
		((CMainFrame*)(g_pApp->m_pMainWnd))->SetSimInfoOnStatusBar(strTmp);

		for (unsigned int i = 0; i < g_pApp->m_miseModel.components.size(); i++)
		{
			//1 ��ȡÿ������Ĳ���
			for (unsigned int j = 0; j < g_pApp->m_miseModel.components[i]->inputs.size(); j++)
			{
				CString strFullName;
				strFullName.Format("%s.%s", g_pApp->m_miseModel.components[i]->name, g_pApp->m_miseModel.components[i]->inputs[j].name);
				string stringFullName(strFullName.GetBuffer(0));

				//��ȡ��input��source
				CString strSource = g_pApp->m_miseModel.components[i]->inputs[j].source;
				string stringSource(strSource.GetBuffer(0));

				//�����source��ʵʱ���ݿ��е������ֵ��Ϊsource��ֵ
				if ( stringSource != "")
				{
					//���⴦�����add��ĳ������˿ڵ�һ�ַ�Ϊ"-"����souce�ĸ������ȥ
					if ( g_pApp->m_miseModel.components[i]->type == MATH_ADD && g_pApp->m_miseModel.components[i]->inputs[j].name.GetAt(0) == '-' )
					{
						m_inputs[j] = -(g_pApp->m_RTDB.GetData(stringSource));
					}
					else
					{
						m_inputs[j] = g_pApp->m_RTDB.GetData(stringSource);
					}
				}
				//������Ϊ�������ȱʡֵ
				else
				{
					m_inputs[j] = g_pApp->m_miseModel.components[i]->inputs[j].default_value.l;
				}
			}

			//2 ����һ��
			int temp = g_pApp->m_miseModel.components[i]->type;
			if ( !g_pApp->m_miseModel.components[i]->Run(m_sim_time, m_step_time, m_inputs, m_outputs))
			{
				return FALSE;
			}

			//3 ���д�����ݿ���
			for (unsigned int j = 0; j < g_pApp->m_miseModel.components[i]->outputs.size(); j++)
			{
				CString strTmp;
				strTmp.Format("%s.%s", g_pApp->m_miseModel.components[i]->name, g_pApp->m_miseModel.components[i]->outputs[j].name);
				string strTmp2(strTmp.GetBuffer(0));
				g_pApp->m_RTDB.SetData(strTmp2, m_outputs[j]);
			}
		}

		//����ʱ��ǰ��һ������
		m_sim_time = TimeAdvance_Default(m_sim_time, m_step_time, &(g_pApp->m_RTDB));

	}

	return TRUE;
}

BOOL MiseEngine::PostRun()
{
	for (unsigned int i = 0; i < g_pApp->m_miseModel.components.size(); i++)
	{
		if ( ! g_pApp->m_miseModel.components[i]->PostRun())
		{
			return false;
		}
	}
	return TRUE;
}

BOOL MiseEngine::Clear()
{
	for (unsigned int i = 0; i < g_pApp->m_miseModel.components.size(); i++)
	{
		g_pApp->m_miseModel.components[i]->Clear();
	}
	return TRUE;
}

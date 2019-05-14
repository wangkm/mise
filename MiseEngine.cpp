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
	//检查模块数量
	if ( g_pApp->m_miseModel.components.size() > COM_MAX_PER_MODEL )
	{
		CString msg;
		msg.Format("抱歉，当前版本在一个模型中最多只支持%d个组件", COM_MAX_PER_MODEL);
		AfxMessageBox(msg);
		return FALSE;
	}
	//检查模块输入输出接口数目
	for ( UINT count = 0; count < g_pApp->m_miseModel.components.size(); count++)
	{
		if ( g_pApp->m_miseModel.components[count]->inputs.size() > INPUT_MAX_SIZE )
		{
			CString msg;
			msg.Format("抱歉，组件%s的输入接口为%d个，超出当前版本的最大允许值%d个", g_pApp->m_miseModel.components[count]->name, g_pApp->m_miseModel.components[count]->inputs.size(), INPUT_MAX_SIZE);
			AfxMessageBox(msg);
			return FALSE;
		}
		if ( g_pApp->m_miseModel.components[count]->outputs.size() > INPUT_MAX_SIZE )
		{
			CString msg;
			msg.Format("抱歉，组件%s的输出接口为%d个，超出当前版本的最大允许值%d个", g_pApp->m_miseModel.components[count]->name, g_pApp->m_miseModel.components[count]->outputs.size(), OUTPUT_MAX_SIZE);
			AfxMessageBox(msg);
			return FALSE;
		}
	}

	//检查仿真时间的设定
	if ( ( start_time < end_time && step_time < 0 ) || ( start_time > end_time && step_time > 0 ) )
	{
		AfxMessageBox("仿真时间设定错误");
		return FALSE;
	}
	//==================================== end of model checking =====================================

	//模块初始化
	unsigned int i;
	for(i = 0; i < g_pApp->m_miseModel.components.size(); i++)
	{
		//设定输入端口的source，便于快速检索
		for (unsigned int j = 0; j < g_pApp->m_miseModel.components[i]->inputs.size(); j++)
		{
			//首先清空当前source
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

		//每个模块初始化
		if ( !g_pApp->m_miseModel.components[i]->Init() )
		{
			CString strTmp;
			strTmp.Format("%s初始化失败", g_pApp->m_miseModel.components[i]->name);
			AfxMessageBox(strTmp);
			return FALSE;
		}

	}
	//动态链接库加载成功，准备实时数据库，设置仿真时间参数，准备开始仿真
	//清空数据库中的现有数据
	g_pApp->m_RTDB.Clear();

	//为所有输出端口在实时数据库中创建节点
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

	//设定仿真时间参数
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
		//在状态栏上显示仿真时间
		CString strTmp;
		strTmp.Format("time=%f", m_sim_time);
		((CMainFrame*)(g_pApp->m_pMainWnd))->SetSimInfoOnStatusBar(strTmp);

		for (unsigned int i = 0; i < g_pApp->m_miseModel.components.size(); i++)
		{
			//1 获取每个输入的参数
			for (unsigned int j = 0; j < g_pApp->m_miseModel.components[i]->inputs.size(); j++)
			{
				CString strFullName;
				strFullName.Format("%s.%s", g_pApp->m_miseModel.components[i]->name, g_pApp->m_miseModel.components[i]->inputs[j].name);
				string stringFullName(strFullName.GetBuffer(0));

				//获取该input的source
				CString strSource = g_pApp->m_miseModel.components[i]->inputs[j].source;
				string stringSource(strSource.GetBuffer(0));

				//如果有source，实时数据库中的输入初值设为source的值
				if ( stringSource != "")
				{
					//特殊处理，如果add的某个输入端口第一字符为"-"，则将souce的负数存进去
					if ( g_pApp->m_miseModel.components[i]->type == MATH_ADD && g_pApp->m_miseModel.components[i]->inputs[j].name.GetAt(0) == '-' )
					{
						m_inputs[j] = -(g_pApp->m_RTDB.GetData(stringSource));
					}
					else
					{
						m_inputs[j] = g_pApp->m_RTDB.GetData(stringSource);
					}
				}
				//否则设为该输入的缺省值
				else
				{
					m_inputs[j] = g_pApp->m_miseModel.components[i]->inputs[j].default_value.l;
				}
			}

			//2 运行一次
			int temp = g_pApp->m_miseModel.components[i]->type;
			if ( !g_pApp->m_miseModel.components[i]->Run(m_sim_time, m_step_time, m_inputs, m_outputs))
			{
				return FALSE;
			}

			//3 结果写到数据库中
			for (unsigned int j = 0; j < g_pApp->m_miseModel.components[i]->outputs.size(); j++)
			{
				CString strTmp;
				strTmp.Format("%s.%s", g_pApp->m_miseModel.components[i]->name, g_pApp->m_miseModel.components[i]->outputs[j].name);
				string strTmp2(strTmp.GetBuffer(0));
				g_pApp->m_RTDB.SetData(strTmp2, m_outputs[j]);
			}
		}

		//仿真时间前进一个步长
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

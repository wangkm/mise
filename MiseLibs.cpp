#include "StdAfx.h"
#include "MiseLibs.h"
#include "MyDataDef.h"
#include "mise.h"
#include "math.h"
#include "FilerDefDialog.h"
#include "SinCosDefDialog.h"
#include "ConstDefDialog.h"
#include "StepDefDialog.h"
#include "TransFunDefDialog.h"
#include "ZeroPoleDefDialog.h"

//--------------- for class Mise_sin	--------------------------------------------------------------------
Mise_source_sin::Mise_source_sin()
{
	m_A = 1;
	m_B = 1;
	m_C = 0;
	GenerateTitle();

	// �趨ģ���С��ȱʡ����ʾ�˿�����
	m_width = 80;
	m_height = 40;
	m_displayPortName = false;

	//��������˿ڵ���Ŀ��Χ
	m_inputs_min = 0;
	m_inputs_max = 0;
	m_outputs_min = 1;
	m_outputs_max = 1;

}
//�����ʼ��
BOOL Mise_source_sin::Init()
{
	if ( this->outputs.size() != 1 )
	{
		return FALSE;
	}
	return TRUE;
}

//��������
BOOL Mise_source_sin::Run(double current_time, double step_time, double *inputs, double *outputs)
{
	outputs[0] = m_A * sin(m_B * current_time + m_C);
	return TRUE;
}

//�ͷ���Դ
BOOL Mise_source_sin::Clear()
{
	return TRUE;
}

BOOL Mise_source_sin::Display()
{
	CSinCosDefDialog dlgDef(this);
	if (dlgDef.DoModal() == IDOK)
	{
		this->m_A = dlgDef.m_A;

		this->m_B = dlgDef.m_B;

		this->m_C = dlgDef.m_C;

		GenerateTitle();
		g_pApp->m_pMdlPanel->InvalidateRect(&rect);
	}

	return true;
}
void Mise_source_sin::GenerateTitle()
{
	m_title = "";
	CString strTmp;
	strTmp.Format("%f", m_A);
	DelEndZeroFromStr(strTmp);
	if ( strTmp != "1" )
	{
		m_title += strTmp;
	}

	m_title += "sin(";

	strTmp.Empty();
	strTmp.Format("%f", m_B);
	DelEndZeroFromStr(strTmp);
	if ( strTmp != "1" )
	{
		m_title += strTmp;
	}
	m_title += "t";

	strTmp.Empty();
	strTmp.Format("%f", m_C);
	DelEndZeroFromStr(strTmp);
	if ( strTmp != "0")
	{
		m_title += "+";
		m_title += strTmp;
	}

	m_title += ")";

}
//--------------- end of class Mise_sin		--------------------------------------------------------------------

//--------------- for class Mise_source_cos --------------------------------------------------------------------
Mise_source_cos::Mise_source_cos()
{
	m_A = 1;
	m_B = 1;
	m_C = 0;
	GenerateTitle();

	// �趨ģ���С��ȱʡ����ʾ�˿�����
	m_width = 80;
	m_height = 40;
	m_displayPortName = false;

	//��������˿ڵ���Ŀ��Χ
	m_inputs_min = 0;
	m_inputs_max = 0;
	m_outputs_min = 1;
	m_outputs_max = 1;

}
//�����ʼ��
BOOL Mise_source_cos::Init()
{
	if ( this->outputs.size() != 1 )
	{
		return FALSE;
	}
	return TRUE;
}

//��������
BOOL Mise_source_cos::Run(double current_time, double step_time, double *inputs, double *outputs)
{
	outputs[0] = m_A * cos(m_B * current_time + m_C);
	return TRUE;
}
//�ͷ���Դ
BOOL Mise_source_cos::Clear()
{
	return TRUE;
}

BOOL Mise_source_cos::Display()
{
	CSinCosDefDialog dlgDef(this);
	if (dlgDef.DoModal() == IDOK)
	{
		this->m_A = dlgDef.m_A;

		this->m_B = dlgDef.m_B;

		this->m_C = dlgDef.m_C;

		GenerateTitle();
		g_pApp->m_pMdlPanel->InvalidateRect(&rect);
	}
	return true;
}
void Mise_source_cos::GenerateTitle()
{
	m_title = "";
	CString strTmp;
	strTmp.Format("%f", m_A);
	DelEndZeroFromStr(strTmp);
	if ( strTmp != "1" )
	{
		m_title += strTmp;
	}

	m_title += "cos(";

	strTmp.Empty();
	strTmp.Format("%f", m_B);
	DelEndZeroFromStr(strTmp);
	if ( strTmp != "1" )
	{
		m_title += strTmp;
	}
	m_title += "t";

	strTmp.Empty();
	strTmp.Format("%f", m_C);
	DelEndZeroFromStr(strTmp);
	if ( strTmp != "0")
	{
		m_title += "+";
		m_title += strTmp;
	}

	m_title += ")";

}
//--------------- end of class Mise_sin		--------------------------------------------------------------------

//--------------- for class Mise_source_const ------------------------------------------------------------------
Mise_source_const::Mise_source_const(void)
{
	m_value = 1.0;

	// �趨ģ���С��ȱʡ����ʾ�˿�����
	m_width = 40;
	m_height = 40;
	m_displayPortName = false;

	//��������˿ڵ���Ŀ��Χ
	m_inputs_min = 0;
	m_inputs_max = 0;
	m_outputs_min = 1;
	m_outputs_max = 1;

}
BOOL Mise_source_const::Init()
{
	return TRUE;
}

BOOL Mise_source_const::Run(double current_time, double step_time, double *inputs, double *outputs)
{
	ASSERT(this->outputs.size() == 1);
	outputs[0] = m_value;
	return TRUE;
}
//�ͷ���Դ
BOOL Mise_source_const::Clear()
{
	return TRUE;
}
BOOL Mise_source_const::Display()
{
	CConstDefDialog dlgDef(this);
	if (dlgDef.DoModal() == IDOK)
	{
		this->m_value = dlgDef.m_value;
		g_pApp->m_pMdlPanel->InvalidateRect(&rect);
	}
	
	return true;
}
//--------------- end of class Mise_source_const ------------------------------------------------------------------

//--------------- for class Mise_math_add -------------------------------------------------------------------------
//�ӷ���
Mise_math_add::Mise_math_add()
{
	// �趨ģ���С��ȱʡ����ʾ�˿�����
	m_width = 40;
	m_height = 40;
	m_displayPortName = true;

	//��������˿ڵ���Ŀ��Χ
	m_inputs_min = 1;
	m_inputs_max = INPUT_MAX_SIZE;
	m_outputs_min =1;
	m_outputs_max = 1;
}

BOOL Mise_math_add::Init(void)
{
	m_count = this->inputs.size();
	return TRUE;
}

BOOL Mise_math_add::Run(double current_time, double step_time, double *inputs, double *outputs)
{
	if ( m_count <= 0 )
	{
		return FALSE;
	}

	outputs[0] = 0.0;
	for ( int i = 0; i < m_count; i++ )
	{
		outputs[0] += inputs[i];
	}
	return TRUE;
}
//�ͷ���Դ
BOOL Mise_math_add::Clear()
{
	return TRUE;
}

BOOL Mise_math_add::Display()
{
	return true;
}
//--------------- end of class Mise_math_add	--------------------------------------------------------------------

//--------------- for class Mise_source_clock	--------------------------------------------------------------------
//����ʱ��
Mise_source_clock::Mise_source_clock()
{
	// �趨ģ���С��ȱʡ����ʾ�˿�����
	m_width = 40;
	m_height = 40;
	m_displayPortName = false;
	// ��������˿ڵ���Ŀ��Χ
	m_inputs_min = 0;
	m_inputs_max = 0;
	m_outputs_min = 1;
	m_outputs_max = 1;

}
BOOL Mise_source_clock::Init()
{
	return TRUE;
}

BOOL Mise_source_clock::Run(double current_time, double step_time, double *inputs, double *outputs)
{
	ASSERT(this->outputs.size() == 1);
	outputs[0] = current_time;
	return TRUE;
}
//�ͷ���Դ
BOOL Mise_source_clock::Clear()
{
	return TRUE;
}

BOOL Mise_source_clock::Display()
{
	return true;
}

//--------------- end of class Mise_source_clock	--------------------------------------------------------------------


//--------------- for class Mise_math_multiply		--------------------------------------------------------------------
// �˷���
Mise_math_multiply::Mise_math_multiply()
{
	// �趨ģ���С��ȱʡ����ʾ�˿�����
	m_width = 40;
	m_height = 40;
	m_displayPortName = false;
	//��������˿ڵ���Ŀ��Χ
	m_inputs_min = 1;
	m_inputs_max = INPUT_MAX_SIZE;
	m_outputs_min = 1;
	m_outputs_max = 1;

}

BOOL Mise_math_multiply::Init()
{
	m_count = this->inputs.size();
	return TRUE;
}

BOOL Mise_math_multiply::Run(double current_time, double step_time, double *inputs, double *outputs)
{
	if ( m_count < 1 )
	{
		return FALSE;
	}
	outputs[0] = 1.0;
	for ( int i = 0; i < m_count; i++ )
	{
		outputs[0] *= inputs[i];
	}
	return TRUE;
}
//�ͷ���Դ
BOOL Mise_math_multiply::Clear()
{
	return TRUE;
}

BOOL Mise_math_multiply::Display()
{
	return true;
}

//--------------- end of class Mise_math_multiply	--------------------------------------------------------------------

//--------------- for class Mise_output_filer --------------------------------------------------------------------------
// ��������������ļ�
Mise_output_filer::Mise_output_filer(void)
{
	m_fileName = "";
	m_withTitle = true;
	m_isFirstLine = true;	//ȷ��ֻ��ӡһ��title
	m_withTime = true;

	// �趨ģ���С��ȱʡ����ʾ�˿�����
	m_width = 40;
	m_height = 40;
	m_displayPortName = false;
	//��������˿ڵ���Ŀ��Χ
	m_inputs_min = 1;
	m_inputs_max = INPUT_MAX_SIZE;
	m_outputs_min = 0;
	m_outputs_max = 0;

}

BOOL Mise_output_filer::Init()
{
	if ( m_fileName == "" )
	{
		AfxMessageBox("��ָ������ļ���");
		return false;
	}
	m_isFirstLine = true;
	return TRUE;
}

BOOL Mise_output_filer::Run(double current_time, double step_time, double *inputs, double *outputs)
{
	FILE *fp;

	fp = fopen(m_fileName, "a");
	if ( !fp )
	{
		AfxMessageBox("�����ļ�ʧ��");
		return FALSE;
	}

	//print title��ֻ������ʱ�Ÿ���ԭ�ļ�������ʱ����׷����ʽ��
	if ( m_isFirstLine )
	{
		CTime t = CTime::GetCurrentTime();
		fprintf(fp, "\n/*-----------------------------------*\n");
		fprintf(fp, " * %s\n", t.Format( "%Y, %m, %d, %H : %M :%S" ));
		fprintf(fp, " *-----------------------------------*/\n");

		if ( m_withTitle )
		{
			if ( m_withTime )
			{
				fprintf(fp, "%s\t", "sim_time");
			}
			for ( unsigned int i = 0; i < this->inputs.size(); i++)
			{
				fprintf(fp, "%s\t", this->inputs[i].source.GetBuffer(0));
			}
			fprintf(fp, "\n");
		}
		m_isFirstLine = false;
	}

	//print data
	if ( m_withTime )
	{
		fprintf(fp, "%f\t", current_time);
	}
	for( unsigned int i = 0; i < this->inputs.size(); i++ )
	{
		fprintf(fp, "%f\t", inputs[i]);
	}

	fprintf(fp, "\n");

	fclose(fp);

	return TRUE;
}

//�ͷ���Դ
BOOL Mise_output_filer::Clear()
{
	return TRUE;
}

BOOL Mise_output_filer::PostRun()
{
	ShellExecute(NULL, "open", m_fileName, NULL, NULL, SW_SHOWNORMAL);
	return true;
}

BOOL Mise_output_filer::Display()
{
	CFilerDefDialog dlgDef(this);
	if (dlgDef.DoModal() == IDOK)
	{
		this->m_fileName = dlgDef.m_fileFullPath;

		this->m_withTime = dlgDef.m_withTime;

		this->m_withTitle = dlgDef.m_withTitle;
	}

	return TRUE;
}
//--------------- end of class Mise_output_filer	--------------------------------------------------------------------

//--------------- for class Mise_math_reciprocal	--------------------------------------------------------------------
// ����
Mise_math_reciprocal::Mise_math_reciprocal()
{
	// �趨ģ���С��ȱʡ����ʾ�˿�����
	m_width = 40;
	m_height = 40;
	m_displayPortName = false;
	//��������˿ڵ���Ŀ��Χ
	m_inputs_min = 1;
	m_inputs_max = 1;
	m_outputs_min = 1;
	m_outputs_max = 1;
}

BOOL Mise_math_reciprocal::Init()
{
	ASSERT( this->inputs.size() == 1 && this->outputs.size() == 1 );
	this->inputs[0].default_value.l = 1.0;
	return TRUE;
}
BOOL Mise_math_reciprocal::Run(double current_time, double step_time, double *inputs, double *outputs)
{
	try
	{
		outputs[0] = 1.0 / inputs[0];
	}
	catch(...)
	{
		AfxMessageBox("ȡ��������ʧ�ܣ����������Ƿ�Ϊ�㡣");
		return false;
	}

	return TRUE;
}

BOOL Mise_math_reciprocal::Clear()
{
	return true;
}

BOOL Mise_math_reciprocal::Display()
{
	return true;
}

//--------------- end of class Mise_math_reciprocal		--------------------------------------------------------------------

//--------------- for class Mise_math_pow	--------------------------------------------------------------------
// ������
Mise_math_pow::Mise_math_pow()
{
	// �趨ģ���С��ȱʡ����ʾ�˿�����
	m_width = 40;
	m_height = 40;
	m_displayPortName = true;
	//��������˿ڵ���Ŀ��Χ
	m_inputs_min = 2;
	m_inputs_max = 2;
	m_outputs_min = 1;
	m_outputs_max = 1;
}

BOOL Mise_math_pow::Init()
{
	ASSERT( this->inputs.size() == 2 && this->outputs.size() == 1 );
	this->inputs[0].default_value.l = 1.0;
	this->inputs[1].default_value.l = 1.0;
	return TRUE;
}
BOOL Mise_math_pow::Run(double current_time, double step_time, double *inputs, double *outputs)
{
	outputs[0] = pow ( inputs[0], inputs[1] );

	return TRUE;
}

BOOL Mise_math_pow::Clear()
{
	return true;
}

BOOL Mise_math_pow::Display()
{
	return true;
}

//--------------- end of class Mise_math_reciprocal		--------------------------------------------------------------------


//--------------- for class Mise_output_monitor			--------------------------------------------------------------------
Mise_output_monitor::Mise_output_monitor()
{
	m_pDlgSimMonitor = new CSimMonitorDialog;
	m_pDlgSimMonitor->Create(IDD_MONITOR_DLG);
	m_pDlgSimMonitor->m_pSimTime	= &m_simTime;
	m_pDlgSimMonitor->m_pVarValues	= &m_varValues;
	m_pDlgSimMonitor->m_pTop		= &m_top;
	m_pDlgSimMonitor->m_pBottom		= &m_bottom;
	m_pDlgSimMonitor->m_pScaleUp	= &m_scaleUp;
	m_pDlgSimMonitor->m_pScaleDown	= &m_scaleDown;

	
	// �趨ģ���С��ȱʡ����ʾ�˿�����
	m_width = 40;
	m_height = 40;
	m_displayPortName = false;

	//��������˿ڵ���Ŀ��Χ
	m_inputs_min = 1;
	m_inputs_max = INPUT_MAX_SIZE;
	m_outputs_min = 0;
	m_outputs_max = 0;

}

Mise_output_monitor::~Mise_output_monitor()
{
	m_pDlgSimMonitor->DestroyWindow();
	delete m_pDlgSimMonitor;
}

BOOL Mise_output_monitor::Init()
{
	Clear();

	m_smallest_value = 0.0;
	m_largest_value = 0.0;
	m_top			= 0.0;
	m_bottom		= 0.0;
	m_scaleDown		= 0.0;
	m_scaleUp		= 0.0;
	
	//������û�����ӵ�����ӿ�
	for( unsigned int i = 0; i < inputs.size(); i++)
	{
		if ( inputs[i].source != "" )
		{
			CString strVarName = inputs[i].name;
			m_varNames.push_back(strVarName);

			vector<double> varValue;
			m_varValues.push_back(varValue);
		}
	}

	return true;
}

BOOL Mise_output_monitor::Run(double current_time, double step_time, double *inputs, double *outputs)
{

	m_simTime.push_back(current_time);


	unsigned int j = 0;
	for(unsigned int i = 0; i < this->inputs.size(); i++)
	{
		if ( this->inputs[i].source != "" )
		{
			m_varValues[j].push_back(inputs[i]);
			
			//�����һ������
			if ( m_varValues[j].size() == 1 )
			{
				continue;
			}

			if ( inputs[i] > m_largest_value )
			{
				m_largest_value = inputs[i];
				if ( m_largest_value < 0 )
				{
					m_top = 0.0;
				}
				else
				{
					m_scaleUp = floor(log10(m_largest_value));
					for ( m_top = pow(10.0, m_scaleUp); m_top <= m_largest_value; m_top += pow(10.0, m_scaleUp))
						;
				}
			}
			if ( inputs[i] < m_smallest_value )
			{
				m_smallest_value = inputs[i];
				if ( m_smallest_value >= 0 )
				{
					m_bottom = 0.0;
				}
				else
				{
					m_scaleDown = floor(log10(-m_smallest_value));
					for ( m_bottom = pow(10.0, m_scaleDown); m_bottom <= -m_smallest_value; m_bottom += pow(10.0, m_scaleDown))
						;
					m_bottom = -m_bottom;
				}
			}
			j++;
		}
	}

	return true;
}
BOOL Mise_output_monitor::Clear()
{
	m_simTime.clear();
	m_varNames.clear();
	for(unsigned int i = 0; i < m_varValues.size(); i++)
	{
		m_varValues[i].clear();
	}
	m_varValues.clear();

	return true;
}
BOOL Mise_output_monitor::Display()
{
	m_pDlgSimMonitor->ShowWindow(SW_SHOW);

	return TRUE;
}
BOOL Mise_output_monitor::PostRun()
{
	m_pDlgSimMonitor->Invalidate();
	m_pDlgSimMonitor->ShowWindow(SW_SHOW);

	return true;
}
//--------------- end of class Mise_output_monitor		--------------------------------------------------------------------

// for class Mise_source_step ----------------------------------------------------------------------------------------------
// ��Ծ����
Mise_source_step::Mise_source_step()
{
	m_A = 0;
	m_B = 1;
	m_T = 0;

	// �趨ģ���С��ȱʡ����ʾ�˿�����
	m_width = 40;
	m_height = 40;
	m_displayPortName = false;
	//��������˿ڵ���Ŀ��Χ
	m_inputs_min = 0;
	m_inputs_max = 0;
	m_outputs_min = 1;
	m_outputs_max = 1;

}

BOOL Mise_source_step::Init()
{
	ASSERT ( this->inputs.size() == 0 && this->outputs.size() == 1);
	return true;
}

BOOL Mise_source_step::Run(double current_time, double step_time, double *inputs, double *outputs)
{
	if ( current_time < m_T )
	{
		outputs[0] = m_A;
	}
	else
	{
		outputs[0] = m_B;
	}
	return true;
}

BOOL Mise_source_step::Display()
{
	CStepDefDialog dlgDef(this);
	if (dlgDef.DoModal() == IDOK)
	{
		this->m_A = dlgDef.m_A;

		this->m_B = dlgDef.m_B;

		this->m_T = dlgDef.m_T;
	}

	return true;
}

BOOL Mise_source_step::Clear()
{
	return true;
}
// end of class Mise_source_step ------------------------------------------------------------------------------------------------

// for class Mise_cont_differential	---------------------------------------------------------------------------------------------
// ΢�ֺ���
Mise_cont_differential::Mise_cont_differential()
{
	// �趨ģ���С��ȱʡ����ʾ�˿�����
	m_width = 40;
	m_height = 40;
	m_displayPortName = false;
	//��������˿ڵ���Ŀ��Χ
	m_inputs_min = 1;
	m_inputs_max = 1;
	m_outputs_min = 1;
	m_outputs_max = 1;
}

BOOL Mise_cont_differential::Init()
{
	ASSERT( this->inputs.size() == 1 && this->outputs.size() == 1 );

	//�趨Ĭ������Ϊ0
	this->inputs[0].default_value.l = 0.0;

	//��һ������������
	m_isFirstStep = true;

	return TRUE;
}
BOOL Mise_cont_differential::Run(double current_time, double step_time, double *inputs, double *outputs)
{
	if ( m_isFirstStep )
	{
		outputs[0] = 0.0;
		m_isFirstStep = false;
	}
	else
	{
		outputs[0] = (inputs[0] - m_prevalue ) / (current_time - m_pretime);
	}
	m_prevalue = inputs[0];
	m_pretime = current_time;

	return TRUE;
}

BOOL Mise_cont_differential::Clear()
{
	return true;
}

BOOL Mise_cont_differential::Display()
{
	return true;
}
// end of class Mise_cont_differential ------------------------------------------------------------------------------------------------

// for class Mise_cont_integral	-------------------------------------------------------------------------------------------------------
// ���ֺ���
Mise_cont_integral::Mise_cont_integral()
{
	// �趨ģ���С��ȱʡ����ʾ�˿�����
	m_width = 40;
	m_height = 40;
	m_displayPortName = false;
	//��������˿ڵ���Ŀ��Χ
	m_inputs_min = 1;
	m_inputs_max = 1;
	m_outputs_min = 1;
	m_outputs_max = 1;
}

BOOL Mise_cont_integral::Init()
{
	ASSERT( this->inputs.size() == 1 && this->outputs.size() == 1 );

	//�趨Ĭ������Ϊ0
	this->inputs[0].default_value.l = 0.0;

	//��һ������������
	m_isFirstStep = true;

	m_sum = 0.0;

	return TRUE;
}
BOOL Mise_cont_integral::Run(double current_time, double step_time, double *inputs, double *outputs)
{
	if ( m_isFirstStep )
	{
		outputs[0] = 0.0;
		m_isFirstStep = false;
	}
	else
	{
		m_sum += (inputs[0] + m_prevalue ) / 2 * (current_time - m_pretime);
		outputs[0] = m_sum;
	}
	m_prevalue = inputs[0];
	m_pretime = current_time;

	return TRUE;
}

BOOL Mise_cont_integral::Clear()
{
	return true;
}

BOOL Mise_cont_integral::Display()
{
	return true;
}
// end of class Mise_cont_integral ------------------------------------------------------------------------------------------------

// for class Mise_cont_transfunction	-------------------------------------------------------------------------------------------------------
// һ�㴫�ݺ���
Mise_cont_transfunction::Mise_cont_transfunction()
{
	// ��ʼ��ϵ��
	m_upList.push_back(1.0);
	m_downList.push_back(1.0);
	m_downList.push_back(1.0);
	GenerateTitle();
	
	// �趨ģ���С��ȱʡ����ʾ�˿�����
	m_width = 70;
	m_height = 40;
	m_displayPortName = false;
	//��������˿ڵ���Ŀ��Χ
	m_inputs_min = 1;
	m_inputs_max = 1;
	m_outputs_min = 1;
	m_outputs_max = 1;
}

BOOL Mise_cont_transfunction::Init()
{
	N = 16;
	m_preInputs.clear();

	return TRUE;
}


double Mise_cont_transfunction::GetGs(double s)
{
	double numerator = 0.0;
	for ( unsigned int i = 0; i < m_upList.size(); i++ )
	{
		numerator += m_upList[i] * pow( s, int(m_upList.size() - i - 1) );
	}

	double denominator = 0.0;
	for ( unsigned int i = 0; i < m_downList.size(); i++ )
	{
		denominator += m_downList[i] * pow( s, int(m_downList.size() - i - 1) );
	}

	//��ֹ���
	if ( fabs(denominator) < 1E-100 )
		denominator = denominator > 0 ? 1E-100 : -1E-100 ;

	double result = numerator / denominator;

	return result;
}

//��Stehfest�㷨��������˹�任
double Mise_cont_transfunction::InLaplace(double time)
{
	double result = 0.0;

	for ( int i = 1; i <= N; i++ )
	{
		//time̫С�����
		if ( fabs(time) < 1E-100 )
			time = time > 0 ? 1E-100 : -1E-100 ;

		double Gs = GetGs(log( 2.0 ) / time * i );
		
		double Vi = 0;
		for ( int k = ( i + 1 ) / 2; k <= min ( i, N / 2 ); k++ )
		{
			int temp = 1;
			for ( int ii = 2 * k; ii >= ( k + 1 ); ii--)
				temp *= ii;

			Vi += pow( (double)k, N/2 ) / jiecheng( N/2 - k) / jiecheng( k - 1 ) * temp / jiecheng( i - k) / jiecheng(2 * k - i );
		}
		
		if ( ( N / 2 + i ) % 2 == 1 )
		{
			Vi = ( -1 ) * Vi;
		}

		result += Vi * Gs;

	}

	result = result * log(2.0) / time;

	return result;
}

BOOL Mise_cont_transfunction::Run(double current_time, double step_time, double *inputs, double *outputs)
{
	// �ѵ�ǰ����ֵ�洢�б�
	std::pair<double, double> simData(current_time, inputs[0]);
	m_preInputs.push_back( simData );

	if ( m_preInputs.size() == 1 )
	{
		outputs[0] = 0.0;
		return TRUE;
	}

	// �൱������
	double result = 0.0;
	double pre_simtime = m_preInputs[0].first;
	double pre_value = m_preInputs[0].second * InLaplace( current_time - m_preInputs[0].first);

	for ( unsigned int i = 1; i < m_preInputs.size(); i++ )
	{
		double curr_value = m_preInputs[i].second * InLaplace(current_time - m_preInputs[i].first);
		result += ( pre_value + curr_value ) / 2 * ( m_preInputs[i].first - pre_simtime );
		pre_simtime = m_preInputs[i].first;
		pre_value = curr_value;
	}

	outputs[0] = result;

	return TRUE;
}

BOOL Mise_cont_transfunction::Clear()
{
	return true;
}

BOOL Mise_cont_transfunction::Display()
{
	CTransFunDefDialog dlgDef(this);

	if (dlgDef.DoModal() == IDOK)
	{
		// ��ȡ���趨��ϵ��
		m_upList.clear();
		m_downList.clear();

		for ( unsigned int i = 0; i < dlgDef.m_upList.size(); i++)
		{
			m_upList.push_back(dlgDef.m_upList[i]);
		}
		for ( unsigned int i = 0; i < dlgDef.m_downList.size(); i++)
		{
			m_downList.push_back(dlgDef.m_downList[i]);
		}

		GenerateTitle();
		g_pApp->m_pMdlPanel->InvalidateRect(&rect);
	}
	return true;
}

void Mise_cont_transfunction::GenerateTitle()
{
	m_title_up.Empty();
	m_title_down.Empty();

	//���ɷ��Ӷ���ʽ
	for(unsigned int i = 0; i < m_upList.size(); i++)
	{
		CString strTmp;
		strTmp.Format("%f", fabs(m_upList[i]));
		DelEndZeroFromStr(strTmp);
		if ( strTmp != "0")
		{
			//��ӼӺ�
			if (m_title_up != "")
			{
				if ( m_upList[i] > 0 )
				{
					m_title_up += " + ";
				}
				else
				{
					m_title_up += " - ";
				}
			}

			if ( strTmp != "1" || m_upList.size() - i - 1 == 0)
			{
				m_title_up += strTmp;
			}

			//ָ����1������ʾָ��
			if ( m_upList.size() - i - 1 == 1 )
			{
				m_title_up += "s";
			}
			//ָ����0������ʾs
			else if ( m_upList.size() - i - 1 == 0 )
			{
				//do nothing
			}
			else
			{
				m_title_up += "s^";
				CString strTmp2;
				strTmp2.Format("%d",  m_upList.size() - i - 1);
				m_title_up += strTmp2;
			}
		}
	}

	//���ɷ�ĸ����ʽ
	for(unsigned int i = 0; i < m_downList.size(); i++)
	{
		CString strTmp;
		strTmp.Format("%f", fabs(m_downList[i]));
		DelEndZeroFromStr(strTmp);
		if ( strTmp != "0")
		{
			//��ӼӺ�
			if (m_title_down != "")
			{
				if ( m_downList[i] > 0 )
				{
					m_title_down += " + ";
				}
				else
				{
					m_title_down += " - ";
				}
			}

			if ( strTmp != "1" || m_downList.size() - i - 1 == 0)
			{
				m_title_down += strTmp;
			}

			//ָ����1������ʾָ��
			if ( m_downList.size() - i - 1 == 1 )
			{
				m_title_down += "s";
			}
			//ָ����0������ʾs
			else if ( m_downList.size() - i - 1 == 0 )
			{
				//do nothing
			}
			else
			{
				m_title_down += "s^";
				CString strTmp2;
				strTmp2.Format("%d",  m_downList.size() - i - 1);
				m_title_down += strTmp2;
			}
		}
	}
}
// end of class Mise_cont_transfunction ------------------------------------------------------------------------------------------------


// for class Mise_cont_ZeroPole	-------------------------------------------------------------------------------------------------------
// �㣭������ʽ�Ĵ��ݺ���
Mise_cont_ZeroPole::Mise_cont_ZeroPole()
{
	// ��ʼ��ϵ��
	m_downList.push_back(-1);
	m_plus = 1.0;
	GenerateTitle();

	// �趨ģ���С��ȱʡ����ʾ�˿�����
	m_width = 70;
	m_height = 40;
	m_displayPortName = false;
	//��������˿ڵ���Ŀ��Χ
	m_inputs_min = 1;
	m_inputs_max = 1;
	m_outputs_min = 1;
	m_outputs_max = 1;
}

BOOL Mise_cont_ZeroPole::Init()
{
	N = 16;
	m_preInputs.clear();

	return TRUE;
}

double Mise_cont_ZeroPole::GetGs(double s)
{
	double numerator = 1.0;
	for ( unsigned int i = 0; i < m_upList.size(); i++ )
	{
		numerator *= ( s - m_upList[i] );
	}

	double denominator = 1.0;
	for ( unsigned int i = 0; i < m_downList.size(); i++ )
	{
		denominator *= ( s - m_downList[i]);
	}

	//��ֹ���
	if ( fabs(denominator) < 1E-100 )
		denominator = denominator > 0 ? 1E-100 : -1E-100 ;

	double result = m_plus * numerator / denominator;

	return result;
}


//��Stehfest�㷨��������˹�任
double Mise_cont_ZeroPole::InLaplace(double time)
{
	double result = 0.0;

	for ( int i = 1; i <= N; i++ )
	{
		//time̫С�����
		if ( fabs(time) < 1E-100 )
			time = time > 0 ? 1E-100 : -1E-100 ;

		double Gs = GetGs(log( 2.0 ) / time * i );
		
		double Vi = 0;
		for ( int k = ( i + 1 ) / 2; k <= min ( i, N / 2 ); k++ )
		{
			int temp = 1;
			for ( int ii = 2 * k; ii >= ( k + 1 ); ii--)
				temp *= ii;

			Vi += pow( (double)k, N/2 ) / jiecheng( N/2 - k) / jiecheng( k - 1 ) * temp / jiecheng( i - k) / jiecheng(2 * k - i );
		}
		
		if ( ( N / 2 + i ) % 2 == 1 )
		{
			Vi = ( -1 ) * Vi;
		}

		result += Vi * Gs;

	}

	result = result * log(2.0) / time;

	return result;
}

BOOL Mise_cont_ZeroPole::Run(double current_time, double step_time, double *inputs, double *outputs)
{
	// �ѵ�ǰ����ֵ�洢�б�
	std::pair<double, double> simData(current_time, inputs[0]);
	m_preInputs.push_back( simData );

	if ( m_preInputs.size() == 1 )
	{
		outputs[0] = 0.0;
		return TRUE;
	}

	// �൱������
	double result = 0.0;
	double pre_simtime = m_preInputs[0].first;
	double pre_value = m_preInputs[0].second * InLaplace( current_time - m_preInputs[0].first);

	for ( unsigned int i = 1; i < m_preInputs.size(); i++ )
	{
		double curr_value = m_preInputs[i].second * InLaplace(current_time - m_preInputs[i].first);
		result += ( pre_value + curr_value ) / 2 * ( m_preInputs[i].first - pre_simtime );
		pre_simtime = m_preInputs[i].first;
		pre_value = curr_value;
	}

	outputs[0] = result;

	return TRUE;
}

BOOL Mise_cont_ZeroPole::Clear()
{
	return true;
}

BOOL Mise_cont_ZeroPole::Display()
{
	CZeroPoleDefDialog dlgDef(this);

	if (dlgDef.DoModal() == IDOK)
	{
		// ��ȡ���趨��ϵ��
		m_upList.clear();
		m_downList.clear();

		for ( unsigned int i = 0; i < dlgDef.m_upList.size(); i++)
		{
			m_upList.push_back(dlgDef.m_upList[i]);
		}
		for ( unsigned int i = 0; i < dlgDef.m_downList.size(); i++)
		{
			m_downList.push_back(dlgDef.m_downList[i]);
		}
		m_plus = dlgDef.m_plus;

		//���·��ӷ�ĸ�Ķ���ʽ��������ʾ
		GenerateTitle();
		g_pApp->m_pMdlPanel->InvalidateRect(&rect);
	}
	return true;
}
void Mise_cont_ZeroPole::GenerateTitle()
{
	m_title_up.Empty();
	m_title_down.Empty();

	//���ɷ��Ӷ���ʽ
	CString strTmp;

	//����
	strTmp.Format("%f", m_plus);
	DelEndZeroFromStr(strTmp);
	if ( strTmp != "1" || m_upList.size() == 0)
	{
		m_title_up = strTmp;
	}

	for(unsigned int i = 0; i < m_upList.size(); i++)
	{
	strTmp.Format("%f", m_upList[i]);
	DelEndZeroFromStr(strTmp);
		//�����0������ʾ���
		if ( strTmp == "0" )
		{
			m_title_up += "(s)";
		}
		//ָ����0������ʾs
		else
		{
			//������㣬ת��Ϊ����
			if ( strTmp.GetAt(0) == '-' )
			{
				strTmp = strTmp.Right(strTmp.GetLength() - 1);
				m_title_up += " (s + ";
				m_title_up += strTmp;
				m_title_up += " )";
			}
			else
			{
				m_title_up += " (s - ";
				m_title_up += strTmp;
				m_title_up += ")";
			}
		}
	}

	//���ɷ�ĸ����ʽ
	for(unsigned int i = 0; i < m_downList.size(); i++)
	{
		strTmp.Format("%f", m_downList[i]);
		DelEndZeroFromStr(strTmp);
		//�����0������ʾ���
		if ( strTmp == "0" )
		{
			m_title_down += "(s)";
		}
		//ָ����0������ʾs
		else
		{
			//������㣬ת��Ϊ����
			if ( strTmp.GetAt(0) == '-' )
			{
				strTmp = strTmp.Right(strTmp.GetLength() - 1);
				m_title_down += " (s + ";
				m_title_down += strTmp;
				m_title_down += ")";
			}
			else
			{
				m_title_down += " (s - ";
				m_title_down += strTmp;
				m_title_down += " )";
			}
		}
	}


}

// end of class Mise_cont_ZeroPole ------------------------------------------------------------------------------------------------







#pragma once
#include "model.h"
#include "SimMonitorDialog.h"
#include "MyDataDef.h"

/* --------------------------------
 * sin resource
 * output = A * sin ( B * time + C )
 ----------------------------------*/
class Mise_source_sin : 
	public MiseComponent
{
public:
	Mise_source_sin();

public:
	double m_A, m_B, m_C;
	CString m_title;

	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL Display();
	void GenerateTitle();
};
/* --------------------------------
 * cos resource
 * output = A * cos ( B * time + C )
 ----------------------------------*/
class Mise_source_cos : 
	public MiseComponent
{
public:
	Mise_source_cos();

public:
	double m_A, m_B, m_C;
	CString m_title;

	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL Display();
	void GenerateTitle();
};

/* --------------------------------
 * clock resource
 * output = ����ʱ��
 ----------------------------------*/
class Mise_source_clock :
	public MiseComponent
{
public:
	Mise_source_clock(void);
public:
	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL Display();
};

/* --------------------------------
 * const resource
 * output = const
 ----------------------------------*/
class Mise_source_const :
	public MiseComponent
{
public:
	Mise_source_const(void);

public:
	double m_value;
	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL Display();
};

/* --------------------------------
 * step resource����Ծ����
 * ��ĳ��ʱ���T��������A��ΪB
 ----------------------------------*/
class Mise_source_step :
	public MiseComponent
{
public:
	Mise_source_step(void);

public:
	double m_T, m_A, m_B;
	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL Display();
};

/* --------------------------------
 * add operation
 * ���㼸�������ĺͣ��������ĳ��������һ����ĸΪ"-"����Ϊ����
 ----------------------------------*/
class Mise_math_add : 
	public MiseComponent
{
public:
	Mise_math_add();

protected:
	double m_count;

public:
	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL Display();
};

/* --------------------------------
 * �˷�����
 * ���㼸�����ĳ˻�
 ----------------------------------*/
class Mise_math_multiply : 
	public MiseComponent
{
public:
	Mise_math_multiply();

protected:
	double m_count;

public:
	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL Display();
};

/* --------------------------------
 * ȡ�����Ĳ���
 * ���Ϊ����ĵ���
 ----------------------------------*/
class Mise_math_reciprocal : 
	public MiseComponent
{
public:
	Mise_math_reciprocal();
public:
	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL Display();
};

/* --------------------------------
 * ������
 * A �� B �η�; result = A ^ B
 ----------------------------------*/
class Mise_math_pow : 
	public MiseComponent
{
public:
	Mise_math_pow();

public:
	double A, B;

public:
	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL Display();
};

/* --------------------------------
 * filler, to store simulation data to file
 * no output
 ----------------------------------*/
class Mise_output_filer :
	public MiseComponent
{
public:
	Mise_output_filer(void);

public:
	CString m_fileName;
	bool	m_withTitle;
	bool	m_isFirstLine;	//ȷ��ֻ��ӡһ��title
	bool	m_withTime;
public:
	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL PostRun();
	BOOL Display();
};

/* --------------------------------
 * ���������������
 ----------------------------------*/
class Mise_output_monitor : 
	public MiseComponent
{
public:
	Mise_output_monitor();
	~Mise_output_monitor();

protected:
	CSimMonitorDialog		*m_pDlgSimMonitor;
	vector<double>			m_simTime;
	vector<CString>			m_varNames;
	vector< vector<double> > m_varValues;
	double					m_largest_value;
	double					m_smallest_value;
	double					m_top;				//������ʱ������
	double					m_bottom;			//������ʱ������
	double					m_scaleUp;			//��ֵ�Ķ�����λ
	double					m_scaleDown;			//��ֵ�Ķ�����λ

public:
	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL PostRun();
	BOOL Display();
};

/* --------------------------------
 * ΢�ֺ���
 * ���Ϊ�����΢��
 ----------------------------------*/
class Mise_cont_differential : 
	public MiseComponent
{
public:
	Mise_cont_differential();
private:
	double	m_prevalue;
	double	m_pretime;
	BOOL	m_isFirstStep;
public:
	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL Display();
};

/* --------------------------------
 * ���ֺ���
 * ���Ϊ����Ļ���
 ----------------------------------*/
class Mise_cont_integral : 
	public MiseComponent
{
public:
	Mise_cont_integral();
private:
	double	m_prevalue;
	double	m_pretime;
	double	m_sum;
	BOOL	m_isFirstStep;
public:
	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL Display();
};


/* --------------------------------
 * ���ݺ���
 ----------------------------------*/
class Mise_cont_transfunction : 
	public MiseComponent
{
public:
	Mise_cont_transfunction();

private:
	int N;		// ��ftehfest����
	vector< std::pair<double, double> > m_preInputs;		//�洢��ǰ������

public:
	vector<double> m_upList;	//����ϵ������
	vector<double> m_downList;	//��ĸϵ������
	CString m_title_up;			//���Ӷ���ʽ��ʾ
	CString m_title_down;		//��ĸ����ʽ

public:
	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL Display();
	double GetGs(double s);			// ��G(s)��ֵ
	double InLaplace(double time);	// ��timeʱ��G(s)�ķ�������˹�任g(t)��ֵ
	void GenerateTitle();
};

/* --------------------------------
 * �㼫����ʽ�Ĵ��ݺ���
 ----------------------------------*/
class Mise_cont_ZeroPole : 
	public MiseComponent
{
public:
	Mise_cont_ZeroPole();

private:
	int N;		// ��ftehfest����
	vector< std::pair<double, double> > m_preInputs;		//�洢��ǰ������
public:
	vector<double> m_upList;	//����ϵ������
	vector<double> m_downList;	//��ĸϵ������
	double m_plus;				//����
	CString m_title_up;			//���Ӷ���ʽ��ʾ
	CString m_title_down;		//��ĸ����ʽ

public:
	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL Display();
	double GetGs(double s);			// ��G(s)��ֵ
	double InLaplace(double time);	// ��timeʱ��G(s)�ķ�������˹�任g(t)��ֵ
	void GenerateTitle();
};

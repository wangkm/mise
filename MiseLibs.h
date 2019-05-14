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
 * output = 仿真时钟
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
 * step resource，阶跃函数
 * 在某个时间点T，输入由A变为B
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
 * 计算几个加数的和，其中如果某个加数第一个字母为"-"，则为减法
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
 * 乘法操作
 * 计算几个数的乘积
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
 * 取倒数的操作
 * 输出为输入的倒数
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
 * 幂运算
 * A 的 B 次方; result = A ^ B
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
	bool	m_isFirstLine;	//确保只打印一次title
	bool	m_withTime;
public:
	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL PostRun();
	BOOL Display();
};

/* --------------------------------
 * 仿真结果用曲线输出
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
	double					m_top;				//画曲线时的上限
	double					m_bottom;			//画曲线时的下限
	double					m_scaleUp;			//正值的度量单位
	double					m_scaleDown;			//正值的度量单位

public:
	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL PostRun();
	BOOL Display();
};

/* --------------------------------
 * 微分函数
 * 输出为输入的微分
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
 * 积分函数
 * 输出为输入的积分
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
 * 传递函数
 ----------------------------------*/
class Mise_cont_transfunction : 
	public MiseComponent
{
public:
	Mise_cont_transfunction();

private:
	int N;		// 用ftehfest计算
	vector< std::pair<double, double> > m_preInputs;		//存储以前的输入

public:
	vector<double> m_upList;	//分子系数序列
	vector<double> m_downList;	//分母系数序列
	CString m_title_up;			//分子多项式显示
	CString m_title_down;		//分母多项式

public:
	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL Display();
	double GetGs(double s);			// 求G(s)的值
	double InLaplace(double time);	// 求time时刻G(s)的反拉普拉斯变换g(t)的值
	void GenerateTitle();
};

/* --------------------------------
 * 零极点形式的传递函数
 ----------------------------------*/
class Mise_cont_ZeroPole : 
	public MiseComponent
{
public:
	Mise_cont_ZeroPole();

private:
	int N;		// 用ftehfest计算
	vector< std::pair<double, double> > m_preInputs;		//存储以前的输入
public:
	vector<double> m_upList;	//分子系数序列
	vector<double> m_downList;	//分母系数序列
	double m_plus;				//增益
	CString m_title_up;			//分子多项式显示
	CString m_title_down;		//分母多项式

public:
	BOOL Init();
	BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	BOOL Clear();
	BOOL Display();
	double GetGs(double s);			// 求G(s)的值
	double InLaplace(double time);	// 求time时刻G(s)的反拉普拉斯变换g(t)的值
	void GenerateTitle();
};

#pragma once

#include "MyDataDef.h"
class MiseEngine
{
public:

	//inputs为输入变量数组的指针，outputs为输出变量数组的指针
	//inputs和outputs的依次对应于自上而下的输入输出端口
	//用户需要自己确保指针不越界

	double m_inputs[INPUT_MAX_SIZE];
	double m_outputs[OUTPUT_MAX_SIZE];
public:
	MiseEngine(void);
	~MiseEngine(void);

	double m_sim_time;		//全局仿真时钟
	double m_start_time;	//仿真开始时间
	double m_end_time;		//仿真结束时间
	double m_step_time;		//仿真步长

	BOOL Init(double start_time = 0.0, double end_time = 5.0, double step_time = 0.05);
	BOOL Run();
	BOOL PostRun();
	BOOL Clear();
};

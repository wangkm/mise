#pragma once

#include "MyDataDef.h"
class MiseEngine
{
public:

	//inputsΪ������������ָ�룬outputsΪ������������ָ��
	//inputs��outputs�����ζ�Ӧ�����϶��µ���������˿�
	//�û���Ҫ�Լ�ȷ��ָ�벻Խ��

	double m_inputs[INPUT_MAX_SIZE];
	double m_outputs[OUTPUT_MAX_SIZE];
public:
	MiseEngine(void);
	~MiseEngine(void);

	double m_sim_time;		//ȫ�ַ���ʱ��
	double m_start_time;	//���濪ʼʱ��
	double m_end_time;		//�������ʱ��
	double m_step_time;		//���沽��

	BOOL Init(double start_time = 0.0, double end_time = 5.0, double step_time = 0.05);
	BOOL Run();
	BOOL PostRun();
	BOOL Clear();
};

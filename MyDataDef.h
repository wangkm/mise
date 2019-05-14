#include "stdafx.h"
#include <math.h>
#include <vector>
#include "MiseRTDB.h"

#pragma once

#define g_pApp		((CmiseApp*)AfxGetApp())

#define COMPONENT_CFG_FILE	"Components.xml"	//�洢�����Ϣ��xml�ļ�

#define COM_MAX_PER_MODEL	100		//ÿ��ģ����������ģ����Ŀ
#define OUTPUT_MAX_SIZE		100		//ÿ��ģ��������������ӿ���Ŀ
#define INPUT_MAX_SIZE		100		//ÿ��ģ��������������ӿ���Ŀ

#define COM_TYPE_MAX_SIZE	100		//����������֧�ֶ��ٸ����

//MISE֧�ֵ���������
#define MISE_DOUBLE			1001
#define MISE_UNKNOWN		1010

//��������
#define STEP_SIMULATION		1		//����������


#define COM_WIDTH	80		//ģ��Ŀ��
#define COM_HEIGHT	100		//ģ��ĸ߶�
#define COM_MINSIZE 30		//ģ����С�ĸ߶Ȼ���

#define LINE_CLR		RGB(0, 0, 255)	//ѡ�е����ߵ���ɫ
#define COMP_CLR		RGB(0, 0, 255)	//ѡ�е��������ɫ
#define SLCT_LINE_CLR	RGB(255, 0, 0)	//ѡ�е����ߵ���ɫ
#define SLCT_COMP_CLR	RGB(255, 0, 0)	//ѡ�е��������ɫ

#define COM_BKCLR	RGB(255, 255, 255)	//ģ��ı�����ɫ

#define DRAG_DELAY	50

//���������ͷ�ĳ���
#define IO_ARROW_SIZE	5

//��ʶ�����Զ��յ�ĳ���
#define BREAK_SPACE		40

// ��ʶ��ǰ����
#define NONE_ACTION 0	//û���κβ���
#define SLCT_COMP	1	//ѡ������������ߣ�
#define CONN_COMP	2	//�����������
#define SLCT_LINE	3	//ѡ������
#define RESZ_COMP_LEFT		4	//�ı������С
#define RESZ_COMP_RIGHT		5	//�ı������С
#define RESZ_COMP_TOP		6	//�ı������С
#define RESZ_COMP_BOTTOM	7	//�ı������С

#define SEL_SENSITIVITY		4	//ѡ������ʱ�������ȣ���λΪ����

//------------- ���������б� -----------------------------------
#define UDF_COMPONENT	0		//�û��Զ����ģ�飬�����޸�

#define SRC_CLOCK		100		//����ʱ��
#define SRC_SIN			101		//sinԴ
#define SRC_COS			102		//cosԴ
#define SRC_CONST		103		//����
#define SRC_STEP		104		//��Ծ����

#define MATH_ADD		201		//�ӷ���
#define MATH_MUL		202		//�˷���
#define MATH_RECIPROCAL	203		//ȡ����
#define MATH_POW		204		//������

#define CONT_DIFF		211		//ȡ΢��
#define CONT_INTEGRAL	212		//ȡ����
#define CONT_TRANSFUN	213		//���ݺ���
#define CONT_ZEROPOLE	214		//��-�������

#define OUT_FILER		301		//���������ļ�
#define OUT_MONITOR		302		//������Ϊͼ��


//���ⶨ��--------------------------------------------------------
#define MONITOR_MAX		5		//����������ڿ�����������������Ŀ



//�Զ��庯��
int jiecheng(int n);
bool ParseParameters(CString strList, std::vector<double>& parList);
void DelEndZeroFromStr(CString& numStr);
double TimeAdvance_Default(double current_time, double step_time, MiseRTDB *db);
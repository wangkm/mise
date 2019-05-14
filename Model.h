#pragma once
#include <vector>
using namespace std;

class MiseModel;

//�洢����ֵ������
union IOValue
{
	double	l;
	int		i;
};
//��������ӿ���
class IOPort
{
public:
	CString name;			//�˿�������һ������ڲ���Ψһ��
	int	type;				//�������ͣ�Ŀǰֻ֧��double
	CString	source;			//����ӿڵ�������Դ����ʵʱ���ݿ��е�keyֵ������������ʱ���ٲ�ѯ��������ӿ���Ч
	IOValue	default_value;	//ȱʡֵ��������˿���Ч
	IOValue	current_value;	//��ǰֵ
	CPoint	pos;
};

//Mise����ģ��
class MiseComponent
{
public:
	MiseModel* parent;		//ָ��������MiseModel����
	CString name;			//name of current component
	int		type;			//��ģ������ͣ�Ŀǰ���Է�ΪCONST_COMPONENT��UDF_COMPONENT����
	int		m_width; 
	int		m_height;
	CRect	rect;			//�������λ�ã���ʱ��֧�־���
	BOOL	isSelected;		//whether this component is selected
	vector<IOPort> inputs;	//����ӿڼ���
	vector<IOPort> outputs;	//����ӿڼ���
	bool	m_displayPortName;	//��ʶ�Ƿ���ʾ�ӿ�����

	CString	dllPath;		//��Ӧ��̬���ӿ��λ��
	CString functionName;	//��Ӧ����������ƣ�ȱʡ��ģ������ͬ

	CString comment;		//ģ���Ӧ��˵��

	unsigned int m_inputs_min, m_inputs_max, m_outputs_min, m_outputs_max;	//������¼��������˿ڵĸ�����Χ

protected:
	HINSTANCE handle;		//��Ӧ��dll���

	// �п�����Ҫ���û�֪������˿ڵ���Ŀ������
	// ����ֵ��0˵��������������������û����������趨�����������
	int (*funEntry)(double current_time, double step_time, double *inputs, char *inputs_list, double *outputs, char *outputs_list);		//���ܺ������

public:
	MiseComponent();
	~MiseComponent();
	CString GetName(){return name;}
	CRect GetRect(){return rect;}

	void Move(int x, int y);
	void MoveEx(CRect newRect);

	void AddInput(CString name, int type, IOValue default_value);
	void AddOutput(CString name, int type);
	void DelInput(int index);
	void DelOutput(int index);
	void AltInput(int index, CString name, int type, IOValue default_value);
	void AltOutput(int index, CString name, int type);

	void AltName(CString newName);		//�޸�ģ�������

	BOOL TestExist(CString IOName);		//�������Ϊname�Ķ˿��Ƿ����

	virtual BOOL Init();
	virtual BOOL Run(double current_time, double step_time, double *inputs, double *outputs);
	virtual BOOL PostRun();				//����֮��ĺ���
	virtual BOOL Clear();
	virtual BOOL Display();				//��ʾ
};

//Miseģ������������
class ConnectLine
{
public:
	MiseModel* parent;	//ָ��������MiseModel����
	CString outport;	//����Ķ˿ڣ���ʽΪ �����.�˿���
	CString inport;		//����Ķ˿ڣ���ʽΪ �����.�˿���

	CPoint startPoint;	//�������
	CPoint endPoint;	//�յ�����
	int break1x;		//��һ���յ��x����
	int break2y;		//�ڶ����յ��y���꣬���Ϊ-1�����޵������͵��ĸ��յ�
	int break3x;		//�������յ��x����

	BOOL m_isSelected;	//whether this line is selected
	UINT selectedPart;	//��Ǳ�ѡ�е����ߣ����������϶� 

	ConnectLine();
	~ConnectLine();
	BOOL NeedBeSelected(CPoint point);	//��⵱�����point���ʱ���Ƿ�ѡ�и�����

};

//Miseģ�ͣ���ģ����������
class MiseModel
{
public:

	vector<MiseComponent*>	components;
	vector<ConnectLine>		connlines;
public:
	MiseModel();
	~MiseModel();
	void AppendComponent(MiseComponent* pComponent);
	void DelComponent(unsigned int index);

	void AddConnLine(ConnectLine* pConnLine);


	void Clear();	//�������Ԫ��
	BOOL IsEmpty();	//��ǰģ���Ƿ�Ϊ��
	BOOL Load(CString fileName);	//���ļ�����ģ����Ϣ
	BOOL Save(CString fileName);	//��ģ����Ϣ�洢�������ļ�
};

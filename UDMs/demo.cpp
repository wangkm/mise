// mise��̬���ӿ�ӿ�ʵ��
// current_timeΪ���ȷ���ʱ��
// step_timeΪ���沽��
// inputs��outputs�ֱ�Ϊ�����������
// inputs_lists��outputs_list�ֱ�Ϊ��������˿ڵ����ƣ���:�ָ�����inputs_list = "input1:input2:input3"��
// ���ִ�гɹ�������0�����򷵻ط���Ĵ�����룬����������ж���

// ����dll�ļ򵥷���������vc�Դ��������л�����ִ�� cl /LD demo.cpp����������demo.dll

extern "C" __declspec( dllexport ) int mise_demo(double current_time, double step_time, double *inputs, char *inputs_list, double *outputs, char *outputs_list)
{
	outputs[0] = ( inputs[0] - 2 ) * ( inputs[0] - 2 ) - 3;
	return 0;
}

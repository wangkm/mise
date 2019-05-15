// mise动态链接库接口实例
// current_time为当先仿真时间
// step_time为仿真步长
// inputs和outputs分别为输入输出数组
// inputs_lists和outputs_list分别为输入输出端口的名称，用:分隔。如inputs_list = "input1:input2:input3"。
// 如果执行成功，返回0；否则返回非零的错误代码，意义可以自行定义

// 生成dll的简单方法：启动vc自带的命令行环境，执行 cl /LD demo.cpp，即可生成demo.dll

extern "C" __declspec( dllexport ) int mise_demo(double current_time, double step_time, double *inputs, char *inputs_list, double *outputs, char *outputs_list)
{
	outputs[0] = ( inputs[0] - 2 ) * ( inputs[0] - 2 ) - 3;
	return 0;
}

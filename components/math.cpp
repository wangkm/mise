#include <math.h>

extern "C" __declspec( dllexport ) int mise_math_sin(double current_time, double step_time, double *inputs, char *inputs_list, double *outputs, char *outputs_list)
{
	outputs[0] = sin(inputs[0]);
	return 0;
}

extern "C" __declspec( dllexport ) int mise_math_asin(double current_time, double step_time, double *inputs, char *inputs_list, double *outputs, char *outputs_list)
{
	if ( fabs(inputs[0]) > 1.0 )
	{
		return 1;
	}
	outputs[0] = asin(inputs[0]);
	return 0;
}
extern "C" __declspec( dllexport ) int mise_math_cos(double current_time, double step_time, double *inputs, char *inputs_list, double *outputs, char *outputs_list)
{
	outputs[0] = cos(inputs[0]);
	return 0;
}

extern "C" __declspec( dllexport ) int mise_math_acos(double current_time, double step_time, double *inputs, char *inputs_list, double *outputs, char *outputs_list)
{
	if ( fabs(inputs[0]) > 1.0 )
	{
		return 1;
	}
	outputs[0] = acos(inputs[0]);
	return 0;
}

extern "C" __declspec( dllexport ) int mise_math_tan(double current_time, double step_time, double *inputs, char *inputs_list, double *outputs, char *outputs_list)
{
	outputs[0] = tan(inputs[0]);
	return 0;
}

extern "C" __declspec( dllexport ) int mise_math_atan(double current_time, double step_time, double *inputs, char *inputs_list, double *outputs, char *outputs_list)
{
	outputs[0] = atan(inputs[0]);
	return 0;
}

extern "C" __declspec( dllexport ) int mise_math_log(double current_time, double step_time, double *inputs, char *inputs_list, double *outputs, char *outputs_list)
{
	if ( inputs[0] <= 0 )
	{
		return 1;
	}
	outputs[0] = log(inputs[0]);
	return 0;
}

extern "C" __declspec( dllexport ) int mise_math_log10(double current_time, double step_time, double *inputs, char *inputs_list, double *outputs, char *outputs_list)
{
	if ( inputs[0] <= 0 )
	{
		return 1;
	}
	outputs[0] = log10(inputs[0]);
	return 0;
}

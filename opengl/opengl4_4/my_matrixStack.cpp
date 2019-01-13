
#include "my_matrixStack.h"

extern std::stack<float *> first;
extern float tempMatrix [16];

void my_push_matrix(float inMatrix[] )
{
	float* tempMatrix2 = new float[16];

	for ( int i = 0; i < 16 ; ++i)
	{
			tempMatrix2[i] = inMatrix[i];
	}

	first.push(tempMatrix2);
}

void my_pop_matrix ( float * inMatrix )
{
	float * tempMatrix = first.top();
	for ( int i = 0; i < 16; ++i )
	{
		inMatrix[i] = tempMatrix[i];
	}
	delete tempMatrix;

	first.pop();
}
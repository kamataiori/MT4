#include "Result.h"

void Result::Initialize()
{
	axis = Normalize({ 1.0f,1.0f,1.0f });
	angle = 0.44f;
	rotateMatrix = MakeRotateAxisAngle(axis, angle);
}

void Result::Update()
{
	
}

void Result::Draw()
{
	MatrixScreenPrint(0, 0, rotateMatrix,"rotateMatrix");
}

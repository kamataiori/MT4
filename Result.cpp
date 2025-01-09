#include "Result.h"

void Result::Initialize()
{
	
}

void Result::Update()
{
	
}

void Result::Draw()
{
	MatrixScreenPrint(0, kRowHeight * 0, rotation, "rotation : ");
	MatrixScreenPrint(0, kRowHeight * 1, rotateMatrix, "rotateMatrix");
	Vector3ScreenPrint(0, kRowHeight * 6, rotateByQuaternion, "rotateByQuaternion : ");
	Vector3ScreenPrint(0, kRowHeight * 7, rotateByMatrix, "rotateByMatrix : ");
}

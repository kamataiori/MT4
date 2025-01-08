#include "Result.h"

void Result::Initialize()
{
	
}

void Result::Update()
{
	
}

void Result::Draw()
{
	MatrixScreenPrint(0, 0, identity, "identity");
	MatrixScreenPrint(0, kRowHeight, conj, "Conjugate");
	MatrixScreenPrint(0, kRowHeight * 2, inv, "Inverse");
	MatrixScreenPrint(0, kRowHeight * 3, normal, "Normalize");
	MatrixScreenPrint(0, kRowHeight * 4, mul1, "Multiply(q1, q2)");
	MatrixScreenPrint(0, kRowHeight * 5, mul2, "Multiply(q2, q1)");
	Novice::ScreenPrintf(0, kRowHeight * 6, "Norm                    %f", norm);
}

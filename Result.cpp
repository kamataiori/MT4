#include "Result.h"

void Result::Initialize()
{
	
}

void Result::Update()
{
	
}

void Result::Draw()
{
	MatrixScreenPrint(0, kRowHeight * 0, interpolate0, "interpolate0 : ");
	MatrixScreenPrint(0, kRowHeight * 1, interpolate1, "interpolate1 : ");
	MatrixScreenPrint(0, kRowHeight * 2, interpolate2, "interpolate2 : ");
	MatrixScreenPrint(0, kRowHeight * 3, interpolate3, "interpolate3 : ");
	MatrixScreenPrint(0, kRowHeight * 4, interpolate4, "interpolate4 : ");
	
}

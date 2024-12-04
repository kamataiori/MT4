#include "Result.h"

void Result::Initialize()
{
	from0 = Normalize(Vector3{ 1.0f,0.7f,0.5f });
	to0 = { -from0.x, -from0.y, -from0.z };
	from1 = Normalize(Vector3{ -0.6f,0.9f,0.2f });
	to1 = Normalize(Vector3{ 0.4f,0.7f,-0.5f });
	rotateMatrix0 = DirectionToDirection(Normalize(Vector3{ 1.0f,0.0f,0.0f }), Normalize(Vector3{ -1.0f,0.0f,0.0f }));
	rotateMatrix1 = DirectionToDirection(from0, to0);
	rotateMatrix2 = DirectionToDirection(from1, to1);
}

void Result::Update()
{
	
}

void Result::Draw()
{
	MatrixScreenPrint(0, 0, rotateMatrix0, "rotateMatrix0");
	MatrixScreenPrint(0, kRowHeight * 5, rotateMatrix1, "rotateMatrix1");
	MatrixScreenPrint(0, kRowHeight * 10, rotateMatrix2, "rotateMatrix2");
}

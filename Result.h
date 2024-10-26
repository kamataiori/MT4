#pragma once
#include "MathFunc.h"

class Result
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	Vector3 axis;
	float angle;
	Matrix4x4 rotateMatrix;

};


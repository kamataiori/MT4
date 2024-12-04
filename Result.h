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

	Vector3 from0{};
	Vector3 to0{};
	Vector3 from1{};
	Vector3 to1{};
	Matrix4x4 rotateMatrix0{};
	Matrix4x4 rotateMatrix1{};
	Matrix4x4 rotateMatrix2{};

};


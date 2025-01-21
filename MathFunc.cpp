#include "MathFunc.h"
#include <algorithm>

Vector3 Add(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;

	result.x = v1.x + v2.x;
	result.y = v1.y + v2.y;
	result.z = v1.z + v2.z;

	return result;
}

Vector3 Subtract(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;

	result.x = v1.x - v2.x;
	result.y = v1.y - v2.y;
	result.z = v1.z - v2.z;

	return result;
}

Vector3 Multiply(const float a, const Vector3& vector)
{
	Vector3 result;

	result.x = vector.x * a;
	result.y = vector.y * a;
	result.z = vector.z * a;

	return result;
}

float Length(Vector3 distance)
{
	return sqrtf(powf(distance.x, 2.0f) + powf(distance.y, 2.0f) + powf(distance.z, 2.0f));
}

Vector3 Normalize(Vector3 distance)
{
	return { distance.x / Length(distance),distance.y / Length(distance),distance.z / Length(distance) };
}

float Dot(Vector3 c, Vector3 d)
{
	return c.x * d.x + c.y * d.y + c.z * d.z;
}

Vector3 Cross(const Vector3& u, const Vector3& v)
{
	return Vector3(
		u.y * v.z - u.z * v.y,
		u.z * v.x - u.x * v.z,
		u.x * v.y - u.y * v.x
	);
}

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2)
{
	Matrix4x4 result;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = 0;
			for (int k = 0; k < 4; ++k) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}

Matrix4x4 MakeRotateXMatrix(float radian)
{
	Matrix4x4 result;

	result = {
		1.0f,0.0f,0.0f,0.0f,
		0.0f,cosf(radian),sinf(radian),0.0f,
		0.0f,-sinf(radian),cosf(radian),0.0f,
		0.0f,0.0f,0.0f,1.0f };
	return result;
}

Matrix4x4 MakeRotateYMatrix(float radian)
{
	Matrix4x4 result;

	result = {
		cosf(radian),0.0f,-sinf(radian),0.0f,
		0.0f,1.0f,0.0f,0.0f,
		sinf(radian),0.0f,cosf(radian),0.0f,
		0.0f,0.0f,0.0f,1.0f };
	return result;
}

Matrix4x4 MakeRotateZMatrix(float radian)
{
	Matrix4x4 result;

	result = {
		cosf(radian),sinf(radian),0.0f,0.0f,
		-sinf(radian),cosf(radian),0.0f,0.0f,
		0.0f,0.0f,1.0f,0.0f,
		0.0f,0.0f,0.0f,1.0f };
	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	Matrix4x4 result;
	result = {
		scale.x * rotateXYZMatrix.m[0][0],scale.x * rotateXYZMatrix.m[0][1],scale.x * rotateXYZMatrix.m[0][2],0.0f,
		scale.y * rotateXYZMatrix.m[1][0],scale.y * rotateXYZMatrix.m[1][1],scale.y * rotateXYZMatrix.m[1][2],0.0f,
		scale.z * rotateXYZMatrix.m[2][0],scale.z * rotateXYZMatrix.m[2][1],scale.z * rotateXYZMatrix.m[2][2],0.0f,
		translate.x,translate.y,translate.z,1.0f
	};
	return result;
}

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRatio, float nearClip, float farClip)
{
	Matrix4x4 result;

	result = {
		1 / aspectRatio * (1 / tanf(fovY / 2)),0.0f,0.0f,0.0f,
		0.0f,(1 / tanf(fovY / 2)),0.0f,0.0f,
		0.0f,0.0f,farClip / (farClip - nearClip),1.0f,
		0.0f,0.0f,-farClip * nearClip / (farClip - nearClip),0.0f
	};

	return result;
}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip)
{
	Matrix4x4 result;
	result = {
	2 / (right - left),0.0f,0.0f,0.0f,
	0.0f,2 / (top - bottom),0.0f,0.0f,
	0.0f,0.0f,1 / (farClip - nearClip),0.0f,
	(left + right) / (left - right),(top + bottom) / (bottom - top),nearClip / (nearClip - farClip),1.0f
	};
	return result;
}

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	Matrix4x4 result;

	result = {
	width / 2,0.0f,0.0f,0.0f,
	0.0f,-(height / 2),0.0f,0.0f,
	0.0f,0.0f,maxDepth - minDepth,0.0f,
	left + (width / 2),top + (height / 2),minDepth,1.0f
	};


	return result;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix)
{
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
	assert(w != 0.0f);

	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];

	result.x /= w;
	result.y /= w;
	result.z /= w;


	return result;
}

Matrix4x4 Inverse(const Matrix4x4& m)
{
	Matrix4x4 result;
	float A;
	A = m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]
		- m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]
		- m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]
		+ m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]
		+ m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]
		- m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]
		- m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]
		+ m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0];

	result.m[0][0] = 1 / A * (m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2]
		- m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]);
	result.m[0][1] = 1 / A * (-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2]
		+ m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]);
	result.m[0][2] = 1 / A * (m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2]
		- m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]);
	result.m[0][3] = 1 / A * (-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2]
		+ m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]);

	result.m[1][0] = 1 / A * (-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2]
		+ m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]);
	result.m[1][1] = 1 / A * (m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2]
		- m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]);
	result.m[1][2] = 1 / A * (-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2]
		+ m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]);
	result.m[1][3] = 1 / A * (m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2]
		- m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]);

	result.m[2][0] = 1 / A * (m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1]
		- m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]);
	result.m[2][1] = 1 / A * (-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1]
		+ m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]);
	result.m[2][2] = 1 / A * (m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1]
		- m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]);
	result.m[2][3] = 1 / A * (-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1]
		+ m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]);

	result.m[3][0] = 1 / A * (-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1]
		+ m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]);
	result.m[3][1] = 1 / A * (m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1]
		- m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]);
	result.m[3][2] = 1 / A * (-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1]
		+ m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]);
	result.m[3][3] = 1 / A * (m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1]
		- m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]);
	return result;
}

Matrix4x4 MakeLookAtMatrix4x4(const Vector3& eye, const Vector3& target, const Vector3& up)
{
	Vector3 zaxis = Normalize(Subtract(target, eye));    // Forward
	Vector3 xaxis = Normalize(Cross(up, zaxis));         // Right
	Vector3 yaxis = Cross(zaxis, xaxis);                 // Up

	Matrix4x4 viewMatrix = {
		xaxis.x, yaxis.x, zaxis.x, 0.0f,
		xaxis.y, yaxis.y, zaxis.y, 0.0f,
		xaxis.z, yaxis.z, zaxis.z, 0.0f,
		-Dot(xaxis, eye), -Dot(yaxis, eye), -Dot(zaxis, eye), 1.0f
	};

	return viewMatrix;
}

Vector3 SphericalToCartesian(float radius, float theta, float phi)
{
	Vector3 cartesian;
	cartesian.x = radius * sinf(theta) * cosf(phi);
	cartesian.y = radius * sinf(theta) * sinf(phi);
	cartesian.z = radius * cosf(theta);
	return cartesian;
}

Vector3 Project(const Vector3& v1, const Vector3& v2)
{
	Vector3 result;
	float a = ((v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z));
	float b = ((v2.x * v2.x) + (v2.y * v2.y) + (v2.z * v2.z));
	result.x = a / b * v2.x;
	result.y = a / b * v2.y;
	result.z = a / b * v2.z;

	return result;
}

Vector3 ClosestPoint(const Vector3& point, const Segment& segment)
{
	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);

	project = Add(project, segment.origin);

	return project;
}

Vector3 Perpendicular(const Vector3& vector)
{
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return { -vector.y,vector.x,0.0f };
	}
	return { 0.0f,-vector.z,vector.y };
}

void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewMatrix, uint32_t color)
{
	Vector3 center = Multiply(plane.distance, plane.normal);
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z };
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);
	perpendiculars[3] = { -perpendiculars[2].x,-perpendiculars[2].y,-perpendiculars[2].z };

	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewMatrix);
	}

	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[2].x), int(points[2].y), int(points[1].x), int(points[1].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[3].x), int(points[3].y), int(points[0].x), int(points[0].y), color);
}

void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjection, const Matrix4x4 viewportMatrix, uint32_t color)
{
	const uint32_t kSubdivision = 16;
	const float kLonEvery = float(M_PI * 2) / kSubdivision;
	const float kLatEvery = float(M_PI) / kSubdivision;
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex)
	{
		float lat = float(-M_PI / 2.0f) + (kLatEvery * latIndex);

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex)
		{
			float lon = float(lonIndex * kLonEvery);

			Vector3 a, b, c;

			a = { (cosf(lat) * cosf(lon) * sphere.radius + sphere.center.x),(sinf(lat) * sphere.radius + sphere.center.y) ,(cosf(lat) * sinf(lon) * sphere.radius + sphere.center.z) };
			b = { (cosf(lat + kLatEvery) * cosf(lon) * sphere.radius + sphere.center.x) ,(sinf(lat + kLatEvery) * sphere.radius + sphere.center.y) ,(cosf(lat + kLatEvery) * sinf(lon) * sphere.radius + sphere.center.z) };
			c = { (cosf(lat) * cosf(lon + kLonEvery) * sphere.radius + sphere.center.x) ,(sinf(lat) * sphere.radius + sphere.center.y) ,(cosf(lat) * sinf(lon + kLonEvery) * sphere.radius + sphere.center.z) };

			Vector3 VecA = Transform(Transform(a, viewProjection), viewportMatrix);
			Vector3 VecB = Transform(Transform(b, viewProjection), viewportMatrix);
			Vector3 VecC = Transform(Transform(c, viewProjection), viewportMatrix);

			Novice::DrawLine(int(VecA.x), int(VecA.y), int(VecB.x), int(VecB.y), color);
			Novice::DrawLine(int(VecA.x), int(VecA.y), int(VecC.x), int(VecC.y), color);

		}
	}
}

void EnsureAABB(AABB& aabb)
{
	aabb.min.x = (std::min)(aabb.min.x, aabb.max.x);
	aabb.max.x = (std::max)(aabb.min.x, aabb.max.x);
	aabb.min.y = (std::min)(aabb.min.y, aabb.max.y);
	aabb.max.y = (std::max)(aabb.min.y, aabb.max.y);
	aabb.min.z = (std::min)(aabb.min.z, aabb.max.z);
	aabb.max.z = (std::max)(aabb.min.z, aabb.max.z);
}

void DrawTriangle(const Triangle& triangle, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	Vector3 VecA = Transform(Transform(triangle.vertices[0], viewProjectionMatrix), viewportMatrix);
	Vector3 VecB = Transform(Transform(triangle.vertices[1], viewProjectionMatrix), viewportMatrix);
	Vector3 VecC = Transform(Transform(triangle.vertices[2], viewProjectionMatrix), viewportMatrix);

	uint32_t color = 0xFFFFFFFF;

	Novice::DrawLine(int(VecA.x), int(VecA.y), int(VecB.x), int(VecB.y), color);
	Novice::DrawLine(int(VecB.x), int(VecB.y), int(VecC.x), int(VecC.y), color);
	Novice::DrawLine(int(VecC.x), int(VecC.y), int(VecA.x), int(VecA.y), color);
}

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix)
{
	const float kGridHalfWidth = 2.0f;
	const uint32_t kSubdivision = 10;
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision);
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {


		Vector3 startPoint(kGridHalfWidth - (kGridEvery * xIndex), 0.0f, -kGridHalfWidth);
		Vector3 endPoint(kGridHalfWidth - (kGridEvery * xIndex), 0, kGridHalfWidth);

		Vector3 screenStartPoint = Transform(startPoint, viewProjectionMatrix);
		Vector3 screenEndPoint = Transform(endPoint, viewProjectionMatrix);

		Vector3 screenStartPoint1 = Transform(screenStartPoint, viewportMatrix);
		Vector3 screenEndPoint1 = Transform(screenEndPoint, viewportMatrix);

		Novice::DrawLine(int(screenStartPoint1.x), int(screenStartPoint1.y), int(screenEndPoint1.x), int(screenEndPoint1.y), 0xAAAAAAFF);


	}
	for (uint32_t zIndex = 0; zIndex <= kSubdivision; ++zIndex) {

		Vector3 startPoint(-kGridHalfWidth, 0.0f, kGridHalfWidth - (kGridEvery * zIndex));
		Vector3 endPoint(kGridHalfWidth, 0, kGridHalfWidth - (kGridEvery * zIndex));

		Vector3 screenStartPoint = Transform(startPoint, viewProjectionMatrix);
		Vector3 screenEndPoint = Transform(endPoint, viewProjectionMatrix);

		Vector3 screenStartPoint1 = Transform(screenStartPoint, viewportMatrix);
		Vector3 screenEndPoint1 = Transform(screenEndPoint, viewportMatrix);

		Novice::DrawLine(int(screenStartPoint1.x), int(screenStartPoint1.y), int(screenEndPoint1.x), int(screenEndPoint1.y), 0xAAAAAAFF);

	}
}

void DrawAABB(const AABB& aabb, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewMatrix, uint32_t color)
{
	Vector3 vertices[8] = {
	   {aabb.min.x, aabb.min.y, aabb.min.z},
	   {aabb.max.x, aabb.min.y, aabb.min.z},
	   {aabb.min.x, aabb.max.y, aabb.min.z},
	   {aabb.max.x, aabb.max.y, aabb.min.z},
	   {aabb.min.x, aabb.min.y, aabb.max.z},
	   {aabb.max.x, aabb.min.y, aabb.max.z},
	   {aabb.min.x, aabb.max.y, aabb.max.z},
	   {aabb.max.x, aabb.max.y, aabb.max.z}
	};
	for (int i = 0; i < 8; ++i) {
		vertices[i] = Transform(Transform(vertices[i], viewProjectionMatrix), viewMatrix);
	}
	Novice::DrawLine(int(vertices[0].x), int(vertices[0].y), int(vertices[1].x), int(vertices[1].y), color);
	Novice::DrawLine(int(vertices[0].x), int(vertices[0].y), int(vertices[2].x), int(vertices[2].y), color);
	Novice::DrawLine(int(vertices[0].x), int(vertices[0].y), int(vertices[4].x), int(vertices[4].y), color);
	Novice::DrawLine(int(vertices[1].x), int(vertices[1].y), int(vertices[3].x), int(vertices[3].y), color);
	Novice::DrawLine(int(vertices[1].x), int(vertices[1].y), int(vertices[5].x), int(vertices[5].y), color);
	Novice::DrawLine(int(vertices[2].x), int(vertices[2].y), int(vertices[3].x), int(vertices[3].y), color);
	Novice::DrawLine(int(vertices[2].x), int(vertices[2].y), int(vertices[6].x), int(vertices[6].y), color);
	Novice::DrawLine(int(vertices[3].x), int(vertices[3].y), int(vertices[7].x), int(vertices[7].y), color);
	Novice::DrawLine(int(vertices[4].x), int(vertices[4].y), int(vertices[5].x), int(vertices[5].y), color);
	Novice::DrawLine(int(vertices[4].x), int(vertices[4].y), int(vertices[6].x), int(vertices[6].y), color);
	Novice::DrawLine(int(vertices[5].x), int(vertices[5].y), int(vertices[7].x), int(vertices[7].y), color);
	Novice::DrawLine(int(vertices[6].x), int(vertices[6].y), int(vertices[7].x), int(vertices[7].y), color);
}

Vector3 ClosestPointOnAABB(const Vector3& point, const AABB& aabb)
{
	Vector3 closestPoint;
	closestPoint.x = std::clamp(point.x, aabb.min.x, aabb.max.x);
	closestPoint.y = std::clamp(point.y, aabb.min.y, aabb.max.y);
	closestPoint.z = std::clamp(point.z, aabb.min.z, aabb.max.z);
	return closestPoint;
}

Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
{
	Vector3 result;
	result.x = t * a.x + (1.0f - t) * b.x;
	result.y = t * a.y + (1.0f - t) * b.y;
	result.z = t * a.z + (1.0f - t) * b.z;
	return result;
}

void RotateInCircle(const Sphere& sphere, Vector3& position, float& angle)
{
	float angularVelocity = 3.14f;
	float deltaTime = 1.0f / 60.0f;
	angle += angularVelocity * deltaTime;

	position.x = sphere.center.x + cos(angle) * sphere.radius;
	position.y = sphere.center.y + sin(angle) * sphere.radius;
	position.z = sphere.center.z;

	Vector3 velocity;
	velocity.x = -sphere.radius * angularVelocity * sin(angle);
	velocity.y = sphere.radius * angularVelocity * cos(angle);
	velocity.z = 0.0f;

	Vector3 acceleration{};
	acceleration.x = -sphere.radius * angularVelocity * angularVelocity * cos(angle);
	acceleration.y = -sphere.radius * angularVelocity * angularVelocity * sin(angle);
	acceleration.z = 0.0f;

	/*velocity.x += acceleration.x;
	velocity.y += acceleration.y;
	velocity.z += acceleration.z;*/

	/*position.x += velocity.x;
	position.y += velocity.y;
	position.z += velocity.z;*/

	position.x += acceleration.x;
	position.y += acceleration.y;
	position.z += acceleration.z;



	////position = { sphere.center.x + sphere.radius * cos(theta), sphere.center.y + sphere.radius * sin(theta) };
	//float angularVelocity = 3.14f;
	//float deltaTime = 1.0f / 60.0f;
	//angle += angularVelocity * deltaTime;

	//position.x = sphere.center.x + cos(angle) * sphere.radius;
	//position.y = sphere.center.y + sin(angle) * sphere.radius;
	//position.z = sphere.center.z;

	//Vector3 velocity;
	//velocity.x = -sphere.radius * angularVelocity * sin(angle);
	//velocity.y = sphere.radius * angularVelocity * cos(angle);
	//velocity.z = 0.0f;

	//Vector3 acceleration{};
	//acceleration.x = -sphere.radius * angularVelocity * angularVelocity * cos(angle);
	//acceleration.y = -sphere.radius * angularVelocity * angularVelocity * sin(angle);
	//acceleration.z = 0.0f;

	//velocity.x += acceleration.x;
	//velocity.y += acceleration.y;
	//velocity.z += acceleration.z;

	//position.x += velocity.x;
	//position.y += velocity.y;
	//position.z += velocity.z;
}

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle)
{
	Matrix4x4 rotationMatrix;
	float cosA = cos(angle);
	float sinA = sin(angle);
	float oneMinusCosA = 1.0f - cosA;

	// 正規化
	Vector3 n = Normalize(axis);
	float x = n.x;
	float y = n.y;
	float z = n.z;

	rotationMatrix.m[0][0] = x * x * oneMinusCosA + cosA;
	rotationMatrix.m[0][1] = y * x * oneMinusCosA + z * sinA;
	rotationMatrix.m[0][2] = x * z * oneMinusCosA - y * sinA;
	rotationMatrix.m[0][3] = 0.0f;

	rotationMatrix.m[1][0] = x * y * oneMinusCosA - z * sinA;
	rotationMatrix.m[1][1] = y * y * oneMinusCosA + cosA;
	rotationMatrix.m[1][2] = y * z * oneMinusCosA + x * sinA;
	rotationMatrix.m[1][3] = 0.0f;

	rotationMatrix.m[2][0] = z * x * oneMinusCosA + y * sinA;
	rotationMatrix.m[2][1] = z * y * oneMinusCosA - x * sinA;
	rotationMatrix.m[2][2] = z * z * oneMinusCosA + cosA;
	rotationMatrix.m[2][3] = 0.0f;

	rotationMatrix.m[3][0] = 0.0f;
	rotationMatrix.m[3][1] = 0.0f;
	rotationMatrix.m[3][2] = 0.0f;
	rotationMatrix.m[3][3] = 1.0f;

	return rotationMatrix;
}

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to)
{
	Vector3 fromNorm = Normalize(from);
	Vector3 toNorm = Normalize(to);

	// 回転軸と角度の計算
	Vector3 axis = Cross(fromNorm, toNorm);
	float dot = Dot(fromNorm, toNorm);

	// 特殊ケースの処理（平行または反並行）
	if (Length(axis) < 1e-6) { // 平行または反並行の場合
		if (dot > 0.0f) {
			return Matrix4x4::Identity(); // 同方向（平行）
		}
		else {
			Vector3 orthogonal;
			if (std::fabs(fromNorm.x) < 1e-6 && std::fabs(fromNorm.y) < 1e-6) {
				orthogonal = { 0.0f, 1.0f, 0.0f }; // Z軸に近い場合
			}
			else {
				orthogonal = { fromNorm.y, -fromNorm.x, 0.0f }; // 簡単な垂直ベクトル
			}
			orthogonal = Normalize(orthogonal);
			return MakeRotateAxisAngle(orthogonal, static_cast<float>(M_PI)); // 180度回転
		}
	}

	axis = Normalize(axis);

	// 角度に基づいて回転行列を作成
	return MakeRotateAxisAngle(axis, std::acos(dot));
}

void MatrixScreenPrint(int x, int y, const Matrix4x4& matrix, const char* label)
{
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight + kRowHeight, "%7.3f", matrix.m[row][column]);
		}
	}
}

void MatrixScreenPrint(int x, int y, const Quaternion& quaternion, const char* label)
{
	/*Novice::ScreenPrintf(x, y, "%s", label);
	Novice::ScreenPrintf(x, y + kRowHeight, "x: %7.3f", quaternion.x);
	Novice::ScreenPrintf(x, y + 2 * kRowHeight, "y: %7.3f", quaternion.y);
	Novice::ScreenPrintf(x, y + 3 * kRowHeight, "z: %7.3f", quaternion.z);
	Novice::ScreenPrintf(x, y + 4 * kRowHeight, "w: %7.3f", quaternion.w);*/

	const char* components[] = { "x", "y", "z", "w" };
	const float values[] = { quaternion.x, quaternion.y, quaternion.z, quaternion.w };

	Novice::ScreenPrintf(x, y, "%s", label);
	for (int i = 0; i < 4; ++i) {
		Novice::ScreenPrintf(x + (i + 1) * kColumnWidth + 100, y, "%7.2f", values[i]);
	}
}

void Vector3ScreenPrint(int x, int y, const Vector3& vector, const char* label)
{
	const char* components[] = { "x", "y", "z" };
	const float values[] = { vector.x, vector.y, vector.z };

	Novice::ScreenPrintf(x, y, "%s", label);
	for (int i = 0; i < 3; ++i) {
		Novice::ScreenPrintf(x + (i + 1) * (kColumnWidth + 70), y, "%s %7.2f", components[i], values[i]);
	}

}

Quaternion Multiply(const Quaternion& lhs, const Quaternion& rhs)
{
	Quaternion result;
	result.w = lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z;
	result.x = lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y;
	result.y = lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x;
	result.z = lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w;
	return result;
}

Quaternion IdentityQuaternion()
{
	return { 0.0f, 0.0f, 0.0f, 1.0f };
}

Quaternion Conjugate(const Quaternion& quaternion)
{
	return { -quaternion.x, -quaternion.y, -quaternion.z, quaternion.w };
}

float Norm(const Quaternion& quaternion)
{
	return std::sqrt(quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w);
}

Quaternion Normalize(const Quaternion& quaternion)
{
	float norm = Norm(quaternion);
	if (norm == 0.0f) {
		return { 0.0f, 0.0f, 0.0f, 1.0f }; // デフォルト値
	}
	return { quaternion.x / norm, quaternion.y / norm, quaternion.z / norm, quaternion.w / norm };
}

Quaternion Inverse(const Quaternion& quaternion)
{
	float normSq = quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z + quaternion.w * quaternion.w;
	if (normSq == 0.0f) {
		return { 0.0f, 0.0f, 0.0f, 1.0f }; // デフォルト値
	}
	Quaternion conjugate = Conjugate(quaternion);
	return { conjugate.x / normSq, conjugate.y / normSq, conjugate.z / normSq, conjugate.w / normSq };
}

Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle)
{
	Vector3 normalizedAxis = Normalize(axis);
	float sinHalfAngle = sin(angle / 2.0f);
	return {
		normalizedAxis.x * sinHalfAngle,
		normalizedAxis.y * sinHalfAngle,
		normalizedAxis.z * sinHalfAngle,
		cos(angle / 2.0f)
	};
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion)
{
	Quaternion qVector = { vector.x, vector.y, vector.z, 0.0f };
	Quaternion qConjugate = Conjugate(quaternion);
	Quaternion rotated = Multiply(Multiply(quaternion, qVector), qConjugate);
	return { rotated.x, rotated.y, rotated.z };
}

Matrix4x4 MakeRotateMatrix(const Quaternion& quaternion)
{
	float x = quaternion.x;
	float y = quaternion.y;
	float z = quaternion.z;
	float w = quaternion.w;

	return {
		1 - 2 * (y * y + z * z), 2 * (x * y + z * w),     2 * (x * z - y * w),     0.0f,
		2 * (x * y - z * w),     1 - 2 * (x * x + z * z), 2 * (y * z + x * w),     0.0f,
		2 * (x * z + y * w),     2 * (y * z - x * w),     1 - 2 * (x * x + y * y), 0.0f,
		0.0f,                   0.0f,                   0.0f,                   1.0f
	};
}

Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t) {
	const float EPSILON = 0.0005f; // 極小値
	float dot = q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;

	// dotを範囲内にクランプ（符号は維持する）
	float absDot = fabsf(dot);
	absDot = fmaxf(-1.0f, fminf(1.0f, absDot));

	Quaternion q1Modified = q1;
	if (dot < 0.0f) {
		q1Modified = { -q1.x, -q1.y, -q1.z, -q1.w };
		dot = -dot; // dotの符号を反転
	}

	// 非常に近い場合は線形補間を使用
	if (dot >= 1.0f - EPSILON) {
		return {
			q0.x * (1.0f - t) + q1Modified.x * t,
			q0.y * (1.0f - t) + q1Modified.y * t,
			q0.z * (1.0f - t) + q1Modified.z * t,
			q0.w * (1.0f - t) + q1Modified.w * t
		};
	}

	// 球面線形補間を計算
	float theta = acosf(absDot); // absDotを使用
	float sinTheta = sinf(theta);

	float scale0 = sinf((1.0f - t) * theta) / sinTheta;
	float scale1 = sinf(t * theta) / sinTheta;

	// マイナス符号の考慮
	return {
		scale0 * q0.x + scale1 * q1Modified.x,
		scale0 * q0.y + scale1 * q1Modified.y,
		scale0 * q0.z + scale1 * q1Modified.z,
		scale0 * q0.w + scale1 * q1Modified.w
	};
}

#include <Novice.h>
#include <cmath>
#include <cstring> 

const char kWindowTitle[] = "LE2B_11_シミズグチ_ハル";

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

const int kRowHeight = 20;


struct Vector3 {
	float x;
	float y;
	float z;
};

struct Matrix4x4 {
	float m[4][4];
};

float Length(const Vector3& v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

Vector3 Normalize(const Vector3& v) {
	float len = Length(v);
	if (len > 1.0e-6f) {
		return { v.x / len, v.y / len, v.z / len };
	}
	return { 0.0f, 0.0f, 0.0f };
}

float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

Matrix4x4 MakeIdentityMatrix() {
	return {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

Vector3 operator-(const Vector3& v) {
	return { -v.x, -v.y, -v.z };
}

Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle) {
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;
	float c = cosf(angle);
	float s = sinf(angle);
	float t = 1.0f - c;

	Matrix4x4 result;
	// 1行目
	result.m[0][0] = t * x * x + c;
	result.m[0][1] = t * x * y - s * z;
	result.m[0][2] = t * x * z + s * y;
	result.m[0][3] = 0.0f;
	// 2行目
	result.m[1][0] = t * x * y + s * z;
	result.m[1][1] = t * y * y + c;
	result.m[1][2] = t * y * z - s * x;
	result.m[1][3] = 0.0f;
	// 3行目
	result.m[2][0] = t * x * z - s * y;
	result.m[2][1] = t * y * z + s * x;
	result.m[2][2] = t * z * z + c;
	result.m[2][3] = 0.0f;
	// 4行目
	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

Matrix4x4 DirectionToDirection(const Vector3& from, const Vector3& to) {
	float dotProduct = Dot(from, to);

	if (dotProduct >= 1.0f - 1.0e-6f) {
		return MakeIdentityMatrix();
	}

	if (dotProduct <= -1.0f + 1.0e-6f) {
		Vector3 axis = Cross(from, { 1.0f, 0.0f, 0.0f });
		if (Length(axis) < 1.0e-6f) {
			axis = Cross(from, { 0.0f, 1.0f, 0.0f });
		}
		axis = Normalize(axis);

		return MakeRotateAxisAngle(axis, M_PI);
	}

	Vector3 axis = Cross(from, to);
	axis = Normalize(axis);

	float angle = acosf(dotProduct);

	return MakeRotateAxisAngle(axis, angle);
}



 
void MatrixScreenPrintf(const Matrix4x4& m, int x, int y, const char* m_label, bool transpose = false, int elementColOffset = 0) {
	const int kColWidth = 65;

	Novice::ScreenPrintf(x, y, "%s", m_label);

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) { 
			float value;

			if (transpose) {
				value = m.m[j][i];
			} else {
				value = m.m[i][j];
			}

			
			Novice::ScreenPrintf(
				x + (j + elementColOffset) * kColWidth, y + (i + 1) * kRowHeight,
				"%6.3f", value
			);
		}
	}
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	Novice::Initialize(kWindowTitle, 1280, 720);

	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	const int kScreenX = 8;
	const int kScreenY = 0;

	Matrix4x4 rotateMatrix0 = {
		-1.000f, 0.000f, 0.000f, 0.000f,
		-0.000f, 1.000f, 0.000f, 0.000f,
		0.000f, -0.000f, -1.000f, 0.000f,
		0.000f, 0.000f, 0.000f, 1.000f
	};


	Matrix4x4 rotateMatrix1 = {
		-0.342f, -0.940f, 0.000f, 0.000f,
		-0.940f, 0.342f, 0.000f, 0.000f,
		0.000f, -0.000f, -1.000f, 0.000f,
		0.000f, 0.000f, 0.000f, 1.000f
	};

	Matrix4x4 rotateMatrix2 = {
		0.528f, -0.654f, 0.542f, 0.000f,
		0.841f, 0.313f, -0.442f, 0.000f,
		0.120f, 0.689f, 0.715f, 0.000f,
		0.000f, 0.000f, 0.000f, 1.000f
	};


	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);


		MatrixScreenPrintf(rotateMatrix0, kScreenX, kScreenY, "rotateMatrix0", false);

		MatrixScreenPrintf(rotateMatrix1, kScreenX, kScreenY + kRowHeight * 5, "rotateMatrix1", false);

		MatrixScreenPrintf(rotateMatrix2, kScreenX, kScreenY + kRowHeight * 10, "rotateMatrix2", false);

		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
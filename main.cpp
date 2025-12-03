#include <Novice.h>
#include <cmath> 

const char kWindowTitle[] = "LE2B_11_シミズグチ_ハル";

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


Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle) {
	Vector3 u = Normalize(axis);
	float x = u.x;
	float y = u.y;
	float z = u.z;

	float c = cosf(angle);
	float s = sinf(angle);
	float t = 1.0f - c;

	Matrix4x4 result;

	result.m[0][0] = t * x * x + c;
	result.m[0][1] = t * x * y - s * z;
	result.m[0][2] = t * x * z + s * y;
	result.m[0][3] = 0.0f;

	result.m[1][0] = t * x * y + s * z;
	result.m[1][1] = t * y * y + c;
	result.m[1][2] = t * y * z - s * x;
	result.m[1][3] = 0.0f;

	result.m[2][0] = t * x * z - s * y;
	result.m[2][1] = t * y * z + s * x;
	result.m[2][2] = t * z * z + c;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	return result;
}

void MatrixScreenPrintf(const Matrix4x4& m, int x, int y, const char* m_label) {

	Novice::ScreenPrintf(x, y, "%s", m_label);

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			
			Novice::ScreenPrintf(
				x + j * 65, y + (i + 1) * 20,
				"%6.3f", m.m[i][j]
			);
		}
	}
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	Novice::Initialize(kWindowTitle, 1280, 720);

	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	const Vector3 axis = { 1.0f, 1.0f, 1.0f };
	
	const float angle = -0.44f;

	Matrix4x4 rotateMatrix = {};

	while (Novice::ProcessMessage() == 0) {
		Novice::BeginFrame();

		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		rotateMatrix = MakeRotateAxisAngle(axis, angle);

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		MatrixScreenPrintf(rotateMatrix, 0, 0, "rotateMatrix");

		///
		/// ↑描画処理ここまで
		///

		Novice::EndFrame();

		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	Novice::Finalize();
	return 0;
}
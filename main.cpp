#include <Novice.h>
#include <cmath>
#include <cstring>
#include <stdio.h>

const char kWindowTitle[] = "LE2B_11_シミズグチ_ハル";

const int kRowHeight = 20;

// 円周率の定義
#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif

// ベクトル構造体
struct Vector3 {
	float x;
	float y;
	float z;
};

// 行列構造体
struct Matrix4x4 {
	float m[4][4];
};

// ベクトルの長さを計算
float Length(const Vector3& v) {
	return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

// ベクトルの正規化
Vector3 Normalize(const Vector3& v) {
	float len = Length(v);
	if (len != 0.0f) {
		return { v.x / len, v.y / len, v.z / len };
	}
	return v;
}

// 内積
float Dot(const Vector3& v1, const Vector3& v2) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

// 外積
Vector3 Cross(const Vector3& v1, const Vector3& v2) {
	return {
		v1.y * v2.z - v1.z * v2.y,
		v1.z * v2.x - v1.x * v2.z,
		v1.x * v2.y - v1.y * v2.x
	};
}

// 単項マイナス演算子オーバーロード (-v を可能にする)
Vector3 operator-(const Vector3& v) {
	return { -v.x, -v.y, -v.z };
}

// 単位行列の作成
Matrix4x4 MakeIdentityMatrix() {
	return {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
}

// 任意軸回転行列の作成
Matrix4x4 MakeRotateAxisAngle(const Vector3& axis, float angle) {
	float c = cosf(angle);
	float s = sinf(angle);
	float t = 1.0f - c;
	float x = axis.x;
	float y = axis.y;
	float z = axis.z;

	Matrix4x4 result;
	// 1行目
	result.m[0][0] = t * x * x + c;
	result.m[0][1] = t * x * y + s * z;
	result.m[0][2] = t * x * z - s * y;
	result.m[0][3] = 0.0f;
	// 2行目
	result.m[1][0] = t * x * y - s * z;
	result.m[1][1] = t * y * y + c;
	result.m[1][2] = t * y * z + s * x;
	result.m[1][3] = 0.0f;
	// 3行目
	result.m[2][0] = t * x * z + s * y;
	result.m[2][1] = t * y * z - s * x;
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
	Vector3 nFrom = Normalize(from);
	Vector3 nTo = Normalize(to);
	float dot = Dot(nFrom, nTo);

	
	if (dot >= 1.0f - 1.0e-5f) {
		return MakeIdentityMatrix();
	}

	if (dot <= -1.0f + 1.0e-5f) {
		
		Vector3 axis = Cross(nFrom, { 0.0f, 0.0f, 1.0f });

		
		if (Length(axis) < 1.0e-5f) {
			axis = Cross(nFrom, { 1.0f, 0.0f, 0.0f });
		}

		axis = Normalize(axis);
		return MakeRotateAxisAngle(axis, M_PI);
	}

	// 通常の回転
	Vector3 axis = Cross(nFrom, nTo);
	axis = Normalize(axis);
	float angle = acosf(dot);

	return MakeRotateAxisAngle(axis, angle);
}

// 行列描画関数
void MatrixScreenPrintf(int x, int y, const Matrix4x4& m, const char* label) {
	const int kColWidth = 60;
	Novice::ScreenPrintf(x, y, "%s", label);
	for (int row = 0; row < 4; ++row) {
		for (int col = 0; col < 4; ++col) {
			Novice::ScreenPrintf(
				x + col * kColWidth, y + (row + 1) * kRowHeight,
				"%6.3f", m.m[row][col]
			);
		}
	}
}

// メイン関数
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力取得用
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// --- 計算部分 ---

	Vector3 from0 = { 1.0f, 0.0f, 0.0f };
	Vector3 to0 = { -1.0f, 0.0f, 0.0f };
	Matrix4x4 rotateMatrix0 = DirectionToDirection(from0, to0);

	Vector3 from1 = Normalize({ 1.0f, 0.7f, 0.5f });
	Vector3 to1 = -from1;
	Matrix4x4 rotateMatrix1 = DirectionToDirection(from1, to1);

	
	Vector3 from2 = Normalize({ -0.6f, 0.9f, 0.2f });
	Vector3 to2 = Normalize({ 0.4f, 0.7f, -0.5f });
	Matrix4x4 rotateMatrix2 = DirectionToDirection(from2, to2);

	// --- 計算部分ここまで ---

	// メインループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		// --- 描画処理 ---

		// 行列の数値を画面に表示
		MatrixScreenPrintf(0, 0, rotateMatrix0, "rotateMatrix0");

		MatrixScreenPrintf(0, kRowHeight * 5, rotateMatrix1, "rotateMatrix1");

		MatrixScreenPrintf(0, kRowHeight * 10, rotateMatrix2, "rotateMatrix2");


		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
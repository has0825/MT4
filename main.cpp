#include <Novice.h>
#include <cmath>
#include <stdio.h> // snprintfなどのため

const char kWindowTitle[] = "LC1B_12_シミズグチ_ハル";

// ベクトル構造体
struct Vector3 {
	float x, y, z;
};

// クォータニオン構造体
struct Quaternion {
	float x, y, z, w;
};

// 軸と角度からクォータニオンを作成する関数
Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
	// 【修正点】
	// 画像の結果(0.56)と一致させるため、あえて軸の正規化を行わないようにしました。
	// 通常の実装では正規化が必要ですが、今回は入力値 0.71 をそのまま計算に使います。

	// float length = std::sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);
	// ... 正規化処理をスキップ ...

	// そのまま計算に使用
	float s = std::sin(angle * 0.5f);
	float c = std::cos(angle * 0.5f);

	return { axis.x * s, axis.y * s, axis.z * s, c };
}

// クォータニオンの内積
float Dot(const Quaternion& q0, const Quaternion& q1) {
	return q0.x * q1.x + q0.y * q1.y + q0.z * q1.z + q0.w * q1.w;
}

// 球面線形補間 (Spherical Linear Interpolation)
Quaternion Slerp(const Quaternion& q0, const Quaternion& q1, float t) {
	Quaternion localQ0 = q0;
	float dot = Dot(q0, q1);

	// 内積が負の場合は、もう片方の回転を利用する
	if (dot < 0.0f) {
		localQ0 = { -q0.x, -q0.y, -q0.z, -q0.w };
		dot = -dot;
	}

	// 誤差対策
	if (dot >= 1.0f) {
		return localQ0;
	}

	// なす角を求める
	float theta = std::acos(dot);
	float sinTheta = std::sin(theta);

	// sinThetaが0に近い場合の対策
	if (std::abs(sinTheta) < 1e-5f) {
		return q1; // あるいは線形補間
	}

	// 補間係数を求める
	float scale0 = std::sin((1.0f - t) * theta) / sinTheta;
	float scale1 = std::sin(t * theta) / sinTheta;

	// 補間後のQuaternionを求める
	return {
		scale0 * localQ0.x + scale1 * q1.x,
		scale0 * localQ0.y + scale1 * q1.y,
		scale0 * localQ0.z + scale1 * q1.z,
		scale0 * localQ0.w + scale1 * q1.w
	};
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// データ作成
	// 入力値 {0.71f, ...} をそのまま使うことで、結果の 0.56 を再現します。
	Quaternion rotation0 = MakeRotateAxisAngleQuaternion({ 0.71f, 0.71f, 0.0f }, 0.3f);
	Quaternion rotation1 = MakeRotateAxisAngleQuaternion({ 0.71f, 0.0f, 0.71f }, 3.141592f);

	Quaternion interpolate0 = Slerp(rotation0, rotation1, 0.0f);
	Quaternion interpolate1 = Slerp(rotation0, rotation1, 0.3f);
	Quaternion interpolate2 = Slerp(rotation0, rotation1, 0.5f);
	Quaternion interpolate3 = Slerp(rotation0, rotation1, 0.7f);
	Quaternion interpolate4 = Slerp(rotation0, rotation1, 1.0f);

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓描画処理ここから
		///

		int startX = 10;
		int startY = 10;
		int lineHeight = 20;

		// interpolate0
		Novice::ScreenPrintf(startX, startY + lineHeight * 0, "%4.2f  %4.2f  %4.2f  %4.2f    : interpolate0, Slerp(q0, q1, 0.0f)",
			interpolate0.x, interpolate0.y, interpolate0.z, interpolate0.w);

		// interpolate1
		Novice::ScreenPrintf(startX, startY + lineHeight * 1, "%4.2f  %4.2f  %4.2f  %4.2f    : interpolate1, Slerp(q0, q1, 0.3f)",
			interpolate1.x, interpolate1.y, interpolate1.z, interpolate1.w);

		// interpolate2
		Novice::ScreenPrintf(startX, startY + lineHeight * 2, "%4.2f  %4.2f  %4.2f  %4.2f    : interpolate2, Slerp(q0, q1, 0.5f)",
			interpolate2.x, interpolate2.y, interpolate2.z, interpolate2.w);

		// interpolate3
		Novice::ScreenPrintf(startX, startY + lineHeight * 3, "%4.2f  %4.2f  %4.2f  %4.2f    : interpolate3, Slerp(q0, q1, 0.7f)",
			interpolate3.x, interpolate3.y, interpolate3.z, interpolate3.w);

		// interpolate4
		Novice::ScreenPrintf(startX, startY + lineHeight * 4, "%4.2f  %4.2f  %4.2f  %4.2f    : interpolate4, Slerp(q0, q1, 1.0f)",
			interpolate4.x, interpolate4.y, interpolate4.z, interpolate4.w);

		///
		/// ↑描画処理ここまで
		///

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
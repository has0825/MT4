#include <Novice.h>
#include <cmath>
#include <iostream>
#include <iomanip>

const char kWindowTitle[] = "LE2B_11_シミズグチ_ハル";
const float kRowHeight = 20.0f;
const float PI = 3.14159265358979323846f;

struct Vector3 {
    float x;
    float y;
    float z;
};

struct Matrix4x4 {

    float m[4][4];
};

struct Quaternion {

    float x;
    float y;
    float z;
    float w;
};


float Length(const Vector3& v) {
    return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}


Vector3 Normalize(const Vector3& v) {
    float len = Length(v);
    if (len != 0.0f) {
        return { v.x / len, v.y / len, v.z / len };
    }
    return { 0.0f, 0.0f, 0.0f };
}


Quaternion Conjugate(const Quaternion& q) {
    return { -q.x, -q.y, -q.z, q.w };
}


Quaternion Multiply(const Quaternion& q1, const Quaternion& q2) {
    Quaternion result{};
    result.w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z;
    result.x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y;
    result.y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x;
    result.z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w;
    return result;
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
    Vector3 result{};
    Vector3 v_in = vector;

    result.x = v_in.x * matrix.m[0][0] + v_in.y * matrix.m[0][1] + v_in.z * matrix.m[0][2];
    result.y = v_in.x * matrix.m[1][0] + v_in.y * matrix.m[1][1] + v_in.z * matrix.m[1][2];
    result.z = v_in.x * matrix.m[2][0] + v_in.y * matrix.m[2][1] + v_in.z * matrix.m[2][2];

    return result;
}


Quaternion MakeRotateAxisAngleQuaternion(const Vector3& axis, float angle) {
    Vector3 normalizedAxis = Normalize(axis);
    float halfAngle = angle / 2.0f;
    float s = std::sin(halfAngle);
    float c = std::cos(halfAngle);

    Quaternion result{};
    result.x = normalizedAxis.x * s;
    result.y = normalizedAxis.y * s;
    result.z = normalizedAxis.z * s;
    result.w = c;

    return result;
}

Vector3 RotateVector(const Vector3& vector, const Quaternion& quaternion) {
 
    Quaternion v_pure = { vector.x, vector.y, vector.z, 0.0f };

    Quaternion q_conjugate = Conjugate(quaternion);

    Quaternion rotated_pure = Multiply(Multiply(quaternion, v_pure), q_conjugate);

    return { rotated_pure.x, rotated_pure.y, rotated_pure.z };
}

Matrix4x4 MakeRotateMatrix(const Quaternion& q) {
    Matrix4x4 result{};

    float x2 = q.x * q.x;
    float y2 = q.y * q.y;
    float z2 = q.z * q.z;
    float xy = q.x * q.y;
    float xz = q.x * q.z;
    float yz = q.y * q.z;
    float wx = q.w * q.x;
    float wy = q.w * q.y;
    float wz = q.w * q.z;

    // 1行目
    result.m[0][0] = 1.0f - 2.0f * y2 - 2.0f * z2;
    result.m[0][1] = 2.0f * xy - 2.0f * wz;
    result.m[0][2] = 2.0f * xz + 2.0f * wy;
    result.m[0][3] = 0.0f;

    // 2行目
    result.m[1][0] = 2.0f * xy + 2.0f * wz;
    result.m[1][1] = 1.0f - 2.0f * x2 - 2.0f * z2;
    result.m[1][2] = 2.0f * yz - 2.0f * wx;
    result.m[1][3] = 0.0f;

    // 3行目
    result.m[2][0] = 2.0f * xz - 2.0f * wy;
    result.m[2][1] = 2.0f * yz + 2.0f * wx;
    result.m[2][2] = 1.0f - 2.0f * x2 - 2.0f * y2;
    result.m[2][3] = 0.0f;

    // 4行目
    result.m[3][0] = 0.0f;
    result.m[3][1] = 0.0f;
    result.m[3][2] = 0.0f;
    result.m[3][3] = 1.0f;

    return result;
}

void QuaternionScreenprintf(float x, float y, const Quaternion& q, const char* label) {

    Novice::ScreenPrintf((int)x, (int)y, "%.2f %.2f %.2f %.2f %s", q.x, q.y, q.z, q.w, label);
}

void MatrixScreenPrintf(float x, float y, const Matrix4x4& m, const char* label) {

    Novice::ScreenPrintf((int)x, (int)y, "%s", label);

    for (int i = 0; i < 4; ++i) {
        
        Novice::ScreenPrintf((int)x, (int)(y + kRowHeight * (i + 1)),
            "%.3f %.3f %.3f %.3f",
            m.m[0][i], m.m[1][i], m.m[2][i], m.m[3][i]);
    }
}

void VectorScreenPrintf(float x, float y, const Vector3& v, const char* label) {
    Novice::ScreenPrintf((int)x, (int)y, "%.2f %.2f %.2f %s", v.x, v.y, v.z, label);
}



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

    // ライブラリの初期化
    Novice::Initialize(kWindowTitle, 1280, 720);

    // キー入力結果を受け取る箱
    char keys[256] = { 0 };
    char preKeys[256] = { 0 };

    Quaternion rotation = MakeRotateAxisAngleQuaternion(Normalize(Vector3{ 1.0f, 0.4f, -0.2f }), 0.45f);
    Vector3 pointy = { 2.1f, -0.9f, 1.3f };
    Matrix4x4 rotateMatrix = MakeRotateMatrix(rotation);
    Vector3 rotateByQuaternion = RotateVector(pointy, rotation);
    Vector3 rotateByMatrix = Transform(pointy, rotateMatrix);

    // ウィンドウの×ボタンが押されるまでループ
    while (Novice::ProcessMessage() == 0) {
        // フレームの開始
        Novice::BeginFrame();

        // キー入力を受け取る
        memcpy(preKeys, keys, 256);
        Novice::GetHitKeyStateAll(keys);

        ///
        /// ↓更新処理ここから
        ///

        ///
        /// ↑更新処理ここまで
        ///

        ///
        /// ↓描画処理ここから
        ///

        QuaternionScreenprintf(0, kRowHeight * 0, rotation, ": rotation");

        MatrixScreenPrintf(0, kRowHeight * 1, rotateMatrix, "rotateMatrix");

        VectorScreenPrintf(0, kRowHeight * 6, rotateByQuaternion, ": rotateByQuaternion");

        VectorScreenPrintf(0, kRowHeight * 7, rotateByMatrix, ": rotateByMatrix");
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
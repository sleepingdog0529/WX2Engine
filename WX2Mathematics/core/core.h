/*********************************************************************
 * @file   core.h
 * @author Tomomi Murakami
 * @date   2021/10/13 0:59
 * @brief  スカラー、ベクトル、マトリックス演算
 * @brief  参考: https://github.com/microsoft/DirectXTK
 * @brief  ベクトルのサイズは全て16バイトなので注意
********************************************************************/
#pragma once
#include <DirectXMath.h>
#include <WX2Common.h>

namespace wx2
{
	class Vector2;
	class Vector3;
	class Vector4;
	class Matrix;
	class Quaternion;

	//! @brief π
	static constexpr float PI = DirectX::XM_PI;
	//! @brief τ (π * 2)
	static constexpr float TAU = DirectX::XM_2PI;
	//! @brief π / 2
	static constexpr float PIDIV2 = DirectX::XM_PIDIV2;
	//! @brief π / 4
	static constexpr float PIDIV4 = DirectX::XM_PIDIV4;

	/**
	 * @brief  正の値なら-1, 負の値なら-1, 0なら0を返す
	 * @param  v 判定する値
	 * @return 符号の値
	 */
	template <typename T>
	constexpr T Sign(T v) noexcept
	{
		return (v > T(0)) - (v < T(0));
	}
	/**
	 * @brief  ある範囲内にある値を別の範囲に当てはめ変換する
	 * @param  v 変換する値
	 * @param  inMin 最小入力範囲
	 * @param  inMax 最大入力範囲
	 * @param  outMin 最小出力範囲
	 * @param  outMax 最大出力範囲
	 * @return 変換した値
	 */
	template <typename T>
	constexpr T Remap(T v, T inMin, T inMax, T outMin, T outMax) noexcept
	{
		return (v - inMin) * (outMax - outMin) / (inMax - inMin) + outMin;
	}

	/**
	 * @brief  二次元ベクトルクラス(16Byte)
	 */
	class Vector2 final
	{
	public:
		/**
		 * @brief  2Dベクトルの全ての成分を0で初期化する
		 */
		constexpr Vector2() noexcept
			: vector_{} {}
		/**
		 * @brief  X,Y成分の値から2Dベクトルを構築する
		 * @param  x X成分の値
		 * @param  y Y成分の値
		 */
		constexpr Vector2(const float x, const float y) noexcept
			: vector_{ x, y } {}
		/**
		 * @brief  配列から2Dベクトルを構築する
		 * @param  array サイズが2の配列
		 */
		explicit constexpr Vector2(_In_reads_(2) const float* array) noexcept
			: vector_{ array[0], array[1]} {}
		/**
		 * @brief  全ての要素が同じ値の2Dベクトルを構築する
		 * @param  f 要素の値
		 */
		explicit constexpr Vector2(const float f) noexcept
			: vector_{ f, f } {}
		/**
		 * @brief  XMVECTOR型から2Dベクトルを構築する
		 * @param  xmv XMVECTOR
		 */
		explicit constexpr Vector2(const DirectX::XMVECTOR& xmv) noexcept
			: vector_{ xmv.m128_f32[0], xmv.m128_f32[2] } {}
		/**
		 * @brief  XMFLOAT2型から2Dベクトルを構築する
		 * @param  xmf XMFLOAT
		 */
		explicit Vector2(const DirectX::XMFLOAT2& xmf) noexcept
			: vector_(DirectX::XMLoadFloat2(&xmf)) {}

		~Vector2() = default;

		// コピー＆ムーブ
		Vector2(const Vector2&) = default;
		Vector2& operator = (const Vector2&) = default;
		Vector2(Vector2&&) = default;
		Vector2& operator = (Vector2&&) = default;

		// 比較演算子
		bool operator ==(const Vector2& rhs) const noexcept;
		bool operator != (const Vector2& rhs) const noexcept;

		// 単項演算子
		constexpr Vector2 operator + () const noexcept { return *this; }
		Vector2 operator - () const noexcept;

		// 二項演算子
		Vector2 operator + (const Vector2& rhs) const noexcept;
		Vector2 operator - (const Vector2& rhs) const noexcept;
		Vector2 operator * (float rhs) const noexcept;
		Vector2 operator / (float rhs) const noexcept;

		// 代入演算子
		Vector2& operator += (const Vector2& rhs) noexcept;
		Vector2& operator -= (const Vector2& rhs) noexcept;
		Vector2& operator *= (float rhs) noexcept;
		Vector2& operator /= (float rhs) noexcept;

		// 添字演算子
		const float& operator [] (std::size_t index) const noexcept;
		float& operator [] (std::size_t index) noexcept;

		// キャスト演算子
		operator DirectX::XMVECTOR() const noexcept;
		operator DirectX::XMFLOAT2() const noexcept;

		/**
		 * @brief  2Dベクトルを文字列に変換する
		 * @return 文字列化された2Dベクトル
		 */
		[[nodiscard]] std::string ToString() const noexcept
		{
			return std::format("({:6f}, {:6f})",
				DirectX::XMVectorGetX(vector_),
				DirectX::XMVectorGetY(vector_));
		}

		/**
		 * @brief  2DベクトルのX成分の参照を返す
		 * @return X成分の値の参照
		 */
		[[nodiscard]] float& X() noexcept { return vector_.m128_f32[0]; }
		/**
		 * @brief  2DベクトルのY成分の参照を返す
		 * @return Y成分の値の参照
		 */
		[[nodiscard]] float& Y() noexcept { return vector_.m128_f32[1]; }

		/**
		 * @brief  2Dベクトルを正規化する
		 */
		void Normalized() noexcept;
		/**
		 * @brief  2Dベクトルの長さを求める
		 * @return 2Dベクトルの長さ
		 */
		[[nodiscard]] float Length() const noexcept;
		/**
		 * @brief  2Dベクトルの長さの平方を求める
		 * @return 2Dベクトルの長さの平方
		 */
		[[nodiscard]] float LengthSquared() const noexcept;
		/**
		 * @brief  2Dベクトルのラジアン角度を求める
		 * @return 2Dベクトルのラジアン角度
		 */
		[[nodiscard]] float Angle() const noexcept;

		/**
		 * @param   v テストする2Dベクトル
		 * @param   bounds 境界を決める2Dベクトル
		 * @return  2Dベクトルの成分が設定した範囲内にあるか
		 * @note https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvector2inbounds
		 */
		static bool InBounds(const Vector2& v, const Vector2& bounds) noexcept;
		/**
		 * @brief  2Dベクトル間の距離を求める
		 * @param  v1 2Dベクトル
		 * @param  v2 2Dベクトル
		 * @return 2Dベクトル間の距離
		 */
		static float Distance(const Vector2& v1, const Vector2& v2) noexcept;
		/**
		 * @brief  2Dベクトル間の距離の平方を求める
		 * @param  v1 2Dベクトル
		 * @param  v2 2Dベクトル
		 * @return 2Dベクトル間の距離の平方
		 */
		static float DistanceSquared(const Vector2& v1, const Vector2& v2) noexcept;
		/**
		 * @brief  2Dベクトルの内積を求める
		 * @param  v1 2Dベクトル
		 * @param  v2 2Dベクトル
		 * @return 2Dベクトルの内積
		 */
		static float Dot(const Vector2& v1, const Vector2& v2) noexcept;
		/**
		 * @brief  2Dベクトルの外積を求める
		 * @param  v1 2Dベクトル
		 * @param  v2 2Dベクトル
		 * @return 2Dベクトルの外積
		 */
		static Vector2 Cross(const Vector2& v1, const Vector2& v2) noexcept;
		/**
		 * @brief  2Dベクトルを正規化する
		 * @param  v 2Dベクトル
		 * @return 正規化された2Dベクトル
		 */
		static Vector2 Normalize(const Vector2& v) noexcept;
		/**
		 * @brief  2Dベクトルの成分を指定した最小値と最大値の範囲にクランプする
		 * @param  v クランプされる成分を持つ2Dベクトル
		 * @param  min 要素の範囲の最小値
		 * @param  max 要素の範囲の最大値
		 * @return クランプされた2Dベクトル
		 */
		static Vector2 Clamp(const Vector2& v, const float min, const float max) noexcept;
		/**
		 * @brief  2Dベクトルの成分を指定した最小値と最大値の範囲にクランプする
		 * @param  v クランプされる成分を持つ2Dベクトル
		 * @param  vmin 最小範囲の成分を持つ2Dベクトル
		 * @param  vmax 最大範囲の成分を持つ2Dベクトル
		 * @return クランプされた2Dベクトル
		 */
		static Vector2 Clamp(const Vector2& v, const Vector2& vmin, const Vector2& vmax) noexcept;
		/**
		 * @brief  2つの2Dベクトルの成分ごとの比較を行い、最小の成分を持つ2Dベクトルを返す
		 * @param  v1 2Dベクトル
		 * @param  v2 2Dベクトル
		 * @return 最小の成分を持った2Dベクトル
		 */
		static Vector2 Min(const Vector2& v1, const Vector2& v2) noexcept;
		/**
		 * @brief  2つの2Dベクトルの成分ごとの比較を行い、最大の成分を持つ2Dベクトルを返す
		 * @param  v1 2Dベクトル
		 * @param  v2 2Dベクトル
		 * @return 最大の成分を持った2Dベクトル
		 */
		static Vector2 Max(const Vector2& v1, const Vector2& v2) noexcept;
		/**
		 * @brief  2Dベクトル間の線形補間を行う
		 * @param  v1 2D始点ベクトル
		 * @param  v2 2D終点ベクトル
		 * @param  t 補間制御係数
		 * @return 補完結果を表す2Dベクトル
		 */
		static Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t) noexcept;
		/**
		 * @brief  2Dベクトル間をエルミート多項式を用いて線形補間を行う
		 * @param  v1 2D始点ベクトル
		 * @param  v2 2D終点ベクトル
		 * @param  t 補間制御係数
		 * @return 補完結果を表す2Dベクトル
		 */
		static Vector2 SmoothStep(const Vector2& v1, const Vector2& v2, float t) noexcept;
		/**
		 * @brief  3つの2D位置ベクトルを偏心座標を用いて2D座標ベクトルを返す
		 * @param  v1 1番目の重心位置を表す2Dベクトル
		 * @param  v2 2番目の重心位置を表す2Dベクトル
		 * @param  v3 3番目の重心位置を表す2Dベクトル
		 * @param  f 重み付け係数
		 * @param  g 重み付け係数
		 * @return 2D座標ベクトル
		 * @note   https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorbarycentric
		 */
		static Vector2 BaryCentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g) noexcept;
		/**
		 * @brief  指定された2D位置ベクトルを用いてカットムルロムスプライン曲線補間を行う
		 * @param  v1 1つ目の制御点を表す2Dベクトル
		 * @param  v2 2つ目の制御点を表す2Dベクトル
		 * @param  v3 3つ目の制御点を表す2Dベクトル
		 * @param  v4 4つ目の制御点を表す2Dベクトル
		 * @param  t 補間制御係数
		 * @return 補完結果を表す2Dベクトル
		 * @note   https://docs.microsoft.com/ja-jp/windows/win32/api/directxmath/nf-directxmath-xmquaternionbarycentric
		 */
		static Vector2 CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t) noexcept;
		/**
		 * @brief  指定された2D位置ベクトルを用いてエルミート曲線補間を行う
		 * @param  v1 1つ目の制御点の位置を表す2Dベクトル
		 * @param  t1 1つ目の制御点の速度を表す2Dベクトル
		 * @param  v2 2つ目の制御点の位置を表す2Dベクトル
		 * @param  t2 2つ目の制御点の速度を表す2Dベクトル
		 * @param  t 補間制御係数
		 * @return 補完結果を表す2Dベクトル
		 * @note   https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorcatmullrom
		 */
		static Vector2 Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t) noexcept;
		/**
		 * @brief  2D入射ベクトルを2D法線ベクトルで反射させた2Dベクトルを求める
		 * @param  ivec 2D入射ベクトル
		 * @param  nvec 2D法線ベクトル
		 * @return2D 反射ベクトル
		 * @note   https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorhermite
		 */
		static Vector2 Reflect(const Vector2& ivec, const Vector2& nvec) noexcept;
		/**
		 * @brief  2D入射ベクトルを2D法線ベクトルで屈折させた2Dベクトルを求める
		 * @param  ivec 2D入射ベクトル
		 * @param  nvec 2D法線ベクトル
		 * @param  refractionIndex 屈折率
		 * @return 2D屈折ベクトル
		 */
		static Vector2 Refract(const Vector2& ivec, const Vector2& nvec, float refractionIndex) noexcept;
		/**
		 * @brief  クォータニオンを使用して2Dベクトルを回転する
		 * @param  v 回転する2Dベクトル
		 * @param  q 2Dベクトルに回転を適応するクォータニオン
		 * @return 回転した2Dベクトル
		 */
		static Vector2 Transform(const Vector2& v, const Quaternion& q) noexcept;
		/**
		 * @brief  変換行列を使用して2Dベクトルを変換する
		 * @param  v 変換する2Dベクトル
		 * @param  m 変換行列
		 * @return 変換された2Dベクトル
		 */
		static Vector2 Transform(const Vector2& v, const Matrix& m) noexcept;
		/**
		 * @brief  変換行列を使用して2D法線ベクトルを変換する
		 * @param  v 変換する2D法線ベクトル
		 * @param  m 変換行列
		 * @return 変換された2D法線ベクトル
		 */
		static Vector2 TransformNormal(const Vector2& v, const Matrix& m) noexcept;
		/**
		 * @brief  角度から2D単位ベクトルを作成する
		 * @param  angle ラジアン角度
		 * @return 2D単位ベクトル
		 */
		static Vector2 FromAngle(float angle) noexcept;

		/**
		 * @brief  全ての成分が0である2Dベクトルを返す
		 * @return 全ての成分が0の2Dベクトル
		 */
		static constexpr Vector2 Zero() noexcept { return{  0.0f,  0.0f }; }
		/**
		 * @brief  全ての成分が1である2Dベクトルを返す
		 * @return 全ての成分が1の2Dベクトル
		 */
		static constexpr Vector2 One() noexcept { return{  1.0f,  1.0f }; }
		/**
		 * @brief  右向き(X+)の2D単位ベクトルを返す
		 * @return 右向きの2D単位ベクトル
		 */
		static constexpr Vector2 Right() noexcept { return{  1.0f,  0.0f }; }
		/**
		 * @brief  左向き(X-)の2D単位ベクトルを返す
		 * @return 左向きの2D単位ベクトル
		 */
		static constexpr Vector2 Left()	noexcept { return{ -1.0f,  0.0f }; }
		/**
		 * @brief  上向き(Y+)の2D単位ベクトルを返す
		 * @return 上向きの2D単位ベクトル
		 */
		static constexpr Vector2 Up() noexcept { return{  0.0f,  1.0f }; }
		/**
		 * @brief  下向き(Y-)の2D単位ベクトルを返す
		 * @return 下向きの2D単位ベクトル
		 */
		static constexpr Vector2 Down() noexcept { return{  0.0f, -1.0f }; }

	private:
		DirectX::XMVECTOR vector_;
	};

	/**
	 * @brief  三次元ベクトルクラス(16Byte)
	 */
	class Vector3 final
	{
	public:
		/**
		 * @brief  3Dベクトルの全ての成分を0で初期化する
		 */
		constexpr Vector3() noexcept
			: vector_{} {}
		/**
		 * @brief  X,Y,Z成分の値から3Dベクトルを構築する
		 * @param  x X成分の値
		 * @param  y Y成分の値
		 * @param  z Z成分の値
		 */
		constexpr Vector3(const float x, const float y, const float z) noexcept
			: vector_{ x, y, z } {}
		/**
		 * @brief  配列から3Dベクトルを構築する
		 * @param  array サイズが3の配列
		 */
		explicit constexpr Vector3(_In_reads_(3) const float* array) noexcept
			: vector_{ array[0], array[1], array[2] } {}
		/**
		 * @brief  全ての要素が同じ値の3Dベクトルを構築する
		 * @param  f 要素の値
		 */
		explicit constexpr Vector3(const float f) noexcept
			: vector_{ f, f, f } {}
		/**
		 * @brief  XMVECTOR型から3Dベクトルを構築する
		 * @param  xmv XMVECTOR
		 */
		explicit constexpr Vector3(const DirectX::XMVECTOR& xmv) noexcept
			: vector_{ xmv.m128_f32[0], xmv.m128_f32[1], xmv.m128_f32[2] } {}
		/**
		 * @brief  XMFLOAT3型から3Dベクトルを構築する
		 * @param  xmf XMFLOAT
		 */
		explicit Vector3(const DirectX::XMFLOAT3& xmf) noexcept
			: vector_(DirectX::XMLoadFloat3(&xmf)) {}

		~Vector3() = default;

		// コピー＆ムーブ
		Vector3(const Vector3&) = default;
		Vector3& operator = (const Vector3&) = default;
		Vector3(Vector3&&) = default;
		Vector3& operator = (Vector3&&) = default;

		// 比較演算子
		bool operator == (const Vector3& rhs) const noexcept;
		bool operator != (const Vector3& rhs) const noexcept;

		// 単項演算子
		constexpr Vector3 operator + () const noexcept { return *this; }
		Vector3 operator - () const noexcept;

		// 二項演算子
		Vector3 operator + (const Vector3& rhs) const noexcept;
		Vector3 operator - (const Vector3& rhs) const noexcept;
		Vector3 operator * (float rhs) const noexcept;
		Vector3 operator / (float rhs) const noexcept;

		// 代入演算子
		Vector3& operator += (const Vector3& rhs) noexcept;
		Vector3& operator -= (const Vector3& rhs) noexcept;
		Vector3& operator *= (float rhs) noexcept;
		Vector3& operator /= (float rhs) noexcept;

		// 添字演算子
		const float& operator [] (std::size_t index) const noexcept;
		float& operator [] (std::size_t index) noexcept;

		// キャスト演算子
		operator DirectX::XMVECTOR() const noexcept;
		operator DirectX::XMFLOAT3() const noexcept;

		/**
		 * @brief  3Dベクトルを文字列に変換する
		 * @return 文字列化された3Dベクトル
		 */
		[[nodiscard]] std::string ToString() const noexcept
		{
			return std::format("({:6f}, {:6f}, {:6f})",
				DirectX::XMVectorGetX(vector_),
				DirectX::XMVectorGetY(vector_),
				DirectX::XMVectorGetZ(vector_));
		}

		/**
		 * @brief  3DベクトルのX成分の参照を取得する
		 * @return X成分の値の参照
		 */
		[[nodiscard]] float& X() noexcept { return vector_.m128_f32[0]; }
		/**
		 * @brief  3DベクトルのY成分の参照を取得する
		 * @return Y成分の値の参照
		 */
		[[nodiscard]] float& Y() noexcept { return vector_.m128_f32[1]; }
		/**
		 * @brief  3DベクトルのZ成分の参照を取得する
		 * @return Z成分の値の参照
		 */
		[[nodiscard]] float& Z() noexcept { return vector_.m128_f32[2]; }

		/**
		 * @brief  3Dベクトルを正規化する
		 */
		void Normalized() noexcept;

		/**
		 * @brief  3Dベクトルの長さを求める
		 * @return 3Dベクトルの長さ
		 */
		[[nodiscard]] float Length() const noexcept;
		/**
		 * @brief  3Dベクトルの長さの平方を求める
		 * @return 3Dベクトルの長さの平方
		 */
		[[nodiscard]] float LengthSquared() const noexcept;

		/**
		 * @param   v テストする3Dベクトル
		 * @param   bounds 境界を決める3Dベクトル
		 * @return  3Dベクトルの成分が設定した範囲内にあるか
		 * @note https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvector3inbounds
		 */
		static bool InBounds(const Vector3& v, const Vector3& bounds) noexcept;
		/**
		 * @brief  3Dベクトル間の距離を求める
		 * @param  v1 3Dベクトル
		 * @param  v2 3Dベクトル
		 * @return 3Dベクトル間の距離
		 */
		static float Distance(const Vector3& v1, const Vector3& v2) noexcept;
		/**
		 * @brief  3Dベクトル間の距離の平方を求める
		 * @param  v1 3Dベクトル
		 * @param  v2 3Dベクトル
		 * @return 3Dベクトル間の距離の平方
		 */
		static float DistanceSquared(const Vector3& v1, const Vector3& v2) noexcept;
		/**
		 * @brief  3Dベクトルの内積を求める
		 * @param  v1 3Dベクトル
		 * @param  v2 3Dベクトル
		 * @return 3Dベクトルの内積
		 */
		static float Dot(const Vector3& v1, const Vector3& v2) noexcept;
		/**
		 * @brief  3Dベクトルの外積を求める
		 * @param  v1 3Dベクトル
		 * @param  v2 3Dベクトル
		 * @return 3Dベクトルの外積
		 */
		static Vector3 Cross(const Vector3& v1, const Vector3& v2) noexcept;
		/**
		 * @brief  3Dベクトルを正規化する
		 * @param  v 3Dベクトル
		 * @return 正規化された3Dベクトル
		 */
		static Vector3 Normalize(const Vector3& v) noexcept;
		/**
		 * @brief  3Dベクトルの成分を指定した最小値と最大値の範囲にクランプする
		 * @param  v クランプされる成分を持つ3Dベクトル
		 * @param  min 要素の範囲の最小値
		 * @param  max 要素の範囲の最大値
		 * @return クランプされた3Dベクトル
		 */
		static Vector3 Clamp(const Vector3& v, const float min, const float max) noexcept;
		/**
		 * @brief  3Dベクトルの成分を指定した最小値と最大値の範囲にクランプする
		 * @param  v クランプされる成分を持つ3Dベクトル
		 * @param  vmin 最小範囲の成分を持つ3Dベクトル
		 * @param  vmax 最大範囲の成分を持つ3Dベクトル
		 * @return クランプされた3Dベクトル
		 */
		static Vector3 Clamp(const Vector3& v, const Vector3& vmin, const Vector3& vmax) noexcept;
		/**
		 * @brief  2つの3Dベクトルの成分ごとの比較を行い、最小の成分を持つ3Dベクトルを返す
		 * @param  v1 3Dベクトル
		 * @param  v2 3Dベクトル
		 * @return 最小の成分を持った3Dベクトル
		 */
		static Vector3 Min(const Vector3& v1, const Vector3& v2) noexcept;
		/**
		 * @brief  2つの3Dベクトルの成分ごとの比較を行い、最大の成分を持つ3Dベクトルを返す
		 * @param  v1 3Dベクトル
		 * @param  v2 3Dベクトル
		 * @return 最大の成分を持った3Dベクトル
		 */
		static Vector3 Max(const Vector3& v1, const Vector3& v2) noexcept;
		/**
		 * @brief  3Dベクトル間の線形補間を行う
		 * @param  v1 3D始点ベクトル
		 * @param  v2 3D終点ベクトル
		 * @param  t 補間制御係数
		 * @return 補完結果を表す3Dベクトル
		 */
		static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) noexcept;
		/**
		 * @brief  3Dベクトル間をエルミート多項式を用いて線形補間を行う
		 * @param  v1 3D始点ベクトル
		 * @param  v2 3D終点ベクトル
		 * @param  t 補間制御係数
		 * @return 補完結果を表す3Dベクトル
		 */
		static Vector3 SmoothStep(const Vector3& v1, const Vector3& v2, float t) noexcept;
		/**
		 * @brief  3つの3D位置ベクトルを偏心座標を用いて3D座標ベクトルを返す
		 * @param  v1 1番目の重心位置を表す3Dベクトル
		 * @param  v2 2番目の重心位置を表す3Dベクトル
		 * @param  v3 3番目の重心位置を表す3Dベクトル
		 * @param  f 重み付け係数
		 * @param  g 重み付け係数
		 * @return 3D座標ベクトル
		 * @note   https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorbarycentric
		 */
		static Vector3 BaryCentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g) noexcept;
		/**
		 * @brief  指定された3D位置ベクトルを用いてカットムルロムスプライン曲線補間を行う
		 * @param  v1 1つ目の制御点を表す3Dベクトル
		 * @param  v2 2つ目の制御点を表す3Dベクトル
		 * @param  v3 3つ目の制御点を表す3Dベクトル
		 * @param  v4 4つ目の制御点を表す3Dベクトル
		 * @param  t 補間制御係数
		 * @return 補完結果を表す3Dベクトル
		 * @note   https://docs.microsoft.com/ja-jp/windows/win32/api/directxmath/nf-directxmath-xmquaternionbarycentric
		 */
		static Vector3 CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t) noexcept;
		/**
		 * @brief  指定された3D位置ベクトルを用いてエルミート曲線補間を行う
		 * @param  v1 1つ目の制御点の位置を表す3Dベクトル
		 * @param  t1 1つ目の制御点の速度を表す3Dベクトル
		 * @param  v2 2つ目の制御点の位置を表す3Dベクトル
		 * @param  t2 2つ目の制御点の速度を表す3Dベクトル
		 * @param  t 補間制御係数
		 * @return 補完結果を表す3Dベクトル
		 * @note   https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorcatmullrom
		 */
		static Vector3 Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t) noexcept;
		/**
		 * @brief  3D入射ベクトルを3D法線ベクトルで反射させた3Dベクトルを求める
		 * @param  ivec 3D入射ベクトル
		 * @param  nvec 3D法線ベクトル
		 * @return 3D反射ベクトル
		 * @note   https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorhermite
		 */
		static Vector3 Reflect(const Vector3& ivec, const Vector3& nvec) noexcept;
		/**
		 * @brief  3D入射ベクトルを3D法線ベクトルで屈折させた3Dベクトルを求める
		 * @param  ivec 3D入射ベクトル
		 * @param  nvec 3D法線ベクトル
		 * @param  refractionIndex 屈折率
		 * @return 3D屈折ベクトル
		 */
		static Vector3 Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex) noexcept;
		/**
		 * @brief  クォータニオンを使用して3Dベクトルを回転する
		 * @param  v 回転する3Dベクトル
		 * @param  q 3Dベクトルに回転を適応するクォータニオン
		 * @return 回転した3Dベクトル
		 */
		static Vector3 Transform(const Vector3& v, const Quaternion& q) noexcept;
		/**
		 * @brief  変換行列を使用して3Dベクトルを変換する
		 * @param  v 変換する3Dベクトル
		 * @param  m 変換行列
		 * @return 変換された3Dベクトル
		 */
		static Vector3 Transform(const Vector3& v, const Matrix& m) noexcept;
		/**
		 * @brief  変換行列を使用して3D法線ベクトルを変換する
		 * @param  v 変換する3D法線ベクトル
		 * @param  m 変換行列
		 * @return 変換された3D法線ベクトル
		 */
		static Vector3 TransformNormal(const Vector3& v, const Matrix& m) noexcept;

		/**
		 * @brief  全ての成分が0である3Dベクトルを返す
		 * @return 全ての成分が0の3Dベクトル
		 */
		static constexpr Vector3 Zero() noexcept { return{  0.0f,  0.0f,  0.0f }; }
		/**
		 * @brief  全ての成分が1である3Dベクトルを返す
		 * @return 全ての成分が1の3Dベクトル
		 */
		static constexpr Vector3 One() noexcept { return{  1.0f,  1.0f,  1.0f }; }
		/**
		 * @brief  右向き(X+)の3D単位ベクトルを返す
		 * @return 右向きの3D単位ベクトル
		 */
		static constexpr Vector3 Right() noexcept { return{  1.0f,  0.0f,  0.0f }; }
		/**
		 * @brief  左向き(X-)の3D単位ベクトルを返す
		 * @return 左向きの3D単位ベクトル
		 */
		static constexpr Vector3 Left() noexcept { return{ -1.0f,  0.0f,  0.0f }; }
		/**
		 * @brief  上向き(Y+)の3D単位ベクトルを返す
		 * @return 上向きの3D単位ベクトル
		 */
		static constexpr Vector3 Up() noexcept { return{  0.0f,  1.0f,  0.0f }; }
		/**
		 * @brief  下向き(Y-)の3D単位ベクトルを返す
		 * @return 下向きの3D単位ベクトル
		 */
		static constexpr Vector3 Down() noexcept { return{  0.0f, -1.0f,  0.0f }; }
		/**
		 * @brief  前向き(Z+)の3D単位ベクトルを返す
		 * @return 前向きの3D単位ベクトル
		 */
		static constexpr Vector3 Forward() noexcept { return{  0.0f,  0.0f,  1.0f }; }
		/**
		 * @brief  後ろ向き(Z-)の3D単位ベクトルを返す
		 * @return 後ろ向きの3D単位ベクトル
		 */
		static constexpr Vector3 Backward() noexcept { return{  0.0f,  0.0f, -1.0f }; }

	private:
		DirectX::XMVECTOR vector_;
	};

	class Vector4 final
	{
	public:
		/**
		 * @brief  4Dベクトルの全ての成分を0で初期化する
		 */
		constexpr Vector4() noexcept
			: vector_{} {}
		/**
		 * @brief  X,Y,Z,W成分の値から4Dベクトルを構築する
		 * @param  x X成分の値
		 * @param  y Y成分の値
		 * @param  z Z成分の値
		 * @param  w W成分の値
		 */
		constexpr Vector4(const float x, const float y, const float z, const float w) noexcept
			: vector_{ x, y, z, w } {}
		/**
		 * @brief  配列から4Dベクトルを構築する
		 * @param  array サイズが4の配列
		 */
		explicit constexpr Vector4(_In_reads_(4) const float* array) noexcept
			: vector_{ array[0], array[1], array[2], array[3] } {}
		/**
		 * @brief  全ての要素が同じ値の4Dベクトルを構築する
		 * @param  f 要素の値
		 */
		explicit constexpr Vector4(const float f) noexcept
			: vector_{ f, f, f, f } {}
		/**
		 * @brief  XMVECTOR型から4Dベクトルを構築する
		 * @param  xmv XMVECTOR
		 */
		explicit constexpr Vector4(const DirectX::XMVECTOR& xmv) noexcept
			: vector_(xmv) {}
		/**
		 * @brief  XMFLOAT4型から4Dベクトルを構築する
		 * @param  xmf XMFLOAT
		 */
		explicit Vector4(const DirectX::XMFLOAT4& xmf) noexcept
			: vector_(DirectX::XMLoadFloat4(&xmf)) {}

		~Vector4() = default;

		// コピー＆ムーブ
		Vector4(const Vector4&) = default;
		Vector4& operator = (const Vector4&) = default;
		Vector4(Vector4&&) = default;
		Vector4& operator = (Vector4&&) = default;

		// 比較演算子
		bool operator == (const Vector4& rhs) const noexcept;
		bool operator != (const Vector4& rhs) const noexcept;

		// 単項演算子
		constexpr Vector4 operator + () const noexcept { return *this; }
		Vector4 operator - () const noexcept;

		// 二項演算子
		Vector4 operator + (const Vector4& rhs) const noexcept;
		Vector4 operator - (const Vector4& rhs) const noexcept;
		Vector4 operator * (float rhs) const noexcept;
		Vector4 operator / (float rhs) const noexcept;

		// 代入演算子
		Vector4& operator += (const Vector4& rhs) noexcept;
		Vector4& operator -= (const Vector4& rhs) noexcept;
		Vector4& operator *= (float rhs) noexcept;
		Vector4& operator /= (float rhs) noexcept;

		// 添字演算子
		const float& operator [] (std::size_t index) const noexcept;
		float& operator [] (std::size_t index) noexcept;

		// キャスト演算子
		operator DirectX::XMVECTOR() const noexcept;
		operator DirectX::XMFLOAT4() const noexcept;

		/**
		 * @brief  4Dベクトルを文字列に変換する
		 * @return 文字列化された4Dベクトル
		 */
		[[nodiscard]] std::string ToString() const noexcept
		{
			return std::format("({:6f}, {:6f}, {:6f}, {:6f})",
				DirectX::XMVectorGetX(vector_),
				DirectX::XMVectorGetY(vector_),
				DirectX::XMVectorGetZ(vector_),
				DirectX::XMVectorGetW(vector_));
		}

		/**
		 * @brief  4DベクトルのX成分の参照を取得する
		 * @return X成分の値の参照
		 */
		[[nodiscard]] float& X() noexcept { return vector_.m128_f32[0]; }
		/**
		 * @brief  4DベクトルのY成分の参照を取得する
		 * @return Y成分の値の参照
		 */
		[[nodiscard]] float& Y() noexcept { return vector_.m128_f32[1]; }
		/**
		 * @brief  4DベクトルのZ成分の参照を取得する
		 * @return Z成分の値の参照
		 */
		[[nodiscard]] float& Z() noexcept { return vector_.m128_f32[2]; }
		/**
		 * @brief  4DベクトルのW成分の参照を取得する
		 * @return W成分の値の参照
		 */
		[[nodiscard]] float& W() noexcept { return vector_.m128_f32[3]; }

		/**
		 * @brief  4Dベクトルを正規化する
		 */
		void Normalized() noexcept;

		/**
		 * @brief  4Dベクトルの長さを求める
		 * @return 4Dベクトルの長さ
		 */
		[[nodiscard]] float Length() const noexcept;
		/**
		 * @brief  4Dベクトルの長さの平方を求める
		 * @return 4Dベクトルの長さの平方
		 */
		[[nodiscard]] float LengthSquared() const noexcept;

		/**
		 * @param   v テストする4Dベクトル
		 * @param   bounds 境界を決める4Dベクトル
		 * @return  4Dベクトルの成分が設定した範囲内にあるか
		 * @note https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvector4inbounds
		 */
		static bool InBounds(const Vector4& v, const Vector4& bounds) noexcept;
		/**
		 * @brief  4Dベクトル間の距離を求める
		 * @param  v1 4Dベクトル
		 * @param  v2 4Dベクトル
		 * @return 4Dベクトル間の距離
		 */
		static float Distance(const Vector4& v1, const Vector4& v2) noexcept;
		/**
		 * @brief  4Dベクトル間の距離の平方を求める
		 * @param  v1 4Dベクトル
		 * @param  v2 4Dベクトル
		 * @return 4Dベクトル間の距離の平方
		 */
		static float DistanceSquared(const Vector4& v1, const Vector4& v2) noexcept;
		/**
		 * @brief  4Dベクトルの内積を求める
		 * @param  v1 4Dベクトル
		 * @param  v2 4Dベクトル
		 * @return 4Dベクトルの内積
		 */
		static float Dot(const Vector4& v1, const Vector4& v2) noexcept;
		/**
		 * @brief  4Dベクトルの外積を求める
		 * @param  v1 4Dベクトル
		 * @param  v2 4Dベクトル
		 * @param  v3 4Dベクトル
		 * @return 4Dベクトルの外積
		 */
		static Vector4 Cross(const Vector4& v1, const Vector4& v2, const Vector4& v3) noexcept;
		/**
		 * @brief  4Dベクトルを正規化する
		 * @param  v 4Dベクトル
		 * @return 正規化された4Dベクトル
		 */
		static Vector4 Normalize(const Vector4& v) noexcept;
		/**
		 * @brief  4Dベクトルの成分を指定した最小値と最大値の範囲にクランプする
		 * @param  v クランプされる成分を持つ4Dベクトル
		 * @param  min 要素の範囲の最小値
		 * @param  max 要素の範囲の最大値
		 * @return クランプされた4Dベクトル
		 */
		static Vector4 Clamp(const Vector4& v, const float min, const float max) noexcept;
		/**
		 * @brief  4Dベクトルの成分を指定した最小値と最大値の範囲にクランプする
		 * @param  v クランプされる成分を持つ4Dベクトル
		 * @param  vmin 最小範囲の成分を持つ4Dベクトル
		 * @param  vmax 最大範囲の成分を持つ4Dベクトル
		 * @return クランプされた4Dベクトル
		 */
		static Vector4 Clamp(const Vector4& v, const Vector4& vmin, const Vector4& vmax) noexcept;
		/**
		 * @brief  2つの4Dベクトルの成分ごとの比較を行い、最小の成分を持つ4Dベクトルを返す
		 * @param  v1 4Dベクトル
		 * @param  v2 4Dベクトル
		 * @return 最小の成分を持った4Dベクトル
		 */
		static Vector4 Min(const Vector4& v1, const Vector4& v2) noexcept;
		/**
		 * @brief  2つの4Dベクトルの成分ごとの比較を行い、最大の成分を持つ4Dベクトルを返す
		 * @param  v1 4Dベクトル
		 * @param  v2 4Dベクトル
		 * @return 最大の成分を持った4Dベクトル
		 */
		static Vector4 Max(const Vector4& v1, const Vector4& v2) noexcept;
		/**
		 * @brief  4Dベクトル間の線形補間を行う
		 * @param  v1 4D始点ベクトル
		 * @param  v2 4D終点ベクトル
		 * @param  t 補間制御係数
		 * @return 補完結果を表す4Dベクトル
		 */
		static Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t) noexcept;
		/**
		 * @brief  4Dベクトル間をエルミート多項式を用いて線形補間を行う
		 * @param  v1 4D始点ベクトル
		 * @param  v2 4D終点ベクトル
		 * @param  t 補間制御係数
		 * @return 補完結果を表す4Dベクトル
		 */
		static Vector4 SmoothStep(const Vector4& v1, const Vector4& v2, float t) noexcept;
		/**
		 * @brief  3つの4D位置ベクトルを偏心座標を用いて4D座標ベクトルを返す
		 * @param  v1 1番目の重心位置を表す4Dベクトル
		 * @param  v2 2番目の重心位置を表す4Dベクトル
		 * @param  v3 3番目の重心位置を表す4Dベクトル
		 * @param  f 重み付け係数
		 * @param  g 重み付け係数
		 * @return 4D座標ベクトル
		 * @note   https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorbarycentric
		 */
		static Vector4 BaryCentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g) noexcept;
		/**
		 * @brief  指定された4D位置ベクトルを用いてカットムルロムスプライン曲線補間を行う
		 * @param  v1 1つ目の制御点を表す4Dベクトル
		 * @param  v2 2つ目の制御点を表す4Dベクトル
		 * @param  v3 3つ目の制御点を表す4Dベクトル
		 * @param  v4 4つ目の制御点を表す4Dベクトル
		 * @param  t 補間制御係数
		 * @return 補完結果を表す4Dベクトル
		 * @note   https://docs.microsoft.com/ja-jp/windows/win32/api/directxmath/nf-directxmath-xmquaternionbarycentric
		 */
		static Vector4 CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t) noexcept;
		/**
		 * @brief  指定された4D位置ベクトルを用いてエルミート曲線補間を行う
		 * @param  v1 1つ目の制御点の位置を表す4Dベクトル
		 * @param  t1 1つ目の制御点の速度を表す4Dベクトル
		 * @param  v2 2つ目の制御点の位置を表す4Dベクトル
		 * @param  t2 2つ目の制御点の速度を表す4Dベクトル
		 * @param  t 補間制御係数
		 * @return 補完結果を表す4Dベクトル
		 * @note   https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorcatmullrom
		 */
		static Vector4 Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t) noexcept;
		/**
		 * @brief  4D入射ベクトルを4D法線ベクトルで反射させた4Dベクトルを求める
		 * @param  ivec 4D入射ベクトル
		 * @param  nvec 4D法線ベクトル
		 * @return4D 反射ベクトル
		 * @note   https://docs.microsoft.com/en-us/windows/win32/api/directxmath/nf-directxmath-xmvectorhermite
		 */
		static Vector4 Reflect(const Vector4& ivec, const Vector4& nvec) noexcept;
		/**
		 * @brief  4D入射ベクトルを4D法線ベクトルで屈折させた4Dベクトルを求める
		 * @param  ivec 4D入射ベクトル
		 * @param  nvec 4D法線ベクトル
		 * @param  refractionIndex 屈折率
		 * @return 4D屈折ベクトル
		 */
		static Vector4 Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex) noexcept;
		/**
		 * @brief  クォータニオンを使用して4Dベクトルを回転する
		 * @param  v 回転する4Dベクトル
		 * @param  q 4Dベクトルに回転を適応するクォータニオン
		 * @return 回転した4Dベクトル
		 */
		static Vector4 Transform(const Vector2& v, const Quaternion& q) noexcept;
		/**
		 * @brief  クォータニオンを使用して2Dベクトルを回転する
		 * @param  v 回転する2Dベクトル
		 * @param  q 2Dベクトルに回転を適応するクォータニオン
		 * @return 回転した4Dベクトル
		 */
		static Vector4 Transform(const Vector3& v, const Quaternion& q) noexcept;
		/**
		 * @brief  クォータニオンを使用して3Dベクトルを回転する
		 * @param  v 回転する3Dベクトル
		 * @param  q 3Dベクトルに回転を適応するクォータニオン
		 * @return 回転した4Dベクトル
		 */
		static Vector4 Transform(const Vector4& v, const Quaternion& q) noexcept;
		/**
		 * @brief  変換行列を使用して4Dベクトルを変換する
		 * @param  v 変換する4Dベクトル
		 * @param  m 変換行列
		 * @return 変換された4Dベクトル
		 */
		static Vector4 Transform(const Vector4& v, const Matrix& m) noexcept;

		/**
		 * @brief  全ての成分が0である4Dベクトルを返す
		 * @return 全ての成分が0の4Dベクトル
		 */
		static constexpr Vector4 Zero() noexcept { return{ 0.0f, 0.0f, 0.0f, 0.0f }; }
		/**
		 * @brief  全ての成分が1である4Dベクトルを返す
		 * @return 全ての成分が1の4Dベクトル
		 */
		static constexpr Vector4 One() noexcept { return{ 1.0f, 1.0f, 1.0f, 1.0f }; }
		/**
		 * @brief  X成分が1の4D単位ベクトルを返す
		 * @return X成分が1の4D単位ベクトル
		 */
		static constexpr Vector4 UnitX() noexcept { return{ 1.0f, 0.0f, 0.0f, 0.0f }; }
		/**
		 * @brief  Y成分が1の4D単位ベクトルを返す
		 * @return Y成分が1の4D単位ベクトル
		 */
		static constexpr Vector4 UnitY() noexcept { return{ 0.0f, 1.0f, 0.0f, 0.0f }; }
		/**
		 * @brief  Z成分が1の4D単位ベクトルを返す
		 * @return Z成分が1の4D単位ベクトル
		 */
		static constexpr Vector4 UnitZ() noexcept { return{ 0.0f, 0.0f, 1.0f, 0.0f }; }
		/**
		 * @brief  W成分だけが1の4D単位ベクトルを返す
		 * @return W成分が1の4D単位ベクトル
		 */
		static constexpr Vector4 UnitW() noexcept { return{ 0.0f, 0.0f, 0.0f, 1.0f }; }

	private:
		DirectX::XMVECTOR vector_;
	};

	/**
	 * @brief  二次元ベクトルクラス(64Byte)
	 */
	class Matrix final
	{
	public:
		/**
		 * @brief  単位行列を構築する
		 */
		constexpr Matrix() noexcept;
		/**
		 * @brief  値から行列を構築する
		 * @param  m00 (0, 0)要素の値
		 * @param  m01 (0, 1)要素の値
		 * @param  m02 (0, 2)要素の値
		 * @param  m03 (0, 3)要素の値
		 * @param  m10 (1, 0)要素の値
		 * @param  m11 (1, 1)要素の値
		 * @param  m12 (1, 2)要素の値
		 * @param  m13 (1, 3)要素の値
		 * @param  m20 (2, 0)要素の値
		 * @param  m21 (2, 1)要素の値
		 * @param  m22 (2, 2)要素の値
		 * @param  m23 (2, 3)要素の値
		 * @param  m30 (3, 0)要素の値
		 * @param  m31 (3, 1)要素の値
		 * @param  m32 (3, 2)要素の値
		 * @param  m33 (3, 3)要素の値
		 */
		constexpr Matrix(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33) noexcept;
		/**
		 * @brief  配列から行列を構築する
		 * @param  array サイズが16の配列
		 */
		explicit Matrix(_In_reads_(16) const float* array) noexcept
			: matrix_(array) {}
		/**
		 * @brief  XMMATRIX型から行列を構築する
		 * @param  xmm XMMATRIX
		 */
		explicit constexpr Matrix(const DirectX::XMMATRIX& xmm) noexcept
			: matrix_(xmm) {}
		/**
		 * @brief  4つのXMVECTOR型から行列を構築する
		 * @param  r0 1列目
		 * @param  r1 2列目
		 * @param  r2 3列目
		 * @param  r3 4列目
		 */
		constexpr Matrix(
			const DirectX::XMVECTOR& r0,
			const DirectX::XMVECTOR& r1,
			const DirectX::XMVECTOR& r2,
			const DirectX::XMVECTOR& r3) noexcept;
		/**
		 * @brief  4つの4Dベクトルから行列を構築する
		 * @param  r0 1列目
		 * @param  r1 2列目
		 * @param  r2 3列目
		 * @param  r3 4列目
		 */
		constexpr Matrix(
			const Vector4& r0,
			const Vector4& r1,
			const Vector4& r2,
			const Vector4& r3) noexcept;

		~Matrix() = default;

		// コピー&ムーブ
		Matrix(const Matrix&) = default;
		Matrix& operator=(const Matrix&) = default;
		Matrix(Matrix&&) = default;
		Matrix& operator=(Matrix&&) = default;

		// 比較演算子
		bool operator == (const Matrix& rhs) const noexcept;
		bool operator != (const Matrix& rhs) const noexcept;

		// 二項演算子
		Matrix operator* (const Matrix& rhs) const noexcept;

		// 代入演算子
		Matrix& operator*= (const Matrix& rhs) noexcept;

		// 添字演算子
		const Vector4& operator [] (std::size_t index) const noexcept;
		Vector4& operator [] (std::size_t index) noexcept;

		// キャスト演算子
		operator DirectX::XMMATRIX() const noexcept;
		operator DirectX::XMFLOAT4X4() const noexcept;

		/**
		 * @brief  行列の右方向を指す単位ベクトルを求める
		 * @return 右方向ベクトル
		 */
		[[nodiscard]] Vector3 Right()    const noexcept;
		/**
		 * @brief  行列の左方向を指す単位ベクトルを求める
		 * @return 左方向ベクトル
		 */
		[[nodiscard]] Vector3 Left()     const noexcept;
		/**
		 * @brief  行列の上方向を指す単位ベクトルを求める
		 * @return 上方向ベクトル
		 */
		[[nodiscard]] Vector3 Up()       const noexcept;
		/**
		 * @brief  行列の下方向を指す単位ベクトルを求める
		 * @return 下方向ベクトル
		 */
		[[nodiscard]] Vector3 Down()     const noexcept;
		/**
		 * @brief  行列の前方向を指す単位ベクトルを求める
		 * @return 前方向ベクトル
		 */
		[[nodiscard]] Vector3 Forward()  const noexcept;
		/**
		 * @brief  行列の後ろ方向を指す単位ベクトルを求める
		 * @return 後ろ方向ベクトル
		 */
		[[nodiscard]] Vector3 Backward() const noexcept;

		/**
		 * @brief  行列式を計算する
		 * @return 計算結果
		 */
		[[nodiscard]] float Determinant() const noexcept;

		/**
		 * @brief  ビルボード行列を求める
		 * @param  object ビルボードの回転の中心座標を表す3Dベクトル
		 * @param  cameraPosition カメラの位置を表す3Dベクトル
		 * @param  cameraUp カメラの上方向を表す3Dベクトル
		 * @param  cameraForward カメラの前方向を表す3Dベクトル
		 * @return ビルボード行列
		 */
		static Matrix Billboard(
			const Vector3& object, 
			const Vector3& cameraPosition,
			const Vector3& cameraUp, 
			const Vector3* cameraForward = nullptr) noexcept;
		/**
		 * @brief  指定した軸を中心に回転する円筒ビルボード行列を求める
		 * @param  object ビルボードの回転の中心座標を表す3Dベクトル
		 * @param  cameraPosition カメラの位置を表す3Dベクトル
		 * @param  rotateAxis ビルボードの回転軸を表す3Dベクトル
		 * @param  cameraForward カメラの前方向を表す3Dベクトル
		 * @param  objectForward オブジェクトの前方向を表す3Dベクトル
		 * @return ビルボード行列
		 */
		static Matrix CreateConstrainedBillboard(
			const Vector3& object,
			const Vector3& cameraPosition, 
			const Vector3& rotateAxis,
			const Vector3* cameraForward = nullptr, 
			const Vector3* objectForward = nullptr) noexcept;
		/**
		 * @brief  転置行列を求める
		 * @param  m 行列
		 * @return 転置行列
		 */
		static Matrix Transpose(const Matrix& m) noexcept;
		/**
		 * @brief  逆行列を求める
		 * @param  m 行列
		 * @return 逆行列
		 */
		static Matrix Inverse(const Matrix& m) noexcept;
		/**
		 * @brief  3Dベクトルから平行移動行列を作成する
		 * @param  position 平行移動を表す3Dベクトル
		 * @return 平行移動行列
		 */
		static Matrix Translation(const Vector3& position) noexcept;
		/**
		 * @brief  X,Y,Z軸の移動量から平行移動行列を作成する
		 * @param x X軸方向の移動量
		 * @param y Y軸方向の移動量
		 * @param z Z軸方向の移動量
		 * @return 平行移動行列
		 */
		static Matrix Translation(float x, float y, float z) noexcept;
		/**
		 * @brief  3Dベクトルから拡大縮小行列を作成する
		 * @param  scales 拡大縮小を表す3Dベクトル
		 * @return 拡大縮小行列
		 */
		static Matrix Scale(const Vector3& scales) noexcept;
		/**
		 * @brief  X,Y,Z軸の拡大率から拡大縮小行列を作成する
		 * @param  xs X軸の拡大率
		 * @param  ys Y軸の拡大率
		 * @param  zs Z軸の拡大率
		 * @return 拡大縮小行列
		 */
		static Matrix Scale(float xs, float ys, float zs) noexcept;
		/**
		 * @brief  拡大率から拡大縮小行列を作成する
		 * @param  scale 拡大率
		 * @return 拡大縮小行列
		 */
		static Matrix Scale(float scale) noexcept;
		/**
		 * @brief  X軸で回転させた回転行列を作成する
		 * @param  rotation X軸のラジアン角度
		 * @return 回転行列
		 */
		static Matrix RotationX(float rotation) noexcept;
		/**
		 * @brief  Y軸で回転させた回転行列を作成する
		 * @param  rotation Y軸のラジアン角度
		 * @return 回転行列
		 */
		static Matrix RotationY(float rotation) noexcept;
		/**
		 * @brief  Z軸で回転させた回転行列を作成する
		 * @param  rotation Z軸のラジアン角度
		 * @return 回転行列
		 */
		static Matrix RotationZ(float rotation) noexcept;
		/**
		 * @brief  3Dベクトルを軸に回転させた回転行列を作成する
		 * @param  axis 回転軸を表す3Dベクトル
		 * @param  radians 軸のラジアン角度
		 * @return 回転行列
		 */
		static Matrix RotationAxisAngle(const Vector3& axis, float radians) noexcept;
		static Matrix RotationFromQuaternion(const Quaternion& q) noexcept;
		static Matrix RotationYawPitchRoll(float yaw, float pitch, float roll) noexcept;
		/**
		 * @brief  視野角と縦横比、ニアおよびファービュー平面距離に基づき透視投影行列を作成する
		 * @param  fov 視野角
		 * @param  aspectRatio 縦横比
		 * @param  nearPlane ニアビュー平面距離
		 * @param  farPlane ファービュー平面距離
		 * @return 透視投影行列
		 */
		static Matrix PerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;
		/**
		 * @brief  
		 * @param  width 
		 * @param  height 
		 * @param  nearPlane 
		 * @param  farPlane 
		 * @return 
		 */
		static Matrix Perspective(float width, float height, float nearPlane, float farPlane) noexcept;
		static Matrix PerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane) noexcept;
		static Matrix Orthographic(float width, float height, float zNearPlane, float zFarPlane) noexcept;
		static Matrix OrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane) noexcept;
		static Matrix LookAt(const Vector3& position, const Vector3& target, const Vector3& up) noexcept;
		static Matrix World(const Vector3& position, const Vector3& forward, const Vector3& up) noexcept;
		static Matrix Lerp(const Matrix& m1, const Matrix& m2, float t) noexcept;
		static Matrix Transform(const Matrix& m, const Quaternion& rotation) noexcept;

		static constexpr Matrix Identity() noexcept;

	private:
		DirectX::XMMATRIX matrix_;
	};

	class Quaternion final
	{
	public:
		constexpr Quaternion() noexcept
			: quaternion_{ 0.0f, 0.0f, 0.0f, 1.0f } {}
		constexpr Quaternion(const float x, const float y, const float z, const float w) noexcept
			: quaternion_{ x, y, z, w } {}
		explicit constexpr Quaternion(_In_reads_(4) const float* array) noexcept
			: quaternion_{ array[0], array[1], array[2], array[3] } {}
		explicit constexpr Quaternion(const DirectX::XMVECTOR& v) noexcept
			: quaternion_(v) {}
		explicit Quaternion(const Vector4& v) noexcept
			: quaternion_{ v[0], v[1], v[2], v[3] } {}
		Quaternion(const Vector3& v, const float angle) noexcept
			: quaternion_{ v[0], v[1], v[2], angle } {}
		~Quaternion() = default;

		Quaternion(const Quaternion&) = default;
		Quaternion& operator = (const Quaternion&) = default;
		Quaternion(Quaternion&&) = default;
		Quaternion& operator = (Quaternion&&) = default;

		bool operator ==(const Quaternion& rhs) const noexcept;
		bool operator != (const Quaternion& rhs) const noexcept;

		Quaternion operator + (const Quaternion& rhs) const noexcept;
		Quaternion operator - (const Quaternion& rhs) const noexcept;
		Quaternion operator * (const Quaternion& rhs) const noexcept;
		Quaternion operator * (float rhs) const noexcept;

		Quaternion& operator += (const Quaternion& rhs) noexcept;
		Quaternion& operator -= (const Quaternion& rhs) noexcept;
		Quaternion& operator *= (const Quaternion& rhs) noexcept;
		Quaternion& operator *= (float rhs) noexcept;

		operator DirectX::XMVECTOR() const noexcept;

		void Normalized() noexcept;

		[[nodiscard]] float Length() const noexcept;
		[[nodiscard]] float LengthSquared() const noexcept;

		/**
		 * @brief  クォータニオンの軸の前方向を指す単位ベクトルを求める
		 * @return 前方向ベクトル
		 */
		[[nodiscard]] Vector3 Forward() const noexcept;
		/**
		 * @brief  クォータニオンの軸の上方向を指す単位ベクトルを求める
		 * @return 前方向ベクトル
		 */
		[[nodiscard]] Vector3 Up() const noexcept;
		/**
		 * @brief  クォータニオンの軸の右方向を指す単位ベクトルを求める
		 * @return 前方向ベクトル
		 */
		[[nodiscard]] Vector3 Right() const noexcept;

		static float Dot(const Quaternion& q1, const Quaternion& q2) noexcept;
		static Quaternion Normalize(const Quaternion& q) noexcept;
		static Quaternion Conjugate(const Quaternion& q) noexcept;
		static Quaternion Inverse(const Quaternion& q) noexcept;
		static Quaternion RotationX(float rotation) noexcept;
		static Quaternion RotationY(float rotation) noexcept;
		static Quaternion RotationZ(float rotation) noexcept;
		static Quaternion AxisAngle(const Vector3& axis, float angle) noexcept;
		static Quaternion YawPitchRoll(float yaw, float pitch, float roll) noexcept;
		static Quaternion FromRotationMatrix(const Matrix& m) noexcept;
		static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept;
		static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept;
		static Quaternion Concatenate(const Quaternion& q1, const Quaternion& q2) noexcept;

		static constexpr Quaternion Identity() noexcept { return { 0.0f, 0.0f, 0.0f, 1.0f }; }

	private:
		DirectX::XMVECTOR quaternion_;
	};

	class Color final
	{
	public:
		constexpr Color() noexcept
			: color_{ 0.0f, 0.0f ,0.0f ,1.0f } {}
		constexpr Color(const float r, const float g, const float b, const float a = 1.0f) noexcept
			: color_{ r, g, b, a } {}
		explicit constexpr Color(_In_reads_(4) const float* array) noexcept
			: color_{ array[0], array[1], array[2], array[3] } {}
		explicit Color(const DirectX::XMVECTOR& xmv) noexcept
			: color_(xmv) {}
		explicit Color(const DirectX::XMFLOAT4& xmf) noexcept
			: color_(DirectX::XMLoadFloat4(&xmf)) {}
		~Color() = default;

		Color(const Color&) = default;
		Color& operator=(const Color&) = default;
		Color(Color&&) = default;
		Color& operator=(Color&&) = default;

		bool operator == (const Color& rhs) const noexcept;
		bool operator != (const Color& rhs) const noexcept;

		constexpr Color operator + () const noexcept { return *this; }
		Color operator - () const noexcept;

		Color operator + (const Color& rhs) const noexcept;
		Color operator - (const Color& rhs) const noexcept;
		Color operator * (float rhs) const noexcept;
		Color operator / (float rhs) const noexcept;

		Color& operator += (const Color& rhs) noexcept;
		Color& operator -= (const Color& rhs) noexcept;
		Color& operator *= (float rhs) noexcept;
		Color& operator /= (float rhs) noexcept;
		
		const float& operator [] (std::size_t index) const noexcept;
		float& operator [] (std::size_t index) noexcept;
		
		operator DirectX::XMVECTOR() const noexcept;
		operator DirectX::XMFLOAT4() const noexcept;

		[[nodiscard]] float& R() noexcept { return color_.m128_f32[0]; }
		[[nodiscard]] float& G() noexcept { return color_.m128_f32[1]; }
		[[nodiscard]] float& B() noexcept { return color_.m128_f32[2]; }
		[[nodiscard]] float& A() noexcept { return color_.m128_f32[3]; }

		static Color Negate(const Color& c) noexcept;
		static Color Saturate(const Color& c) noexcept;
		static Color Premultiply(const Color& c) noexcept;
		static Color AdjustSaturation(const Color& c, float sat) noexcept;
		static Color AdjustContrast(const Color& c, float contrast) noexcept;
		static Color Modulate(const Color& c1, const Color& c2) noexcept;
		static Color Lerp(const Color& c1, const Color& c2, float t) noexcept;
		static Color FromHSL(const float h, const float s, const float l, const float a = 1.0f) noexcept;
		static Color FromHSV(const float h, const float s, const float v, const float a = 1.0f) noexcept;

	private:
		DirectX::XMVECTOR color_;
	};
}

#include "core.inl"
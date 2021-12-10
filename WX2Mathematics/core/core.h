/*********************************************************************
 * @file   core.h
 * @author Tomomi Murakami
 * @date   2021/10/13 0:59
 * @brief  スカラー、ベクトル、マトリックス演算
 * @brief  参考: https://github.com/microsoft/DirectXTK
 * @brief  ベクトルのサイズは全て4バイトなので注意
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
	 * @brief  二次元ベクトルクラス(4Byte)
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
		 * @brief  X,Y座標から2Dベクトルを構築する
		 * @param  x X座標の値
		 * @param  y Y座標の値
		 */
		constexpr Vector2(const float x, const float y) noexcept
			: vector_{ x, y } {}
		/**
		 * @brief  配列から2Dベクトルを構築する
		 * @param  array 要素の値がセットされた配列ポインタ
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
		 * @brief  2DベクトルのX成分を取得する
		 * @return X成分の値
		 */
		[[nodiscard]] float X() const noexcept { return DirectX::XMVectorGetX(vector_); }
		/**
		 * @brief  2DベクトルのY成分を取得する
		 * @return Y成分の値
		 */
		[[nodiscard]] float Y() const noexcept { return DirectX::XMVectorGetY(vector_); }

		/**
		 * @brief  2DベクトルのX成分を設定する
		 * @param  x 書き換えるX成分の値
		 */
		void X(const float x) noexcept { vector_ = DirectX::XMVectorSetX(vector_, x); }
		/**
		 * @brief  2DベクトルのY成分を設定する
		 * @param  y 書き換えるY成分の値
		 */
		void Y(const float y) noexcept { vector_ = DirectX::XMVectorSetY(vector_, y); }

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
		 * @return ベクトル間の距離
		 */
		static float Distance(const Vector2& v1, const Vector2& v2) noexcept;

		/**
		 * @brief  2Dベクトル間の距離の平方を求める
		 * @param  v1 2Dベクトル
		 * @param  v2 2Dベクトル
		 * @return ベクトル間の距離の平方
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
		 * @brief  3つの2D位置ベクトルを用いて2D偏差重心座標ベクトルを返す
		 * @param  v1 1番目の重心位置を表す2Dベクトル
		 * @param  v2 2番目の重心位置を表す2Dベクトル
		 * @param  v3 3番目の重心位置を表す2Dベクトル
		 * @param  f 重み付け係数
		 * @param  g 重み付け係数
		 * @return 2D偏差重心座標ベクトル
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

	class Vector3 final
	{
	public:
		constexpr Vector3() noexcept
			: vector_{} {}
		constexpr Vector3(const float x, const float y, const float z) noexcept
			: vector_{ x, y, z } {}
		explicit constexpr Vector3(_In_reads_(3) const float* array) noexcept
			: vector_{ array[0], array[1], array[2] } {}
		explicit constexpr Vector3(const float f) noexcept
			: vector_{ f, f, f } {}
		explicit constexpr Vector3(const DirectX::XMVECTOR& xmv) noexcept
			: vector_{ xmv.m128_f32[0], xmv.m128_f32[1], xmv.m128_f32[2] } {}
		~Vector3() = default;

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

		const float& operator [] (std::size_t index) const noexcept;
		float& operator [] (std::size_t index) noexcept;

		operator DirectX::XMVECTOR() const noexcept;

		[[nodiscard]] float X() const noexcept { return DirectX::XMVectorGetX(vector_); }
		[[nodiscard]] float Y() const noexcept { return DirectX::XMVectorGetY(vector_); }
		[[nodiscard]] float Z() const noexcept { return DirectX::XMVectorGetZ(vector_); }

		void X(const float x) noexcept { vector_ = DirectX::XMVectorSetX(vector_, x); }
		void Y(const float y) noexcept { vector_ = DirectX::XMVectorSetY(vector_, y); }
		void Z(const float z) noexcept { vector_ = DirectX::XMVectorSetZ(vector_, z); }

		void Normalized() noexcept;

		[[nodiscard]] float Length() const noexcept;
		[[nodiscard]] float LengthSquared() const noexcept;

		static bool InBounds(const Vector3& v, const Vector3& bounds) noexcept;
		static float Distance(const Vector3& v1, const Vector3& v2) noexcept;
		static float DistanceSquared(const Vector3& v1, const Vector3& v2) noexcept;
		static float Dot(const Vector3& v1, const Vector3& v2) noexcept;
		static Vector3 Cross(const Vector3& v1, const Vector3& v2) noexcept;
		static Vector3 Normalize(const Vector3& v) noexcept;
		static Vector3 Clamp(const Vector3& v, const Vector3& vmin, const Vector3& vmax) noexcept;
		static Vector3 Min(const Vector3& v1, const Vector3& v2) noexcept;
		static Vector3 Max(const Vector3& v1, const Vector3& v2) noexcept;
		static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) noexcept;
		static Vector3 SmoothStep(const Vector3& v1, const Vector3& v2, float t) noexcept;
		static Vector3 BaryCentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g) noexcept;
		static Vector3 CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t) noexcept;
		static Vector3 Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t) noexcept;
		static Vector3 Reflect(const Vector3& ivec, const Vector3& nvec) noexcept;
		static Vector3 Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex) noexcept;
		static Vector3 Transform(const Vector3& v, const Quaternion& q) noexcept;
		static Vector3 Transform(const Vector3& v, const Matrix& m) noexcept;
		static Vector3 TransformNormal(const Vector3& v, const Matrix& m) noexcept;

		static constexpr Vector3 Zero()		noexcept { return{  0.0f,  0.0f,  0.0f }; }
		static constexpr Vector3 One()		noexcept { return{  1.0f,  1.0f,  1.0f }; }
		static constexpr Vector3 Right()	noexcept { return{  1.0f,  0.0f,  0.0f }; }
		static constexpr Vector3 Left()		noexcept { return{ -1.0f,  0.0f,  0.0f }; }
		static constexpr Vector3 Up()		noexcept { return{  0.0f,  1.0f,  0.0f }; }
		static constexpr Vector3 Down()		noexcept { return{  0.0f, -1.0f,  0.0f }; }
		static constexpr Vector3 Forward()	noexcept { return{  0.0f,  0.0f,  1.0f }; }
		static constexpr Vector3 Backward()	noexcept { return{  0.0f,  0.0f, -1.0f }; }

	private:
		DirectX::XMVECTOR vector_;
	};

	class Vector4 final
	{
	public:
		constexpr Vector4() noexcept
			: vector_{} {}
		constexpr Vector4(const float x, const float y, const float z, const float w) noexcept
			: vector_{ x, y, z, w } {}
		explicit constexpr Vector4(_In_reads_(4) const float* array) noexcept
			: vector_{ array[0], array[1], array[2], array[3] } {}
		explicit constexpr Vector4(const float f) noexcept
			: vector_{ f, f, f, f } {}
		explicit constexpr Vector4(const DirectX::XMVECTOR& xmv) noexcept
			: vector_(xmv) {}
		~Vector4() = default;

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

		const float& operator [] (std::size_t index) const noexcept;
		float& operator [] (std::size_t index) noexcept;

		operator DirectX::XMVECTOR() const noexcept;

		[[nodiscard]] float X() const noexcept { return DirectX::XMVectorGetX(vector_); }
		[[nodiscard]] float Y() const noexcept { return DirectX::XMVectorGetY(vector_); }
		[[nodiscard]] float Z() const noexcept { return DirectX::XMVectorGetZ(vector_); }
		[[nodiscard]] float W() const noexcept { return DirectX::XMVectorGetW(vector_); }

		void X(const float x) noexcept { vector_ = DirectX::XMVectorSetX(vector_, x); }
		void Y(const float y) noexcept { vector_ = DirectX::XMVectorSetY(vector_, y); }
		void Z(const float z) noexcept { vector_ = DirectX::XMVectorSetZ(vector_, z); }
		void W(const float w) noexcept { vector_ = DirectX::XMVectorSetW(vector_, w); }

		void Normalized() noexcept;

		[[nodiscard]] float Length() const noexcept;
		[[nodiscard]] float LengthSquared() const noexcept;

		static bool InBounds(const Vector4& v, const Vector4& bounds) noexcept;
		static float Distance(const Vector4& v1, const Vector4& v2) noexcept;
		static float DistanceSquared(const Vector4& v1, const Vector4& v2) noexcept;
		static float Dot(const Vector4& v1, const Vector4& v2) noexcept;
		static Vector4 Cross(const Vector4& v1, const Vector4& v2, const Vector4& v3) noexcept;
		static Vector4 Normalize(const Vector4& v) noexcept;
		static Vector4 Clamp(const Vector4& v, const Vector4& vmin, const Vector4& vmax) noexcept;
		static Vector4 Min(const Vector4& v1, const Vector4& v2) noexcept;
		static Vector4 Max(const Vector4& v1, const Vector4& v2) noexcept;
		static Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t) noexcept;
		static Vector4 SmoothStep(const Vector4& v1, const Vector4& v2, float t) noexcept;
		static Vector4 BaryCentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g) noexcept;
		static Vector4 CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t) noexcept;
		static Vector4 Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t) noexcept;
		static Vector4 Reflect(const Vector4& ivec, const Vector4& nvec) noexcept;
		static Vector4 Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex) noexcept;
		static Vector4 Transform(const Vector2& v, const Quaternion& q) noexcept;
		static Vector4 Transform(const Vector3& v, const Quaternion& q) noexcept;
		static Vector4 Transform(const Vector4& v, const Quaternion& q) noexcept;
		static Vector4 Transform(const Vector4& v, const Matrix& m) noexcept;

		static constexpr Vector4 Zero()		noexcept { return{ 0.0f, 0.0f, 0.0f, 0.0f }; }
		static constexpr Vector4 One()		noexcept { return{ 1.0f, 1.0f, 1.0f, 1.0f }; }
		static constexpr Vector4 UnitX()	noexcept { return{ 1.0f, 0.0f, 0.0f, 0.0f }; }
		static constexpr Vector4 UnitY()	noexcept { return{ 0.0f, 1.0f, 0.0f, 0.0f }; }
		static constexpr Vector4 UnitZ()	noexcept { return{ 0.0f, 0.0f, 1.0f, 0.0f }; }
		static constexpr Vector4 UnitW()	noexcept { return{ 0.0f, 0.0f, 0.0f, 1.0f }; }

	private:
		DirectX::XMVECTOR vector_;
	};

	class Matrix final
	{
	public:
		constexpr Matrix() noexcept;
		explicit constexpr Matrix(const DirectX::XMMATRIX& xmm) noexcept
			: matrix_(xmm) {}
		constexpr Matrix(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33) noexcept;
		explicit Matrix(_In_reads_(16) const float* array) noexcept
			: matrix_(array) {}
		constexpr Matrix(
			const DirectX::XMVECTOR& r0,
			const DirectX::XMVECTOR& r1,
			const DirectX::XMVECTOR& r2,
			const DirectX::XMVECTOR& r3) noexcept;
		constexpr Matrix(
			const Vector4& r0,
			const Vector4& r1,
			const Vector4& r2,
			const Vector4& r3) noexcept;
		~Matrix() = default;

		Matrix(const Matrix&) = default;
		Matrix& operator=(const Matrix&) = default;
		Matrix(Matrix&&) = default;
		Matrix& operator=(Matrix&&) = default;

		bool operator == (const Matrix& rhs) const noexcept;
		bool operator != (const Matrix& rhs) const noexcept;

		Matrix operator* (const Matrix& rhs) const noexcept;

		Matrix& operator*= (const Matrix& rhs) noexcept;

		const Vector4& operator [] (std::size_t index) const noexcept;
		Vector4& operator [] (std::size_t index) noexcept;

		operator DirectX::XMMATRIX() const noexcept;

		[[nodiscard]] Vector3 Right()    const noexcept;
		[[nodiscard]] Vector3 Left()     const noexcept;
		[[nodiscard]] Vector3 Up()       const noexcept;
		[[nodiscard]] Vector3 Down()     const noexcept;
		[[nodiscard]] Vector3 Forward()  const noexcept;
		[[nodiscard]] Vector3 Backward() const noexcept;

		[[nodiscard]] float Determinant() const noexcept;

		static Matrix Billboard(
			const Vector3& object, 
			const Vector3& cameraPosition,
			const Vector3& cameraUp, 
			const Vector3* cameraForward = nullptr) noexcept;
		static Matrix CreateConstrainedBillboard(
			const Vector3& object,
			const Vector3& cameraPosition, 
			const Vector3& rotateAxis,
			const Vector3* cameraForward = nullptr, 
			const Vector3* objectForward = nullptr) noexcept;
		static Matrix Transpose(const Matrix& m) noexcept;
		static Matrix Inverse(const Matrix& m) noexcept;
		static Matrix Translation(const Vector3& position) noexcept;
		static Matrix Translation(float x, float y, float z) noexcept;
		static Matrix Scale(const Vector3& scales) noexcept;
		static Matrix Scale(float xs, float ys, float zs) noexcept;
		static Matrix Scale(float scale) noexcept;
		static Matrix RotationX(float rotation) noexcept;
		static Matrix RotationY(float rotation) noexcept;
		static Matrix RotationZ(float rotation) noexcept;
		static Matrix FromAxisAngle(const Vector3& axis, float radians) noexcept;
		static Matrix PerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;
		static Matrix Perspective(float width, float height, float nearPlane, float farPlane) noexcept;
		static Matrix PerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane) noexcept;
		static Matrix Orthographic(float width, float height, float zNearPlane, float zFarPlane) noexcept;
		static Matrix OrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane) noexcept;
		static Matrix LookAt(const Vector3& position, const Vector3& target, const Vector3& up) noexcept;
		static Matrix World(const Vector3& position, const Vector3& forward, const Vector3& up) noexcept;
		static Matrix FromQuaternion(const Quaternion& q) noexcept;
		static Matrix FromYawPitchRoll(float yaw, float pitch, float roll) noexcept;
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

		static float Dot(const Quaternion& q1, const Quaternion& q2) noexcept;
		static Quaternion Normalize(const Quaternion& q) noexcept;
		static Quaternion Conjugate(const Quaternion& q) noexcept;
		static Quaternion Inverse(const Quaternion& q) noexcept;
		static Quaternion FromAxisAngle(const Vector3& axis, float angle) noexcept;
		static Quaternion FromYawPitchRoll(float yaw, float pitch, float roll) noexcept;
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

		operator DirectX::XMVECTOR() const noexcept;

		[[nodiscard]] float R() const noexcept { return DirectX::XMVectorGetX(color_); }
		[[nodiscard]] float G() const noexcept { return DirectX::XMVectorGetY(color_); }
		[[nodiscard]] float B() const noexcept { return DirectX::XMVectorGetZ(color_); }
		[[nodiscard]] float A() const noexcept { return DirectX::XMVectorGetW(color_); }

		void R(const float r) noexcept { color_ = DirectX::XMVectorSetX(color_, r); }
		void G(const float g) noexcept { color_ = DirectX::XMVectorSetX(color_, g); }
		void B(const float b) noexcept { color_ = DirectX::XMVectorSetX(color_, b); }
		void A(const float a) noexcept { color_ = DirectX::XMVectorSetX(color_, a); }

		static Color Negate(const Color& c) noexcept;
		static Color Saturate(const Color& c) noexcept;
		static Color Premultiply(const Color& c) noexcept;
		static Color AdjustSaturation(const Color& c, float sat) noexcept;
		static Color AdjustContrast(const Color& c, float contrast) noexcept;
		static Color Modulate(const Color& c1, const Color& c2) noexcept;
		static Color Lerp(const Color& c1, const Color& c2, float t) noexcept;

	private:
		DirectX::XMVECTOR color_;
	};

}

#include "core.inl"
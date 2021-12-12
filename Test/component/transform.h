/*********************************************************************
 * @file   transform.h
 * @author Tomomi Murakami
 * @date   2021/12/05 13:19
 * @brief  トランスフォームコンポーネント
 ********************************************************************/
#pragma once
#include <DirectXMath.h>

namespace wx2 {

	class Transform : public AbstractComponent
	{
	private:
		using XMVECTOR = DirectX::XMVECTOR;
		using XMMATRIX = DirectX::XMMATRIX;

		WX2_DECLARE_COMPONENTS_IMPL(Transform);

	public:
		Transform() = default;
		virtual ~Transform() = default;

		[[nodiscard]] const XMMATRIX& GetWorldMatrix();

		[[nodiscard]] const XMVECTOR& GetPosition();
		[[nodiscard]] const XMVECTOR& GetRotation();
		[[nodiscard]] const XMVECTOR& GetScale();

		[[nodiscard]] const XMVECTOR& GetRight();
		[[nodiscard]] const XMVECTOR& GetUp();
		[[nodiscard]] const XMVECTOR& GetForward();

		void SetWorldMatrix(const XMMATRIX& world_matrix);

		void SetPosition(const XMVECTOR& position);
		void SetRotation(const XMVECTOR& rotation);
		void SetScale(const XMVECTOR& scale);

		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z);
		void SetScale(float x, float y, float z);

		static constexpr DirectX::XMVECTORF32 Right = { 1.0f, 0.0f, 0.0f, 1.0f };
		static constexpr DirectX::XMVECTORF32 Up = { 0.0f, 1.0f, 0.0f, 1.0f };
		static constexpr DirectX::XMVECTORF32 Forward = { 0.0f, 0.0f, 1.0f, 1.0f };

	private:
		void UpdateWorldMatrix();
		void UpdateDirectionVectors();

		XMMATRIX world_matrix_ = XMMatrixIdentity();

		XMVECTOR position_ = XMVectorZero();
		XMVECTOR rotation_ = XMVectorZero();
		XMVECTOR scale_ = XMVectorSplatOne();

		XMVECTOR right_ = Right;
		XMVECTOR up_ = Up;
		XMVECTOR forward_ = Forward;
	};
}
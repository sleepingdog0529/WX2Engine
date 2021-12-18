/*********************************************************************
 * @file	gamepad.h
 * @author	Tomomi Murakami
 * @date	2021/10/17 23:43
 * @brief	XInput対応コントローラー入力
*********************************************************************/
#pragma once
#pragma warning(push, 0)
#include <WX2Common.h>
#define STRICT
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h> // Xinputより先にインクルードしないとエラー
#include <Xinput.h>
#include <wrl/client.h>
#include <bitset>
#include <array>
#pragma warning(pop)

namespace wx2
{
	//! @brief コントローラーのボタンの種類
	enum GamepadButtons
	{
		DPadUp = 0,
		DPadDown,
		DPadLeft,
		DPadRight,
		Start,
		Back,
		LThumb,
		RThumb,
		LShoulder,
		RShoulder,
		A,
		B,
		X,
		Y
	};

	// @brief コントローラーの軸の種類
	enum GamepadAxises
	{
		LTrigger = 0,
		RTrigger,
		LThumbX,
		LThumbY,
		RThumbX,
		RThumbY
	};

	/**
	 * @brief  コントローラー入力
	 */
	class Gamepad final
	{
	public:
		Gamepad() noexcept;
		~Gamepad();
		
		WX2_DISALLOW_COPY(Gamepad);
		WX2_DISALLOW_MOVE(Gamepad);

		//! @brief コントローラーの入力状態を更新
		void Update() const noexcept;

		/**
		 * @brief  ボタンが押されているか
		 * @param  button ボタンの種類
		 * @param  user コントローラーの番号
		 * @return 押されているか
		 */
		[[nodiscard]] bool IsDown(const GamepadButtons button, const size_t user = 0) const noexcept;
		/**
		 * @brief  ボタンが押されたか
		 * @param  button ボタンの種類
		 * @param  user コントローラーの番号
		 * @return ボタンが押されたか
		 */
		[[nodiscard]] bool IsPressed(const GamepadButtons button, const size_t user = 0) const noexcept;
		/**
		 * @brief  ボタンが離されたか
		 * @param  button ボタンの種類
		 * @param  user コントローラーの番号
		 * @return ボタンが離されたか
		 */
		[[nodiscard]] bool IsReleased(const GamepadButtons button, const size_t user = 0) const noexcept;
		/**
		 * @brief  軸の入力値を取得する
		 * @param  axis 軸の種類
		 * @param  user コントローラーの番号
		 * @return 軸の入力値
		 */
		[[nodiscard]] float GetAxisValue(const GamepadAxises axis, const size_t user = 0) const noexcept;
		/**
		 * @brief  軸の入力速度を取得する
		 * @param  axis 軸の種類
		 * @param  user コントローラーの番号
		 * @return 軸の入力速度
		 */
		[[nodiscard]] float GetAxisVelocity(const GamepadAxises axis, const size_t user = 0) const noexcept;

	private:
		class Impl;
		std::unique_ptr<Impl> pimpl_;
	};
}
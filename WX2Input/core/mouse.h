/*********************************************************************
 * @file	mouse.h
 * @author	Tomomi Murakami
 * @date	2021/10/10 15:09
 * @brief	マウス入力
*********************************************************************/
#pragma once
#pragma warning(push, 0) 
#include <bitset>
#include <dinput.h>
#include <wrl/client.h>
#include <Eigen/Core>
#pragma warning(pop)

namespace wx2
{
	/// <summary>
	/// マウス入力を扱うクラス
	/// </summary>
	class Mouse
	{
	public:
		enum Buttons
		{
			Left = 0,
			Right,
			Middle,
			X1,
			X2
		};

		enum Axises
		{
			CursorX,
			CursorY,
			WheellScroll
		};

	private:
		using DInputPtr = Microsoft::WRL::ComPtr<IDirectInput8>;
		using DevicePtr = Microsoft::WRL::ComPtr<IDirectInputDevice8>;

		static constexpr size_t NUM_BUTTONS_ = 5;	// ボタン数
		static constexpr size_t NUM_AXISES_ = 3;	// 軸数

		struct MouseState
		{
			struct 
			{
				std::bitset<NUM_BUTTONS_> buttons;
			} current, previous;
			std::array<float, NUM_AXISES_> axises;
		};

		struct MouseInfo
		{
			DevicePtr device;
			DIDEVICEINSTANCE instance;
			DIDEVCAPS capability;
		};

	public:
		Mouse();
		virtual ~Mouse();

		void Initialize(DInputPtr directInput, HWND hwnd);
		void Regist();
		void Update();

		bool IsDown(Buttons button) const;
		bool IsPressed(Buttons button) const;
		bool IsReleased(Buttons button) const;
		float GetAxisVelocity(Axises axises) const;

	private:
		static BOOL CALLBACK SetupMouseCallback(LPCDIDEVICEINSTANCE lpddi, LPVOID pvRef);

		DInputPtr directInput_;
		HWND hwnd_;

		std::vector<MouseInfo> mouses_;
		MouseState state_;
		DIMOUSESTATE2 stateBuffer_;
	};
}
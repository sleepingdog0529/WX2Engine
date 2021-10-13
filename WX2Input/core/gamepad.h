///*********************************************************************
// * @file	gamepad.h
// * @author	Tomomi Murakami
// * @date	2021/10/13 21:59
// * @brief	ゲームパッド入力
//*********************************************************************/
//#pragma once
//#pragma warning(push, 0) 
//#include <dinput.h>
//#include <bitset>
//#include <vector>
//#include <wrl/client.h>
//#include <Eigen/Core>
//#pragma warning(pop)
//
//namespace wx2
//{
//	class Gamepad
//	{
//	public:
//		enum ButtonCode : uint8_t
//		{
//			UP = 0,
//			Down,
//			Left,
//			Right,
//			Start,
//			Back,
//			LThumb,
//			RThumb,
//			LShoulder,
//			RShoulder,
//			A,
//			B,
//			X,
//			Y,
//			NUM_BUTTONS
//		};
//
//		static void Initialize(HWND hwnd);
//		static void Finalize();
//		static void Update();
//
//		static void StateReset();
//		
//		static BOOL CALLBACK EnumDeviceObjCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);
//
//		static BOOL CALLBACK EnumJoyStickCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);
//		static BOOL CALLBACK EnumAxesCallback(LPCDIDEVICEOBJECTINSTANCE ipddoi, LPVOID pvRef);
//
//	private:
//		static constexpr float MAX_STICK_MAGNITUDE_ = 1.0f;
//
//		static inline std::vector<Microsoft::WRL::ComPtr<IDirectInputDevice8>> devices_;
//		static inline DIJOYSTATE2 state_;
//
//		static inline std::bitset<NUM_BUTTONS> buttons_[2];
//		static inline float leftTrigger_[2];
//		static inline float rightTrigger_[2];
//		static inline Eigen::Vector2f rightStick_[2];
//		static inline Eigen::Vector2f leftStick_[2];
//	};
//}
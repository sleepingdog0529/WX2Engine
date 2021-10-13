/*********************************************************************
 * @file	keyboard.h
 * @author	Tomomi Murakami
 * @date	2021/10/13 1:13
 * @brief	キーボード入力
*********************************************************************/
#pragma once
#pragma warning(push, 0) 
#include <bitset>
#include <dinput.h>
#include <wrl/client.h>
#pragma warning(pop)

namespace wx2
{
	class Keyboard
	{
	public:
		enum KeyCode : uint8_t
		{ 
			Escape			= 0x01,
			D1				= 0x02,
			D2				= 0x03,
			D3				= 0x04,
			D4				= 0x05,
			D5				= 0x06,
			D6				= 0x07,
			D7				= 0x08,
			D8				= 0x09,
			D9				= 0x0A,
			D0				= 0x0B,
			Minus			= 0x0C,
			Equals			= 0x0D,
			Backspace		= 0x0E,
			Tab				= 0x0F,
			Q				= 0x10,
			W				= 0x11,
			E				= 0x12,
			R				= 0x13,
			T				= 0x14,
			Y				= 0x15,
			U				= 0x16,
			I				= 0x17,
			O				= 0x18,
			P				= 0x19,
			LBracket		= 0x1A,	// [
			EBracket		= 0x1B,	// ]
			Return			= 0x1C,
			LControl		= 0x1D,
			A				= 0x1E,
			S				= 0x1F,
			D				= 0x20,
			F				= 0x21,
			G				= 0x22,
			H				= 0x23,
			J				= 0x24,
			K				= 0x25,
			L				= 0x26,
			Semicolon		= 0x27,	// ;
			Apostrophe		= 0x28,	// '
			Grave			= 0x29,	// `
			LShift			= 0x2A,
			Backslash		= 0x2B,	// \ 
			Z				= 0x2C,
			X				= 0x2D,
			C				= 0x2E,
			V				= 0x2F,
			B				= 0x30,
			N				= 0x31,
			M				= 0x32,
			Comma			= 0x33,
			Period			= 0x34,
			Slash			= 0x35,
			RShift			= 0x36,
			Multiply		= 0x37,
			LAlt			= 0x38,
			Space			= 0x39,
			CapsLock		= 0x3A,
			F1				= 0x3B,
			F2				= 0x3C,
			F3				= 0x3D,
			F4				= 0x3E,
			F5				= 0x3F,
			F6				= 0x40,
			F7				= 0x41,
			F8				= 0x42,
			F9				= 0x43,
			F10				= 0x44,
			NumLock			= 0x45,
			ScrollLock		= 0x46,
			Numpad7			= 0x47,
			Numpad8			= 0x48,
			Numpad9			= 0x49,
			Subtract		= 0x4A,
			Numpad4			= 0x4B,
			Numpad5			= 0x4C,
			Numpad6			= 0x4D,
			Add				= 0x4E,
			Numpad1			= 0x4F,
			Numpad2			= 0x50,
			Numpad3			= 0x51,
			Numpad0			= 0x52,
			Decimal			= 0x53,
			F11				= 0x57,
			F12				= 0x58,
			Kana			= 0x70,
			Convert			= 0x79,
			NoConvert		= 0x7B,
			Yen				= 0x7D,
			NumpadEquals	= 0x8D,
			Circumflex		= 0x90,	// ^
			Atsign			= 0x91,	// @
			Colon			= 0x92,	// :
			Underline		= 0x93,	// _
			Kanji			= 0x94,
			NumpadEnter		= 0x9C,
			RControl		= 0x9D,
			Divide			= 0xB5,
			RAlt			= 0xB8,
			Pause			= 0xC5,
			Home			= 0xC7,
			Up				= 0xC8,
			PageUp			= 0xC9,
			Left			= 0xCB,
			Right			= 0xCD,
			End				= 0xCF,
			Down			= 0xD0,
			PageDown		= 0xD1,
			Insert			= 0xD2,
			Delete			= 0xD3,
			LWin			= 0xDB,
			RWin			= 0xDC,
			Apps			= 0xDD,	// Menu
			NUM_KEYS
		};

		static void Initialize(HWND hwnd);
		static void Finalize();
		static void Update();

		static void StateReset();

		static bool IsDown(KeyCode key) { return keys_[0][key]; }
		static bool IsUp(KeyCode key) { return !keys_[0][key]; }
		static bool IsPressed(KeyCode key) { return keys_[0][key] && !keys_[1][key]; }
		static bool IsReleased(KeyCode key) { return !keys_[0][key] && keys_[1][key]; }

	private:
		static inline HWND hwnd_;
		static inline Microsoft::WRL::ComPtr<IDirectInputDevice8> device_;
		static inline std::array<uint8_t, 256> buffer_;

		static inline std::bitset<NUM_KEYS> keys_[2];
	};
}
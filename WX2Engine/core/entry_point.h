/*********************************************************************
 * @file	entry_point.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 1:28
 * @brief	エントリポイント
 ********************************************************************/
#pragma once
#include "application.h"

extern std::unique_ptr<wx2::Application> wx2::CreateApp();

/**
 * @brief エントリポイント
 */
#ifdef NDEBUG
INT WINAPI WinMain(
	_In_ HINSTANCE hInstance, 
	_In_opt_ HINSTANCE hPrevInstance, 
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd )
#else
int main(int argc, char** argv)
#endif
{
	auto app = wx2::CreateApp();

	app->Run();
	return EXIT_SUCCESS;
}
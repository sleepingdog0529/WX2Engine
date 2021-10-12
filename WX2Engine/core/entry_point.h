/*********************************************************************
 * @file	entry_point.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 1:28
 * @brief	エントリポイント
*********************************************************************/
#pragma once
#include "application.h"

extern std::unique_ptr<wx2::Application> wx2::CreateApp();

/// <summary>
/// エントリポイント
/// </summary>
int main(int argc, char** argv)
{
	auto app = wx2::CreateApp();

	app->Run();
	return EXIT_SUCCESS;
}
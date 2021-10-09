/*********************************************************************
 * @file	entry_point.h
 * @author	Tomomi Murakami
 * @date	2021/09/19 1:28
 * @brief	�G���g���|�C���g
*********************************************************************/
#pragma once
#include "application.h"

extern std::unique_ptr<wx2::application> wx2::create_app();

/// <summary>
/// �G���g���|�C���g
/// </summary>
int main(int argc, char** argv)
{
	auto app = wx2::create_app();

	app->run();
	return 0;
}
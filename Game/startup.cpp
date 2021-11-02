/*********************************************************************
 * @file	startup.cpp
 * @author	Tomomi Murakami
 * @date	2021/11/02 10:27
 * @brief	アプリケーション動作の定義
 ********************************************************************/

/**
 * @brief アプリケーション
 */
class Game : public wx2::Application
{
public:
	Game()
	{
	}

	~Game()
	{
	}
};

/**
 * @brief  動作定義済アプリケーションの作成関数定義
 * @return アプリケーションポインタ
 */
std::unique_ptr<wx2::Application> wx2::CreateApp()
{
	return std::make_unique<Game>();
}
#include <WX2Engine.h>

class Game : public wx2::application
{
public:
	Game()
	{
	}

	~Game()
	{
	}
};

std::unique_ptr<wx2::application> wx2::create_app()
{
	return std::make_unique<Game>();
}
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

std::unique_ptr<wx2::Application> wx2::CreateApp()
{
	return std::make_unique<Game>();
}
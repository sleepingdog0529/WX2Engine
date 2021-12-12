#include <sol/sol.hpp>

namespace wx2
{
	class Game final : public wx2::Application
	{
	private:
		void Start() noexcept override
		{
			sol::state lua;
			lua.open_libraries(sol::lib::base, sol::lib::package);
			lua["intval"] = 80;
			auto res = lua.script_file(".\\asset\\script\\hello.lua");
		}

		bool Update() noexcept override
		{
			using namespace std::chrono_literals;

			input_.Update();
			const auto& keyboard = input_.GetKeyboard();

			// �`��
			graphics_.RenderFrame();

			// ESC�L�[��������Ă�����A�v���P�[�V�����I��
			return !keyboard.IsPressed(Keyboard::Escape);
		}
	};
}

std::unique_ptr<wx2::Application> wx2::CreateApp() noexcept
{
	return std::make_unique<Game>();
}
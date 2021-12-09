/*********************************************************************
 * @file	startup.cpp
 * @author	Tomomi Murakami
 * @date	2021/11/02 10:27
 * @brief	�A�v���P�[�V��������̒�`
 ********************************************************************/

/**
 * @brief �A�v���P�[�V����
 */
class Game final : public wx2::Application
{
public:
	Game() = default;
	~Game() override = default;
};

/**
 * @brief  �����`�σA�v���P�[�V�����̍쐬�֐���`
 * @return �A�v���P�[�V�����|�C���^
 */
std::unique_ptr<wx2::Application> wx2::CreateApp() noexcept
{
	return std::make_unique<Game>();
}
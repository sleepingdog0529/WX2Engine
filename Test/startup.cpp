#include <boost/flyweight.hpp>
#include <boost/flyweight/key_value.hpp>

namespace wx2
{
	/**
	 * @brief �A�v���P�[�V����
	 */
	class Game final : public wx2::Application
	{
	private:
		//! @brief �A�v���P�[�V�����J�n���ɌĂяo�����  
		void Start() noexcept override
		{
			// �f�o�C�X�ƃf�o�C�X�R���e�L�X�g���擾
			auto& devices = graphics_.GetDevice();

			// �C���v�b�g���C�A�E�g
			D3D11_INPUT_ELEMENT_DESC layoutDescs[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			// ���_�V�F�[�_�[�̏�����
			vertexShader_.Initialize(&devices, ".\\asset\\shader\\simple.hlsl", layoutDescs);

			// �s�N�Z���V�F�[�_�[�̏�����
			pixelShader_.Initialize(&devices, ".\\asset\\shader\\simple.hlsl");

			// ���f���̃��[�h
			modelLoader_.Initialize(&devices);
			std::filesystem::path modelPath{};
			while (modelPath.empty())
			{
				modelPath = OpenFileDialog("*.fbx\0\0");
			}
			model_ = modelLoader_.Load(modelPath);
			auto model1 = modelLoader_.Load(modelPath);
			auto model2 = modelLoader_.Load(modelPath);
			auto model3 = modelLoader_.Load(modelPath);
			auto model4 = modelLoader_.Load(modelPath);
			auto model5 = modelLoader_.Load(modelPath);
			auto model6 = modelLoader_.Load(modelPath);
			auto model7 = modelLoader_.Load(modelPath);
			auto model8 = modelLoader_.Load(modelPath);
			auto model9 = modelLoader_.Load(modelPath);

			auto* mat = physics_.CreateMaterial(0.5f, 0.5f, 1.0f);
			box_ = physics_.CreateDynamic(
				physx::PxTransform(physx::PxVec3(0.0f, 10.0f, 0.0f)),
				physx::PxBoxGeometry(1.0f, 1.0f, 2.0f),
				*mat);

			plane_ = physics_.CreateStatic(
				physx::PxTransform(physx::PxVec3(0.0f, 0.0f, 0.0f)),
				physx::PxBoxGeometry(100.0f, 1.0f, 100.0f),
				*mat);

			camera_.SetPositon(0.0f, 5.0f, -10.0f);
			camera_.SetLookAt(Vector3::Zero());
		}

		/**
		 * @brief  �A�v���P�[�V�����̍X�V���ɌĂяo�����
		 * @param  deltaTime �O��̍X�V����̌o�ߎ���
		 * @return false�ŃA�v���P�[�V�������I��
		 */
		bool Update(const float deltaTime) noexcept override
		{
			// �L�[�{�[�h�ƃ}�E�X�̓��͂��擾
			const auto& keyboard = input_.GetKeyboard();
			const auto& mouse = input_.GetMouse();
			const auto& gamepad = input_.GetGamepad();

			// �J�[�\���̓����ɂ��킹�ĉ�]
			const float cursorX = mouse.GetAxisVelocity(Mouse::CursorX);
			const float cursorY = mouse.GetAxisVelocity(Mouse::CursorY);
			camera_.AddRotation(Quaternion::RotationY(cursorX * deltaTime * 0.3f));
			camera_.AddRotation(Quaternion::AxisAngle(camera_.Right(), cursorY * deltaTime * 0.3f));

			// �L�[�̓��͂���ړ�������ݒ�
			Vector3 camMove;
			if (keyboard.IsDown(Keyboard::D))		camMove[0] += 1.0f;
			if (keyboard.IsDown(Keyboard::A))		camMove[0] -= 1.0f;
			if (keyboard.IsDown(Keyboard::W))		camMove[2] += 1.0f;
			if (keyboard.IsDown(Keyboard::S))		camMove[2] -= 1.0f;
			camMove = Vector3::Transform(camMove, camera_.GetRotation());

			if (keyboard.IsDown(Keyboard::Space))	camMove[1] += 1.0f;
			if (keyboard.IsDown(Keyboard::LShift))	camMove[1] -= 1.0f;
			camMove.Normalized();
			
			camera_.AddPositon(camMove * deltaTime * 10.0f);

			if (keyboard.IsDown(Keyboard::R))
			{
				box_.SetPosition(Vector3::Up() * 10.0f);
			}

			transform_.SetPositon(box_.GetPosition());
			transform_.SetRotation(box_.GetRotation());

			// ESC�L�[��������Ă�����A�v���P�[�V�����I��
			return !keyboard.IsPressed(Keyboard::Escape);
		}

		/**
		 * @brief  �A�v���P�[�V�����̕`�掞�ɌĂяo�����
		 * @param  deltaTime �O��̕`�悩��̌o�ߎ���
		 */
		void Draw(const float deltaTime) noexcept override
		{
			// �V�F�[�_�[���o�C���h
			vertexShader_.Bind();
			pixelShader_.Bind();

			// WVP�ϊ��s��p�̒萔�o�b�t�@���擾
			auto& constantBufferWVP = graphics_.GetConstantBufferWVP();

			// �E�B���h�E�����擾
			const auto& windowProp = mainWindow_->GetWindowProperty();
			camera_.SetProjectionValues(PIDIV2, windowProp.AspectRatio(), 0.1f, 10000.0f);

			// WVP�s������Z�b�g
			constantBufferWVP.data.world = transform_.GetWorldMatrix();
			constantBufferWVP.data.projection = camera_.GetProjectionMatrix();
			constantBufferWVP.data.view = camera_.GetViewMatrix();

			// WVP�s��萔�o�b�t�@�𒸓_�V�F�[�_�[�Ƀo�C���h
			constantBufferWVP.ApplyChange();
			constantBufferWVP.Bind(ShaderType::Vertex, 0);

			// ���f���̕`��
			model_->Draw();
		}

		void DrawImGui() noexcept override
		{
			//ImGui::Begin("Transform");
			//ImGui::DragFloat3("Position", &pos_[0], 0.1f);
			//ImGui::End();
		}

		Transform transform_;

		Camera camera_{};

		phys::RigidDynamic box_{};
		phys::RigidStatic plane_{};

		VertexShader vertexShader_{};	//! ���_�V�F�[�_�[
		PixelShader pixelShader_{};		//! �s�N�Z���V�F�[�_�[
		ModelLoader modelLoader_{};		//!	���f���ǂݍ��݃N���X
		std::shared_ptr<Model> model_;		//! ���f��
	};
}

// �A�v���P�[�V�����쐬�֐��̒�`
std::unique_ptr<wx2::Application> wx2::CreateApp() noexcept
{
	return std::make_unique<Game>();
}
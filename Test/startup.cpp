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
			model_ = modelLoader_.Load(OpenFileDialog("*.fbx\0\0"));
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

			// R�L�[�������ꂽ���]��0��
			if (keyboard.IsDown(Keyboard::R))
			{
				rot_ = Quaternion::Identity();
			}

			// �J�[�\���̓����ɂ��킹�ĉ�]
			const float cursorX = mouse.GetAxisVelocity(Mouse::CursorX);
			const float cursorY = mouse.GetAxisVelocity(Mouse::CursorY);
			rot_ *= Quaternion::FromAxisAngle(Vector3::Up(), cursorX * 0.001f);
			rot_ *= Quaternion::FromAxisAngle(Vector3::Right(), cursorY * 0.001f);

			// �L�[�̓��͂���ړ�������ݒ�
			Vector3 move;
			if (keyboard.IsDown(Keyboard::D))		++move[0];
			if (keyboard.IsDown(Keyboard::A))		--move[0];
			if (keyboard.IsDown(Keyboard::Space))	++move[1];
			if (keyboard.IsDown(Keyboard::LShift))	--move[1];
			if (keyboard.IsDown(Keyboard::W))		++move[2];
			if (keyboard.IsDown(Keyboard::S))		--move[2];

			// �ړ������ɉ�]��K�����Đi�s�����ɂ��킹��
			pos_ += Vector3::Transform(Vector3::Normalize(move), rot_) * deltaTime * 3.0f;

			// �X�P�[���Œ�
			scale_ = Vector3(1.0f);

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

			// �g�k��]�ړ������[���h�s��ɓK��
			Matrix world;
			world *= Matrix::Scale(scale_);
			world *= Matrix::RotationFromQuaternion(rot_);
			world *= Matrix::Translation(pos_);

			// WVP�s������Z�b�g
			constantBufferWVP.data.world = world;
			constantBufferWVP.data.projection = Matrix::PerspectiveFieldOfView(
				PIDIV2,
				windowProp.AspectRatio(),
				0.01f,
				1000.0f);
			constantBufferWVP.data.view = Matrix::LookAt(
				Vector3::Backward() * 50,
				Vector3::Zero(),
				Vector3::Up());

			// WVP�s��萔�o�b�t�@�𒸓_�V�F�[�_�[�Ƀo�C���h
			constantBufferWVP.ApplyChange();
			constantBufferWVP.VSBind(0);

			// ���f���̕`��
			model_.Draw();
		}

		void DrawImGui() noexcept override
		{
			//ImGui::Begin("Transform");
			//ImGui::DragFloat3("Position", &pos_[0], 0.1f);
			//ImGui::End();
		}

		Vector3 pos_{};		//! ���f���ʒu
		Vector3 scale_{};	//! ���f���g�k
		Quaternion rot_{};	//! ���f����]

		VertexShader vertexShader_{};	//! ���_�V�F�[�_�[
		PixelShader pixelShader_{};		//! �s�N�Z���V�F�[�_�[
		ModelLoader modelLoader_{};		//!	���f���ǂݍ��݃N���X
		Model model_{};		//! ���f��
	};
}

// �A�v���P�[�V�����쐬�֐��̒�`
std::unique_ptr<wx2::Application> wx2::CreateApp() noexcept
{
	return std::make_unique<Game>();
}
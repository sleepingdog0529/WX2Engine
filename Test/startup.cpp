//#include <imgui/imgui.h>

namespace wx2
{
	class Game final : public wx2::Application
	{
	private:
		void Start() noexcept override
		{
			auto& devices = graphics_.GetDevice();

			D3D11_INPUT_ELEMENT_DESC layoutDescs[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};
			vertexShader_.Initialize(&devices, ".\\asset\\shader\\simple.hlsl", layoutDescs);

			pixelShader_.Initialize(&devices, ".\\asset\\shader\\simple.hlsl");

			modelLoader_.Initialize(&devices);
			model_ = modelLoader_.Load(".\\asset\\model\\VAWS-012\\VAWS-012_TypeA.fbx");
		}

		bool Update(const float deltaTime) noexcept override
		{
			input_.Update();
			const auto& keyboard = input_.GetKeyboard();
			const auto& mouse = input_.GetMouse();

			if (keyboard.IsDown(Keyboard::R))
			{
				rot_ = Quaternion::Identity();
			}
			const float cursorX = mouse.GetAxisVelocity(Mouse::CursorX);
			const float cursorY = mouse.GetAxisVelocity(Mouse::CursorY);
			rot_ *= Quaternion::FromAxisAngle(Vector3::Up(), cursorX * 0.01f);
			rot_ *= Quaternion::FromAxisAngle(Vector3::Right(), cursorY * 0.01f);

			Vector3 move;
			if (keyboard.IsDown(Keyboard::D))		++move[0];
			if (keyboard.IsDown(Keyboard::A))		--move[0];
			if (keyboard.IsDown(Keyboard::Space))	++move[1];
			if (keyboard.IsDown(Keyboard::LShift))	--move[1];
			if (keyboard.IsDown(Keyboard::W))		++move[2];
			if (keyboard.IsDown(Keyboard::S))		--move[2];

			pos_ += Vector3::Transform(Vector3::Normalize(move), rot_) * deltaTime * 3.0f;
			scale_ = Vector3(1.0f);

			// ESCキーが押されていたらアプリケーション終了
			return !keyboard.IsPressed(Keyboard::Escape);
		}

		void Draw(const float deltaTime) noexcept override
		{
			vertexShader_.Bind();
			pixelShader_.Bind();

			auto& constantBufferWVP = graphics_.GetConstantBufferWVP();
			const auto& windowProp = mainWindow_->GetWindowProperty();

			Matrix world;
			world *= Matrix::Scale(scale_);
			world *= Matrix::RotationFromQuaternion(rot_);
			world *= Matrix::Translation(pos_);

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
			constantBufferWVP.ApplyChange();
			constantBufferWVP.VSBind(0);

			model_.Draw();
		}

		void DrawImGui() noexcept override
		{
			//ImGui::Begin("Transform");
			//ImGui::DragFloat3("Position", &pos_[0], 0.1f);
			//ImGui::End();
		}

		Vector3 pos_{};
		Vector3 scale_{};
		Quaternion rot_{};

		VertexShader vertexShader_{};
		PixelShader pixelShader_{};
		ModelLoader modelLoader_{};
		Model model_{};
	};
}

std::unique_ptr<wx2::Application> wx2::CreateApp() noexcept
{
	return std::make_unique<Game>();
}
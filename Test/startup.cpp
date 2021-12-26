#include <boost/flyweight.hpp>
#include <boost/flyweight/key_value.hpp>

namespace wx2
{
	/**
	 * @brief アプリケーション
	 */
	class Game final : public wx2::Application
	{
	private:
		//! @brief アプリケーション開始時に呼び出される  
		void Start() noexcept override
		{
			// デバイスとデバイスコンテキストを取得
			auto& devices = graphics_.GetDevice();

			// インプットレイアウト
			D3D11_INPUT_ELEMENT_DESC layoutDescs[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT   , 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "NORMAL"  , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "TANGENT" , 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "BINORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			// 頂点シェーダーの初期化
			vertexShader_.Initialize(&devices, ".\\asset\\shader\\simple.hlsl", layoutDescs);

			// ピクセルシェーダーの初期化
			pixelShader_.Initialize(&devices, ".\\asset\\shader\\simple.hlsl");

			// モデルのロード
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
		 * @brief  アプリケーションの更新時に呼び出される
		 * @param  deltaTime 前回の更新からの経過時間
		 * @return falseでアプリケーションを終了
		 */
		bool Update(const float deltaTime) noexcept override
		{
			// キーボードとマウスの入力を取得
			const auto& keyboard = input_.GetKeyboard();
			const auto& mouse = input_.GetMouse();
			const auto& gamepad = input_.GetGamepad();

			// カーソルの動きにあわせて回転
			const float cursorX = mouse.GetAxisVelocity(Mouse::CursorX);
			const float cursorY = mouse.GetAxisVelocity(Mouse::CursorY);
			camera_.AddRotation(Quaternion::RotationY(cursorX * deltaTime * 0.3f));
			camera_.AddRotation(Quaternion::AxisAngle(camera_.Right(), cursorY * deltaTime * 0.3f));

			// キーの入力から移動方向を設定
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

			// ESCキーが押されていたらアプリケーション終了
			return !keyboard.IsPressed(Keyboard::Escape);
		}

		/**
		 * @brief  アプリケーションの描画時に呼び出される
		 * @param  deltaTime 前回の描画からの経過時間
		 */
		void Draw(const float deltaTime) noexcept override
		{
			// シェーダーをバインド
			vertexShader_.Bind();
			pixelShader_.Bind();

			// WVP変換行列用の定数バッファを取得
			auto& constantBufferWVP = graphics_.GetConstantBufferWVP();

			// ウィンドウ情報を取得
			const auto& windowProp = mainWindow_->GetWindowProperty();
			camera_.SetProjectionValues(PIDIV2, windowProp.AspectRatio(), 0.1f, 10000.0f);

			// WVP行列情報をセット
			constantBufferWVP.data.world = transform_.GetWorldMatrix();
			constantBufferWVP.data.projection = camera_.GetProjectionMatrix();
			constantBufferWVP.data.view = camera_.GetViewMatrix();

			// WVP行列定数バッファを頂点シェーダーにバインド
			constantBufferWVP.ApplyChange();
			constantBufferWVP.Bind(ShaderType::Vertex, 0);

			// モデルの描画
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

		VertexShader vertexShader_{};	//! 頂点シェーダー
		PixelShader pixelShader_{};		//! ピクセルシェーダー
		ModelLoader modelLoader_{};		//!	モデル読み込みクラス
		std::shared_ptr<Model> model_;		//! モデル
	};
}

// アプリケーション作成関数の定義
std::unique_ptr<wx2::Application> wx2::CreateApp() noexcept
{
	return std::make_unique<Game>();
}
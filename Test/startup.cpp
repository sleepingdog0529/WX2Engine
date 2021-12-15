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
			model_ = modelLoader_.Load(OpenFileDialog("*.fbx\0\0"));
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

			// Rキーが押されたら回転を0に
			if (keyboard.IsDown(Keyboard::R))
			{
				rot_ = Quaternion::Identity();
			}

			// カーソルの動きにあわせて回転
			const float cursorX = mouse.GetAxisVelocity(Mouse::CursorX);
			const float cursorY = mouse.GetAxisVelocity(Mouse::CursorY);
			rot_ *= Quaternion::FromAxisAngle(Vector3::Up(), cursorX * 0.001f);
			rot_ *= Quaternion::FromAxisAngle(Vector3::Right(), cursorY * 0.001f);

			// キーの入力から移動方向を設定
			Vector3 move;
			if (keyboard.IsDown(Keyboard::D))		++move[0];
			if (keyboard.IsDown(Keyboard::A))		--move[0];
			if (keyboard.IsDown(Keyboard::Space))	++move[1];
			if (keyboard.IsDown(Keyboard::LShift))	--move[1];
			if (keyboard.IsDown(Keyboard::W))		++move[2];
			if (keyboard.IsDown(Keyboard::S))		--move[2];

			// 移動方向に回転を適応して進行方向にそわせる
			pos_ += Vector3::Transform(Vector3::Normalize(move), rot_) * deltaTime * 3.0f;

			// スケール固定
			scale_ = Vector3(1.0f);

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

			// 拡縮回転移動をワールド行列に適応
			Matrix world;
			world *= Matrix::Scale(scale_);
			world *= Matrix::RotationFromQuaternion(rot_);
			world *= Matrix::Translation(pos_);

			// WVP行列情報をセット
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

			// WVP行列定数バッファを頂点シェーダーにバインド
			constantBufferWVP.ApplyChange();
			constantBufferWVP.VSBind(0);

			// モデルの描画
			model_.Draw();
		}

		void DrawImGui() noexcept override
		{
			//ImGui::Begin("Transform");
			//ImGui::DragFloat3("Position", &pos_[0], 0.1f);
			//ImGui::End();
		}

		Vector3 pos_{};		//! モデル位置
		Vector3 scale_{};	//! モデル拡縮
		Quaternion rot_{};	//! モデル回転

		VertexShader vertexShader_{};	//! 頂点シェーダー
		PixelShader pixelShader_{};		//! ピクセルシェーダー
		ModelLoader modelLoader_{};		//!	モデル読み込みクラス
		Model model_{};		//! モデル
	};
}

// アプリケーション作成関数の定義
std::unique_ptr<wx2::Application> wx2::CreateApp() noexcept
{
	return std::make_unique<Game>();
}
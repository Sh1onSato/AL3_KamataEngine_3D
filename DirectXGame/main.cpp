#include <Windows.h>
#include"KamataEngine.h"
#include"GameScene.h"



// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	using namespace KamataEngine;
	// エンジンの初期化
	KamataEngine::Initialize(L" LE2C_12_サトウ_シオン_AL3");

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	// ImGuiManagerインスタンスの取得
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	// ゲームシーンのインスタンスを生成
	GameScene* gameScene = new GameScene();
	

	// ゲームシーンの初期化
	gameScene->Initialize();

	while (true) {
	//エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}
		// ImGui受付開始
		imguiManager->Begin();

		// ゲームシーンの更新
		gameScene->Update();

		// ImGui受付終了
		imguiManager->End();

		//描画開始
		dxCommon->PreDraw();

		// ゲームシーンの描画
		gameScene->Draw();

		//軸表示の描画
		AxisIndicator::GetInstance()->Draw();

		// ImGuiの描画
		imguiManager->Draw();

		//描画終了
		dxCommon->PostDraw();
	}

	// ゲームシーンの解放
	delete gameScene;

	// nullptrを代入
	gameScene = nullptr;
	
	//エンジンの終了処理
	KamataEngine::Finalize();
	return 0;
}

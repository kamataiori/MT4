#include "MainLoop.h"

MainLoop::MainLoop()
{
	//char keys[256] = 
	for (int i = 0; i < 256; i++)
	{
		keys[i] = 0;
		preKeys[i] = 0;
	}

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	result_ = new Result;

}

MainLoop::~MainLoop() { delete result_; }

void MainLoop::InPut()
{
	// キー入力を受け取る
	memcpy(preKeys, keys, 256);
	Novice::GetHitKeyStateAll(keys);
}

void MainLoop::Initialize()
{
	result_->Initialize();
}

void MainLoop::Update() { result_->Update(); }

void MainLoop::Draw() { result_->Draw(); }

void MainLoop::WholeLoop()
{
	Initialize();

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		InPut();

		///
		/// ↓更新処理ここから
		///

		Update();

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///

		Draw();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}
}

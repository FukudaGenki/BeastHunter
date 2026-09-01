#include "DxLib.h"
#include "SceneManager.h"
#include "Pad.h"
#include "SoundManager.h"
#include "InputBuffer.h"
#include <memory>
using namespace Game;


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	int WIDTH = kScreenWidth, HEIGHT = kScreenHeight;
	SetWindowText("Fighting Test");
	SetGraphMode(WIDTH, HEIGHT, 32);
	ChangeWindowMode(false);

	if (DxLib_Init() == -1)
	{
		return -1;
	}
	SetBackgroundColor(0, 0, 0);
	SetDrawScreen(DX_SCREEN_BACK);

	// シーンの初期化
	std::unique_ptr<SceneManager> m_Scene;
	m_Scene = std::make_unique<SceneManager>();
	m_Scene->Init();

	// コントローラーの初期化
	Pad m_pad;
	Pad m_pad2;
	m_pad.Init(1);
	m_pad2.Init(2);

	// サウンドマネージャーの初期化
	// 音声ファイルの読み込み
	SoundManager::Instance().Init();

	// FPS計測用の変数
	int frameCount = 0;
	LONGLONG fpsPrevTime = GetNowHiPerformanceCount();
	float currentFps = 0.0f;

	// デバック用のfpsを変更する変数
	float frameChange = 1.0f;

	while (ProcessMessage() == 0)
	{
		LONGLONG time = GetNowHiPerformanceCount();
		ClearDrawScreen();

		m_pad.Update();
		m_pad2.Update();

		m_Scene->Update(m_pad, m_pad2);
		m_Scene->Render();

		// --- FPSの計算 ---
		frameCount++;
		LONGLONG currentTime = GetNowHiPerformanceCount();
		// 1秒（1000000マイクロ秒）経過したら描画フレーム数からFPSを計測
		if (currentTime - fpsPrevTime >= 1000000)
		{
			currentFps = static_cast<float>(frameCount * 1000000.0 / (currentTime - fpsPrevTime));
			frameCount = 0;
			fpsPrevTime = currentTime;
		}

#ifdef _DEBUG
		// --- FPSの表示（画面左上に白文字） ---
		DrawFormatString(0, 0, GetColor(255, 255, 255), "FPS: %.1f", currentFps);

		// fpsを変更する
		if (CheckHitKey(KEY_INPUT_R))
		{
			if (frameChange < 10.0)frameChange += 2.0f;
			else frameChange = 1.0f;
		}
#endif

		ScreenFlip();

		while (GetNowHiPerformanceCount() - time < 16667 * frameChange)
		{

		}
		if (ProcessMessage() == -1) break;
		if (CheckHitKey(KEY_INPUT_ESCAPE))
		{
			break;
		}
	}

	DxLib_End();

	return 0;				// ソフトの終了 
}
#include "SceneManager.h"
class SceneBase;
class TitleScene;
class SceneMain;
class ResultScene;
class TitleScene;
class SelectScene;

// ここでメモリの確保を行う
std::unique_ptr<SceneBase> SceneManager::m_Scene = nullptr;
GameData SceneManager::m_data;

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}
// シーンごとの更新処理や描画処理を行う
// シーン遷移そのものはシーンの処理内に書かれる
void SceneManager::ChangeScene(SceneID scene)
{
	switch (scene)
	{
	case SceneID::Title:
		m_Scene = std::make_unique<TitleScene>();
		break;
	case SceneID::CharaSelect:
		m_Scene = std::make_unique<SelectScene>();
		break;
	case SceneID::Game:
		m_Scene = std::make_unique<SceneMain>();
		break;
	case SceneID::Result:
		m_Scene = std::make_unique<ResultScene>();
		break;
	}

	if (m_Scene)m_Scene->Init(m_data);
}

void SceneManager::Init()
{
	m_Scene = std::make_unique<TitleScene>();
	m_Scene->Init(m_data);
}

void SceneManager::Update(const Pad& _pad, const Pad& _pad2)
{
	// 現在のシーンの更新処理
	m_Scene->Update(m_data, _pad, _pad2);
}

void SceneManager::Render()
{
	// 現在のシーンの描画処理
	m_Scene->Draw(m_data);
}
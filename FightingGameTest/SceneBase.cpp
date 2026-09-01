#include "SceneBase.h"
#include "DxLib.h"
#include "SceneManager.h" // 循環参照にならないようにここに書く

SceneBase::SceneBase()
{
	//m_background = std::make_unique<Background>();

}

SceneBase::~SceneBase()
{
}

void SceneBase::ChangeScene(SceneID scene)
{
	SceneManager::ChangeScene(scene);
}
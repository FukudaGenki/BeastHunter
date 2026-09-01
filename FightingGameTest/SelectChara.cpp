#include "SelectChara.h"
#include "GameDefine.h"
#include <DxLib.h>
namespace
{
	constexpr int kDisCharaGraphToNamePos = 300;
}

SelectChara::SelectChara()
{
}

SelectChara::~SelectChara()
{
	DeleteGraph(m_VSFont.handle);
	DeleteGraph(m_shartelCharaName.handle);
	DeleteGraph(m_novaCharaName.handle);
}

void SelectChara::Init()
{
	// 文字のロード
	//Nova
	m_novaCharaName.handle = LoadGraph("UI/CharaFontNova.png");
	GetGraficSizeFromData(m_novaCharaName);

	m_shartelCharaName.handle = LoadGraph("UI/CharaFontShartel.png");
	GetGraficSizeFromData(m_shartelCharaName);

	// VS
	m_VSFont.handle = LoadGraph("UI/VS0.png");
	GetGraficSizeFromData(m_VSFont);

	// アニメーションのロード（メモリの解放はアニメーションクラス内で行われる）
	// アニメーションデータの定義リスト
	const std::map<SelectAnimID, SelectAnimData> kAnimConfig =
	{
		// {データID、{ハンドル、1コマの表示フレーム、全体コマ数、ループするかどうか}}を示す
		// Nova
		{SelectAnimID::NovaIdle, {"anim/idle.png", 10, 7, true}},

		//Shartel
		{SelectAnimID::ShartelIdle, {"chara/shartel/idle.png", 10, 9, true}}
	};

	// データをもとにAnimationクラスを生成してマップに登録
	for (const auto& [id, data] : kAnimConfig)
	{
		int handle = LoadGraph(data.filePath);
		// マップ内に新しくAnimationを作り、Initを呼ぶ
		m_animations[id].Init(handle, data.animFrame, data.animNum, data.isLoop);
	}

	m_pCurrentAnim = &m_animations[SelectAnimID::NovaIdle];

	
}

void SelectChara::Update(Chara _selectChara)
{
	m_selectChara = _selectChara;

	// アニメーションの変更をチェック、ポインタを切り替える
	CheckChangeAnim();

	m_lastAnimID = m_currentAnimID;
	// アニメーションの更新処理
	m_pCurrentAnim->Update();
}

void SelectChara::Draw(float _posX, float _posY, bool _isTurn)
{
	// 選択キャラクターの描画
	m_pCurrentAnim->DrawCenter(_posX, _posY, _isTurn);

	// 現在のキャラクターと名前を一致させる
	GraficData currentCharaName = m_novaCharaName;
	if (m_selectChara == Chara::Shartel) currentCharaName = m_shartelCharaName;

	// 名前の描画
	DrawCenter(currentCharaName, _posX, _posY - kDisCharaGraphToNamePos, false);

	// VSの描画
	// 画面の中心に描画する
	DrawCenter(m_VSFont, Game::kScreenWidth / 2, Game::kScreenHeight / 2, false);
}

void SelectChara::CheckChangeAnim()
{
	// セレクトされているキャラクターが切り替わったらそのキャラクターを表示する
	switch (m_selectChara)
	{
	case Chara::Nova:
		m_currentAnimID = SelectAnimID::NovaIdle;
		break;
	case Chara::Shartel:
		m_currentAnimID = SelectAnimID::ShartelIdle;
		break;
	}

	if (m_lastAnimID == m_currentAnimID) return; // アニメーションが変わらないなら何も行わない
	// アニメーションのポインタを切り替える
	m_pCurrentAnim = &m_animations[m_currentAnimID];
	// アニメーションを初期化
	m_pCurrentAnim->Reset();
}

void SelectChara::ChangeCharaName()
{
	
}

void SelectChara::DrawCenter(GraficData _data, float _posX, float _posY, bool _isTurn)
{
	// 画像サイズが0ならリターンする
	if (_data.sizeX <= 0 || _data.sizeY <= 0) return;

	int x = static_cast<int>(_posX);
	int y = static_cast<int>(_posY);
	int centerX = x - _data.sizeX / 2;
	int centerY = y - _data.sizeY / 2;

	DrawGraph(centerX, centerY, _data.handle, true);
}

void SelectChara::GetGraficSizeFromData(GraficData& _data)
{
	GetGraphSize(_data.handle, &_data.sizeX, &_data.sizeY);
}

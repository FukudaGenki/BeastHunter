#include "WinChara.h"
#include "GameDefine.h"
#include <DxLib.h>
namespace
{
	constexpr int kDisCharaGraphToNamePos = 300;
}

WinChara::WinChara()
{}

WinChara::~WinChara()
{
	DeleteGraph(m_p1Name.handle);
	DeleteGraph(m_p2Name.handle);
}

void WinChara::Init(Chara _winChara)
{
	m_WinChara = _winChara;

	// 文字のロード
	//Nova
	m_p2Name.handle = LoadGraph("UI/CharaFontNova.png");
	GetGraficSizeFromData(m_p2Name);

	m_p1Name.handle = LoadGraph("UI/CharaFontShartel.png");
	GetGraficSizeFromData(m_p1Name);

	// アニメーションのロード（メモリの解放はアニメーションクラス内で行われる）
	// アニメーションデータの定義リスト
	const std::map<WinAnimID, WinAnimData> kAnimConfig =
	{
		// {データID、{ハンドル、1コマの表示フレーム、全体コマ数、ループするかどうか}}を示す
		// Nova
		{WinAnimID::Nova, {"anim/idle.png", 10, 7, true}},

		//Shartel
		{WinAnimID::Shartel, {"chara/shartel/idle.png", 10, 9, true}}
	};

	// データをもとにAnimationクラスを生成してマップに登録
	for (const auto& [id, data] : kAnimConfig)
	{
		int handle = LoadGraph(data.filePath);
		// マップ内に新しくAnimationを作り、Initを呼ぶ
		m_animations[id].Init(handle, data.animFrame, data.animNum, data.isLoop);
	}

	const WinAnimID animID = m_WinChara == Chara::Shartel
		? WinAnimID::Shartel
		: WinAnimID::Nova;
	m_pCurrentAnim = &m_animations[animID];
	m_pCurrentAnim->Reset();
}

void WinChara::Update()
{
	// アニメーションの更新処理
	m_pCurrentAnim->Update();
}

void WinChara::Draw(float _posX, float _posY, bool _isTurn)
{
	// 選択キャラクターの描画
	m_pCurrentAnim->DrawCenter(_posX, _posY, _isTurn);

	// 現在のキャラクターと名前を一致させる
	WinGraficData currentCharaName = m_p2Name;
	if (m_WinChara == Chara::Shartel) currentCharaName = m_p1Name;

	// 名前の描画
	//DrawCenter(currentCharaName, _posX, _posY - kDisCharaGraphToNamePos, false);
}
void WinChara::DrawCenter(WinGraficData _data, float _posX, float _posY, bool _isTurn)
{
	// 画像サイズが0ならリターンする
	if (_data.sizeX <= 0 || _data.sizeY <= 0) return;

	int x = static_cast<int>(_posX);
	int y = static_cast<int>(_posY);
	int centerX = x - _data.sizeX / 2;
	int centerY = y - _data.sizeY / 2;

	DrawGraph(centerX, centerY, _data.handle, true);
}

void WinChara::GetGraficSizeFromData(WinGraficData& _data)
{
	GetGraphSize(_data.handle, &_data.sizeX, &_data.sizeY);
}

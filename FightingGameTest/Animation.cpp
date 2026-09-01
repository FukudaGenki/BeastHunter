#include "Animation.h"
#include"DxLib.h"

Animation::Animation()
{

}

Animation::~Animation()
{
	if(m_animHandle != 0)DeleteGraph(m_animHandle);
}

void Animation::Init(int _handle, int _animFrame, int _animNum, bool _isLoop)
{
	// アニメハンドルの取得
	m_animHandle = _handle;
	// 1枚の画像を何フレーム表示するか
	m_animFrame = _animFrame;
	// アニメのコマ数の取得
	if (_animNum <= 0)
	{
		m_animNum = 1;
	}
	else
	{
		m_animNum = _animNum;
	}
	// アニメーションの全体フレーム
	m_allFrame = _animFrame * _animNum;
	//読み込んだ画像の高さと横幅を取得
	int sizeX = 0, sizeY = 0;
	GetGraphSize(m_animHandle, &sizeX, &sizeY);
	// １アニメの横幅を計算
	m_animSizeX = sizeX / m_animNum;
	// １アニメの縦幅を取得
	m_animSizeY = sizeY;
	// アニメーションをループするかどうかを取得
	m_isLoop = _isLoop;
	m_isEnd = false;
}

void Animation::Draw(float _posX, float _posY, bool _isTurn)
{
	// アニメーションの描画
	DrawRectGraph(
		static_cast<int>(_posX), static_cast<int>(_posY),//位置
		m_animSizeX * m_animIdx,0,//描画位置の左上の座標
		m_animSizeX, m_animSizeY, // 描画するアニメのサイズ
		m_animHandle, // 描画するアニメーションのハンドル
		true, // 画像の透明度
		_isTurn // 画像反転の有無
	);
}

void Animation::DrawCenter(float _posX, float _posY, bool _isTurn)
{
	DrawRectRotaGraph
	(
		static_cast<int>(_posX), static_cast<int>(_posY), // 位置
		m_animSizeX * m_animIdx, 0, // 描画位置の左上座標
		m_animSizeX, m_animSizeY, // アニメーションのサイズ
		1.0f, 0.0f, // 拡大率、ラジアン
		m_animHandle, // アニメーションのハンドル
		true, // 透明
		_isTurn // 反転の有無
	);
}

void Animation::Update()
{
	// フレームタイマーの更新
	m_stopAnimFrameTimer--;
	if (m_stopAnimFrameTimer < 0 || m_frameTimer == 0)  m_frameTimer++;
	if (m_isEnd) return;
	// アニメーションの更新
	if (m_frameTimer >= m_animFrame && m_stopAnimFrameTimer < 0)
	{
		m_animIdx++;
		m_frameTimer = 0;
		m_stopAnimFrameTimer = -1;
	}
	
	
	if (m_animIdx >= m_animNum)
	{
		if (m_isLoop)
		{
			// 最終フレームを超えた時１フレーム目に戻す
			m_animIdx = 0;
		}
		else
		{
			// ループを行わないならアニメーション終了とする
			m_animIdx = m_animNum - 1;
			m_isEnd = true;
		}
	}
}

void Animation::Reset()
{
	m_frameTimer = 0;
	m_animIdx = 0;
	m_isEnd = false;
}

bool Animation::RequestAnimIdx(int _animIdx)
{
	if (m_isEnd) return false;
	if (m_animIdx == _animIdx)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Animation::RequestAnimIdxAndFrameRange(int _animIdxMin, int _animIdxMax, int _frameMin, int _frameMax)
{
	if (m_isEnd) return false;
	// 範囲内にいるかどうかをチェック
	if (m_animIdx >= _animIdxMin && m_animIdx <= _animIdxMax
		&& (m_frameTimer >= _frameMin || m_animIdx != _animIdxMin)
		&& (m_frameTimer <= _frameMax || m_animIdx != _animIdxMax))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Animation::RequestAnimIdxAndFrame(int _animIdx, int _animFrame)
{
	if (m_isEnd) return false;
	if (m_animIdx == _animIdx && m_frameTimer == _animFrame)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Animation::StopAnim(int _stopFrame)
{
	m_stopAnimFrameTimer = _stopFrame;
}



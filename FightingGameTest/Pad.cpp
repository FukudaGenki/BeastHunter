#include "DxLib.h"
#include "Pad.h"

Pad::Pad()
{

}

Pad::~Pad()
{

}

void Pad::Init(int _playerIdx)
{
	m_playerIdx = _playerIdx;

	// ButtonTypeを実数に変換するためのショートカットを作成
	auto idx = [](ButtonType b) { return static_cast<int>(b);};
	// キーボードのキー
	switch (m_playerIdx)
	{
	// プレイヤー１の場合
	case 1:
		padInputID = DX_INPUT_PAD1;
		keyMap[idx(ButtonType::Right)] = KEY_INPUT_D;
		keyMap[idx(ButtonType::Left)] = KEY_INPUT_A;
		keyMap[idx(ButtonType::Up)] = KEY_INPUT_W;
		keyMap[idx(ButtonType::Down)] = KEY_INPUT_S;
		keyMap[idx(ButtonType::A)] = KEY_INPUT_Z;
		keyMap[idx(ButtonType::B)] = KEY_INPUT_X;
		keyMap[idx(ButtonType::X)] = KEY_INPUT_C;
		keyMap[idx(ButtonType::Y)] = KEY_INPUT_V;
		break;

	// プレイヤー2の場合
	case 2:
		padInputID = DX_INPUT_PAD2;
		keyMap[idx(ButtonType::Right)] = KEY_INPUT_RIGHT;
		keyMap[idx(ButtonType::Left)] = KEY_INPUT_LEFT;
		keyMap[idx(ButtonType::Up)] = KEY_INPUT_UP;
		keyMap[idx(ButtonType::Down)] = KEY_INPUT_DOWN;
		keyMap[idx(ButtonType::A)] = KEY_INPUT_U;
		keyMap[idx(ButtonType::B)] = KEY_INPUT_I;
		keyMap[idx(ButtonType::X)] = KEY_INPUT_O;
		keyMap[idx(ButtonType::Y)] = KEY_INPUT_P;
		break;
	}

	padMap[idx(ButtonType::Right)] = PAD_INPUT_RIGHT;
	padMap[idx(ButtonType::Left)] = PAD_INPUT_LEFT;
	padMap[idx(ButtonType::Up)] = PAD_INPUT_UP;
	padMap[idx(ButtonType::Down)] = PAD_INPUT_DOWN;
	padMap[idx(ButtonType::A)] = PAD_INPUT_1;
	padMap[idx(ButtonType::B)] = PAD_INPUT_2;
	padMap[idx(ButtonType::X)] = PAD_INPUT_3;
	padMap[idx(ButtonType::Y)] = PAD_INPUT_4;
}

void Pad::Update()
{
	// 前のフレームに取得したパッド情報を一つ古い情報にする
	lastPad = nowPad;
	for (int i = 0; i < 256; i++)
	{
		lastKey[i] = nowKey[i];
	}

	// 現在のパッドの情報を取得する
	nowPad = GetJoypadInputState(padInputID);
	GetHitKeyStateAll(nowKey);

	// キーが押されているかを取得する(パッドだけ)
	if (nowPad != 0) pressAnyKey = true;
	else pressAnyKey = false;

	// 入力方向を0~9で返す
	InputDirection();
}

// 使用する関数--------------------------------------------
bool Pad::Press(ButtonType _button) const
{
	int i = static_cast<int>(_button);
	// 配列から必要となるキーを取り出す
	return IsPressPad(padMap[i]) || IsPressKey(keyMap[i]);
}

bool Pad::Trigger(ButtonType _button) const
{
	int i = static_cast<int>(_button);
	// 配列から必要となるキーを取り出す
	return IsTriggerPad(padMap[i]) || IsTriggerKey(keyMap[i]);
}

bool Pad::Release(ButtonType _button) const
{
	int i = static_cast<int>(_button);
	// 配列から必要となるキーを取り出す
	return IsReleasePad(padMap[i]) || IsReleaseKey(keyMap[i]);
}
// ----------------------------------------------------------

bool Pad::IsPressPad(int key) const
{
	// 現在のフレームで押されている
	return (nowPad & key);
}

bool Pad::IsPressKey(int key) const
{
	// 現在のフレームで押されている
	return (nowKey[key] == 1);
}

bool Pad::IsTriggerPad(int key) const
{
	bool isNow = (nowPad & key);	// 現在フレーム
	bool isLast = (lastPad & key);	// 前のフレーム

	// 現在のフレームに押されていて前回のフレームに押されていない
	if (isNow && !isLast)
	{
		return true;
	}
	return false;
}

bool Pad::IsTriggerKey(int key) const
{
	return (nowKey[key] == 1 && lastKey[key] == 0);
}

bool Pad::IsReleasePad(int key) const
{
	bool isNow = (nowPad & key);	// 現在のフレーム
	bool isLast = (lastPad & key);	// 前のフレーム

	// 現在フレームに押されていなくて前回のフレームは押されていた
	if (!isNow && isLast)
	{
		return true;
	}
	return false;
}

bool Pad::IsReleaseKey(int key) const
{
	return (nowKey[key] == 0 && lastKey[key] == 1);
}

bool Pad::IsPressAnyKey() const
{
	return pressAnyKey;
}

void Pad::InputDirection()
{
	// 変数の更新
	inputUp = false;
	inputDown = false;
	inputLeft = false;
	inputRight = false;

	// 入力をチェックする
	if (Press(ButtonType::Up)) inputUp = true;
	if (Press(ButtonType::Down)) inputDown = true;
	if (Press(ButtonType::Left)) inputLeft = true;
	if (Press(ButtonType::Right)) inputRight = true;

	// 入力方向を決定
	if ((inputUp && inputDown) || (inputLeft && inputRight))
	{
		inputLever = 5;
	}
	else if (inputLeft && inputDown)
	{
		inputLever = 1;
	}
	else if (inputLeft && inputUp)
	{
		inputLever = 7;
	}
	else if (inputRight && inputDown)
	{
		inputLever = 3;
	}
	else if (inputRight && inputUp)
	{
		inputLever = 9;
	}
	else if (inputDown)
	{
		inputLever = 2;
	}
	else if (inputLeft)
	{
		inputLever = 4;
	}
	else if (inputUp)
	{
		inputLever = 8;
	}
	else if (inputRight)
	{
		inputLever = 6;
	}
	else
	{
		inputLever = 5;
	}
}

int Pad::ConvertToTenKey() const
{
	return inputLever;
}




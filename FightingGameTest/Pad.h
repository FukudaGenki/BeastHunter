#pragma once
#include <string>
#include "ButtonType.h"

/// <summary>
/// ゲームパッドの入力を管理するクラス
/// </summary>
class Pad
{
private:
	int m_playerIdx = 0;
	int padInputID = 0;
	// 前のフレームのパッド押し下げ状態
	int lastPad = 0;
	// このフレームのパッド押し下げ状態
	int nowPad = 0;
	// キーが押されているかどうか
	bool pressAnyKey = 0;
	// 入力方向を0～9で示す
	int inputLever = 5;
	// 4方向のキーの状態を示す
	bool inputUp = 0;
	bool inputDown = 0;
	bool inputLeft = 0;
	bool inputRight = 0;

	// キーボードの入力
	char nowKey[256] = {};
	char lastKey[256] = {};

	int keyMap[static_cast<int>(ButtonType::Max)] = {};
	int padMap[static_cast<int>(ButtonType::Max)] = {};

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Pad();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Pad();
	/// <summary>
	/// 初期化
	/// </summary>
	void Init(int _playerIdx);
	/// <summary>
	/// パッドの更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 押されているかどうか
	/// </summary>
	/// <param name="_button"></param>
	/// <returns></returns>
	bool Press(ButtonType _button) const;

	/// <summary>
	/// トリガーかどうか
	/// </summary>
	/// <param name="_button"></param>
	/// <returns></returns>
	bool Trigger(ButtonType _button) const;

	/// <summary>
	/// 離されたかどうか
	/// </summary>
	/// <param name="_button"></param>
	/// <returns></returns>
	bool Release(ButtonType _button) const;

	/// <summary>
	/// 押されているか
	/// </summary>
	/// <param name="key">入力キー</param>
	/// <returns>押されているかどうか</returns>
	bool IsPressPad(int key) const;

	/// <summary>
	/// キーボードが押されているか
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	bool IsPressKey(int key) const;

	/// <summary>
	/// 押された瞬間を取得
	/// </summary>
	/// <param name="key">入力キー</param>
	/// <returns>押されたかどうか</returns>
	bool IsTriggerPad(int key) const;

	/// <summary>
	/// 押された瞬間を取得
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	bool IsTriggerKey(int key) const;

	/// <summary>
	/// 離した瞬間を取得
	/// </summary>
	/// <param name="key">入力キー</param>
	/// <returns>離されたかどうか</returns>
	bool IsReleasePad(int key) const;

	/// <summary>
	/// 離された瞬間
	/// </summary>
	/// <param name="key"></param>
	/// <returns></returns>
	bool IsReleaseKey(int key) const;

	/// <summary>
	/// 何らかのキーが押されているならtrueを返す
	/// </summary>
	/// <returns>キーの状態</returns>
	bool IsPressAnyKey() const;

	/// <summary>
	/// 入力方向を管理する
	/// </summary>
	void InputDirection();

	/// <summary>
	/// 入力方向を0～9で返す関数
	/// </summary>
	/// <returns></returns>
	int ConvertToTenKey() const;
	const int GetPlayerIdx() const { return m_playerIdx; };
};
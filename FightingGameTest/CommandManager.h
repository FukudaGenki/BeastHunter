#pragma once
#include "InputBuffer.h"
#include "DxLib.h"
#include <map>
#include <vector>
#include <string_view>
#include "Pad.h"

enum class CommandID
{
	Hadou,
	Shoryu,
	Tatsumaki,
	DashForward,
	DashBack,
	// 最大数
	Max
};

// コマンドのレシピ保存や成立をチェックするクラス
class CommandManager
{
private:
	// 仮
	int isHadou = 0;

	// コマンドのレシピ登録をするリスト
	std::map<std::string_view, std::vector<int>> m_commandList;
	InputBuffer m_inputBuffer;

	// 入力履歴
	std::deque <InputLog> m_history;
	std::string m_arrow;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CommandManager();
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CommandManager();
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Init(int _pIdx);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(const Pad& _pad);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// コマンドが成立しているか確認する
	/// </summary>
	/// <param name="_rightLeft">左：-1、右：1</param>
	/// <returns></returns>
	std::string CheckCommand(int _rightLeft);
	bool IsCheckCommand(CommandID _command, int _rightLeft);
	/// <summary>
	/// 履歴からコマンドの成立をチェックする
	/// </summary>
	/// <param name="_commandDir">コマンドの方向</param>
	/// <param name="_history">履歴</param>
	/// <param name="_inputBufferFrame">入力猶予</param>
	/// <param name="_arrowOther">間違いの許容</param>
	/// <returns>成立しているかどうか</returns>
	bool CheckLeverDir(std::string_view _commandDir, std::deque <InputLog> _history, int _inputBufferFrame, bool _arrowOther);
	/// <summary>
	/// インプットのログを取得
	/// </summary>
	/// <returns></returns>
	const std::deque<InputLog>& GetHistory() const { return m_history; }
};
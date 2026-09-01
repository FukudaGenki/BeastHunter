#include "CommandManager.h"

namespace
{
	// 入力猶予フレームを設定（レバー方向を変えるまでの猶予）
	constexpr int kIBFrameDash = 8;
	constexpr int kInputBufferFrame = 10;
	// Nova
	constexpr std::string_view kDashForwardRight = "65";
	constexpr std::string_view kDashForwardLeft = "45";

	constexpr std::string_view kDashBackRight = kDashForwardLeft;
	constexpr std::string_view kDashBackLeft = kDashForwardRight;
	
	constexpr std::string_view kHadouRight = "236";
	constexpr std::string_view kHadouLeft = "214";
	
	constexpr std::string_view kTatsumakiRight = "214";
	constexpr std::string_view kTatsumakiLeft = "236";
	// Shartel
	constexpr std::string_view kShoryuRight = "623";
	constexpr std::string_view kShoryuLeft = "421";

}

CommandManager::CommandManager()
{
}

CommandManager::~CommandManager()
{
}

void CommandManager::Init(int _pIdx)
{
	m_inputBuffer.Init(_pIdx);
	// コマンドのレシピを登録
	// 入力方向を後ろ（新しいもの）から取得するため反対向きに入れる
	// 前ダッシュ
	m_commandList[kDashForwardRight] = { 5, 6 }; // 右向き
	m_commandList[kDashForwardLeft] = { 5, 4 }; // 左向き
	// 波動拳
	m_commandList[kHadouRight] = { 6, 3, 2 }; // 右向き
	m_commandList[kHadouLeft] = { 4, 1, 2 }; // 左向き
	// 竜巻
	m_commandList[kTatsumakiRight] = { 4, 1, 2 }; // 右向き
	m_commandList[kTatsumakiLeft] = { 6, 3, 2 }; // 左向き
	// 昇竜
	m_commandList[kShoryuRight] = { 3, 2, 6 };
	m_commandList[kShoryuLeft] = { 1, 2, 4 };
}

void CommandManager::Update(const Pad& _pad)
{
	m_inputBuffer.Update(_pad);
}

void CommandManager::Draw()
{
	m_inputBuffer.Draw();
}

std::string CommandManager::CheckCommand(int _rightLeft)
{
	// コマンドの成立プラス左右の向きをチェックしていく
	if ((CheckLeverDir(kHadouRight, m_inputBuffer.GetHistory(), kInputBufferFrame, true) && _rightLeft == 1) ||
		(CheckLeverDir(kHadouLeft, m_inputBuffer.GetHistory(), kInputBufferFrame, true) && _rightLeft == -1))
	{
		// 波動拳
		return "Hadou";
	}

	else if ((CheckLeverDir(kShoryuRight, m_inputBuffer.GetHistory(), kInputBufferFrame, true) && _rightLeft == 1) ||
		(CheckLeverDir(kShoryuLeft, m_inputBuffer.GetHistory(), kInputBufferFrame, true) && _rightLeft == -1))
	{
		// 昇竜
		return "Shoryu";
	}

	else if ((CheckLeverDir(kTatsumakiRight, m_inputBuffer.GetHistory(), kInputBufferFrame, true) && _rightLeft == 1) ||
		(CheckLeverDir(kTatsumakiLeft, m_inputBuffer.GetHistory(), kIBFrameDash, true) && _rightLeft == -1))
	{
		// 竜巻
		return "Tatsumaki";
	}

	else if ((CheckLeverDir(kDashForwardRight, m_inputBuffer.GetHistory(), kIBFrameDash, false) && _rightLeft == 1) ||
		(CheckLeverDir(kDashForwardLeft, m_inputBuffer.GetHistory(), kIBFrameDash, false) && _rightLeft == -1))
	{
		// 前ダッシュ
		return "DashForward";
	}

	// 前ダッシュの逆コマンドを設定してる
	else if ((CheckLeverDir(kDashForwardLeft, m_inputBuffer.GetHistory(), kIBFrameDash, false) && _rightLeft == 1) ||
		(CheckLeverDir(kDashForwardRight, m_inputBuffer.GetHistory(), kIBFrameDash, false) && _rightLeft == -1))
	{
		// 前ダッシュ
		return "DashBack";
	}

	return "None";
}

bool CommandManager::IsCheckCommand(CommandID _command, int _rightLeft)
{
	// 左右の向きをboolに変換
	bool left = _rightLeft == -1;
	bool right = !left;

	// 波動拳
	bool hadouLeft = CheckLeverDir(kHadouLeft, m_inputBuffer.GetHistory(), kInputBufferFrame, true) && left;
	bool hadouRight = CheckLeverDir(kHadouRight, m_inputBuffer.GetHistory(), kInputBufferFrame, true) && right;

	// 昇竜
	bool shoryuLeft = CheckLeverDir(kShoryuLeft, m_inputBuffer.GetHistory(), kInputBufferFrame, true) && left;
	bool shoryuRight = CheckLeverDir(kShoryuRight, m_inputBuffer.GetHistory(), kInputBufferFrame, true) && right;
	bool noShoryuLeft = CheckLeverDir(kHadouLeft, m_inputBuffer.GetHistory(), kInputBufferFrame, false) && left;
	bool noShoryuRight = CheckLeverDir(kHadouRight, m_inputBuffer.GetHistory(), kInputBufferFrame, false) && right;

	// 竜巻
	bool tatsuLeft = CheckLeverDir(kTatsumakiLeft, m_inputBuffer.GetHistory(), kInputBufferFrame, true) && left;
	bool tatsuRight = CheckLeverDir(kTatsumakiRight, m_inputBuffer.GetHistory(), kInputBufferFrame, true) && right;

	// 前ステップ
	bool dashForwardLeft = CheckLeverDir(kDashForwardLeft, m_inputBuffer.GetHistory(), kInputBufferFrame, false) && left;
	bool dashForwardRight = CheckLeverDir(kDashForwardRight, m_inputBuffer.GetHistory(), kInputBufferFrame, false) && right;

	// 後ろステップ
	bool dashBackLeft = CheckLeverDir(kDashBackLeft, m_inputBuffer.GetHistory(), kInputBufferFrame, false) && left;
	bool dashBackRight = CheckLeverDir(kDashBackRight, m_inputBuffer.GetHistory(), kInputBufferFrame, false) && right;

	// 指定されたコマンドの条件をもってきてTrueFalseを返す。コマンドを指定しなければ優先順位がついてしまうため
	switch (_command)
	{
	case CommandID::Hadou:

		if (hadouLeft || hadouRight) return true;
		break;

	case CommandID::Shoryu:
		// 昇竜の時、波動でも誤爆してしまうので、波動コマンドになっているときは昇竜ではなくすようにした
		if (shoryuLeft && !noShoryuLeft || shoryuRight && !noShoryuRight) return true;
		break;

	case CommandID::Tatsumaki:

		if (tatsuLeft || tatsuRight) return true;
		break;

	case CommandID::DashForward:

		if (dashForwardLeft || dashForwardRight) return true;
		break;

	case CommandID::DashBack:

		if (dashBackLeft || dashBackRight) return true;
		break;
	}
	return false;
}

bool CommandManager::CheckLeverDir(std::string_view _commandDir, std::deque <InputLog> _history, int _inputBufferFrame, bool _arrowOther)
{
	// 履歴が足りない場合は失敗とする
	if (_history.size() < m_commandList[_commandDir].size())
	{
		return false;
	}
	
	int commandIdx = 0; // 要素の数をカウント
	int elapsedFrame = 0; // 次の要素が見つかるまでのフレームをカウント

	// 一定フレーム間隔内でレバー方向が変更されているかを確かめる
	// 直近のフレームから確認していく
	// 入力履歴の分繰り返す
	for (int i = 0; i < _history.size(); i++)
	{
		int historyIdx = static_cast<int>(_history.size()) - 1 - i;

		// まず猶予フレームを計算する
		// 最初の入力以外で計算を行う
		if (commandIdx < m_commandList[_commandDir].size() - 1)
		{
			elapsedFrame += _history[historyIdx].frameCount;
			// 猶予フレームを超えていた場合失敗
			if (elapsedFrame > _inputBufferFrame)
			{
				return false;
			}
		}

		// レバー方向のチェック
		// 成功した場合
		if (m_commandList[_commandDir][commandIdx] == _history[historyIdx].leverDirection)
		{
			// 猶予フレームカウンターのリセット
			elapsedFrame = 0;
			// 次に進む
			commandIdx++;
			// コマンドがすべて揃ったとき成功
			if (commandIdx == m_commandList[_commandDir].size())
			{
				return true;
			}
		}
		// 方向入力の間違いを許容しない場合
		else if (!_arrowOther && commandIdx > 0)
		{
			// 前回成功した方向から方向が変わってるなら実行
			if (_history[historyIdx].leverDirection != m_commandList[_commandDir][commandIdx - 1])
			{
				return false;
			}
		}
	}

	// 履歴のすべてをチェックしても揃わなければ失敗
	return false;
}






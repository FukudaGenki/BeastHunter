#pragma once
#include "Player.h"

class Nova : public Player
{
protected:
	/// <summary>
	/// アニメーションのハンドルやフレーム数などの情報をセット
	/// </summary>
	void SetAnimData() override;
	/// <summary>
	/// 当たり判定の情報をセット
	/// </summary>
	void SetColData() override;
	/// <summary>
	/// 現在のアニメーションや動きを設定する
	/// </summary>
	void SetCurrentAction(StateID _state) override;
	/// <summary>
	/// アクションの動きを変えたり、フレーム指定でアクションに変更を加える
	/// </summary>
	void SetActionDetail() override;
	/// <summary>
	/// 攻撃など一時的に生成する当たり判定の処理
	/// </summary>
	void SetAttackCol() override;
	StateID InputAction(const Pad& _pad) override;
};
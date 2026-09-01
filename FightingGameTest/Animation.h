#pragma once
class Animation
{
private:
	// メンバ変数
	int m_animHandle = 0;
	int m_frameNum = 0;
	int m_animSizeX = 0;
	int m_animSizeY = 0;
	int m_animFrame = 0;
	int m_animNum = 0;
	int m_animIdx = 0;
	int m_frameTimer = 0;
	bool m_isEnd = true;
	bool m_isLoop = false;
	int m_allFrame = 0;
	int m_stopAnimFrameTimer = 0;

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Animation();
	/// <summary>
	///  デストラクタ
	/// </summary>
	~Animation();
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="_handle">画像</param>
	/// <param name="_animFrame">１アニメーションのフレーム数</param>
	/// <param name="_animNum">アニメーションの数</param>
	void Init(int _handle, int _animFrame, int _animNum, bool _isLoop);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw(float _posX, float _posY, bool _isTurn);
	/// <summary>
	/// 中心座標を基準として描画する
	/// </summary>
	/// <param name="_posX">x中心座標</param>
	/// <param name="_posY">y中心座標</param>
	/// <param name="_isTurn">反転するかどうか</param>
	void DrawCenter(float _posX, float _posY, bool _isTurn);
	/// <summary>
	/// アニメーションの画像番号とフレームタイマーの初期化を行う
	/// </summary>
	void Reset();
	/// <summary>
	/// アニメーションが終了したかどうかを取得
	/// </summary>
	/// <returns>アニメーションの終了</returns>
	bool IsEnd() const { return m_isEnd; }
	/// <summary>
	/// アニメーションの全体フレームを返す
	/// </summary>
	/// <returns>全体フレーム</returns>
	int GetAllFrame() const { return m_allFrame; }
	/// <summary>
	/// 現在のアニメーションのコマ数を返す
	/// </summary>
	/// <returns>animIdx</returns>
	int GetCurrentAnimIdx() const { return m_animIdx; }
	/// <summary>
	/// 指定されたAnimFrame内かどうか
	/// </summary>
	/// <param name="_animIdx">指定するAnimIdx</param>
	/// <returns></returns>
	bool RequestAnimIdx(int _animIdx);
	/// <summary>
	/// 現在のコマ数のフレーム内にいるかどうかを返す
	/// </summary>
	/// <param name="_animIdx">コマ数</param>
	/// <param name="_frameMin">フレーム最小値</param>
	/// <param name="_frameMax">フレーム最大値</param>
	/// <returns></returns>
	bool RequestAnimIdxAndFrameRange(int _animIdxMin, int _animIdxMax, int _frameMin, int _frameMax);
	/// <summary>
	/// アニメーションの指定したフレームを返す
	/// </summary>
	/// <param name="_animIdx"></param>
	/// <param name="_animFrame"></param>
	/// <returns>指定したフレームかどうか</returns>
	bool RequestAnimIdxAndFrame(int _animIdx, int _animFrame);
	/// <summary>
	/// アニメーションを指定したフレーム数止める
	/// </summary>
	/// <param name="_stopFrame">止めるフレーム数</param>
	void StopAnim(int _stopFrame);
};
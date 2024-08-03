#pragma once
class TitleScene {
public:
	void Initialize();
	void Update();
	void Draw();

	public:
	//終了フラグ
	bool finished_ = false;
	//デスフラグのgetter
	bool isFinished() const { return finished_; }
};

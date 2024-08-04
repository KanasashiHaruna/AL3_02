#include "TitleScene.h"
#include "Input.h"
 
void TitleScene::Initialize() {


}

void TitleScene::Update() {

	if (Input::GetInstance()->PushKey(DIK_T)) {
		finished_ = true;
	}
}

void TitleScene::Draw() {


}
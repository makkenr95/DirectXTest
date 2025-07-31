#pragma once
class CCamera;
class CScene;
class CModelInstance;

class Game
{
public:
	void Init();
	void Update(float aDeltaTime);
private:
	CScene* myScene = nullptr;
	CCamera* myCamera = nullptr;

	CModelInstance* model2 = nullptr;
	CModelInstance* model3 = nullptr;
	CModelInstance* head0 = nullptr;
};


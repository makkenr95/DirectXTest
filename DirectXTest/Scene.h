#pragma once 
#include <vector>

class CCamera;
class CSpotLight;
class CPointLight;
class CModelInstance;
class CEnvironmentLight;

class CScene
{
public:
	~CScene();
	CCamera* GetMainCamera();
	std::vector<CPointLight*> CullLights(CModelInstance* anInstance) const;
	std::vector<CSpotLight*> CullSpotLights(CModelInstance* anInstance) const;
	static CScene* GetInstance();
	bool Init();
	bool AddInstance(CModelInstance* aModelInstance);
	bool AddInstance(CCamera* aCameraInstance);
	void AddInstance(CEnvironmentLight* const aEnvironmentLight);
	bool AddInstance(CPointLight* aPointLight);
	bool AddInstance(CSpotLight* aSpotLight);
	std::vector<CModelInstance*> CullModels(CCamera* aCamera) const;
	void SetMainCamera(CCamera* aMainCamera);
	[[nodiscard]] CEnvironmentLight* GetEnvironmentLight() const;
private:
	CScene();
	static CScene* ourInstance;
	std::vector<CModelInstance*> myModels;
	std::vector<CCamera*> myCameras;
	CCamera* myMainCamera = nullptr;
	// std::array<CPointLight*, 8> myPointLights;
	std::vector<CPointLight*> myPointLights;
	std::vector<CSpotLight*> mySpotLights;

	CEnvironmentLight* myEnvironmentLight = nullptr;
};


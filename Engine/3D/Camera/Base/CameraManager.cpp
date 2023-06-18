#include "CameraManager.h"

CameraManager* CameraManager::GetInstance()
{
    static CameraManager* instance;
    return instance;
}

void CameraManager::Update()
{
}

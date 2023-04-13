#include "DirLight.h"
#include "NormalCamera.h"

void DirLight::SetShadow(bool shadowflag)
{
	//	dirtyflag‚ªon‚¾‚Á‚½‚ç
	if (shadowing != shadowflag) {
		shadowing = shadowflag;

		if (shadowing) {
			lightCamera = std::make_unique<NormalCamera>();
			lightCamera->Initialize(center - lightdir * 50.0f , center, Vector3D{ 0.0f,1.0f,0.0f });
		}
		else {
			lightCamera.release();
		}
	}
}

#include "TouchableObject.h"
#include "MeshCollider.h"
#include "CollisionAttribute.h"

TouchableObject* TouchableObject::Create(IModel* model)
{
	TouchableObject* instance = new TouchableObject();
	if (instance == nullptr) {
		return nullptr;
	}

	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	return instance;
}

bool TouchableObject::Initialize(IModel* model_)
{
	Object3D::Initialize();

	SetModel(model_);

	// �R���C�_�[�̒ǉ�
	MeshCollider* collider_ = new MeshCollider;
	collider_->ConstructTriangles(this->GetModel());
	collider_->SetAttribute(COLLISION_ATTR_LANDSHAPE);
	SetCollider(collider_);

	return true;
}

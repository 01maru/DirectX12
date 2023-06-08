//#include "LoadingObject.h"
//#include "TextureManager.h"
//#include "Window.h"
//#include "Easing.h"
//
//void LoadingObject::Initialize()
//{
//
//    //loadModel = std::make_unique<ObjModel>("sapling");
//    //loadObj.reset(Object3D::Create(loadModel.get()));
//    //loadObj->SetSilhouetteColor({ 1.0f,1.0f,1.0f,1.0f });
//    //loadObj->SetScale({ 50.0f,50.0f,50.0f });
//    //loadObj->SetPosition({ Window::window_width / 2.0f - 100.0f,-Window::window_height / 2.0f + 50.0f,0.0f });
//
//    //camera = std::make_unique<ObjCamera2D>();
//}
//
//void LoadingObject::Update()
//{
//    if (!loading_ && !fadeIn_) {
//        //  ���[�f�B���O������Ȃ�fadeIn�ς݂�������
//        if (easeCount_ > 0) {
//            easeCount_--;
//
//            float alphaColor = Easing::EaseOut(1.0f, 0.0f, 1.0f - (float)easeCount_ / sEaseMaxCount, 4);
//            
//            loadSprite_->SetColor({ 1.0f,1.0f,1.0f,alphaColor });
//            //loadObj_->SetSilhouetteColor({ 1.0f,1.0f,1.0f,alphaColor });
//        }
//    }
//    else {
//        bool isEasing = easeCount_ < sEaseMaxCount;
//
//        if (isEasing) {
//            easeCount_++;
//
//            float alphaColor = Easing::EaseOut(0.0f, 1.0f, (float)easeCount_ / sEaseMaxCount, 4);
//
//            loadSprite_->SetColor({ 1.0f,1.0f,1.0f,alphaColor });
//            //loadObj->SetSilhouetteColor({ 1.0f,1.0f,1.0f,alphaColor });
//        }
//
//        //  ���ߒl1.0�ɂȂ�����false��
//        if (!isEasing)  fadeIn_ = false;
//
//        //camera->MatUpdate();
//        float rot_ = loadSprite_->GetRotation();
//        loadSprite_->SetRotation(rot_ - 0.1f);
//        loadSprite_->MatUpdate();
//    }
//
//    ////  ���f���\�����͉�]������
//    //if (easeCount > 0) {
//    //    Vector3D rot = loadObj->GetRotation();
//    //    rot.y -= 0.1f;
//    //    loadObj->SetRotation(rot);
//    //    loadObj->MatUpdate(camera.get());
//    //}
//}
//
//void LoadingObject::Draw()
//{
//    if (easeCount_ > 0) {
//        //loadObj->Draw(PipelineManager::GetInstance()->GetPipeline("Loading"), true);
//
//        loadSprite_->Draw(true);
//    }
//}
//
//void LoadingObject::SetIsLoading(bool loading)
//{
//    loading_ = loading;
//    if (loading_) fadeIn_ = true;
//}

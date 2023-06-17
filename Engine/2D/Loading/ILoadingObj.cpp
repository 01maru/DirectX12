#include "ILoadingObj.h"

void ILoadingObj::SetIsLoading(bool loading)
{
    loading_ = loading;
    if (loading_) fadeIn_ = true;
}

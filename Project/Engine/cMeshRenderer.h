#pragma once
#include "cRenderComponent.h"

#include "Ptr.h"

class cMeshRenderer :
    public cRenderComponent
{
    CLONE(cMeshRenderer);
public:
    cMeshRenderer();
    //복사할 멤버변수가 없으므로 복사생성 필요 없음.
    ~cMeshRenderer();

public:

public:
    virtual void finaltick() override;
    virtual bool render() override;

    virtual void cleanup() override {}

private:

public:
    



};

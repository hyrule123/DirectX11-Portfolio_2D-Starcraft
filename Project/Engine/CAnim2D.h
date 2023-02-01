#pragma once
#include "CEntity.h"

#include "ptr.h"

class CTexture;
class CAnimator2D;

class CAnim2D :
    public CEntity
{
public:
    CAnim2D();
    ~CAnim2D();
    CLONE(CAnim2D);
    friend class CAnimator2D;

public:
    void finaltick();

private:
    CAnimator2D* m_pOwner;
    vector<tAnim2DFrm>  m_vecFrm;
    Vec2                m_vBackSize;
    Ptr<CTexture>       m_AtlasTex;

    int                 m_iCurFrm;
    float               m_fTime;

    bool                m_bFinish;

public:
    void Create(const string& _strAnimName, Ptr<CTexture> _AtlasTex, Vec2 _vLeftTop, Vec2 _vSlice, Vec2 _vBackSize, int _FrameCount, int _FPS);
    const tAnim2DFrm& GetCurFrame() { return m_vecFrm[m_iCurFrm]; }
    Vec2 GetBackSize() { return m_vBackSize; }
    Ptr<CTexture> GetAtlasTex() { return m_AtlasTex; }

    bool IsFinish() { return m_bFinish; }
    void Reset()
    {
        m_iCurFrm = 0;
        m_fTime = 0.f;
        m_bFinish = false;
    }
};

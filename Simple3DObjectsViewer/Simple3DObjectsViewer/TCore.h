#pragma once

#include "COMMON/OglForCLI.h"
#include "COMMON/tmath.h"
#include "COMMON/tmesh.h"

//class TCore 
// have two 3D objects (obj)
// mouse listener 
// draw scene function

#pragma unmanaged


//NOTE : 
// - object 2 �� ground truth 
// - object 1 �� object 2�ɍ��킹��
// - object 1 ���� object 2�ւ̋������v�Z����





// object1 �� object2�̍������v�Z�����ۂɁC�����������邽�߂̏���ێ�
// object1�̒��_id / ���_�ʒu / object2��̍ŋߖT�_ / ������ێ�����
class TVtxDistInfo
{
public:
  int     m_idx; // ��surface��̒��_id
  double  m_dist; // �Ή���surface�ւ̍ŏ�����
  EVec3f  m_pSrc; // �Ή����̒��_�ʒu
  EVec3f  m_pTgt; // �Ή���surface��̈ʒu

  ~TVtxDistInfo() {}
  TVtxDistInfo() {}
  void Copy(const TVtxDistInfo& src) {
    m_idx = src.m_idx;
    m_dist = src.m_dist;
    m_pSrc = src.m_pSrc;
    m_pTgt = src.m_pTgt;
  }

  TVtxDistInfo(const TVtxDistInfo& src) { Copy(src); }
  TVtxDistInfo& operator=(const TVtxDistInfo& src) { Copy(src); return *this; }

  TVtxDistInfo(int idx, double dist, EVec3f pSrc, EVec3f pTgt) {
    m_idx = idx;
    m_dist = dist;
    m_pSrc = pSrc;
    m_pTgt = pTgt;
  }
};






class TCore
{
  bool m_bL, m_bR, m_bM;
  TMesh m_obj1, m_obj2;
  
  std::vector<TVtxDistInfo> m_vtxinfo_1to2; 
  
  TCore();
public:
  
  static TCore* GetInst() {
    static TCore p;
    return &p;
  }

  void LoadModel1(std::string path) { m_obj1.initialize(path.c_str()); }
  void LoadModel2(std::string path) { m_obj2.initialize(path.c_str()); }

  void MouseMove(EVec2i p, OglForCLI* ogl);
  void LBtnDown(EVec2i p, OglForCLI* ogl);
  void RBtnDown(EVec2i p, OglForCLI* ogl);
  void MBtnDown(EVec2i p, OglForCLI* ogl);
  void LBtnUp(EVec2i p, OglForCLI* ogl);
  void RBtnUp(EVec2i p, OglForCLI* ogl);
  void MBtnUp(EVec2i p, OglForCLI* ogl);
  void DrawScene(OglForCLI* ogl);
  void KeyDown(int keycode);


  void NormalizeSize_MeanDistFromGC();
  void NormalizeSize_DistFathestVerts();
  void CalcDifferenceOfTwoObjs();
  void AlignObjsByICP();

};



inline bool isCtrKeyOn() { return GetKeyState(VK_CONTROL) < 0; }
inline bool isSpaceKeyOn() { return GetKeyState(VK_SPACE) < 0; }
inline bool isShiftKeyOn() { return GetKeyState(VK_SHIFT) < 0; }
inline bool isAltKeyOn() { return GetKeyState(VK_MENU) < 0; }


#pragma managed

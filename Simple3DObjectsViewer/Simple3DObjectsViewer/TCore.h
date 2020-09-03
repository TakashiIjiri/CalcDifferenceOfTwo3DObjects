#pragma once

#include "COMMON/OglForCLI.h"
#include "COMMON/tmath.h"
#include "COMMON/tmesh.h"




#pragma unmanaged

inline bool isCtrKeyOn() 
{ 
  return GetKeyState(VK_CONTROL) < 0; 
}


inline bool isSpaceKeyOn() 
{ 
  return GetKeyState(VK_SPACE) < 0; 
}


inline bool isShiftKeyOn() 
{ 
  return GetKeyState(VK_SHIFT) < 0; 
}


inline bool isAltKeyOn() 
{ 
  return GetKeyState(VK_MENU) < 0; 
}


/////////////////////////////////////////////////////////
// class TVtxDistInfo 
// obj1 と obj2の差分計算時の情報を保持
// obj1の頂点id / obj1の頂点位置 / obj2上の最近傍点 / 距離
class TVtxDistInfo
{
public:
  int     m_idx ; // 元surface上の頂点id
  double  m_dist; // 対応先surfaceへの最小距離
  EVec3f  m_pSrc; // 対応元の頂点位置
  EVec3f  m_pTgt; // 対応先surface上の位置

  ~TVtxDistInfo() {}
  TVtxDistInfo() {}

  void Copy(const TVtxDistInfo& src) 
  {
    m_idx = src.m_idx;
    m_dist = src.m_dist;
    m_pSrc = src.m_pSrc;
    m_pTgt = src.m_pTgt;
  }

  TVtxDistInfo(const TVtxDistInfo& src) 
  { 
    Copy(src); 
  }
  
  TVtxDistInfo& operator=(const TVtxDistInfo& src) 
  { 
    Copy(src); 
    return *this; 
  }

  TVtxDistInfo(int idx, double dist, EVec3f pSrc, EVec3f pTgt) 
  {
    m_idx = idx;
    m_dist = dist;
    m_pSrc = pSrc;
    m_pTgt = pTgt;
  }
};




//////////////////////////////////////////////
//class TCore 
// 2個の3Dメッシュの管理 TMesh m_obj1, m_obj2
// Mouse Listener 
// 3D scene rendering with OGL
//
// note : obj1 is supposed to be a ground truth
//      : obj2 is supposed to be generated with some algorithms
//

class TCore
{
  //mouse btn state
  bool m_bL;
  bool m_bR;
  bool m_bM;
  
  //two objects
  TMesh m_obj1, m_obj2;

  //diff info
  std::vector<TVtxDistInfo> m_vtxinfo_1to2;

  //singleton pattern  
private:
  TCore();

public:  
  static TCore* GetInst() 
  {
    static TCore p;
    return &p;
  }

  void LoadModel1(std::string path) 
  { 
    m_obj1.initialize(path.c_str()); 
  }
  
  void LoadModel2(std::string path) 
  { 
    m_obj2.initialize(path.c_str()); 
  }

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




#pragma managed

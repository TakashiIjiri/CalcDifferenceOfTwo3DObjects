#pragma once

#include "COMMON/OglForCLI.h"
#include "COMMON/tmath.h"
#include "COMMON/tmesh.h"

//class TCore 
// have two 3D objects (obj)
// mouse listener 
// draw scene function

#pragma unmanaged


class TCore
{
  bool m_bL, m_bR, m_bM;
  TMesh m_obj1, m_obj2;

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
};


#pragma managed

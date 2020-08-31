#include "pch.h"
#include "MainForm.h"
#include "TCore.h"


#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glu32.lib" )
#pragma comment( lib, "gdi32.lib" )
#pragma comment( lib, "User32.lib" )
#pragma comment( lib, "glew32.lib" )



using namespace std;
using namespace Simple3DObjectsViewer;


MainForm::MainForm(void)
{
  InitializeComponent();
  m_ogl = new OglForCLI(GetDC((HWND)m_panel->Handle.ToPointer()));
  m_ogl->SetCam(EVec3f(0, 0, 5), EVec3f(0, 0, 0), EVec3f(0, 1, 0));
  m_ogl->SetBgColor(0.3f, 0.3f, 0.3f, 0.5f);

}



System::Void MainForm::m_panel_MouseDown(
    System::Object^ sender, 
    System::Windows::Forms::MouseEventArgs^ e)
{
  if (e->Button == System::Windows::Forms::MouseButtons::Left)
    TCore::GetInst()->LBtnDown(EVec2i(e->X, e->Y), m_ogl);
  if (e->Button == System::Windows::Forms::MouseButtons::Middle)
    TCore::GetInst()->MBtnDown(EVec2i(e->X, e->Y), m_ogl);
  if (e->Button == System::Windows::Forms::MouseButtons::Right)
    TCore::GetInst()->RBtnDown(EVec2i(e->X, e->Y), m_ogl);

}

System::Void MainForm::m_panel_MouseMove(
    System::Object^ sender, 
    System::Windows::Forms::MouseEventArgs^ e) 
{
  TCore::GetInst()->MouseMove(EVec2i(e->X, e->Y), m_ogl);
}


System::Void MainForm::m_panel_MouseUp(
    System::Object^ sender, 
    System::Windows::Forms::MouseEventArgs^ e)
{
  if (e->Button == System::Windows::Forms::MouseButtons::Left)
    TCore::GetInst()->LBtnUp(EVec2i(e->X, e->Y), m_ogl);
  if (e->Button == System::Windows::Forms::MouseButtons::Middle)
    TCore::GetInst()->MBtnUp(EVec2i(e->X, e->Y), m_ogl);
  if (e->Button == System::Windows::Forms::MouseButtons::Right)
    TCore::GetInst()->RBtnUp(EVec2i(e->X, e->Y), m_ogl);
}

System::Void MainForm::m_panel_Paint(
    System::Object^ sender, 
    System::Windows::Forms::PaintEventArgs^ e) 
{
  RedrawMainPanel();
}


System::Void MainForm::m_panel_Resize(
    System::Object^ sender, 
    System::EventArgs^ e) 
{

}



System::Void MainForm::MainForm_KeyDown(
    System::Object^ sender, 
    System::Windows::Forms::KeyEventArgs^ e) 
{
  TCore::GetInst()->KeyDown((int)e->KeyCode);
}








static void initializeLights()
{
  GLfloat pos0[4] = { 0,0, 3000,1 };
  GLfloat pos1[4] = { 0,0,-3000,1 };
  GLfloat pos2[4] = { 3000,-3000, 3000,1 };
  GLfloat dir0[3] = { 0,0,-1 };
  GLfloat dir1[3] = { 0,0, 1 };
  GLfloat dir2[3] = { -1,1,-1 };


  GLfloat ambi0[3] = { 0.5f,0.5f,0.5f };
  GLfloat ambi1[3] = { 0,0,0 };
  GLfloat ambi2[3] = { 0,0,0 };
  GLfloat diff0[3] = { 0.5f,0.5f,0.5f };
  GLfloat diff1[3] = { 0.5f,0.5f,0.5f };
  GLfloat diff2[3] = { 0.5f,0.5f,0.5f };
  GLfloat spec0[3] = { 0.3f,0.3f,0.3f };
  GLfloat spec1[3] = { 0.3f,0.3f,0.3f };
  GLfloat spec2[3] = { 0.3f,0.3f,0.3f };

  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_POSITION, pos0);
  glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, dir0);
  glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 180.0);
  glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambi0);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
  glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);

  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_POSITION, pos1);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir1);
  glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 180.0);
  glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 0);
  glLightfv(GL_LIGHT1, GL_AMBIENT, ambi1);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, diff1);
  glLightfv(GL_LIGHT1, GL_SPECULAR, spec1);

  glEnable(GL_LIGHT2);
  glLightfv(GL_LIGHT2, GL_POSITION, pos2);
  glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, dir2);
  glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 180.0);
  glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 0);

  glLightfv(GL_LIGHT2, GL_AMBIENT, ambi2);
  glLightfv(GL_LIGHT2, GL_DIFFUSE, diff2);
  glLightfv(GL_LIGHT2, GL_SPECULAR, spec2);
}




void MainForm::RedrawMainPanel()
{
  // unit : [m]
  float  nearDist = 0.1f;
  float  farDist = 100.0f;

  m_ogl->OnDrawBegin(m_panel->Width, m_panel->Height, 45.0, nearDist, farDist);

  initializeLights();
  TCore::GetInst()->DrawScene(m_ogl);

  m_ogl->OnDrawEnd();

}

using namespace System;
using namespace System::IO;
using namespace System::Runtime::InteropServices;

static bool ShowFileOpenDlgToGetPath(
  const std::string &filter,
  std::string &fname
)
{
  OpenFileDialog^ dlg = gcnew OpenFileDialog();
  dlg->Filter = gcnew String(filter.c_str());
  dlg->Multiselect = false;

  if (dlg->ShowDialog() == System::Windows::Forms::DialogResult::Cancel) return false;

  IntPtr mptr = Marshal::StringToHGlobalAnsi(dlg->FileName);
  fname = static_cast<const char*>(mptr.ToPointer());
  return true;

}




System::Void MainForm::m_btn_loadobjcts_Click(
    System::Object^ sender, 
    System::EventArgs^ e)
{

  string filter ("wavefront obj (*.obj)|*.obj");
  string path;
  if ( !ShowFileOpenDlgToGetPath(filter, path) ) return;
  TCore::GetInst()->LoadModel1(path);

  if (!ShowFileOpenDlgToGetPath(filter, path)) return;
  TCore::GetInst()->LoadModel2(path);

  RedrawMainPanel();
}

System::Void MainForm::m_btn_normalize1_Click(System::Object^ sender, System::EventArgs^ e)
{
  TCore::GetInst()->NormalizeSize_MeanDistFromGC();
  RedrawMainPanel();
}

System::Void MainForm::m_btn_normalize2_Click(System::Object^ sender, System::EventArgs^ e)
{
  TCore::GetInst()->NormalizeSize_DistFathestVerts();
  RedrawMainPanel();
}


System::Void MainForm::m_btn_alignobjs_Click(
    System::Object^ sender, System::EventArgs^ e)
{
  TCore::GetInst()->AlignObjsByICP();
  RedrawMainPanel();
}

System::Void MainForm::m_btn_calcdiff_Click(
    System::Object^ sender, System::EventArgs^ e)
{
  TCore::GetInst()->CalcDifferenceOfTwoObjs();
  RedrawMainPanel();
}



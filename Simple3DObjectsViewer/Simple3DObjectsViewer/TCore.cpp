#include "pch.h"
#include "TCore.h"
#include "MainForm.h"


#pragma unmanaged
using namespace Simple3DObjectsViewer;

TCore::TCore()
{
  m_bL = m_bR = m_bM = false;
}




void TCore::MouseMove(EVec2i p, OglForCLI* ogl)
{
  if ( !m_bL && !m_bR && !m_bM ) return;
  ogl->MouseMove( p );
  RedrawMainWindow();
}


void TCore::LBtnDown(EVec2i p, OglForCLI* ogl)
{
  m_bL = true;
  ogl->BtnDown_Trans(p);
}


void TCore::RBtnDown(EVec2i p, OglForCLI* ogl)
{
  m_bR = true;
  ogl->BtnDown_Rot(p);
}


void TCore::MBtnDown(EVec2i p, OglForCLI* ogl)
{
  m_bM = true;
  ogl->BtnDown_Zoom(p);
}

void TCore::LBtnUp(EVec2i p, OglForCLI* ogl)
{
  m_bL= false;
  ogl->BtnUp();
}

void TCore::RBtnUp(EVec2i p, OglForCLI* ogl)
{
  m_bR = false;
  ogl->BtnUp();
}

void TCore::MBtnUp(EVec2i p, OglForCLI* ogl)
{
  m_bM = false;
  ogl->BtnUp();
}



static void DrawAxis()
{
  glLineWidth(3);
  glDisable(GL_LIGHTING);
  glBegin(GL_LINES);
  glColor3d(1, 0, 0); glVertex3d(0, 0, 0); glVertex3d(1, 0, 0);
  glColor3d(0, 1, 0); glVertex3d(0, 0, 0); glVertex3d(0, 1, 0);
  glColor3d(0, 0, 1); glVertex3d(0, 0, 0); glVertex3d(0, 0, 1);
  glEnd();
}


float diff_lb[4] = { 0.3f, 0.3f, 0.8f, 1.0f };
float ambi_lb[4] = { 0.3f, 0.3f, 0.8f, 1.0f };
float diff_lr[4] = { 0.8f, 0.3f, 0.3f, 1.0f };
float ambi_lr[4] = { 0.8f, 0.3f, 0.3f, 1.0f };
float spec[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float shin[1] = { 64.0f};


void TCore::DrawScene(OglForCLI* ogl)
{

  DrawAxis();

  glEnable(GL_DEPTH_TEST);
  
  glEnable(GL_LIGHTING);
  m_obj1.draw(diff_lb, ambi_lb, spec, shin);
  m_obj2.draw(diff_lr, ambi_lr, spec, shin);


  glDisable(GL_LIGHTING);
  m_obj1.DrawEdges(2, 0.5, 0, 0.5);
  m_obj2.DrawEdges(2, 0.5, 0.5, 0);

  
}

void TCore::KeyDown(int keycode) 
{

}



void TCore::NormalizeSize_MeanDistFromGC()
{
  if ( m_obj1.m_vVerts->size() > 0 )
  {
    m_obj1.Translate(-m_obj1.getGravityCenter());

    float len = m_obj1.GetMeanDistOfVertsFromGravityCenter();
    m_obj1.Scale( 1.0f / len );
  }

  if (m_obj2.m_vVerts->size() > 0)
  {
    m_obj2.Translate(-m_obj2.getGravityCenter());

    float len = m_obj2.GetMeanDistOfVertsFromGravityCenter();
    m_obj2.Scale(1.0f / len);
  }
  
}


void TCore::NormalizeSize_DistFathestVerts()
{
  if (m_obj1.m_vVerts->size() > 0)
  {
    m_obj1.Translate(-m_obj1.getGravityCenter());

    float len = m_obj1.GetDistBetweenFathestTwoVerts();
    m_obj1.Scale(1.0f / len);
  }

  if (m_obj2.m_vVerts->size() > 0)
  {
    m_obj2.Translate(-m_obj2.getGravityCenter());

    float len = m_obj2.GetDistBetweenFathestTwoVerts();
    m_obj2.Scale(1.0f / len);
  }


}



#pragma managed

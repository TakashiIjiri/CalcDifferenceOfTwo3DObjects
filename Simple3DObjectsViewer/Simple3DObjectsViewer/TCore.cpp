#include "pch.h"
#include "TCore.h"
#include "MainForm.h"

#pragma unmanaged
using namespace Simple3DObjectsViewer;
using namespace std;

//for ICP
//openCV
#include "opencv/cv.h"
#include "opencv2/surface_matching.hpp"
#include "opencv2/surface_matching/ppf_helpers.hpp"
#include "opencv2/core/utility.hpp"

#pragma comment(lib,"opencv_core310.lib")
#pragma comment(lib,"opencv_surface_matching310.lib")


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


void TCore::CalcDifferenceOfTwoObjs(){}





static void t_SetMat(const TMesh& mesh, cv::Mat& mat)
{
  mat = cv::Mat(mesh.m_vSize, 6, CV_32F);
  for (int n = 0; n < (int)mesh.m_vSize; ++n)
  {
    mat.at<float>(n, 0) = mesh.m_vVerts[n][0];
    mat.at<float>(n, 1) = mesh.m_vVerts[n][1];
    mat.at<float>(n, 2) = mesh.m_vVerts[n][2];
    mat.at<float>(n, 3) = mesh.m_vNorms[n][0];
    mat.at<float>(n, 4) = mesh.m_vNorms[n][1];
    mat.at<float>(n, 5) = mesh.m_vNorms[n][2];
  }
}


//this function returns rigid transformation matrix 
//from srcPC to trgtPC ( RigidMat * src ‚ð trgt ‚ÉƒtƒBƒbƒg‚³‚¹‚é)
static EMat4d t_ICP(
    const float		 icp_reject, // icp parameter
    const int			 icp_levels, // icp parameter
    EMat4d				 init_mat  , // initial 4x4 matrix
    const cv::Mat& src_mat   , // src point cloud
    const cv::Mat& trgt_mat    // trgt point cloud
)
{
  cout << "\n -t_ICP-- \n";
  cout << "rejScale: " << icp_reject << " numLv: " << icp_levels << "\n";
  cout << "srcmat  : row:" << src_mat.rows << " col:" << src_mat.cols << "\n";
  cout << "trgtmat : row:" << trgt_mat.rows << " col:" << trgt_mat.cols << "\n";
  Trace(init_mat);

  init_mat.transposeInPlace();

  cv::ppf_match_3d::Pose3DPtr M1 = new cv::ppf_match_3d::Pose3D();
  M1->updatePose(init_mat.data());
  std::vector<cv::ppf_match_3d::Pose3DPtr> poses;
  poses.push_back(M1);

  try
  {
    // Create an instance of ICP
    cv::ppf_match_3d::ICP icp(300, 0.0001f, icp_reject, icp_levels);
    int i = icp.registerModelToScene(src_mat, trgt_mat, poses);
  }
  catch (...)
  {
    cout << "*** surface‚ÌŒ`ó‚Ü‚½‚ÍˆÊ’u‚ª‘å‚«‚­ˆÙ‚È‚é‚½‚ß’ÇÕ‚ÉŽ¸”s‚µ‚Ü‚µ‚½ ***\n";
  }

  EMat4d m;
  m(0, 0) = poses[0]->pose[0]; m(1, 0) = poses[0]->pose[4]; m(2, 0) = poses[0]->pose[8];  m(3, 0) = poses[0]->pose[12];
  m(0, 1) = poses[0]->pose[1]; m(1, 1) = poses[0]->pose[5]; m(2, 1) = poses[0]->pose[9];  m(3, 1) = poses[0]->pose[13];
  m(0, 2) = poses[0]->pose[2]; m(1, 2) = poses[0]->pose[6]; m(2, 2) = poses[0]->pose[10]; m(3, 2) = poses[0]->pose[14];
  m(0, 3) = poses[0]->pose[3]; m(1, 3) = poses[0]->pose[7]; m(2, 3) = poses[0]->pose[11]; m(3, 3) = poses[0]->pose[15];

  cout << "result matrix\n";
  Trace(m);
  cout << "fin icp\n";

  return m;
}



void TCore::AlignObjsByICP()
{
  if (m_obj1.m_vSize <= 0 || m_obj2.m_vSize <= 0) return;
  m_obj1.updateNormal();
  m_obj2.updateNormal();

  cv::Mat m1, m2;
  t_SetMat(m_obj1, m1);
  t_SetMat(m_obj2, m2);

  //calc icp 
  double ICP_REJECT = 0.2; //³‹K‰»‚³‚ê‚Ä‚¢‚é‚Ì‚Å 0.2’ö“x‚ÉÝ’è
  double ICP_LEVEL  = 5;   //ŠK‘w‚Í5’ö“x
  EMat4d init_mat;
  init_mat.setIdentity();
  EMat4d rigid_1To2 = t_ICP(ICP_REJECT, ICP_LEVEL, init_mat, m1, m2);
  
  m_obj1.MultMat(rigid_1To2.cast<float>());
  
}



#pragma managed

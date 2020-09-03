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
  glEnable(GL_DEPTH_TEST);

  //draw frame 
  DrawAxis();
 
  //draw 2 models 
  glEnable(GL_LIGHTING);
  m_obj1.draw(diff_lb, ambi_lb, spec, shin);
  m_obj2.draw(diff_lr, ambi_lr, spec, shin);

  glDisable(GL_LIGHTING);
  m_obj1.DrawEdges(1, 0.5, 0, 0.5);
  m_obj2.DrawEdges(1, 0.5, 0.5, 0);

  //draw diff data
  glDisable(GL_LIGHTING);
  glPointSize(4);
  glBegin(GL_POINTS);
  for (const auto& it : m_vtxinfo_1to2)
  {
    glColor3d(0.5 * it.m_dist, 0, 0);
    glVertex3fv(it.m_pSrc.data());
  }
  glEnd();

  if (isShiftKeyOn())
  {
    glColor3d(0, 1, 1);
    glLineWidth(1);
    glBegin(GL_LINES);
    for (const auto& it : m_vtxinfo_1to2)
    {
      glVertex3fv(it.m_pSrc.data());
      glVertex3fv(it.m_pTgt.data());
    }
    glEnd();
  }
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





//近傍探索 
//verts1の各点に近い verts2の点を検索する
static void t_GetNearestVtxIdx
(
  const int num1, const EVec3f* verts1, 
  const int num2, const EVec3f* verts2,
  int* vid_src2tgt // size : num1
)
{
  //0. prepare bin
  const int DIV_N = 30;
  vector<int> idsets[DIV_N][DIV_N][DIV_N];


  //1. calc bounding box 
  EVec3f bb_min, bb_max, tmp_min, tmp_max;
  t_calcBoundBox3D(num1, verts1, bb_min, bb_max);
  t_calcBoundBox3D(num2, verts2, tmp_min, tmp_max);

  for ( int i=0; i < 3; ++i )
  {
    bb_min[i] = min(bb_min[i], tmp_min[i]);
    bb_max[i] = max(bb_max[i], tmp_max[i]);
  }

  
  //2. registration 
  for (int i = 0; i < num2; ++i)
  {
    const EVec3f& v = verts2[i];
    //DIV_N = 4 のとき, [0,1/4) -> 0, [1/4,2/4)-->1...
    float xp = (v[0] - bb_min[0]) / (bb_max[0] - bb_min[0]);
    float yp = (v[1] - bb_min[1]) / (bb_max[1] - bb_min[1]);
    float zp = (v[2] - bb_min[2]) / (bb_max[2] - bb_min[2]);
    int xi = (int)min(xp * DIV_N, DIV_N - 1.0f);
    int yi = (int)min(yp * DIV_N, DIV_N - 1.0f);
    int zi = (int)min(zp * DIV_N, DIV_N - 1.0f);
    
    //ここを中心にして27近傍に入れておく（本当は8近傍で良いけど）
    for (int dz = -1; dz <= 1; ++dz) if (0 <= zi + dz && zi + dz < DIV_N)
    {
      for (int dy = -1; dy <= 1; ++dy) if (0 <= yi + dy && yi + dy < DIV_N)
      {
        for (int dx = -1; dx <= 1; ++dx) if (0 <= xi + dx && xi + dx < DIV_N)
        {
          idsets[zi][yi][xi].push_back(i);
        }
      }
    }
  }
  cout << "registration DONE..\n";

  //検索
  for (int i = 0; i < num1; ++i)
  {
    const EVec3f& v = verts1[i];
    //DIV_N = 4 のとき, [0,1/4) -> 0, [1/4,2/4)-->1...
    float xp = (v[0] - bb_min[0]) / (bb_max[0] - bb_min[0]);
    float yp = (v[1] - bb_min[1]) / (bb_max[1] - bb_min[1]);
    float zp = (v[2] - bb_min[2]) / (bb_max[2] - bb_min[2]);
    int xi = (int)min(xp * DIV_N, DIV_N - 1.0f);
    int yi = (int)min(yp * DIV_N, DIV_N - 1.0f);
    int zi = (int)min(zp * DIV_N, DIV_N - 1.0f);

    float  min_d = FLT_MAX;
    int    min_i = -1;
    const vector<int> &ids = idsets[zi][yi][xi];
    for (const auto& id : ids)
    {
      float d = t_dist_sq(v, verts2[id]);
      if (d < min_d)
      {
        min_d = d;
        min_i = id;
      }
    }

    if (min_i == -1)
    {
      cout << "cannnot find in bin " << i << " " << ids.size() << " ... searchAll- \n";
      t_verts_GetNearestPoint(num2, verts2, v, min_i, min_d);
    }
    vid_src2tgt[i] = min_i;
    
    if (i % 1000 == 0) cout << "calc nearest verts" << i << "/" << num1 << "\n";
  }
}








void TCore::CalcDifferenceOfTwoObjs()
{
  const int num1 = m_obj1.m_vSize;
  const int num2 = m_obj2.m_vSize;
  const EVec3f* verts1 = m_obj1.m_vVerts;
  const EVec3f* verts2 = m_obj2.m_vVerts;

  int* nearest_vids = new int[num1];
  t_GetNearestVtxIdx(num1, verts1, num2, verts2, nearest_vids);



  //compute nearest points on surf verts
  m_vtxinfo_1to2.resize( num1 );
  
  for (int i = 0; i < num1; ++i)
  {
    const EVec3f& p = verts1[i];

    EVec3f pos;
    double dist;// = DBL_MAX;
    m_obj2.GetDistToPoint(p, nearest_vids[i], pos, dist);
    m_vtxinfo_1to2[i] = TVtxDistInfo(i, t_dist(p, pos), p, pos);

    if (i % 50000 == 0)
      cout << i << "/" << num1 << "\n";
  }

  delete[] nearest_vids;

  
  double max_dist = 0;
  double mean_dist = 0;
  for (int i = 0; i < num1; ++i)
  {
    max_dist = max(max_dist, m_vtxinfo_1to2[i].m_dist );
    mean_dist += m_vtxinfo_1to2[i].m_dist;
  }
  mean_dist /= num1;


  std::cout << "---------------------DIFF---------------\n";
  std::cout << "max_diff  " << max_dist << "\n";
  std::cout << "mean_diff  " << mean_dist << "\n";
}





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
//from srcPC to trgtPC ( RigidMat * src を trgt にフィットさせる)
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
    cout << "*** surfaceの形状または位置が大きく異なるため追跡に失敗しました ***\n";
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


//NOTE : object 2 を object 1 フィットする
void TCore::AlignObjsByICP()
{
  if (m_obj1.m_vSize <= 0 || m_obj2.m_vSize <= 0) return;
  m_obj1.updateNormal();
  m_obj2.updateNormal();

  cv::Mat m1, m2;
  t_SetMat(m_obj1, m1);
  t_SetMat(m_obj2, m2);

  //calc icp 
  float ICP_REJECT = 0.2f; //正規化されているので 0.2程度に設定
  int   ICP_LEVEL  = 5;   //階層は5程度
  EMat4d init_mat;
  init_mat.setIdentity();
  EMat4d rigid_1To2 = t_ICP(ICP_REJECT, ICP_LEVEL, init_mat, m1, m2);
  
  m_obj1.MultMat(rigid_1To2.cast<float>());
}



#pragma managed

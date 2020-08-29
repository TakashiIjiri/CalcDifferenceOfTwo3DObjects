#include "pch.h"
#include "MainForm.h"

using namespace System;


// form　作る OK 
// TCore 作る OK
// OpenGLつなぐ OK
// OBJ読み込んで表示 
// diff計算する
// 大きさ揃える
// 位置を揃える
// データを提示する


using namespace Simple3DObjectsViewer;


[STAThreadAttribute]
int main()
{
  MainForm::GetInst()->ShowDialog();
  return 0;
}

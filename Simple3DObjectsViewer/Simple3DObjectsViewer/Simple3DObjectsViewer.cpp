#include "pch.h"
#include "MainForm.h"

using namespace System;


// form�@��� OK 
// TCore ��� OK
// OpenGL�Ȃ� OK
// OBJ�ǂݍ���ŕ\�� 
// diff�v�Z����
// �傫��������
// �ʒu�𑵂���
// �f�[�^��񎦂���


using namespace Simple3DObjectsViewer;


[STAThreadAttribute]
int main()
{
  MainForm::GetInst()->ShowDialog();
  return 0;
}

# CalcDifferenceOfTwo3DObjects
C++CLI環境でOpenGLを利用して2つのwavefront objモデルを読み込み，その差分を計算する．  
2つのオブジェクトのサイズの正規化のため，重心から各頂点までの平均距離を1にする処理を実装．  
2つのオブジェクトの位置合わせのためOpenCVのICP（iterative closest points）を利用．  

2つのオブジェクト(object1とobject2)の差の計算では、、、  
- object1の各頂点から最も近いobject2への位置を計算し  
- 得られる線分の長さの平均を標準出力に出力し，
- 得られる線分の長さの最大値を標準出力に出力する


## コンパイルに必要な作業
### 1. glew
- ./Simple3DObjectsViewer/Simple3DObjectsViewerのなかに3rdpartyというフォルダを作成
- http://glew.sourceforge.net/ ここからダウンロードしてzipを展開
- 展開したフォルダをglewという名前に変更
- そのフォルダを3rdpartyに入れる
- 対応するglew32.libをフォルダ内から探して，./Simple3DObjectsViewer\x64\Release に入れる  

### 2. Eigen
- http://eigen.tuxfamily.org/index.php?title=Main_Page Eigenをダウンロードしてzipを展開
- 展開したフォルダの名前をeigen3に変更
- eigen3を上で作成した3rdpartyフォルダに入れる

### 3. OpenCV
- NuGetが自動的に作る「Simple3DObjectsViewer\packages\opencvcontrib.redist.3.1.0\build\native\bin\x64\v140\Release」を開く
- 以下のファイルを探して./Simple3DObjectsViewer\x64\Release に入れる
  - opencv_core310.dll
  - opencv_flann310.dll
  - opencv_surface_matching310.dll








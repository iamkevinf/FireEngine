# FireEngine

## build

 - git pull
 - git submodule update
 - ./generatersln.bat
 - open FireEngine.sln, select [bgfx/bimg/bimg_decode/bx and so on in bgfx filter] then right click and click "Properties", then C/C++ -> Code Generation -> **Runtime Library Multi-threaded Debug DLL(/MDd)** in debug or **Runtime Library Multi-threaded DLL (/MD)** in release
 - change [bgfx/bimg/bimg_decode/bx and so on in bgfx filter] project to x64
 - FireEngine -> Properties -> Debug -> Working directory  set to **runtime** path
 - assimp -> Properties -> C/C++ -> General -> Treat Warnings As Errors -> No(/WX-)
 - build it
 - copy assimp dll to runtime path

## todo
 - [x] log
 - [x] dragdrop treenode的时候包含children
 - [x] tree name 在create的时候由C#这边设置
 - [ ] tree rename (暂时这样还有待优化)
 - [x] 挂载显示内容在editor窗口从grid开始
 - [x] gameobject component 系统
 - [x] camera
 - [ ] 四元数 <-> 欧拉角
 - [ ] 分开game和scene，可能需要增加一个scene world，只有一台相机并且无视world中的相机
 - [ ] 导出现有的gameobject transform camera
 - [ ] editor窗口怎么接受鼠标拖拽framebuffer中渲染出来的gizmos？
 - [ ] mesh material

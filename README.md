# FireEngine

## build

 - git pull
 - git submodule update
 - ./generatersln.bat
 - open FireEngine.sln, select [bgfx/bimg/bimg_decode/bx and so on in bgfx filter] then right click and click "Properties", then C/C++ -> Code Generation -> **Runtime Library Multi-threaded Debug DLL(/MDd)** in debug or **Runtime Library Multi-threaded DLL (/MD)** in release
 - change [bgfx/bimg/bimg_decode/bx and so on in bgfx filter] project to x64
 - FireEngine -> Properties -> Debug -> Working directory  set to **runtime** path
 - build it

## todo
 - [x] log
 - [x] dragdrop treenode��ʱ�����children
 - [x] tree name ��create��ʱ����C#�������
 - [ ] tree rename

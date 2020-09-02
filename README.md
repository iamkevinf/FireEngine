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
 - [x] dragdrop treenode��ʱ�����children
 - [x] tree name ��create��ʱ����C#�������
 - [ ] tree rename (��ʱ�������д��Ż�)
 - [x] ������ʾ������editor���ڴ�grid��ʼ
 - [x] gameobject component ϵͳ
 - [x] camera
 - [ ] ��Ԫ�� <-> ŷ����
 - [ ] �ֿ�game��scene��������Ҫ����һ��scene world��ֻ��һ̨�����������world�е����
 - [ ] �������е�gameobject transform camera
 - [ ] editor������ô���������קframebuffer����Ⱦ������gizmos��
 - [ ] mesh material

echo build buildin shader

set rcpath="../../3rdparty/bgfx/.build/win64_vs2019/bin/shadercRelease.exe"
set srcpath="./shaders"
set dstpath="../../runtime/shaders/dx11"
set depends="../../3rdparty/bgfx/src"

%rcpath% -f %srcpath%/vs_buildin.sc -o %dstpath%/vs_buildin.bin --depends -i %depends% --varying.def varying.def.sc --platform windows -p vs_4_0 -O 3 --type vertex -O3
%rcpath% -f %srcpath%/fs_buildin.sc -o %dstpath%/fs_buildin.bin --depends -i %depends% --varying.def varying.def.sc --platform windows -p ps_4_0 -O 3 --type fragment -O3


%rcpath% -f %srcpath%/vs_fbo.sc -o %dstpath%/vs_fbo.bin --depends -i %depends% --varying.def fbo_varying.def.sc --platform windows -p vs_4_0 -O 3 --type vertex -O3
%rcpath% -f %srcpath%/fs_fbo.sc -o %dstpath%/fs_fbo.bin --depends -i %depends% --varying.def fbo_varying.def.sc --platform windows -p ps_4_0 -O 3 --type fragment -O3

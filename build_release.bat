@echo off

setlocal

rem call "%ProgramFiles(x86)%\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64

set application_name=patcher
set compiler=cl.exe

set build_options= /DBUILD_WIN32=1 /D_CRT_SECURE_NO_WARNINGS
set compile_flags= -nologo /W4 /permissive- /std:c17 /FC /TC /O2 /I ../source/
set link_flags= -opt:ref -incremental:no /Debug:none

if not exist build mkdir build
pushd build
%compiler% %build_options% %compile_flags% ../source/win32/build.c /link %link_flags% /out:%application_name%.exe
popd

endlocal

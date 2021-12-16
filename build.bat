@echo off
setlocal enabledelayedexpansion

setlocal

where /Q cl.exe || (
  set __VSCMD_ARG_NO_LOGO=1
  for /f "tokens=*" %%i in ('"C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.VisualStudio.Workload.NativeDesktop -property installationPath') do set VS=%%i
  if "!VS!" equ "" (
    echo ERROR: Visual Studio installation not found
    exit /b 1
  )  
  call "!VS!\VC\Auxiliary\Build\vcvarsall.bat" amd64 || exit /b 1
)

if "%VSCMD_ARG_TGT_ARCH%" neq "x64" (
  echo ERROR: Please run this from the Visual Studio x64 Native Tools Command Prompt
  exit /b 1
)

set version=%1
if "%version%"=="" (
	set version=debug
)

if "%version%"=="release" (
	set build_options= /D_CRT_SECURE_NO_WARNINGS
	set compile_flags= -nologo /W4 /permissive- /std:c17 /FC /TC /O2 /I ../source/
	set link_flags= -opt:ref -incremental:no /Debug:none
) else (
	set version=debug
	set build_options= /D_DEBUG /D_CRT_SECURE_NO_WARNINGS
	set compile_flags= -nologo /W4 /permissive- /std:c17 /FC /TC /FS /Zi /I ../source/
	set link_flags= -opt:ref -incremental:no /Debug:full
)

set application_name=patcher
set compiler=cl.exe

echo Building %application_name% (%version%)
if not exist build mkdir build
pushd build
%compiler% %build_options% %compile_flags% ../source/win32/build.c /link %link_flags% /out:%application_name%.exe
popd

endlocal

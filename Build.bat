@echo off
if not exist "bin\" mkdir "bin"

cls
pushd bin

set common_linker_flags= /incremental:no /opt:ref User32.lib Gdi32.lib
set defines= /D_DB

cl /Fe"Tetris in a day.exe" %defines% /GR- /EHa- /W3 /options:strict /sdl /nologo /Zi /fp:fast /fp:except- /GA /std:c++17 /Od ../src/WindowsMain.cpp /link %common_linker_flags%
popd



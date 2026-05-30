@echo off
IF NOT EXIST otava_inst.exe xcopy /Y /D ..\x64\Release\otava_inst.exe .
IF NOT EXIST ..\bin mkdir ..\bin
IF NOT EXIST ..\lib mkdir ..\lib
soul_inst -v ..\x64\Debug\*.exe ..\bin
soul_inst -v ..\x64\Release\*.exe ..\bin
soul_inst -v ..\x64\Debug\*.lib ..\lib
soul_inst -v ..\x64\Release\*.lib ..\lib

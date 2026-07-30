@echo off
IF NOT EXIST otava_inst.exe xcopy /Y /D ..\x64\Release\otava_inst.exe .
IF NOT EXIST ..\bin mkdir ..\bin
IF NOT EXIST ..\lib mkdir ..\lib
otava_inst -v ..\x64\Debug\*.exe ..\bin
otava_inst -v ..\x64\Release\*.exe ..\bin
otava_inst -v ..\x64\Debug\*.lib ..\lib
otava_inst -v ..\x64\Release\*.lib ..\lib
otava_inst -v ..\projects\soul\oslg\bin\release\2\oslg.exe ..\bin
otava_inst -v ..\projects\soul\ospg\bin\release\2\ospg.exe ..\bin

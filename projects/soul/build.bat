@echo off
pushd token
call build.bat
popd
pushd common
call build.bat
popd
pushd cpp
call build.bat
popd
pushd lex
call build.bat
popd
pushd re
call build.bat
popd
pushd oslg
call build.bat
popd
pushd ospg
call build.bat
popd

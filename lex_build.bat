@echo off
call custom\bin\build_one_time.bat custom\languages\4coder_cpp_lexer_gen.cpp
call one_time.exe
del 4coder_*_lexer_gen.obj
call build.bat
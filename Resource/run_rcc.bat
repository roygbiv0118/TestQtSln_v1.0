::@echo off
del /q ..\Resource\res.rcc
rcc -binary ..\Resource\res.qrc -o ..\Resource\res.rcc
pause
@echo off
set Path=C:\c++\WiX;%PATH%
candle -nologo *.wxs
if ERRORLEVEL 1 goto ende
light -nologo QtCore.wixobj
light -nologo QtGui.wixobj
light -nologo QtOpenGL.wixobj
light -nologo QtSql.wixobj
light -nologo QtNetwork.wixobj
light -nologo QtSvg.wixobj
light -nologo QtXML.wixobj
:ende
del /Q *.wixobj
pause

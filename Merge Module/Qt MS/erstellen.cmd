@echo off
set Path=C:\c++\WiX3;%PATH%
candle -nologo -sw1044 *.wxs
if ERRORLEVEL 1 goto ende
echo "Die Kerze brennt"
rem light  -notidy -v0 QtCore.wixobj
light -nologo -w0 -v0 QtCore.wixobj
light -nologo QtXML.wixobj
light -nologo QtGui.wixobj
light -nologo QtNetwork.wixobj
light -nologo QtOpenGL.wixobj
light -nologo QtSql.wixobj
light -nologo QtSvg.wixobj
light -nologo Qt3Support.wixobj
light -nologo QtAssistentClient.wixobj
:ende
del /Q *.wixobj
pause

@echo off
set Path=C:\c++\WiX3;%PATH%
candle -nologo -sw1044 *.wxs
if ERRORLEVEL 1 goto ende
echo "Die Kerze brennt"
rem light  -notidy -v0 QtCore.wixobj
rem -sh Wird gebraucht, da in den Qt DLL's das Feld Sprache fehlt.
light -nologo -sh QtCore.wixobj
light -nologo -sh QtXML.wixobj
light -nologo -sh QtGui.wixobj
light -nologo -sh QtNetwork.wixobj
light -nologo -sh QtOpenGL.wixobj
light -nologo -sh QtSql.wixobj
light -nologo -sh QtSvg.wixobj
light -nologo -sh Qt3Support.wixobj
light -nologo -sh QtAssistentClient.wixobj
:ende
del /Q *.wixobj
pause

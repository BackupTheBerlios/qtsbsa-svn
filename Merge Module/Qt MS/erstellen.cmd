@echo off
set Path=C:\c++\WiX3;%PATH%
candle *.wxs
rem candle -nologo -sw1044 *.wxs
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
rem light -nologo -sh QtMySQL.wixobj
light -nologo -sh QtODBC.wixobj
light -nologo -sh QtMySQL_QtBug.wixobj
light -nologo -sh QtODBC_QtBug.wixobj
light -nologo QtUebersetzung.wixobj

rem light -nologo QtAssistentClientKatalog.wixobj
rem light -nologo Qt3SupportKatalog.wixobj
rem light -nologo QtCoreKatalog.wixobj
rem light -nologo QtGuiKatalog.wixobj
rem light -nologo QtNetworkKatalog.wixobj
rem light -nologo QtOpenGLKatalog.wixobj
rem light -nologo QtSqlKatalog.wixobj
rem light -nologo QtSvgKatalog.wixobj
rem light -nologo QtXMLKatalog.wixobj
:ende
del /Q *.wixobj
pause

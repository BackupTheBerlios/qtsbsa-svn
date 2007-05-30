/* Copyright (C) 2007 Frank Büttner frank-buettner@gmx.net

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version
 2 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software Foundation,
 Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.*/

#include "WixDateiUebersetzen.h"
#include "Parameter.h"

using namespace QFrank;

QtSBSAWixDateiUebersetzen::QtSBSAWixDateiUebersetzen(QtSBSAParameter* parameter,QObject* eltern)
								:QtSBSABasisThread(parameter,eltern)
{	
	K_Arbeitsschritt=QtSBSAWixDateiUebersetzen::Kerze;
	K_Prozess=NULL;
}
QtSBSAWixDateiUebersetzen::~QtSBSAWixDateiUebersetzen()
{
	delete K_Prozess;
}
void QtSBSAWixDateiUebersetzen::run()
{	
	if(!K_Loslegen())
	{
		K_Fehlercode=1;
		//emit
		fertig(this);
	}
	K_Fehlercode=exec();
	//emit
	fertig(this);
}
void QtSBSAWixDateiUebersetzen::K_ProzessFertig(const int &statuscode)
{	
	QString Fehlermeldung=QString(K_Prozess->readAll());
	Fehlermeldung.remove("\r");
	Fehlermeldung.remove("\n");
#ifndef QT_NO_DEBUG	
	qDebug("%s K_ProzessFertig: Nummer: %i\r\nStatuscode: %i\r\nFehlertext: %s\r\nAusgabe:\r\n%s",this->metaObject()->className(),K_Dateinummer,statuscode,
																							qPrintable(QString(K_Prozess->errorString())),
									  														qPrintable(Fehlermeldung));	
#endif
	if(statuscode!=0)
	{
		K_Fehlermeldung=Fehlermeldung;
		exit(1);
	}
	else
	{
		if(K_Arbeitsschritt==QtSBSAWixDateiUebersetzen::Licht)
			exit(0);
		else
		{
			K_Arbeitsschritt++;
			if(!K_Loslegen())
			{
				exit(1);
			}
		}
	}
}
bool QtSBSAWixDateiUebersetzen::K_Loslegen()
{	
	if(K_Prozess!=NULL)
	{
		delete K_Prozess;
	}
	K_Prozess=new QProcess();
	K_Prozess->setWorkingDirectory(K_Parameter->ZielverzeichnisHohlen());
	connect(K_Prozess,SIGNAL(finished(int)),this,SLOT(K_ProzessFertig(const int&)));
	QStringList Argumente("-nologo");
	QString Befehl=K_Parameter->WixPfadHohlen()+"\\";
	QString Dateiname=K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).DateinameHohlen();;
	if(K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).istPlugIn())
	{
		Dateiname="Qt_"+K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).PlugInTypeHohlen()+"_"+Dateiname;
		Dateiname=Dateiname.left(Dateiname.indexOf("."));
	}
	else
		Dateiname=Dateiname.left(Dateiname.indexOf(".")-1);
	switch(K_Arbeitsschritt)
	{
		case QtSBSAWixDateiUebersetzen::Kerze:
					Befehl.append("candle.exe");
					Dateiname.append(".wxs");
					Argumente<<Dateiname;
					break;
		case QtSBSAWixDateiUebersetzen::Licht:
					Befehl.append("light.exe");
					Dateiname.append(".wixobj");
					Argumente<<"-sh"<<Dateiname;
					break;
		default:
					qFatal("%s K_Loslegen: unbekannter Arbeitsschritt %i",this->metaObject()->className(),K_Arbeitsschritt);
					break;
	}
	K_Prozess->start(Befehl,Argumente);
	if(!K_Prozess->waitForStarted(5000))
	{		
		K_Fehlermeldung=trUtf8("Das Werkzeug %1 konnte nicht ausgeführt werden.").arg(Befehl);
		return false;
	}
	return true;
}

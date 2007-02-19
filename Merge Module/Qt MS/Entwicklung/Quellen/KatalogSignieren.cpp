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

#include "KatalogSignieren.h"
#include "Parameter.h"

QFrankQtSBSAKatalogSignieren::QFrankQtSBSAKatalogSignieren(const QFrankQtSBSAParameter* parameter,QObject* eltern)
									:QFrankQtSBSABasisThread(parameter,eltern)
{
	K_signtoolProzess=NULL;
}
QFrankQtSBSAKatalogSignieren::~QFrankQtSBSAKatalogSignieren()
{
	if(K_signtoolProzess!=NULL)
		delete K_signtoolProzess;
}
void QFrankQtSBSAKatalogSignieren::run()
{
	//Qt Plug-Ins haben kein Katalog, also gibt es nix zu signieren.
	if(K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).istPlugIn())
	{
		K_Fehlercode=0;
		emit fertig(this);
		return;
	}
	QString Dateipfad="";
	if(K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).istPlugIn())
		Dateipfad=K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).PlugInTypeHohlen()+"\\";
	K_signtoolProzess=new QProcess();
	K_signtoolProzess->setProcessChannelMode(QProcess::MergedChannels);
	connect(K_signtoolProzess,SIGNAL(finished(int)),this,SLOT(K_signtoolFertig(int)));
	QStringList Argumente;
	Argumente<<"sign"<<"/sha1"<<K_Parameter->ZertSHA1Hohlen()<<"/t"<<"http://timestamp.verisign.com/scripts/timestamp.dll"
			 <<K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).DateinameHohlen()+".cat";
	K_signtoolProzess->setWorkingDirectory(K_Parameter->ZielverzeichnisHohlen()+"\\"+Dateipfad);
	K_signtoolProzess->start(K_Parameter->WindowsSDKPfadHohlen()+"\\signtool.exe",Argumente);
	if(!K_signtoolProzess->waitForStarted(5000))
	{
		K_Fehlercode=1;
		K_Fehlermeldung=trUtf8("Das Werkzeug signtool.exe konnte nicht ausgeführt werden.");
		emit fertig(this);
		return;
	}
	K_Fehlercode=exec();
	emit fertig(this);
}
void QFrankQtSBSAKatalogSignieren::K_signtoolFertig(int statusCode)
{
	/*
		exit Code:
		0 alles ok
		1 Fehler
	*/
	QString Fehlermeldung=QString(K_signtoolProzess->readAll());
	Fehlermeldung.remove("\r");
	Fehlermeldung.remove("\n");
#ifndef QT_NO_DEBUG	
	qDebug("%s K_mtFertig: Nummer: %i\r\nStatuscode: %i\r\nFehlertext: %s\r\nAusgabe:\r\n%s",this->metaObject()->className(),K_Dateinummer,statusCode,
																							qPrintable(QString(K_signtoolProzess->errorString())),
									  														qPrintable(Fehlermeldung));
#endif
	if(statusCode!=0)
	{
		K_Fehlermeldung=Fehlermeldung;
		exit(1);
	}
	else
#ifndef QT_NO_DEBUG
		//Fehler Simmulation
		/*if(K_Dateinummer==3)
		{
			K_Fehlermeldung="Simmulierter Fehler";
			exit(1);
		}
		else*/
			exit(0);
#else
		exit(0);
#endif
}

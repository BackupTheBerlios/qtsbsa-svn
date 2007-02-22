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

#include "KatalogErstellen.h"
#include "Parameter.h"

QFrankQtSBSAKatalogErstellen::QFrankQtSBSAKatalogErstellen(QFrankQtSBSAParameter* parameter,QObject* eltern)
									  :QFrankQtSBSABasisThread(parameter,eltern)
{	
	K_AktuellerSchritt=KatalogvorlageErstellen;
	K_Dateipfad="";
}
QFrankQtSBSAKatalogErstellen::~QFrankQtSBSAKatalogErstellen()
{
	delete K_Prozess;
}
void QFrankQtSBSAKatalogErstellen::run()
{
	K_Prozess=new QProcess();
	//Qt Plug-Ins brauchen kein Katlog
	if(K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).istPlugIn())
	{
		K_Fehlercode=0;
		emit fertig(this);
		return;
	}
	connect(K_Prozess,SIGNAL(finished(int)),this,SLOT(K_ProzessIstFertig(int)));
	K_Prozess->setProcessChannelMode(QProcess::MergedChannels);
	QStringList Argumente;
	if(K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).istPlugIn())
		K_Dateipfad=K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).PlugInTypeHohlen()+"\\";
	Argumente<<"-manifest"
			<<K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).DateinameHohlen()+".manifest"
			<<"-makecdfs"<<"-nologo"<<"-hashupdate";
	K_Prozess->setWorkingDirectory(K_Parameter->ZielverzeichnisHohlen()+"\\"+K_Dateipfad);
	K_Prozess->start(K_Parameter->WindowsSDKPfadHohlen()+"\\mt.exe",Argumente);
	if(!K_Prozess->waitForStarted(5000))
	{
		K_Fehlercode=1;
		K_Fehlermeldung=trUtf8("Das Werkzeug mt.exe konnte nicht ausgeführt werden.");
		emit fertig(this);
		return;
	}
	K_Fehlercode=exec();
	emit fertig(this);
}
void QFrankQtSBSAKatalogErstellen::K_ProzessIstFertig(int rueckgabe)
{
	QString Fehlermeldung=QString(K_Prozess->readAll());
	Fehlermeldung.remove("\r");
	Fehlermeldung.remove("\n");
#ifndef QT_NO_DEBUG	
	qDebug("%s K_ProzessIstFertig: Nummer: %i\r\nStatuscode: %i\r\nFehlertext: %s\r\nAusgabe:\r\n%s",this->metaObject()->className(),K_Dateinummer,rueckgabe,
																								qPrintable(QString(K_Prozess->errorString())),
										  														qPrintable(Fehlermeldung));
#endif
	if(rueckgabe!=0)
	{
		K_Fehlermeldung=Fehlermeldung;
		exit(1);
	}
	else
	{
		if(K_AktuellerSchritt==KatalogErstellen)
		{
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
		else
		{			
			//Katalog erstellen
			K_AktuellerSchritt=KatalogErstellen;
			delete K_Prozess;
			K_Prozess=new QProcess();
			connect(K_Prozess,SIGNAL(finished(int)),this,SLOT(K_ProzessIstFertig(int)));
			K_Prozess->setWorkingDirectory(K_Parameter->ZielverzeichnisHohlen()+"\\"+K_Dateipfad);
			K_Prozess->setProcessChannelMode(QProcess::MergedChannels);
			K_Prozess->start(K_Parameter->WindowsSDKPfadHohlen()+"\\makecat.exe",
							 QStringList()<<"-v"<<K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).DateinameHohlen()+".manifest.cdf");
			if(!K_Prozess->waitForStarted(5000))
			{
				K_Fehlercode=1;
				K_Fehlermeldung=trUtf8("Das Werkzeug makecat.exe konnte nicht ausgeführt werden.");
				emit fertig(this);
				exit(1);
				return;
			}
		}
	}
}


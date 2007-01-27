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

QFrankQt4MergemoduleKatalogErstellen::QFrankQt4MergemoduleKatalogErstellen(const QFrankQt4MergemoduleParameter* parameter,QObject* eltern)
									  :QFrankQt4MergemoduleBasisThread(parameter,eltern)
{	
	K_AktuellerSchritt=KatalogvorlageErstellen;
}
QFrankQt4MergemoduleKatalogErstellen::~QFrankQt4MergemoduleKatalogErstellen()
{
	delete K_Prozess;
}
void QFrankQt4MergemoduleKatalogErstellen::run()
{
	K_Prozess=new QProcess();
	connect(K_Prozess,SIGNAL(finished(int)),this,SLOT(K_ProzessIstFertig(int)));
	QStringList Argumente;
	K_Dateiname=K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer);
	K_Dateiname=K_Dateiname.right(K_Dateiname.length()-K_Dateiname.lastIndexOf("\\"))+".manifest";
	Argumente<<"-manifest"<<K_Dateiname.remove(0,1)<<"-makecdfs"<<"-nologo"<<"-hashupdate";
	K_Prozess->setWorkingDirectory(K_Parameter->ZielverzeichnisHohlen());
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
void QFrankQt4MergemoduleKatalogErstellen::K_ProzessIstFertig(int rueckgabe)
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
			K_Prozess->setWorkingDirectory(K_Parameter->ZielverzeichnisHohlen());
			K_Prozess->setProcessChannelMode(QProcess::MergedChannels);
			K_Prozess->start(K_Parameter->WindowsSDKPfadHohlen()+"\\makecat.exe",QStringList()<<"-v"<<K_Dateiname+".cdf");
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


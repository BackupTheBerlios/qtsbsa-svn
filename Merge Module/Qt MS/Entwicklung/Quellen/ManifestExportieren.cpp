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

#include "ManifestExportieren.h"
#include "Parameter.h"

/*
	Manifest exportieren: mt.exe -inputresource:dll_with_manifest.dll;#2 -out:extracted.manifest (Die Manifeste von Qt stehen in #2 statt #1
	Manifest importieren: mt.exe -manifest XXX.manifest -outputresource:XXXX.dll;#1	
*/

QFrankQt4MergemoduleManifestExportieren::QFrankQt4MergemoduleManifestExportieren(const QFrankQt4MergemoduleParameter* parameter,QObject *eltern)
										:QFrankQt4MergemoduleBasisThread(parameter,eltern)
{	
	K_mtProzess=NULL;
}
QFrankQt4MergemoduleManifestExportieren::~QFrankQt4MergemoduleManifestExportieren()
{
	if(K_mtProzess!=NULL)
		delete K_mtProzess;
}
void QFrankQt4MergemoduleManifestExportieren::run()
{
	K_mtProzess=new QProcess();
	K_mtProzess->setProcessChannelMode(QProcess::MergedChannels);
	connect(K_mtProzess,SIGNAL(finished(int)),this,SLOT(K_mtFertig(int)));
	QStringList Argumente;
	QString Datei=K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer);
	Datei.replace('/','\\');
	Datei=Datei.right(Datei.length()-Datei.lastIndexOf("\\")).remove(0,1);
	Argumente<<"-inputresource:"+Datei+";#2"
			 <<"-out:"+Datei+".manifest"
			 <<"-verbose"<<"-nologo";
	K_mtProzess->setWorkingDirectory(K_Parameter->ZielverzeichnisHohlen());
	K_mtProzess->start("\""+K_Parameter->WindowsSDKPfadHohlen()+"\\mt.exe\"",Argumente);
		
	if(!K_mtProzess->waitForStarted(5000))
	{
		K_Fehlercode=1;
		K_Fehlermeldung=trUtf8("Das Werkzeug mt.exe konnte nicht ausgeführt werden.");
		emit fertig(this);
		return;
	}
	K_Fehlercode=exec();
	emit fertig(this);
}
void QFrankQt4MergemoduleManifestExportieren::K_mtFertig(int statusCode)
{	
	/*
		exit Code:
		0 alles ok
		1 Fehler
	*/
	QString Fehlermeldung=QString(K_mtProzess->readAll());
	Fehlermeldung.remove("\r");
	Fehlermeldung.remove("\n");
#ifndef QT_NO_DEBUG	
	qDebug("%s K_mtFertig: Nummer: %i\r\nStatuscode: %i\r\nFehlertext: %s\r\nAusgabe:\r\n%s",this->metaObject()->className(),K_Dateinummer,statusCode,
																							qPrintable(QString(K_mtProzess->errorString())),
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

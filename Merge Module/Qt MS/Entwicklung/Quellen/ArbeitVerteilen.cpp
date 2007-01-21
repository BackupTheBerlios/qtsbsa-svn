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

#include "ArbeitVerteilen.h"
#include "Parameter.h"
#include "Manifestexportieren.h"
#include <Windows.h>
#include <Winver.h>

QFrankQt4MergemoduleArbeitVerteilen::QFrankQt4MergemoduleArbeitVerteilen(QObject *eltern,QFrankQt4MergemoduleParameter* parameter):QObject(eltern)
{
	K_Parameter=parameter;
}
void QFrankQt4MergemoduleArbeitVerteilen::Loslegen()
{
	if(!K_WindowsSDKPruefen())
		return;
	if(!K_QtPruefen())
		return;
	if(!K_ZielverzeichnisPruefen())
		return;
	if(!K_DateienKopieren(K_Parameter->QtBibliothekenHohlen(),K_Parameter->ZielverzeichnisHohlen()))
		return;
	K_ManifesteExportieren();
	//K_ErstellenErfolgreich();
	//emit fertig();
}
void QFrankQt4MergemoduleArbeitVerteilen::K_ManifesteExportieren()
{
	emit Meldung(tr("Extrahiere Manifeste"));
	K_AnzahlDerProzesse=K_Parameter->QtBibliothekenHohlen().count();
#ifndef QT_NO_DEBUG
	qDebug("%s K_ManifesteExportieren: Anzahl der Prozesse: %i",this->metaObject()->className(),K_AnzahlDerProzesse);
#endif
	for(int Threadnummer=0;Threadnummer<K_AnzahlDerProzesse;Threadnummer++)
	{
		QFrankQt4MergemoduleManifestExportieren *Export=new QFrankQt4MergemoduleManifestExportieren(K_Parameter,this);
		Export->DateinummerFestlegen(Threadnummer);
		connect(Export,SIGNAL(fertig(QThread*)),this,SLOT(K_ThreadFertig( QThread*)));
		Export->start();
	}		
}
void QFrankQt4MergemoduleArbeitVerteilen::K_ThreadFertig(QThread *welcher)
{
	static bool FehlerAufgetreten=false;
	K_AnzahlDerProzesse--;
	int Fehlercode=-1;
	QMetaObject::invokeMethod(welcher,"FehlercodeHohlen",Q_RETURN_ARG(int,Fehlercode));
	if(Fehlercode!=0)
		FehlerAufgetreten=true;
#ifndef QT_NO_DEBUG
	int Threadnummer=-1;
	QMetaObject::invokeMethod(welcher,"Threadnummer",Q_RETURN_ARG(int,Threadnummer));
	qDebug("%s K_ThreadFertig: Thread %i ist fertig mit dem Fehlercode %i",this->metaObject()->className(),Threadnummer,Fehlercode);
#endif
	if(K_AnzahlDerProzesse==0)
	{
		if(!FehlerAufgetreten)
			K_SchrittFertig();
		else
			K_SchrittFehlgeschlagen();
		//Wenn alle fertig sind, emit fertig() aufrufen damit es weiter geht
		emit fertig();
	}	
}
bool QFrankQt4MergemoduleArbeitVerteilen::K_DateienKopieren(const QStringList &dateiliste,const QString &zielverzeichnis)
{
	emit Meldung(tr("Kopiere Qt4 Bibliotheken"));
	QString Datei;
	Q_FOREACH(Datei,dateiliste)
	{
		Datei.replace("/","\\");		
		if(!QFile::copy(Datei,zielverzeichnis+Datei.right(Datei.length()-Datei.lastIndexOf("\\"))))
		{
			emit Meldung(tr("Die Datei %1 konnte nicht nach %2 kopiert werden.")
							.arg(Datei).arg(zielverzeichnis+Datei.right(Datei.length()-Datei.lastIndexOf("\\"))));
			K_SchrittFehlgeschlagen();
			K_ErstellenGescheitert();
			emit fertig();
			return false;
			break;
		}
	}
	K_SchrittFertig();	
	return true;
}
bool QFrankQt4MergemoduleArbeitVerteilen::K_WindowsSDKPruefen()
{
	emit Meldung(trUtf8("Prüfe Windows SDK"));
	//Haben wir alle SDK Werkzeuge??
	QStringList Werkzeugliste;
	Werkzeugliste<< K_Parameter->WindowsSDKPfadHohlen()+"\\mt.exe"<<
					K_Parameter->WindowsSDKPfadHohlen()+"\\signtool.exe";
	if(!K_DateienVorhanden(Werkzeugliste))
	{
		K_SchrittFehlgeschlagen();
		K_ErstellenGescheitert();
		emit fertig();
		return false;
	}	
	K_SchrittFertig();
	return true;
}
bool QFrankQt4MergemoduleArbeitVerteilen::K_QtPruefen()
{
	emit Meldung(trUtf8("Prüfe Qt"));
	//sind alle Qt Libs da??
	QStringList Bibliotheken;
	Bibliotheken << K_Parameter->QtPfadHohlen()+"\\Qt3Support4.dll"<<
					K_Parameter->QtPfadHohlen()+"\\QtAssistantClient4.dll"<<
					K_Parameter->QtPfadHohlen()+"\\QtCore4.dll"<<
					K_Parameter->QtPfadHohlen()+"\\QtGui4.dll"<<
					K_Parameter->QtPfadHohlen()+"\\QtNetwork4.dll"<<
					K_Parameter->QtPfadHohlen()+"\\QtOpenGL4.dll"<<
					K_Parameter->QtPfadHohlen()+"\\QtSql4.dll"<<
					K_Parameter->QtPfadHohlen()+"\\QtSvg4.dll"<<
					K_Parameter->QtPfadHohlen()+"\\QtXml4.dll";
	K_Parameter->QtBibliothekenSetzen(Bibliotheken);
	if(!K_DateienVorhanden(Bibliotheken))
	{
		K_SchrittFehlgeschlagen();
		K_ErstellenGescheitert();
		emit fertig();
		return false;
	}
	K_Parameter->QtVersionSetzen(K_Dateiversion(Bibliotheken.at(0)));
	if(K_Parameter->QtVersionHohlen().isEmpty())
	{
		K_SchrittFehlgeschlagen();
		K_ErstellenGescheitert();
		emit fertig();
		return false;
	}	
	K_SchrittFertig();
	return true;
}
bool QFrankQt4MergemoduleArbeitVerteilen::K_ZielverzeichnisPruefen()
{
	emit Meldung(trUtf8("Prüfe Zielverzeichnis für die Mergemodule"));
	//Ist das Verzeichnis existen und leer? Wenn es nicht da ist, dann soll es angelegt werden
	QDir Verzeichnis(K_Parameter->ZielverzeichnisHohlen());
	if(!Verzeichnis.exists())
	{
		//versuche es zu erstellen
		if(!Verzeichnis.mkdir(K_Parameter->ZielverzeichnisHohlen()))
		{
			emit Meldung(tr("Das Zielverzeichnis %1 konnte nicht erstellt werden.").arg(K_Parameter->ZielverzeichnisHohlen()));
			K_SchrittFehlgeschlagen();
			K_ErstellenGescheitert();
			emit fertig();
			return false;
		}
	}
	//ist es leer?
	if(Verzeichnis.count()>2)
	{
		emit Meldung(tr("Das Zielverzeichnis %1 ist nicht leer.").arg(K_Parameter->ZielverzeichnisHohlen()));
		K_SchrittFehlgeschlagen();
		K_ErstellenGescheitert();
		emit fertig();
		return false;
	}
	K_SchrittFertig();
	return true;
}
const QString QFrankQt4MergemoduleArbeitVerteilen::K_Dateiversion(const QString &datei)
{
	VS_FIXEDFILEINFO *Dateiinfos;
	DWORD Dummy;
	char *Puffer;
	DWORD LaengeDerInfo=GetFileVersionInfoSize((TCHAR*)datei.utf16(),&Dummy);
	if(LaengeDerInfo>0)
	{
		Puffer=new char[LaengeDerInfo];
		if(GetFileVersionInfo((TCHAR*)datei.utf16(),Dummy,LaengeDerInfo,Puffer))
		{
			UINT Dummy2;
			if(VerQueryValue(Puffer,(TCHAR*)QString("\\").utf16(),(void**)&Dateiinfos,&Dummy2))
			{				
				QString Version=QString("%1.%2.%3.%4").arg(HIWORD(Dateiinfos->dwFileVersionMS)).arg(LOWORD(Dateiinfos->dwFileVersionMS))
													  .arg(HIWORD(Dateiinfos->dwFileVersionLS)).arg(LOWORD(Dateiinfos->dwFileVersionLS));
				delete Puffer;
				return Version;
			}			
		}
		delete Puffer;
	}	
	emit Meldung(tr("Die Version der Datei %1 konnte nicht ermittelt werden.").arg(datei));
	return "";
}
bool QFrankQt4MergemoduleArbeitVerteilen::K_DateienVorhanden(const QStringList &liste)
{
	QString Datei;
	Q_FOREACH(Datei,liste)
	{
		if(!QFile::exists(Datei))
		{
			emit Meldung(tr("Die Datei %1 wurde nicht gefunden.").arg(Datei));
			return false;
			break;
		}
	}
	return true;
}
void QFrankQt4MergemoduleArbeitVerteilen::K_ErstellenGescheitert()
{
	emit Meldung(tr("<html><b><span style=\"color:#ff0000;\">Erstellung der Mergemodule gescheitert.</span></b></html>")); //Rot und Fett
}
void QFrankQt4MergemoduleArbeitVerteilen::K_ErstellenErfolgreich()
{
	emit Meldung(tr("<html><b><span style=\"color:#008000;\">Erstellung der Mergemodule erfolgreich.</span></b></html>")); //Grün und Fett
}
void QFrankQt4MergemoduleArbeitVerteilen::K_SchrittFertig()
{
	emit Meldung(tr("<html><span style=\"color:#008000;\">fertig</span></html>"));
}
void QFrankQt4MergemoduleArbeitVerteilen::K_SchrittFehlgeschlagen()
{
	emit Meldung(tr("<html><span style=\"color:#ff0000;\">fehlgeschlagen</span></html>"));
}

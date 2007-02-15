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
#include "ManifestBearbeiten.h"
#include "KatalogErstellen.h"
#include "KatalogSignieren.h"
#include "WixDateiErstellen.h"
#include <Windows.h>
#include <Winver.h>

QFrankQtSBSAArbeitVerteilen::QFrankQtSBSAArbeitVerteilen(QObject *eltern,QFrankQtSBSAParameter* parameter):QObject(eltern)
{
	K_Parameter=parameter;
}
void QFrankQtSBSAArbeitVerteilen::Loslegen()
{
	if(!K_WindowsSDKPruefen())
		return;
	if(!K_QtPruefen())
		return;
	/*if(!K_ZielverzeichnisPruefen())
		return;
	if(!K_DateienKopieren(K_Parameter->QtBibliothekenHohlen(),K_Parameter->ZielverzeichnisHohlen()))
		return;*/
	//Damit man zum Testen nicht laufend die Dateien löschen muss.
	//Test Anfang
	qDebug()<<"Hier ist noch Testcode";
	//Da die Liste nur beim kopierren korregiert wird, muss es hier nochmal erfolgen, wenn wir kopieren überspringen
	QStringList Temp=K_Parameter->QtBibliothekenHohlen();
	QString NeuerEintrag;
	QRegExp Suchterm("*iconengines/*svg*");
	Suchterm.setPatternSyntax(QRegExp::Wildcard);
	int Position=Temp.indexOf(Suchterm);
	NeuerEintrag=Temp.at(Position);
	NeuerEintrag.replace("svg","iconengineSVG");
	Temp.replace(Position,NeuerEintrag);
	K_Parameter->QtBibliothekenSetzen(Temp);
	//nur zum testen!!!
	//K_Arbeitsschritt=QFrankQtSBSAArbeitVerteilen::KatalogErstellen;
	//K_KatalogeErstellen();
	K_Arbeitsschritt=QFrankQtSBSAArbeitVerteilen::WixDateienErstellen;
	K_WixDateienErstellen();
	//testEnde
	
	//K_ManifesteExportieren();	
}
void QFrankQtSBSAArbeitVerteilen::K_ManifesteExportieren()
{
	K_Arbeitsschritt=QFrankQtSBSAArbeitVerteilen::ManifestExportieren;
	emit Meldung(tr("Extrahiere Manifeste"));
	//zum testen nur 1 Thread 
	//K_AnzahlDerProzesse=1;

	K_AnzahlDerProzesse=K_Parameter->QtBibliothekenHohlen().count();
#ifndef QT_NO_DEBUG
	qDebug("%s K_ManifesteExportieren: Anzahl der Prozesse: %i",this->metaObject()->className(),K_AnzahlDerProzesse);
#endif
	emit FortschrittsanzeigeMaximum(K_AnzahlDerProzesse);
	for(int Threadnummer=0;Threadnummer<K_AnzahlDerProzesse;Threadnummer++)
	{
		QFrankQtSBSAManifestExportieren *Export=new QFrankQtSBSAManifestExportieren(K_Parameter,this);
		Export->DateinummerFestlegen(Threadnummer);
		connect(Export,SIGNAL(fertig(QFrankQtSBSABasisThread*)),this,SLOT(K_ThreadFertig( QFrankQtSBSABasisThread*)));
		Export->start();
	}		
}
void QFrankQtSBSAArbeitVerteilen::K_ManifesteBearbeiten()
{
	emit Meldung(tr("Bearbeite Manifeste"));
	//zum testen nur 1 Thread 
	//K_AnzahlDerProzesse=1;
	K_AnzahlDerProzesse=K_Parameter->QtBibliothekenHohlen().count();
#ifndef QT_NO_DEBUG
	qDebug("%s K_ManifesteBearbeiten: Anzahl der Prozesse: %i",this->metaObject()->className(),K_AnzahlDerProzesse);
#endif
	emit FortschrittsanzeigeMaximum(K_AnzahlDerProzesse);
	for(int Threadnummer=0;Threadnummer<K_AnzahlDerProzesse;Threadnummer++)
	{
		QFrankQtSBSAManifestBearbeiten *bearbeiten=new QFrankQtSBSAManifestBearbeiten(K_Parameter,this);
		bearbeiten->DateinummerFestlegen(Threadnummer);
		connect(bearbeiten,SIGNAL(fertig(QFrankQtSBSABasisThread*)),this,SLOT(K_ThreadFertig( QFrankQtSBSABasisThread*)));		
		bearbeiten->start();
	}	
}
void QFrankQtSBSAArbeitVerteilen::K_KatalogeErstellen()
{
	emit Meldung(tr("Erstelle Kataloge"));
	K_AnzahlDerProzesse=K_Parameter->QtBibliothekenHohlen().count();
	//zum testen nur 1 Thread 
	//K_AnzahlDerProzesse=1;
	emit FortschrittsanzeigeMaximum(K_AnzahlDerProzesse);
	for(int Threadnummer=0;Threadnummer<K_AnzahlDerProzesse;Threadnummer++)
	{
		QFrankQtSBSAKatalogErstellen *erstellen=new QFrankQtSBSAKatalogErstellen(K_Parameter,this);
		erstellen->DateinummerFestlegen(Threadnummer);
		connect(erstellen,SIGNAL(fertig(QFrankQtSBSABasisThread*)),this,SLOT(K_ThreadFertig( QFrankQtSBSABasisThread*)));		
		erstellen->start();
	}	
}
void QFrankQtSBSAArbeitVerteilen::K_KatalogeSignieren()
{
	emit Meldung(tr("Signiere Kataloge"));
	K_AnzahlDerProzesse=K_Parameter->QtBibliothekenHohlen().count();
	//zum testen nur 1 Thread 
	//K_AnzahlDerProzesse=1;
	emit FortschrittsanzeigeMaximum(K_AnzahlDerProzesse);
	for(int Threadnummer=0;Threadnummer<K_AnzahlDerProzesse;Threadnummer++)
	{
		QFrankQtSBSAKatalogSignieren *signieren=new QFrankQtSBSAKatalogSignieren(K_Parameter,this);
		signieren->DateinummerFestlegen(Threadnummer);
		connect(signieren,SIGNAL(fertig(QFrankQtSBSABasisThread*)),this,SLOT(K_ThreadFertig( QFrankQtSBSABasisThread*)));		
		signieren->start();
	}	
}
void QFrankQtSBSAArbeitVerteilen::K_WixDateienErstellen()
{
	emit Meldung(tr("Erstelle Wix Dateien"));
	K_AnzahlDerProzesse=K_Parameter->QtBibliothekenHohlen().count();
	emit FortschrittsanzeigeMaximum(K_AnzahlDerProzesse);
	for(int Threadnummer=0;Threadnummer<K_AnzahlDerProzesse;Threadnummer++)
	{
		QFrankQtSBSAWixDateiErstellen *erstellen=new QFrankQtSBSAWixDateiErstellen(K_Parameter,this);
		erstellen->DateinummerFestlegen(Threadnummer);
		connect(erstellen,SIGNAL(fertig(QFrankQtSBSABasisThread*)),this,SLOT(K_ThreadFertig( QFrankQtSBSABasisThread*)));		
		erstellen->start();
	}
}
void QFrankQtSBSAArbeitVerteilen::K_ThreadFertig(QFrankQtSBSABasisThread *welcher)
{
	emit FortschrittsanzeigeSchritt();
	static bool FehlerAufgetreten=false;
	K_AnzahlDerProzesse--;
	int Fehlercode=welcher->FehlercodeHohlen();	
	if(Fehlercode!=0)
	{
		FehlerAufgetreten=true;
		emit Meldung(welcher->FehlermeldungHohlen());
	}
#ifndef QT_NO_DEBUG
	int Threadnummer=welcher->Threadnummer();
	qDebug("%s K_ThreadFertig: Thread %i ist fertig mit dem Fehlercode %i",this->metaObject()->className(),Threadnummer,Fehlercode);
#endif
	if(K_AnzahlDerProzesse==0)
	{
		if(!FehlerAufgetreten)
		{
			K_SchrittFertig();
			//Da es keinen Fehler gab, den nächsten Schritt bitte:)
			FehlerAufgetreten=false;
			K_NaechsterArbeitsschritt();
		}
		else
		{
			K_SchrittFehlgeschlagen();
			K_ErstellenGescheitert();
			emit fertig();
		}		
	}	
}
void QFrankQtSBSAArbeitVerteilen::K_NaechsterArbeitsschritt()
{
	K_Arbeitsschritt++;
	//Was kommt als nächtses??
	switch(K_Arbeitsschritt)
	{
		case QFrankQtSBSAArbeitVerteilen::ManifestBearbeiten:
																		K_ManifesteBearbeiten();
																		break;
		case QFrankQtSBSAArbeitVerteilen::KatalogErstellen:
																		K_KatalogeErstellen();
																		break;
		case QFrankQtSBSAArbeitVerteilen::KatalogSignieren:
																		K_KatalogeSignieren();
																		break;
		case QFrankQtSBSAArbeitVerteilen::WixDateienErstellen:
																		break;
																		K_WixDateienErstellen();
		default:
#ifndef QT_NO_DEBUG
																		qDebug("%s K_NaechsterArbeitsschritt: alle Schritt fertig",this->metaObject()->className());
#endif
																		K_ErstellenErfolgreich();
																		emit fertig();
																		break;
	};
}
bool QFrankQtSBSAArbeitVerteilen::K_DateienKopieren(const QStringList &dateiliste,const QString &zielverzeichnis)
{
	emit Meldung(tr("Kopiere Qt4 Bibliotheken"));
	emit FortschrittsanzeigeMaximum(dateiliste.count());
	QString Datei;
	QString Ziel;
	Q_FOREACH(Datei,dateiliste)
	{
		Datei.replace("/","\\");
		Ziel=zielverzeichnis+Datei.right(Datei.length()-Datei.lastIndexOf("\\"));	
		//Den Namen qsvg gibt es 2 mal. Das iconengine PlugIn wird umbenannt
		bool EintragKorregieren=false;
		if(Datei.contains("iconengines") && Datei.contains("svg"))
		{
			Ziel.replace("svg","iconengineSVG");
			EintragKorregieren=true;
		}
		if(!QFile::copy(Datei,Ziel))
		{
			emit Meldung(tr("Die Datei %1 konnte nicht nach %2 kopiert werden.")
							.arg(Datei).arg(zielverzeichnis+Datei.right(Datei.length()-Datei.lastIndexOf("\\"))));
			K_SchrittFehlgeschlagen();
			K_ErstellenGescheitert();
			emit fertig();
			return false;
			break;
		}
		if(EintragKorregieren)
		{
			QStringList Temp=dateiliste;
			QString NeuerEintrag;
			QRegExp Suchterm("*iconengines/*svg*");
			Suchterm.setPatternSyntax(QRegExp::Wildcard);
			int Position=Temp.indexOf(Suchterm);
			NeuerEintrag=Temp.at(Position);
			NeuerEintrag.replace("svg","iconengineSVG");
			Temp.replace(Position,NeuerEintrag);
			K_Parameter->QtBibliothekenSetzen(Temp);
		}
		emit FortschrittsanzeigeSchritt();
	}
	K_SchrittFertig();	
	return true;
}
bool QFrankQtSBSAArbeitVerteilen::K_WindowsSDKPruefen()
{
	emit Meldung(trUtf8("Prüfe Windows SDK"));

	//Haben wir alle SDK Werkzeuge??
	QStringList Werkzeugliste;
	Werkzeugliste<< K_Parameter->WindowsSDKPfadHohlen()+"\\mt.exe"<<
					K_Parameter->WindowsSDKPfadHohlen()+"\\signtool.exe"<<
					K_Parameter->WindowsSDKPfadHohlen()+"\\Depends.Exe"<<
					K_Parameter->WindowsSDKPfadHohlen()+"\\MakeCat.Exe";
	emit FortschrittsanzeigeMaximum(Werkzeugliste.count());
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
bool QFrankQtSBSAArbeitVerteilen::K_QtPruefen()
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
	//Optionale Dateien (Plug-Ins)
	QString Plugin;
	QString QtPlugInBereich;
	QDir PluginVerzeichnis;
	QStringList QtPlugInBereiche;
	QtPlugInBereiche <<"sqldrivers"<<"accessible"<<"codecs"<<"iconengines"<<"imageformats";
	Q_FOREACH(QtPlugInBereich,QtPlugInBereiche)
	{
		PluginVerzeichnis.setPath(K_Parameter->QtPfadHohlen()+"\\..\\plugins\\"+QtPlugInBereich);
		//imageformats iconengines accessible
		if(QtPlugInBereich=="sqldrivers" || QtPlugInBereich=="codecs")
			PluginVerzeichnis.setNameFilters(QStringList("*[a-c,e-z,0-9].dll"));
		else
			PluginVerzeichnis.setNameFilters(QStringList("*[a-c,e-z,0-9][0-9].dll"));		
		Q_FOREACH(Plugin,PluginVerzeichnis.entryList(QDir::Files))
		{			
			Bibliotheken<<PluginVerzeichnis.canonicalPath()+"/"+Plugin;			
		}
	}	
	K_Parameter->QtBibliothekenSetzen(Bibliotheken);
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
bool QFrankQtSBSAArbeitVerteilen::K_ZielverzeichnisPruefen()
{
	emit Meldung(trUtf8("Prüfe Zielverzeichnis für die Mergemodule"));
	//Ist das Verzeichnis existen und leer? Wenn es nicht da ist, dann soll es angelegt werden
	QDir Verzeichnis(K_Parameter->ZielverzeichnisHohlen());
	if(!Verzeichnis.exists())
	{
		//versuche es zu erstellen
		if(!Verzeichnis.mkpath(K_Parameter->ZielverzeichnisHohlen()))
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
const QString QFrankQtSBSAArbeitVerteilen::K_Dateiversion(const QString &datei)
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
bool QFrankQtSBSAArbeitVerteilen::K_DateienVorhanden(const QStringList &liste)
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
		emit FortschrittsanzeigeSchritt();
	}
	return true;
}
void QFrankQtSBSAArbeitVerteilen::K_ErstellenGescheitert()
{
	emit Meldung(tr("<html><b><span style=\"color:#ff0000;\">Erstellung der Mergemodule gescheitert.</span></b></html>")); //Rot und Fett
}
void QFrankQtSBSAArbeitVerteilen::K_ErstellenErfolgreich()
{
	emit Meldung(tr("<html><b><span style=\"color:#008000;\">Erstellung der Mergemodule erfolgreich.</span></b></html>")); //Grün und Fett
}
void QFrankQtSBSAArbeitVerteilen::K_SchrittFertig()
{
	emit Meldung(tr("<html><span style=\"color:#008000;\">fertig</span></html>"));
}
void QFrankQtSBSAArbeitVerteilen::K_SchrittFehlgeschlagen()
{
	emit Meldung(tr("<html><span style=\"color:#ff0000;\">fehlgeschlagen</span></html>"));
}

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
	if(!K_DateienKopieren())
		return;*/
	//Damit man zum Testen nicht laufend die Dateien löschen muss.
	//Test Anfang
	qDebug()<<"Hier ist noch Testcode";
	
	//nur zum testen!!!
	//qDebug()<<K_Parameter->QtBibliothekenHohlen();
	//K_Arbeitsschritt=QFrankQtSBSAArbeitVerteilen::KatalogErstellen;
	//K_KatalogeErstellen();
	K_Arbeitsschritt=QFrankQtSBSAArbeitVerteilen::ManifestBearbeiten;
	K_ManifesteBearbeiten();
	
	//K_Arbeitsschritt=QFrankQtSBSAArbeitVerteilen::WixDateienErstellen;
	//K_WixDateienErstellen();
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
																		K_WixDateienErstellen();
																		break;																		
		default:
#ifndef QT_NO_DEBUG
																		qDebug("%s K_NaechsterArbeitsschritt: alle Schritt fertig",this->metaObject()->className());
#endif
																		K_ErstellenErfolgreich();
																		emit fertig();
																		break;
	};
}
bool QFrankQtSBSAArbeitVerteilen::K_DateienKopieren()
{	
	emit Meldung(tr("Kopiere Qt4 Bibliotheken"));
	emit FortschrittsanzeigeMaximum(K_Parameter->QtBibliothekenHohlen().count());
	QString Ziel,Quelle;
	Q_FOREACH(QFrankQtSBSAQtModul Datei,K_Parameter->QtBibliothekenHohlen())
	{
		if(!Datei.istPlugIn())
		{
			Ziel=K_Parameter->ZielverzeichnisHohlen()+"\\"+Datei.DateinameHohlen();
			Quelle=K_Parameter->QtPfadHohlen()+"\\"+Datei.DateinameHohlen();
		}
		else
		{
			//Muss die Kategorie erst noch erstellt werden??
			QDir PlugInVerzeichnis(K_Parameter->ZielverzeichnisHohlen()+"\\"+Datei.PlugInTypeHohlen());			
			if(!PlugInVerzeichnis.exists())
				PlugInVerzeichnis.mkdir(PlugInVerzeichnis.absolutePath());
			Ziel=K_Parameter->ZielverzeichnisHohlen()+"\\"+Datei.PlugInTypeHohlen()+"\\"+Datei.DateinameHohlen();
			QDir PluginsBasisverzeichnis(K_Parameter->QtPfadHohlen());
			PluginsBasisverzeichnis.cdUp();
			Quelle=PluginsBasisverzeichnis.absolutePath()+"\\plugins\\"+Datei.PlugInTypeHohlen()+"\\"+Datei.DateinameHohlen();
		}
		if(!QFile::copy(Quelle,Ziel))
		{
			emit Meldung(tr("Die Datei %1 konnte nicht nach %2 kopiert werden.")
							.arg(Quelle).arg(Ziel));
			K_SchrittFehlgeschlagen();
			K_ErstellenGescheitert();
			emit fertig();
			return false;
			break;
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
	QList<QFrankQtSBSAQtModul> Werkzeugliste;
	Werkzeugliste<< QFrankQtSBSAQtModul(K_Parameter->WindowsSDKPfadHohlen()+"\\mt.exe")<<
					QFrankQtSBSAQtModul(K_Parameter->WindowsSDKPfadHohlen()+"\\signtool.exe")<<
					QFrankQtSBSAQtModul(K_Parameter->WindowsSDKPfadHohlen()+"\\Depends.Exe")<<
					QFrankQtSBSAQtModul(K_Parameter->WindowsSDKPfadHohlen()+"\\MakeCat.Exe");
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
	QList<QFrankQtSBSAQtModul> Bibliotheken;
	Bibliotheken << QFrankQtSBSAQtModul("Qt3Support4.dll")<<
					QFrankQtSBSAQtModul("QtAssistantClient4.dll")<<
					QFrankQtSBSAQtModul("QtCore4.dll")<<
					QFrankQtSBSAQtModul("QtGui4.dll")<<
					QFrankQtSBSAQtModul("QtNetwork4.dll")<<
					QFrankQtSBSAQtModul("QtOpenGL4.dll")<<
					QFrankQtSBSAQtModul("QtSql4.dll")<<
					QFrankQtSBSAQtModul("QtSvg4.dll")<<
					QFrankQtSBSAQtModul("QtXml4.dll");
	K_Parameter->QtBibliothekenSetzen(Bibliotheken);
	//true=Qt Bestandteile
	if(!K_DateienVorhanden(Bibliotheken,true))
	{
		K_SchrittFehlgeschlagen();
		K_ErstellenGescheitert();
		emit fertig();
		return false;
	}
	//Optionale Dateien (Plug-Ins)
	QFrankQtSBSAQtModul Plugin;
	QDir PluginVerzeichnis;
	QStringList QtPlugInBereiche;
	QtPlugInBereiche <<"sqldrivers"<<"accessible"<<"codecs"<<"iconengines"<<"imageformats";
	Q_FOREACH(QString QtPlugInBereich,QtPlugInBereiche)
	{
		PluginVerzeichnis.setPath(K_Parameter->QtPfadHohlen()+"\\..\\plugins\\"+QtPlugInBereich);
		//imageformats iconengines accessible
		if(QtPlugInBereich=="sqldrivers" || QtPlugInBereich=="codecs")
			PluginVerzeichnis.setNameFilters(QStringList("*[a-c,e-z,0-9].dll"));
		else
			PluginVerzeichnis.setNameFilters(QStringList("*[a-c,e-z,0-9][0-9].dll"));		
		Q_FOREACH(QString QtPlugin,PluginVerzeichnis.entryList(QDir::Files))
		{
			Plugin=QFrankQtSBSAQtModul(QtPlugin,true);
			Plugin.PlugInTypeSetzen(QtPlugInBereich);
			Bibliotheken<<Plugin;			
		}
	}	
	K_Parameter->QtBibliothekenSetzen(Bibliotheken);
	K_Parameter->QtVersionSetzen(K_Dateiversion(K_Parameter->QtPfadHohlen()+"\\"+Bibliotheken.at(0).DateinameHohlen()));
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
bool QFrankQtSBSAArbeitVerteilen::K_DateienVorhanden(const QList<QFrankQtSBSAQtModul> &liste,bool qtdateien)
{	
	Q_FOREACH(QFrankQtSBSAQtModul Datei,liste)
	{
		QString Pruefling;
		if(qtdateien)
		{
			if(Datei.istPlugIn())
			{
				QDir Basisverzeichnis(K_Parameter->QtPfadHohlen());
				Basisverzeichnis.cdUp();
				Pruefling=Basisverzeichnis.absolutePath()+"\\plugins\\"+Datei.PlugInTypeHohlen()+"\\"+Datei.DateinameHohlen();
			}
			else
				Pruefling=K_Parameter->QtPfadHohlen()+"\\"+Datei.DateinameHohlen();
		}
		else
			Pruefling=Datei.DateinameHohlen();
		if(!QFile::exists(Pruefling))
		{
			emit Meldung(tr("Die Datei %1 wurde nicht gefunden.").arg(Pruefling));
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

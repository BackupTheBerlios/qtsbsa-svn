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

#include "WixDateiErstellen.h"
#include "QtModul.h"
#include "Parameter.h"

using namespace QFrank;

//<Dependency RequiredId="QtCoreModul" RequiredLanguage="0" RequiredVersion="4.2.2.0" />
QtSBSAWixDateiErstellen::QtSBSAWixDateiErstellen( QtSBSAParameter* parameter,QObject* eltern)
							  :QtSBSABasisThread(parameter,eltern)
{
}
void QtSBSAWixDateiErstellen::run()
{
	QString Dateiname=K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).DateinameHohlen();
	QString Modulname=Dateiname.left(Dateiname.size()-4);// 4= .dll
	QFile Vorlagendatei;
	if(K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).istPlugIn())
	{
		Vorlagendatei.setFileName(":/QtPlugIn.wxs");
		Dateiname="Qt_"+K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).PlugInTypeHohlen()+"_"+Modulname;
		Modulname=Dateiname;
	}
	else if(K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).istSprachpaket())
	{
		Vorlagendatei.setFileName(":/QtSprachpaket.wxs");
		Dateiname="QtSprachpaket_"+Dateiname.mid(3,Dateiname.size()-6);
		Modulname=Dateiname;
	}
	else
	{
		Vorlagendatei.setFileName(":/QtModul.wxs");	
		Modulname=Modulname.left(Modulname.size()-1);//Nummer abschneiden z.B. QtGui4
		Dateiname=Modulname;
	}
	QFile Zieldatei(K_Parameter->ZielverzeichnisHohlen()+"/"+Dateiname+".wxs");
	if(!Zieldatei.open(QIODevice::WriteOnly))
	{
		K_Fehlercode=1;
		K_Fehlermeldung=tr("Die Datei %1 konnte nicht angelegt werden.").arg(K_Parameter->ZielverzeichnisHohlen()+"\\"+Dateiname);
		//emit
		fertig(this);
	}
	Vorlagendatei.open(QIODevice::ReadOnly);
	QTextStream Quelle(&Vorlagendatei);
	QTextStream Ziel(&Zieldatei);
	Quelle.setCodec("UTF-8");
	Ziel.setCodec("UTF-8");
	QString Zeile;
	while(!Quelle.atEnd())
	{
		Zeile=Quelle.readLine();
		K_PlatzhalterErsetzen(Zeile,Modulname);
		Ziel<<Zeile;
	}
	Vorlagendatei.close();
	Ziel.flush();
	Zieldatei.close();
#ifndef QT_NO_DEBUG
		qDebug("%s run: Datei %s erstellt.",this->metaObject()->className(),qPrintable(Zieldatei.fileName()));
#endif
	K_Fehlercode=0;
	//emit
	fertig(this);
}
void QtSBSAWixDateiErstellen::K_PlatzhalterErsetzen(QString &zeile,const QString &modulname)const
{
	//Platzhalter ersetzen;
	const static uchar GUUIDLAENGE=36;
	const static uchar MAXMODULEIDLAENGE=72;
	static uint Textlaenge;
	if(zeile.contains("$MODULID"))
	{
		// Die Modul ID darf nicht länger als 72 Zeichen sein. +1 Da ein Punkt später in der MSI Tabelle eingefügt wird
		Textlaenge=QString(modulname+"Modul").size()+GUUIDLAENGE+1;
		if(Textlaenge>MAXMODULEIDLAENGE)
		{
			//kürzen wir ein was zu lang ist.
			Textlaenge=Textlaenge-MAXMODULEIDLAENGE;
			zeile.replace("$MODULID",QString(modulname).remove(modulname.size()-Textlaenge,Textlaenge)+"Modul");
		}
		else
			zeile.replace("$MODULID",modulname+"Modul");
	}

	if(zeile.contains("$MODULVERSIONLANG"))
		zeile.replace("$MODULVERSIONLANG",K_Parameter->QtVersionHohlen());

	if(zeile.contains("$MODULVERSIONKURZ"))
	{
		QString tmp=K_Parameter->QtVersionHohlen();
		tmp=tmp.left(tmp.lastIndexOf("."));
		zeile.replace("$MODULVERSIONKURZ",tmp);
	}

	if(zeile.contains("$MODULPAKETGUID"))
		zeile.replace("$MODULPAKETGUID",QUuid::createUuid().toString().remove("{").remove("}"));

	if(zeile.contains("$MODULHERSTELLER"))
		zeile.replace("$MODULHERSTELLER",K_Parameter->MergemodulentwicklerHohlen());

	if(zeile.contains("$MODULASSEMBLYENDTWICKLER"))
		zeile.replace("$MODULASSEMBLYENDTWICKLER",K_Parameter->ManifestentwicklerHohlen());

	if(zeile.contains("$PLUINKATEGORIE"))
		zeile.replace("$PLUINKATEGORIE",K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).PlugInTypeHohlen());

	if(zeile.contains("$KOMPONENTENGUID"))
		zeile.replace("$KOMPONENTENGUID",QUuid::createUuid().toString().remove("{").remove("}"));

	if(zeile.contains("$MODULMANIFESTQUELLE"))
	{
		QString tmp=K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).DateinameHohlen()+".manifest";
		if(K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).istPlugIn())
			tmp=K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).PlugInTypeHohlen()+"\\"+tmp;
		zeile.replace("$MODULMANIFESTQUELLE",tmp);
	}
	
	if(zeile.contains("$MODULMANIFESTDATEINAME"))
		zeile.replace("$MODULMANIFESTDATEINAME",K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).DateinameHohlen()+".manifest");

	if(zeile.contains("$MODULDATEIQUELLE"))
	{
		QString tmp=K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).DateinameHohlen();
		if(K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).istPlugIn())
			tmp=K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).PlugInTypeHohlen()+"\\"+tmp;
		else if(K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).istSprachpaket())
			tmp="Uebersetzungen\\"+tmp;
		zeile.replace("$MODULDATEIQUELLE",tmp);
	}

	if(zeile.contains("$MODULDATEINAME"))
		zeile.replace("$MODULDATEINAME",K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).DateinameHohlen());

	if(zeile.contains("$MODULKATALOGQUELLE"))
		zeile.replace("$MODULKATALOGQUELLE",K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).DateinameHohlen()+".cat");

	if(zeile.contains("$MODULKATALOGDATEINAME"))
		zeile.replace("$MODULKATALOGDATEINAME",K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).DateinameHohlen()+".cat");

	if(zeile.contains("$MODULMANIFESTID"))
	{
		QString tmp=K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).DateinameHohlen();
		tmp=tmp.left(tmp.size()-4);// 4= .dll
		zeile.replace("$MODULMANIFESTID",tmp.toLower()+"manifest");
	}

	if(zeile.contains("$MODULDATEIID"))
		zeile.replace("$MODULDATEIID",K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).DateinameHohlen().toLower().replace(".",""));

	if(zeile.contains("$MODULKATALOGID"))
		zeile.replace("$MODULKATALOGID",K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).DateinameHohlen().toLower().replace(".","")
										+"katalog");

	if(zeile.contains("$MODULKOMPONENTENID"))
	{
		QString tmp=K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).DateinameHohlen().toLower();
		tmp=tmp.left(tmp.size()-4);// 4= .dll
		zeile.replace("$MODULKOMPONENTENID",tmp);
	}
	if(zeile.contains("$MODULABHAENGIGKEIT"))
		K_AbhaengigkeitenEintragen(zeile);
}
void QtSBSAWixDateiErstellen::K_AbhaengigkeitenEintragen(QString &zeile)const
{
	//Damit der Platzhalter entfernt wird
	zeile="";
	Q_FOREACH(QString Abhaengigkeit,K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer).AbhaenigkeitenHohlen())
	{		
		zeile=zeile+"<Dependency RequiredId=\""+Abhaengigkeit+"Modul\" RequiredLanguage=\"0\" RequiredVersion=\""+K_Parameter->QtVersionHohlen()+"\" />";
	}	
}

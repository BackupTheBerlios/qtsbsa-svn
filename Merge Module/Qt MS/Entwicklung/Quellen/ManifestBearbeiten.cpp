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

#include "ManifestBearbeiten.h"
#include "Parameter.h"

QFrankQt4MergemoduleManifestBearbeiten::QFrankQt4MergemoduleManifestBearbeiten(const QFrankQt4MergemoduleParameter* parameter,QObject* eltern)
										:QFrankQt4MergemoduleBasisThread(parameter,eltern)
{
}
void QFrankQt4MergemoduleManifestBearbeiten::run()
{
	QString Dateiname=K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer);
	Dateiname.replace('/','\\');
	K_Dateiname=Dateiname.right(Dateiname.length()-Dateiname.lastIndexOf("\\"));
	Dateiname=K_Parameter->ZielverzeichnisHohlen()+K_Dateiname+".manifest";
	if(!K_QtKomponenteErmitteln(Dateiname))
	{
		K_Fehlercode=1;
		K_Fehlermeldung=trUtf8("Nicht erkannte oder ungültige Qt4 Komponente %1.").arg(Dateiname);
		emit fertig(this);
		return;
	}
	QDomDocument Manifest;
	QFile Datei(Dateiname);
	if(!Datei.open(QIODevice::ReadOnly))
	{
		K_Fehlercode=1;
		K_Fehlermeldung=trUtf8("Die Datei %1 konnte nicht göffnet werden.").arg(Dateiname);
		emit fertig(this);
		return;
	}
	if(!Manifest.setContent(&Datei))
	{
		K_Fehlercode=1;
		K_Fehlermeldung=trUtf8("%1 ist keine gültige XML Datei.").arg(Dateiname);
		Datei.close();
		emit fertig(this);
		return;
	}
	Datei.close();
	// Auf gehts:)

	QDomElement Rootelement=Manifest.documentElement();
	QDomElement AssemblyID=K_AssemblyIdentifikationEinfuegen(Manifest);
	Rootelement.insertBefore(AssemblyID,Rootelement.firstChild());
	//File Tag einfügen
	QDomElement DateiTag=Manifest.createElement("file");
	DateiTag.setAttribute("name",K_Dateiname.remove(0,1));
	Rootelement.insertAfter(DateiTag,AssemblyID);
	//Abhängigkeiten von anderen Qt Komponenten ermitteln
	if(!K_AbhaengigkeitenErmitteln())
	{
		K_Fehlercode=1;
		emit fertig(this);
		return;
	}
	QString Komponente;
	Q_FOREACH(Komponente,K_BenoetigeQtKomponenten)
	{
		if(!K_AbhaengigkeitEinfuegen(Manifest,Komponente))
		{
			K_Fehlercode=1;
			emit fertig(this);
			return;
			break;
		}
	}

	//Und wieder in die Datei Speichern:)
	if(!Datei.open(QIODevice::WriteOnly))
	{
		K_Fehlercode=1;
		K_Fehlermeldung=trUtf8("%1 konnte nicht überschrieben werden.").arg(Dateiname);
		Datei.close();
		emit fertig(this);
		return;
	}
	QTextStream Datenstrom(&Datei);
	Datenstrom.setCodec("UTF-8");
	Datenstrom<<Manifest.toString();
	Datenstrom.flush();
	Datei.close();
	K_Fehlercode=0;
	emit fertig(this);
}
const QDomElement QFrankQt4MergemoduleManifestBearbeiten::K_AssemblyIdentifikationEinfuegen(QDomDocument &manifest)
{
	QDomElement Identifikation=manifest.createElement("assemblyIdentity");
	Identifikation.setAttribute("type","win32");
	Identifikation.setAttribute("name",K_Parameter->EntwicklerHohlen()+".Qt."+K_QtKomponente);
	Identifikation.setAttribute("processorArchitecture",K_Parameter->CPUTypeHohlen());
	Identifikation.setAttribute("version",K_Parameter->QtVersionHohlen());
	Identifikation.setAttribute("publicKeyToken",K_Parameter->publicKeyTokenHohlen());
	return QDomElement(Identifikation);
}
bool QFrankQt4MergemoduleManifestBearbeiten::K_AbhaengigkeitEinfuegen(QDomDocument &manifest,const QString &welche)
{												 
	/*QDomNode Abhaengigkeiten=manifest.documentElement().namedItem("dependency");
	if(Abhaengigkeiten.isNull())
	{
		K_Fehlermeldung=trUtf8("Die Manifestdatei %1 ist beschädigt.(Ast \"dependency\" fehlt)").arg(K_Parameter->ZielverzeichnisHohlen()+K_Dateiname+".manifest");
		return false;
	}
	QDomNode AbhaengigeAssemblys=Abhaengigkeiten.namedItem("dependentAssembly");
	if(AbhaengigeAssemblys.isNull())
	{
		K_Fehlermeldung=trUtf8("Die Manifestdatei %1 ist beschädigt.(Ast \"dependency/dependentAssembly\" fehlt)")
								.arg(K_Parameter->ZielverzeichnisHohlen()+K_Dateiname+".manifest");
		return false;
	}
	//So jetzt ein neuer Eintrag einfügen
	K_QtKomponente=welche;
	AbhaengigeAssemblys.appendChild(K_AssemblyIdentifikationEinfuegen(manifest));
	*/
	QDomElement Root=manifest.documentElement();
	QDomElement Abhaengigkeiten=manifest.createElement("dependency");
	QDomElement AnhaengigeAssemblys=manifest.createElement("dependentAssembly");
	Abhaengigkeiten.appendChild(AnhaengigeAssemblys);
	Root.appendChild(Abhaengigkeiten);
	//So jetzt ein neuer Eintrag einfügen
	K_QtKomponente=welche;
	AnhaengigeAssemblys.appendChild(K_AssemblyIdentifikationEinfuegen(manifest));	
	return true;
}
bool QFrankQt4MergemoduleManifestBearbeiten::K_QtKomponenteErmitteln(const QString &datei)
{	
	//Welche Komponete wird bearbeitet??
	if(datei.contains("core4",Qt::CaseInsensitive))
		K_QtKomponente="QtCore";
	else if (datei.contains("gui4",Qt::CaseInsensitive))
		K_QtKomponente="QtGui";		
	else if (datei.contains("AssistantClient4",Qt::CaseInsensitive))
		K_QtKomponente="QtAssistantClient";
	else if (datei.contains("network4",Qt::CaseInsensitive))
		K_QtKomponente="QtNetwork";
	else if (datei.contains("opengl4",Qt::CaseInsensitive))
		K_QtKomponente="QtOpenGl";
	else if (datei.contains("sql4",Qt::CaseInsensitive))
		K_QtKomponente="QtSQL";
	else if (datei.contains("svg4",Qt::CaseInsensitive))
		K_QtKomponente="QtSvg";
	else if (datei.contains("xml4",Qt::CaseInsensitive))
		K_QtKomponente="QtXML";
	else if (datei.contains("3Support4",Qt::CaseInsensitive))
		K_QtKomponente="Qt3Support";
	else if (datei.contains("cncodecs",Qt::CaseInsensitive))
		K_QtKomponente="PlugIn-TextCodecChina";
	else if (datei.contains("jpcodecs",Qt::CaseInsensitive))
		K_QtKomponente="PlugIn-TextCodecJapan";
	else if (datei.contains("krcodecs",Qt::CaseInsensitive))
		K_QtKomponente="PlugIn-TextCodecKorea";
	else if (datei.contains("twcodecs",Qt::CaseInsensitive))
		K_QtKomponente="PlugIn-TextCodecTaiwan";
	else if (datei.contains("odbc",Qt::CaseInsensitive))
		K_QtKomponente="PlugIn-SqlODBC";
	else if (datei.contains("mysql",Qt::CaseInsensitive))
		K_QtKomponente="PlugIn-SqlMySQL";
	else if (datei.contains("sqlite",Qt::CaseInsensitive))
		K_QtKomponente="PlugIn-SqlSqlite";
	else if (datei.contains("gif",Qt::CaseInsensitive))
		K_QtKomponente="PlugIn-BildGif";
	else if (datei.contains("jpeg",Qt::CaseInsensitive))
		K_QtKomponente="PlugIn-BildJpeg";
	else if (datei.contains("mng",Qt::CaseInsensitive))
		K_QtKomponente="PlugIn-BildMng";
	else if (datei.contains("svg",Qt::CaseInsensitive) && !datei.contains("iconengine",Qt::CaseInsensitive))
		K_QtKomponente="PlugIn-BildSvg";
	else if (datei.contains("svg",Qt::CaseInsensitive) && datei.contains("iconengine",Qt::CaseInsensitive))
		K_QtKomponente="PlugIn-IconengineSvg";
	else if (datei.contains("accessiblewidgets",Qt::CaseInsensitive))
		K_QtKomponente="PlugIn-Eingabehilfen";
	else if (datei.contains("accessiblecompatwidgets",Qt::CaseInsensitive))
		K_QtKomponente="PlugIn-Qt3Eingabehilfen";
	else
		return false;
	return true;
}
bool QFrankQt4MergemoduleManifestBearbeiten::K_AbhaengigkeitenErmitteln()
{
	/*
		Abängigkeiten bearbeiten
		Microsoft SDKs\Windows\v6.0\Bin>Depends.Exe /c /oc:f:\temp\Qt4\Merrgemodule\1.txt f:\temp\Qt4\Merrgemodule\QtGui4.dll
	*/
	QProcess Prozess;
	Prozess.setProcessChannelMode(QProcess::MergedChannels);
	QStringList Argumente;
	Argumente<<"/c"<<"/oc:"+K_Dateiname+".braucht"<<K_Dateiname;
	Prozess.setWorkingDirectory(K_Parameter->ZielverzeichnisHohlen());
	Prozess.start("\""+K_Parameter->WindowsSDKPfadHohlen()+"\\Depends.Exe\"",Argumente);
	connect(&Prozess,SIGNAL(finished(int)),this,SLOT(K_ProzessFertig(int)));
	if(!Prozess.waitForStarted(3000))
	{
		K_Fehlermeldung=trUtf8("Prozess zur Abhängigkeitsermittlung für %1 konnte nicht gestartet werden.").arg(K_Parameter->ZielverzeichnisHohlen()+"\\"
																												+K_Dateiname);
		return false;
	}
	//depends gibt 522 als Rückegabecode zurück, wenn alles gut ging.
	if(exec()!=522)
	{		
#ifndef QT_NO_DEBUG
		QString Fehlermeldung=QString(Prozess.readAll());
		Fehlermeldung.remove("\r");
		Fehlermeldung.remove("\n");
		qDebug("%s K_AbhaengigkeitenErmitteln: Nummer: %i\r\nStatuscode: %i\r\nFehlertext: %s\r\nAusgabe:\r\n%s",this->metaObject()->className(),
																												 K_Dateinummer,Prozess.exitCode(),
																												 qPrintable(QString(Prozess.errorString())),
									  																			 qPrintable(Fehlermeldung));
#endif
		K_Fehlermeldung=trUtf8("Fehler beim Ausführen von depens.exe für %1").arg(K_Parameter->ZielverzeichnisHohlen()+"\\"+K_Dateiname);
		return false;
	}
	//Dann schauen wir mal in der Datei nach
	QFile DateiMitDenAbhaengigkeiten(K_Parameter->ZielverzeichnisHohlen()+"\\"+K_Dateiname+".braucht");
	if(!DateiMitDenAbhaengigkeiten.open(QIODevice::ReadOnly))
	{
		K_Fehlermeldung=trUtf8("Fehler beim lesen der Abhängigkeiten aus %1").arg(K_Parameter->ZielverzeichnisHohlen()+"\\"+K_Dateiname+".braucht");
		return false;
	}
	QTextStream Dateiinhalt(&DateiMitDenAbhaengigkeiten);
	QStringList Zeile;
	QString Eintrag;
#ifndef QT_NO_DEBUG
		QString Debugtext=K_Parameter->ZielverzeichnisHohlen()+"\\"+K_Dateiname+" braucht: ";
#endif	
	while(!Dateiinhalt.atEnd())
	{
		Zeile=Dateiinhalt.readLine().split(',');
		//Steht in der 2. Spalte was mit QT???
		Eintrag=Zeile.at(1);
		Eintrag.remove("\"");
		if(Eintrag.startsWith("QT"))
		{
			//Leider steht auch die Datei selbst in der Liste. K_Dateiname hat als 1. Zeichen das Pfadtrennzeichen!! 
			if(!Eintrag.contains(K_Dateiname,Qt::CaseInsensitive))
			{
#ifndef QT_NO_DEBUG
				Debugtext.append(Eintrag+" ");
#endif
				if(K_QtKomponenteErmitteln(Eintrag))
					K_BenoetigeQtKomponenten.append(K_QtKomponente);
				else
				{
					K_Fehlermeldung=trUtf8("Unbekante Abhängigkeit entdeckt.");
					DateiMitDenAbhaengigkeiten.close();
					return false;
				}
			}
				
		}
	}
#ifndef QT_NO_DEBUG
	qDebug()<<Debugtext;
#endif
	DateiMitDenAbhaengigkeiten.close();
	return true;
}
void QFrankQt4MergemoduleManifestBearbeiten::K_ProzessFertig(int rueckgabe)
{
	exit(rueckgabe);
}

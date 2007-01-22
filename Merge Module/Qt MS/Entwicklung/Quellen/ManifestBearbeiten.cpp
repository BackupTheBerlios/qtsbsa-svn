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

#include <QtXml>

QFrankQt4MergemoduleManifestBearbeiten::QFrankQt4MergemoduleManifestBearbeiten(const QFrankQt4MergemoduleParameter* parameter,QObject* eltern)
										:QFrankQt4MergemoduleBasisThread(parameter,eltern)
{
}
void QFrankQt4MergemoduleManifestBearbeiten::run()
{
	QString Dateiname=K_Parameter->QtBibliothekenHohlen().at(K_Dateinummer);
	Dateiname=K_Parameter->ZielverzeichnisHohlen()+Dateiname.right(Dateiname.length()-Dateiname.lastIndexOf("\\"))+".manifest";
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
	QDomElement Identifikation=Manifest.createElement("assemblyIdentity");
	Identifikation.setAttribute("type","win32");
	Identifikation.setAttribute("name",K_Parameter->EntwicklerHohlen()+".Qt."+K_QtKomponente);
	Identifikation.setAttribute("processorArchitecture",K_Parameter->CPUTypeHohlen());
	Identifikation.setAttribute("version",K_Parameter->QtVersionHohlen());
	Identifikation.setAttribute("publicKeyToken",K_Parameter->publicKeyTokenHohlen());
	Rootelement.insertBefore(Identifikation,Rootelement.firstChild());

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
	else
		return false;
	return true;
}

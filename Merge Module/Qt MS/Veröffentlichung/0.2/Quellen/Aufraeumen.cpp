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

#include "Aufraeumen.h"
#include "Parameter.h"

using namespace QFrank;
QtSBSAAufraeumen::QtSBSAAufraeumen(const QtSBSAParameter* parameter,QObject *eltern)
					  :QObject(eltern)
{
	K_Parameter=parameter;
}
bool QtSBSAAufraeumen::putzen()
{
	/*
		Am Schluss sollen nur die die Mergemodule übrig bleiben.
		Und im Debugfall gar nix gelöscht werden.
	*/
#ifndef QT_NO_DEBUG
	qDebug("%s putzen: Es wird nix aufgeräumt, da im Debugmodus.",this->metaObject()->className());
#endif
	QDir Zielverzeichnis(K_Parameter->ZielverzeichnisHohlen());
	QStringList Verzeichnissse=Zielverzeichnis.entryList(QDir::AllDirs|QDir::NoDotAndDotDot);
	//Unterverzeichnisse löschen
	Q_FOREACH(QString Eintrag, Verzeichnissse)
	{
		if(!K_VerzeichnisLoeschen(Eintrag))
		{
			return false;
			break;
		}
	}
	/*
		Hier den Rest aufräumen.
		nur .wxs und .msm dürfen bleiben.
	*/
	QSet<QString> bleibt= QSet<QString>::fromList(Zielverzeichnis.entryList(QStringList()<<"*.wxs"<<"*.msm",QDir::Files|QDir::NoDotAndDotDot));
	QSet<QString> alles= QSet<QString>::fromList(Zielverzeichnis.entryList(QDir::Files|QDir::NoDotAndDotDot));
	alles.subtract(bleibt);
	Q_FOREACH(QString Datei,alles)
	{
		if(!K_DateiLoeschen(K_Parameter->ZielverzeichnisHohlen()+"/"+Datei))
		{
			break;
			return false;
		}
	}
	return true;
}
bool QtSBSAAufraeumen::K_DateiLoeschen(const QString &datei)
{
#ifdef QT_NO_DEBUG
	QDir Datei;
	if(Datei.remove(datei))
		return true;
	else
		K_Fehlermeldung=trUtf8("Die Datei %1 konnte nicht gelöscht werden.").arg(datei);
	return false;
#else
	qDebug()<<datei;
	return true;
#endif
}
bool QtSBSAAufraeumen::K_VerzeichnisLoeschen(const QString &verzeichnis)
{
	QDir Dateien(K_Parameter->ZielverzeichnisHohlen()+"/"+verzeichnis);
	QStringList Dateiliste=Dateien.entryList(QDir::NoDotAndDotDot|QDir::Files);
	Q_FOREACH(QString Datei,Dateiliste)
	{
		K_DateiLoeschen(Dateien.path()+"/"+Datei);
	}
#ifdef QT_NO_DEBUG
	if(Dateien.rmdir(Dateien.path()))
		return true;
	else
		K_Fehlermeldung=trUtf8("Das Verzeichnis %1 konnte nicht gelöscht werden.").arg(Dateien.path());
	return false;
#else
	qDebug()<<Dateien.path();
	return true;
#endif
}

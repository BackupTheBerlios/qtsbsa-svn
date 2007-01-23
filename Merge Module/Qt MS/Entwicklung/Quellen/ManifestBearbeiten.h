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

#ifndef QFRANKQT4MERGEMODULEMANIFESTBEARBEITEN
#define QFRANKQT4MERGEMODULEMANIFESTBEARBEITEN

#include <QtCore>
#include <QtXml>
#include "BasisThread.h"

class QFrankQt4MergemoduleParameter;
class QFrankQt4MergemoduleManifestBearbeiten: public QFrankQt4MergemoduleBasisThread
{
	Q_OBJECT
	public:
				QFrankQt4MergemoduleManifestBearbeiten(const QFrankQt4MergemoduleParameter* parameter,QObject* eltern=0);
	private slots:
				void				K_ProzessFertig(int rueckgabe);
	private:
				QString				K_Dateiname;
				QString				K_QtKomponente;
				QStringList			K_BenoetigeQtKomponenten;
				bool				K_QtKomponenteErmitteln(const QString &datei);
				bool				K_AbhaengigkeitenErmitteln();
				const QDomElement	K_AssemblyIdentifikationEinfuegen(QDomDocument &manifest);
				bool				K_AbhaengigkeitEinfuegen(QDomDocument &manifest,const QString &welche);
	protected:
				void				run();
};
#endif
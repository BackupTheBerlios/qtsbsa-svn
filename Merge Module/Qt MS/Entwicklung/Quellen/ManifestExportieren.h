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

#ifndef QFRANKQT4MERGEMODULEMANIFESTEXPORTIEREN
#define QFRANKQT4MERGEMODULEMANIFESTEXPORTIEREN

#include <QtCore>

class QFrankQt4MergemoduleParameter;
class QFrankQt4MergemoduleManifestExportieren:public QThread
{
	Q_OBJECT
	public:
				QFrankQt4MergemoduleManifestExportieren(const QFrankQt4MergemoduleParameter* parameter,QObject *eltern=0);	
				~QFrankQt4MergemoduleManifestExportieren();
				void									DateinummerFestlegen(const int &nummer){K_Dateinummer=nummer;}
	public slots:
				const int								FehlercodeHohlen(){return K_Fehlercode;}
#ifndef QT_NO_DEBUG
				const int								Threadnummer(){return K_Dateinummer;}
#endif
	signals:
				void									fertig( QThread * welcher);
	private slots:
				void									K_mtFertig(int statusCode);
	private:
				const QFrankQt4MergemoduleParameter*	K_Parameter;				
				int										K_Dateinummer;
				int										K_Fehlercode;
				QProcess*								K_mtProzess;
	protected:
				void									run();
};
#endif

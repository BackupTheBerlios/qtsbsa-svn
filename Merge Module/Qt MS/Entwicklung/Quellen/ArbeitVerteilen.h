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

#ifndef QFRANKQT4MERGEMODULEARBEITVERTEILEN
#define QFRANKQT4MERGEMODULEARBEITVERTEILEN

#include <QtCore>

class QFrankQt4MergemoduleParameter;
class QFrankQt4MergemoduleArbeitVerteilen:public QObject
{
	Q_OBJECT
	public:
				QFrankQt4MergemoduleArbeitVerteilen(QObject *eltern,const QFrankQt4MergemoduleParameter* parameter);				
	public slots:
				void									Loslegen();
	signals:
				void									fertig();
				void									Meldung(const QString &meldung);
	private:
				const QFrankQt4MergemoduleParameter*	K_Parameter;
				bool									K_WindowsSDKPruefen();
				void									K_ErstellenGescheitert();
				void									K_ErstellenErfolgreich();
				void									K_SchrittFertig();
				void									K_SchrittFehlgeschlagen();

};
#endif

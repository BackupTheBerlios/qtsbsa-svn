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

#ifndef QFRANKQT4MERGEMODULEEREIGNISFILTER
#define QFRANKQT4MERGEMODULEEREIGNISFILTER

#include <QtCore>

class QFrankQt4MergemoduleEreignisfilter : public QObject
{
	Q_OBJECT
	public:
				QFrankQt4MergemoduleEreignisfilter(QObject* eltern=0);
	protected:
				bool eventFilter(QObject *objekt, QEvent *ereignis);
};
#endif
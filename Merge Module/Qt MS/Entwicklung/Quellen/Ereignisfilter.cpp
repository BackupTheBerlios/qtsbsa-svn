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

#include "Ereignisfilter.h"

QFrankQt4MergemoduleEreignisfilter::QFrankQt4MergemoduleEreignisfilter(QObject* eltern):QObject(eltern)
{
}
bool QFrankQt4MergemoduleEreignisfilter::eventFilter(QObject *objekt, QEvent *ereignis)
{
	/*
		Der Fortschrittsdialog darf nur geschlossen werden, wenn er fertig ist.
	*/
	qDebug()<<objekt->metaObject()->className();
	qDebug()<<ereignis;
	if(objekt->metaObject()->className()=="QFrankQt4MergemoduleDlgFortschritt")
	{
		qDebug()<<"teste";
		if(ereignis->type()==QEvent::Close)
		{
			
			//darf es geschlossen werden??
			bool schliessen=false;
			QMetaObject::invokeMethod(objekt,"darfGeschlossenWerden",Qt::DirectConnection,Q_RETURN_ARG(bool,schliessen));
			if(!schliessen)
				return true;
			else
				return false;
		}
	}
	//nix für uns
	return QObject::eventFilter(objekt, ereignis);
}

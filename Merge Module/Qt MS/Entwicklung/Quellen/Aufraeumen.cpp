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

void QFrankQtSBSAAufraeumen::putzen()
{
	/*
		Am Schluss sollen nur die die Mergemodule übrig bleiben.
		Und im Debugfall gar nix gelöscht werden.
	*/
#ifndef QT_NO_DEBUG
	qDebug("Es wird nix aufgeräumt, da im Debugmodus.");
	return;
#endif
}
bool QFrankQtSBSAAufraeumen::K_Dateiloeschen(const QString &datei)
{
	return true;
}

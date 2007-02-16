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

#include "QtModul.h"

QFrankQtSBSAQtModul::QFrankQtSBSAQtModul(const QString &dateiname,const bool &istPlugIn)
{
	K_Dateiname=dateiname;
	K_istPlugIn=istPlugIn;
}
QFrankQtSBSAQtModul::QFrankQtSBSAQtModul()
{
	K_Dateiname="";
	K_istPlugIn=false;
	K_PlugInType="";
}
#ifndef QT_NO_DEBUG
QDebug  operator<<(QDebug debug, const QFrankQtSBSAQtModul &modul)
{
	QString Plugin;
	if(modul.istPlugIn())
	{
		Plugin="Ja Type: "+modul.PlugInTypeHohlen();
	}
	else
		Plugin="Nein";
	return debug<<QString("Dateiname: %1 PlugIn: %2").arg(modul.DateinameHohlen()).arg(Plugin);
}
#endif

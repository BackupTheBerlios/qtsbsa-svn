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

#include "ArbeitVerteilen.h"
#include "Parameter.h"

QFrankQt4MergemoduleArbeitVerteilen::QFrankQt4MergemoduleArbeitVerteilen(QObject *eltern,const QFrankQt4MergemoduleParameter* parameter):QObject(eltern)
{
	K_Parameter=parameter;
}
void QFrankQt4MergemoduleArbeitVerteilen::Loslegen()
{
	if(!K_WindowsSDKPruefen())
		return;
	K_ErstellenErfolgreich();
	emit fertig();
}
bool QFrankQt4MergemoduleArbeitVerteilen::K_WindowsSDKPruefen()
{
	emit Meldung(trUtf8("Prüfe Windows SDK"));
	//Haben wir alle SDK Werkzeuge??
	QStringList Werkzeugliste;
	Werkzeugliste<<"mt.exe"<<"signtool.exe";
	QString Werkzeug;
	Q_FOREACH(Werkzeug,Werkzeugliste)
	{
		if(!QFile::exists(K_Parameter->WindowsSDKPfadHohlen()+"\\"+Werkzeug))
		{
			emit Meldung(tr("Die Datei %1 wurde nicht gefunden.").arg(Werkzeug));
			K_SchrittFehlgeschlagen();
			K_ErstellenGescheitert();
			emit fertig();
			return false;
			break;
		}
	}
	K_SchrittFertig();
	return true;
}
void QFrankQt4MergemoduleArbeitVerteilen::K_ErstellenGescheitert()
{
	emit Meldung(tr("<html><b><span style=\"color:#ff0000;\">Erstellung der Mergemodule gescheitert.</span></b></html>")); //Rot und Fett
}
void QFrankQt4MergemoduleArbeitVerteilen::K_ErstellenErfolgreich()
{
	emit Meldung(tr("<html><b><span style=\"color:#008000;\">Erstellung der Mergemodule erfolgreich.</span></b></html>")); //Grün und Fett
}
void QFrankQt4MergemoduleArbeitVerteilen::K_SchrittFertig()
{
	emit Meldung(tr("<html><span style=\"color:#008000;\">fertig</span></html>"));
}
void QFrankQt4MergemoduleArbeitVerteilen::K_SchrittFehlgeschlagen()
{
	emit Meldung(tr("<html><span style=\"color:#ff0000;\">fehlgeschlagen</span></html>"));
}

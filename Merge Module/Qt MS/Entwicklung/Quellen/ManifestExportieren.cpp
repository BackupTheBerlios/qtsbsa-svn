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

#include "ManifestExportieren.h"
#include "Parameter.h"

/*
	Manifest exportieren: mt.exe -inputresource:dll_with_manifest.dll;#2 -out:extracted.manifest (Die Manifeste von Qt stehen in #2 statt #1
	Manifest importieren: mt.exe -manifest XXX.manifest -outputresource:XXXX.dll;#1	
*/

QFrankQt4MergemoduleManifestExportieren::QFrankQt4MergemoduleManifestExportieren(const QFrankQt4MergemoduleParameter* parameter,QObject *eltern):QThread(eltern)
{
	K_Parameter=parameter;
	K_mtProzess=NULL;
}
QFrankQt4MergemoduleManifestExportieren::~QFrankQt4MergemoduleManifestExportieren()
{
	if(K_mtProzess!=NULL)
		delete K_mtProzess;
}
void QFrankQt4MergemoduleManifestExportieren::run()
{
	K_mtProzess=new QProcess();
	connect(K_mtProzess,SIGNAL(finished(int)),this,SLOT(K_mtFertig(int)));
	K_mtProzess->start("\""+K_Parameter->WindowsSDKPfadHohlen()+"\\mt.exe\"");
	if(!K_mtProzess->waitForStarted(5000))
	{
		return;
	}
	K_Fehlercode=exec();
	emit fertig(this);
}
void QFrankQt4MergemoduleManifestExportieren::K_mtFertig(int statusCode)
{	
	/*
		0 alles ok
		1 Fehler
	*/

	//nur zum Testen!!!!
	if(K_Dateinummer==3)
		exit(1);
	else
		exit(0);
	//exit(0);
}

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

#ifndef QFRANKQTSBSAARBEITVERTEILEN
#define QFRANKQTSBSAARBEITVERTEILEN

#include <QtCore>
#include "QtModul.h"

class QFrankQtSBSAParameter;
class QFrankQtSBSABasisThread;
class QFrankQtSBSAArbeitVerteilen:public QObject
{
	Q_OBJECT
	public:
				QFrankQtSBSAArbeitVerteilen(QObject *eltern,QFrankQtSBSAParameter* parameter);				
	public slots:
				void							Loslegen();
	signals:
				void							fertig();
				void							Meldung(const QString &meldung);
				void							FortschrittsanzeigeSchritt();
				void							FortschrittsanzeigeMaximum(const int schritte);
	private slots:
				void							K_ThreadFertig( QFrankQtSBSABasisThread *welcher);
	private:
				enum							K_Arbeitsschritte{ManifestExportieren=0x0,ManifestBearbeiten=0x1,KatalogErstellen=0x2,KatalogSignieren=0x3,
																  WixDateienErstellen=0x4,WixDateienUebersetzen=0x5,Aufraeumen=0x6};
				enum							K_WieAufraeumen{Normal=0x0,NachFehler=0x1};
				QFrankQtSBSAParameter*	K_Parameter;
				int								K_AnzahlDerProzesse;	
				bool							K_WindowsSDKPruefen();
				bool							K_QtPruefen();
				bool							K_ZielverzeichnisPruefen();
				bool							K_DateienVorhanden(const QList<QFrankQtSBSAQtModul> &liste,bool qtdateien=false);
				bool							K_DateienKopieren();
				void							K_ErstellenGescheitert();
				void							K_ErstellenErfolgreich();
				void							K_SchrittFertig();
				void							K_SchrittFehlgeschlagen();
				void							K_ManifesteExportieren();
				void							K_ManifesteBearbeiten();
				void							K_KatalogeErstellen();
				void							K_KatalogeSignieren();
				void							K_WixDateienErstellen();
				void							K_WixDateienUebersetzen();
				void							K_Aufraeumen(const uchar &wie);
				void							K_NaechsterArbeitsschritt();
				const QString					K_Dateiversion(const QString &datei);
				uchar							K_Arbeitsschritt;

};
#endif

/* Copyright (C) 2007 Frank B�ttner frank-buettner@gmx.net

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

#ifndef QFRANKQTSBSAPARAMETER
#define QFRANKQTSBSAPARAMETER

#include <QtCore>
#include "QtModul.h"

namespace QFrank
{
	class QtSBSAParameter: public QObject
	{
		Q_OBJECT
		public:
					QtSBSAParameter(QObject* eltern=0);
					void								WindowsSDKPfadSetzen(const QString &pfad){K_WindowsSDKPfad=pfad;}
					void								QtPfadSetzen(const QString &pfad){K_QtPfad=pfad;}
					void								WixPfadSetzen(const QString &pfad){K_WixPfad=pfad;}
					void								QtVersionSetzen(const QString &version){K_QtVersion=version;}
					void								QtBibliothekenSetzen(const QList<QtSBSAQtModul> &bibliotheken){K_QtBibliotheken=bibliotheken;}
					void								ZertSHA1Setzen(const QString &sha1){K_ZertSHA1=sha1;}
					void								publicKeyTokenSetzen(const QString &token){K_publicKeyToken=token;}
					void								CPUTypeSetzen(const QString &cpu){K_CPU=cpu;}
					void								ZielverzeichnisSetzen(const QString &verzeichnis){K_Zielverzeichnis=verzeichnis;}
					void								ManifestentwicklerSetzen(const QString &entwickler){K_Entwickler=entwickler;}
					void								MergemodulentwicklerSetzen(const QString &entwickler){K_Mergemodulentwickler=entwickler;}
					const QString&						WindowsSDKPfadHohlen()const{return K_WindowsSDKPfad;}
					const QString&						QtPfadHohlen()const{return K_QtPfad;}
					const QString&						WixPfadHohlen()const{return K_WixPfad;}
					const QString&						QtVersionHohlen()const{return K_QtVersion;}
					QList<QtSBSAQtModul>&				QtBibliothekenHohlen(){return K_QtBibliotheken;}
					const QString&						ZertSHA1Hohlen()const{return K_ZertSHA1;}
					const QString&						publicKeyTokenHohlen()const{return K_publicKeyToken;}
					const QString&						CPUTypeHohlen()const{return K_CPU;}
					const QString&						ZielverzeichnisHohlen()const{return K_Zielverzeichnis;}
					const QString&						ManifestentwicklerHohlen()const{return K_Entwickler;}
					const QString&						MergemodulentwicklerHohlen()const{return K_Mergemodulentwickler;}
					bool								AllesVorliegend();
		private:
					QString								K_WindowsSDKPfad;
					QString								K_QtPfad;
					QString								K_WixPfad;
					QString								K_QtVersion;
					QList<QtSBSAQtModul>				K_QtBibliotheken;
					QString								K_ZertSHA1;
					QString								K_publicKeyToken;
					QString								K_CPU;
					QString								K_Zielverzeichnis;
					QString								K_Entwickler;
					QString								K_Mergemodulentwickler;
	};
}
#endif

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

#ifndef QFRANKQTSBSAKATALOGERSTELLEN
#define QFRANKQTSBSAKATALOGERSTELLEN

#include <QtCore>
#include "BasisThread.h"

class QFrankQtSBSAKatalogErstellen: public QFrankQtSBSABasisThread
{
	Q_OBJECT
	public:
				QFrankQtSBSAKatalogErstellen(QFrankQtSBSAParameter* parameter,QObject* eltern=0);
				~QFrankQtSBSAKatalogErstellen();
	private:
				enum		K_Schritt{KatalogvorlageErstellen=0x0,KatalogErstellen=0x01};
				uchar		K_AktuellerSchritt;
				QProcess	*K_Prozess;
				QString		K_Dateipfad;
	private Q_SLOTS:
				void		K_ProzessIstFertig(int rueckgabe);
	protected:
				void		run();
};
#endif

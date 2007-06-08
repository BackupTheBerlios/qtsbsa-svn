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

#ifndef QFRANKQTSBSAWIXDATEIUEBERSETZEN
#define QFRANKQTSBSAWIXDATEIUEBERSETZEN

#include <QtCore>
#include "BasisThread.h"

namespace QFrank
{
	class QtSBSAWixDateiUebersetzen:public QtSBSABasisThread
	{
		Q_OBJECT
		public:
					QtSBSAWixDateiUebersetzen(QtSBSAParameter* parameter,QObject* eltern=0);
					~QtSBSAWixDateiUebersetzen();
		private:
				enum		Arbeitsschritte{Kerze=0x0,Licht=0x1};
				QProcess*	K_Prozess;
				bool		K_Loslegen();
				uchar		K_Arbeitsschritt;
		private Q_SLOTS:
				void		K_ProzessFertig(const int &statuscode);
		protected:
				void		run();
	};
}
#endif

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

#ifndef QFRANKQTSBSADLGFORTSCHRITT
#define QFRANKQTSBSADLGFORTSCHRITT

#include "ui_FortschrittBasis.h"

namespace QFrank
{
	class QtSBSAParameter;
	class QtSBSADlgFortschritt : public QDialog, private Ui::dlgFortschrittBasis
	{
		Q_OBJECT
		public:
					QtSBSADlgFortschritt(QWidget *eltern,QtSBSAParameter* parameter);
					virtual bool					event(QEvent *ereignis);					
		private:
					bool							K_darfGeschlossenWerden;				
		private Q_SLOTS:
					void							on_sfSchliessen_clicked();
					void							K_NeueMeldung(const QString &meldung);
					void							K_ErstellungBeendet();
					void							K_FortschrittsanzeigeMaximum(int endpunkt);
					void							K_FortschrittsanzeigeSchritt();
	};
}
#endif

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

#ifndef QFRANKQTSBSADLGEINSTELLUNGEN
#define QFRANKQTSBSADLGEINSTELLUNGEN

#include <QtGui>
#include "ui_EinstellungenBasis.h"

class QFrankQtSBSADlgEinstellungen:public QDialog, private Ui::dlgEinstellungenBasis
{
	Q_OBJECT
	public:
				QFrankQtSBSADlgEinstellungen(QWidget *eltern=0);
	private:
				QFileDialog*	K_Verzeichnisauswahl;
				bool			AlleAngabenVollstaendig();
				QString			K_FeldNachHex(const QByteArray &feld); 
	private slots:
				void			on_sfBox_clicked(QAbstractButton *schaltflaeche);
				void			on_sfQtPfadSuchen_clicked();
				void			on_sfWixPfadSuchen_clicked();
				void			on_sfWindowsSDKPfadSuchen_clicked();
				void			on_sfZielpfadSuchen_clicked();
				void			on_sfZertifikatSuchen_clicked();
};
#endif

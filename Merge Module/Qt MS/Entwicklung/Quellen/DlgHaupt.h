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

#ifndef QFRANKQTSBSADLGHAUPT
#define QFRANKQTSBSADLGHAUPT

#include "ui_HauptBasis.h"

class QFileDialog;
class QFrankQtSBSAParameter;
class QFrankQtSBSADlgHaupt : public QMainWindow, private Ui::dlgHauptBasis
{
	Q_OBJECT
	public:
				QFrankQtSBSADlgHaupt(QWidget *eltern = 0);
	private:
				QFileDialog*			K_Verzeichnisauswahl;
				QFrankQtSBSAParameter*	K_Parameter;
				QString					K_FeldNachHex(const QByteArray &feld); 
	private slots:
				void					on_sfQtPfadSuchen_clicked();
				void					on_sfWixPfadSuchen_clicked();
				void					on_sfZielpfadSuchen_clicked();
				void					on_sfZertifikatSuchen_clicked();
				void					on_sfWindowsSDKPfadSuchen_clicked();
				void					on_sfBox_accepted();
				void					on_Menue_UeberQt_triggered();
				void					on_Menue_UeberDasProgramm_triggered();
};
#endif
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

#ifndef QFRANKQTSBSAQTMODUL
#define QFRANKQTSBSAQTMODUL

#include <QtCore>

class QFrankQtSBSA
{
	public:
			const QString&	DateinameHohlen()const{return K_Dateiname;}
			const bool&		IstPlugInHohlen()const{return K_istPlugIn;}
			void			DateinameSetzen(const QString &dateiname){K_Dateiname=dateiname;}
			void			IstPlugInSetzen(const bool& istPlugIn){K_istPlugIn=istPlugIn;}
	private:
			QString			K_Dateiname;
			bool			K_istPlugIn;
};
#endif
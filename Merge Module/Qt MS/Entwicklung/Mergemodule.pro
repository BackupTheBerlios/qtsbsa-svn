# Copyright (C) 2007 Frank Büttner frank-buettner@gmx.net
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version
# 2 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

TEMPLATE					= app
TARGET						= Qt4MergemoduleErzeugen
DESTDIR						= bin
QT							+= xml
debug {
	CONFIG					+= console
}
MOC_DIR						= tmp/moc
OBJECTS_DIR					= tmp/obj
UI_HEADERS_DIR				= tmp/ui_headers
RCC_DIR						= tmp/resourcen
# geht nicht wegen Bug
#RC_FILE						= $$RCC_DIR/Windows.rc
RC_FILE						= Windows.rc
RESOURCES					= Resourcen.qrc
LIBS						+= -lCrypt32 -lCryptui -lversion
TRANSLATIONS				= Uebersetzungen/Uebersetzung_en.ts\
							  Uebersetzungen/Uebersetzung_XX.ts
FORMS						= Dialoge/HauptBasis.ui\
							  Dialoge/FortschrittBasis.ui
HEADERS						= Quellen/Parameter.h\
							  Quellen/BasisThread.h\
							  Quellen/ManifestExportieren.h\
							  Quellen/ManifestBearbeiten.h\
							  Quellen/KatalogErstellen.h\
							  Quellen/ArbeitVerteilen.h\
							  Quellen/DlgFortschritt.h\
							  Quellen/DlgHaupt.h
SOURCES						= Quellen/Parameter.cpp\
							  Quellen/BasisThread.cpp\
							  Quellen/ManifestExportieren.cpp\
							  Quellen/ManifestBearbeiten.cpp\
							  Quellen/KatalogErstellen.cpp\
							  Quellen/ArbeitVerteilen.cpp\
							  Quellen/DlgFortschritt.cpp\
							  Quellen/DlgHaupt.cpp\
							  Quellen/Haupt.cpp
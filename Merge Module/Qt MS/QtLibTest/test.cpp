#include <QtCore>
int main(int argc, char **argv)
{
	QCoreApplication Programm(argc,argv);	
#ifdef QT_SIDE_BY_SIDE_ASSEMBLY
	/*
		Alle Side-By-Side PlugIns müssen den Schema Hersteller.Qt.PlugIn-XXXX entsprechen!!!
		Wobei XXXX einem Plugin-Namen entspricht. z.B. HansMutermann.Qt.PlugIn-mysql
		All side-by-side plugIns must fit to the pattern vendor.Qt.PlugIn-XXXX!!
		XXXX is the name of the PlugIN. Example: HansMustermann.Qt.PlugIn-mysql
	*/
	QString Manifestname=QCoreApplication::instance()->arguments().at(0)+".manifest";
	QFile Manifest(Manifestname);
	if(Manifest.open(QIODevice::ReadOnly))
	{
		QTextStream Inhalt(&Manifest);
		QString Hersteller;
		QString Plattform;
		int PlattformStart;
		int PlattformEnde;
		int HerstellerStart;
		int HerstellerEnde;
		QString Zeile;
		while(!Inhalt.atEnd())
		{
			//Wie ist der Platfom/Hersteller meiner Qt Lib??
			Zeile=Inhalt.readLine();
			//Jedes Qt Programm benötigt QtCore. Darauf kann man sicher testen.
			if(Zeile.contains("QtCore"))
			{
				PlattformStart=Zeile.indexOf("processorArchitecture");				
				PlattformStart=Zeile.indexOf(QRegExp("[x,X,i]"),PlattformStart+21);//wo fängt der Platformtext an 21=Länge processorArchitecture
				PlattformEnde=Zeile.indexOf(QRegExp("[\",\']"),PlattformStart+1);
				Plattform=Zeile.mid(PlattformStart,PlattformEnde-PlattformStart);
				HerstellerStart=Zeile.indexOf("name");
				HerstellerStart=Zeile.indexOf(QRegExp("[\",\']"),HerstellerStart+4)+1;
				HerstellerEnde=Zeile.indexOf(QRegExp("[\",\']"),HerstellerStart+1);
				Hersteller=Zeile.mid(HerstellerStart,HerstellerEnde-HerstellerStart).split('.').first();				
				break;
			}
		}
		Manifest.close();
		//Welche Verzeichnise haben wir??
		QDir WindowsSXS(QProcess::systemEnvironment().filter("SystemRoot").first().split('=').last()+"\\winsxs");
		QStringList VerzeichnisseMitPlugINs=WindowsSXS.entryList(QStringList(Plattform+"_"+Hersteller+".Qt.PlugIn-*"),QDir::Dirs|QDir::CaseSensitive);
		for(int Verzeichnis=0;Verzeichnis<VerzeichnisseMitPlugINs.size();Verzeichnis++)
		{
			QCoreApplication::addLibraryPath(WindowsSXS.path().replace('/','\\')+"\\"+VerzeichnisseMitPlugINs.at(Verzeichnis));
		}
	}	
#endif
	qDebug()<<"Plugins:"<<QLibraryInfo::location(QLibraryInfo::PluginsPath)
			<<"Uebersetzungen:"<<QLibraryInfo::location(QLibraryInfo::TranslationsPath);
	qDebug()<<QCoreApplication::libraryPaths();  
	return 0;
}

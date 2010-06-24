#include <QFile>
#include <QInputDialog>
#include <QMessageBox>
#include <QRegExp>
#include <QStringList>
#include <fstream>

bool loadSubject(QString &enhance, QString &task, QString path)
{
	QString output, input, prior;
	QFile qfile;
	QMessageBox::StandardButton reply;
	int out;
	bool ok=false;
	
	while(!ok)
	{
		out=QInputDialog::getInt(0, "Subject # Number Selection","Subject number from consent form:",1,1,48,1, &ok);
		if((!ok)||(out<0)) return false;
		output.setNum(out);
		if(out!=1)
		{
			//Check that prior subject is used.
			prior.setNum(out-1);
			qfile.setFileName(QString(path+prior+"Cont.dat"));
			if(!qfile.exists())
			{
				reply = QMessageBox::question(0, "Danger Will Robinson! Danger!", "Prior subject number unused!?!? Procede anyways?", QMessageBox::Yes | QMessageBox::No);
				if (reply==QMessageBox::No) {ok=false; continue;}
			}
		}
		//Check that present value is unused.
		qfile.setFileName(path+output+"Cont.dat");
		if(qfile.exists())
		{
			
			reply = QMessageBox::question(0, "Danger Will Robinson! Danger!", "This subject number is already used. Overwrite the old data?", QMessageBox::Yes | QMessageBox::No);
			if (reply==QMessageBox::No) {ok=false; continue;}
		}
		output=path+output;
		std::ifstream ifs("/research/EAvs2xFF/subs.dat");
		int count=0;
		std::string line;
		while(count<out)
		{
			std::getline(ifs,line);
			count++;
		}
		QRegExp rx("(\\d{1,2}) (\\d{3}) (\\d{3})");
		ifs.close();
		rx.indexIn(line.c_str());
		QStringList qsl=rx.capturedTexts();
		QStringList::const_iterator  base=qsl.begin();
		++base;
		++base;
		enhance=(*base).toInt();
		++base;
		task=(*base).toInt();
		std::cout << enhance << " " << task << std::endl;
	}
	return true;
}


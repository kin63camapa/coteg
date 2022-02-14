/*
Copyright (C) 2022 Kin63camapa

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QCoreApplication>
#include <QThread>
#include <QDir>
#include <QDebug>
#include "mew.h"
#include "settings.h"

int main(int argc, char *argv[]) {

    QCoreApplication *app = new QCoreApplication(argc, argv);
#ifdef _WIN32
    AppSettings = new QSettings((QCoreApplication::applicationDirPath()+QDir::separator()+"config.ini"),QSettings::IniFormat);
#else
    AppSettings = new QSettings((QDir::homePath()+QDir::separator()+".config"+QDir::separator()+QFileInfo(QCoreApplication::applicationFilePath()).fileName()+QDir::separator()+"config.ini"),QSettings::IniFormat);
#endif


    if (!AppSettings->contains("BotToken"))
    {
        if (!AppSettings->isWritable())
        {
            qDebug() << QString::fromUtf8("Не найден работоспособный конфиг, и нет возможности его создать! \n");
        }
    }
    setDefaultConfig(false);
    AppSettings->sync();


    QThread* thread = new QThread;
    Mew* mew = new Mew(app);
    mew->moveToThread(thread);
    //connect(mew, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    //QObject::connect(thread, SIGNAL(&QThread::started), mew, SLOT(&TG::process));
    //QObject::connect(mew, SIGNAL(&QThread::finished), thread, SLOT(&QThread::quit));
    //QObject::connect(mew, SIGNAL(&QThread::finished), mew, SLOT(&TG::deleteLater));
    //QObject::connect(thread, SIGNAL(&QThread::finished), thread, SLOT(&QThread::deleteLater));
    //QObject::connect(thread, SIGNAL(&QThread::finished), &app, SLOT(&QCoreApplication::quit));
    thread->start();
    return app->exec();
}


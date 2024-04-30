// Copyright 2018 Florian Muecke. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE.txt file.

#include "MainWindow.h"
#include "MainWindowTeam.h"
//#include "../core/TournamentMode.h"
#include "SplashScreen.h"
#include "versioninfo.h"
#include "UpdateChecker.h"
#include "../util/path_helpers.h"

#include <QApplication>
#include <QTranslator>
#include <QMessageBox>
#include <QSettings>
#include <QFile>
#include <QLocale>
//#include <QtextCodec>
#if QT_VERSION >= 0x050000
#include <QCommandLineParser>
#include <QDebug>
#include <QTextStream>
#endif

#ifndef _WIN32
#ifndef NO_ERROR
#define NO_ERROR 0
#endif
#ifndef ERROR_INVALID_PARAMETER
#define ERROR_INVALID_PARAMETER 1
#endif
#endif

void LangNotFound(const QString& fileName)
{
	QMessageBox::critical(nullptr,
						  QCoreApplication::applicationName(),
						  "Unable to read language file: " + fileName +
						  "\nThe default language is being used.");
}

void SetTranslation(QApplication& app, QTranslator& translator, QString const& langStr)
{
    Q_UNUSED(app);

	if (langStr == QString("en"))
	{
		return; // default
	}

	if (langStr == QString("de") || langStr == QString("nl"))
	{
		const QString& langPath = QCoreApplication::applicationDirPath() + QString("/lang");

		if (translator.load(langStr, langPath))
		{
			app.installTranslator(&translator);
		}
		else
		{
			LangNotFound(langStr);
		}
	}
}

int ShowSplashScreen()
{
	auto t1 = QCoreApplication::tr("the score board for judoka by judoka");
	auto t2 = QCoreApplication::tr("Judo is part of our lives.");
	auto t3 = QCoreApplication::tr("Therefore, Ipponboard is not just a simple Judo display program, but developed with passion by judoka for judoka. It was intended to not just support the people at the timekeeper table, but also for the trainers, the audience and the fighters themselves.");
	auto t4 = QCoreApplication::tr("Ipponboard is not only clearly readable but also revolutionary easy and intuitive to use. Therefore, it is appreciated by clubs and organizations around the world and has been used for many years at major championships.");
	//auto t5 = QCoreApplication::tr("This version can be used without restriction. Copying in unchanged form is permitted.");
	auto t5 = QCoreApplication::tr("If you like Ipponboard, please support its development by:");
	auto t6 = QCoreApplication::tr("feedback");
	auto t7 = QCoreApplication::tr("wishes and suggestions");
	auto t8 = QCoreApplication::tr("or by giving appreciative donations");

	auto text = QString("<html><body><p><big><span style=\"color:#336699;font-weight:bold\">Ipponboard</span> - %1</big></p>"
						"<blockquote><p><em><b>%2</b> %3 %4</em></p></blockquote>"
						"<p>%5<ul>"
						"<li>%6</li>"
						"<li>%7</li>"
						"<li>%8</li></ul></p>").arg(t1, t2, t3, t4, t5, t6, t7, t8);
	
	auto t10 = QCoreApplication::tr("For more see the");
	auto t11 = QCoreApplication::tr("Ipponboard project on GitHub");
	auto t12 = QCoreApplication::tr("Thank you very much!");

	text += QString("%10 <a href=\"https://github.com/fmuecke/Ipponboard\">%11</a>."
						"<p><em>%12</em></p></body></html>").arg(t10, t11, t12);

	SplashScreen::Data splashData;
	splashData.text = text;
	splashData.info = QCoreApplication::applicationName()
					  + " v" + QCoreApplication::applicationVersion()
					  + "\n"
					  + "Build: " + VersionInfo::Date;
	SplashScreen splash(splashData);
	//splash.SetImageStyleSheet("image: url(:/res/images/logo_team.png);");
	//splash.resize(480, 410);

	auto dlgResult = splash.exec();

	if (QDialog::Rejected == dlgResult)
	{
		return 0;
	}

	return dlgResult;
}

#if QT_VERSION >= 0x050000
void CustomMessageHandler(QtMsgType type, const QMessageLogContext& context, const QString &msg)
{
    Q_UNUSED(context)
    QString logLevel;
    switch (type)
    {
    case QtDebugMsg:
        logLevel = "DBUG";
        break;
    case QtInfoMsg:
        logLevel = "INFO";
        break;
    case QtWarningMsg:
        logLevel = "WARN";
        break;
    case QtCriticalMsg:
        logLevel = "CRIT";
        break;
    case QtFatalMsg:
        logLevel = "FATL";
        break;
    }

    QString logMsg = QString("%1 %2 %3").arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz")).arg(logLevel).arg(msg);

    QFile logFile(QCoreApplication::applicationName() + ".log");
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append))
    {
        QTextStream out(&logFile);
#if QT_VERSION >= 0x050D00        
        out << logMsg << Qt::endl;
#else
        out << logMsg << ::endl;
#endif
    }
}
#endif

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

#if QT_VERSION >= 0x050000
    // Open the log file and truncate existing content
    QFile logFile(QCoreApplication::applicationName() + ".log");
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        logFile.close(); // Close file after truncating
    }

    qInstallMessageHandler(CustomMessageHandler);

#endif
	QCoreApplication::setApplicationVersion(VersionInfo::VersionStr);
    QCoreApplication::setOrganizationName(QString::fromUtf8("Florian Mücke"));
	QCoreApplication::setOrganizationDomain("ipponboard.koe-judo.de");
	QCoreApplication::setApplicationName("Ipponboard");

    qInfo() << QCoreApplication::applicationName() << QCoreApplication::applicationVersion();

#if QT_VERSION >= 0x050000
    QCommandLineParser parser;
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption modeOption(QStringList() << "mode", "Selects starting mode: single|team|ask.", "mode", "ask");
    parser.addOption(modeOption);
    parser.process(a);
    QString mode = parser.value(modeOption);
    QStringList validModes { "single", "team", "ask" };
    if (!validModes.contains(mode))
    {
        parser.showHelp(ERROR_INVALID_PARAMETER);
    }
#endif

	// read language code
	QString langStr = QLocale::system().name();
	langStr.truncate(langStr.lastIndexOf('_'));

	const QString ini(QString::fromStdString(fm::GetSettingsFilePath("Ipponboard.ini")));

	QSettings settings(ini, QSettings::IniFormat, &a);
	settings.beginGroup(str_tag_Main);

	if (settings.contains(str_tag_Language))
	{
		langStr = settings.value(str_tag_Language).toString();
	}

	settings.endGroup();

	QTranslator translator;  // Note: this object needs to remain in scope.
	SetTranslation(a, translator, langStr);

	if (UpdateChecker::CheckForNewerVersion())
	{
        return NO_ERROR;
	}

    int dlgResult {0};

#if QT_VERSION >= 0x050000
    if (mode == "ask")
    {
        dlgResult = ShowSplashScreen();
    }
    else if (mode == "single")
    {
        dlgResult = QDialog::Accepted;
    }
    else if (mode == "team")
    {
        dlgResult = QDialog::Accepted + 1;
    }
#else
    dlgResult = ShowSplashScreen();
#endif
    if (dlgResult == 0)
    {
        return NO_ERROR;
    }

    std::unique_ptr<MainWindowBase> pMainWnd {nullptr};

	if (dlgResult == QDialog::Accepted + 1)
	{
        pMainWnd = std::make_unique<MainWindowTeam>();
	}
	else
	{
		pMainWnd = std::make_unique<MainWindow>();
	}

	try
	{
		pMainWnd->Init();
	}
	catch (std::exception const&)
	{
        return NO_ERROR;
	}

	pMainWnd->show();

	return a.exec();
}

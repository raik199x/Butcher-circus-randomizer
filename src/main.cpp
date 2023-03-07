#include <QApplication>
#include <QLocale>
#include <QScreen>
#include <QTranslator>

#include "../include/mainwindow.h"

/*!
 * \file main.cpp
 * \brief Entrance point for the app, 主函数
 *
 * \author raik, 小白
 */

/*!
 * \fn main
 * \brief The main function of the application.
 *
 * \param argc The number of command line arguments
 * \param argv The array of command line arguments
 * \return int return code
 */
int main(int argc, char *argv[]) {
	QApplication a(argc, argv);

	QTranslator translator;
	const QStringList uiLanguages = QLocale::system().uiLanguages();
	for (const QString &locale : uiLanguages) {
		const QString baseName = "Butcher_Circus_Randomizer_" + QLocale(locale).name();
		if (translator.load(":/i18n/" + baseName)) {
			a.installTranslator(&translator);
			break;
		}
	}
	MainWindow w;

	QScreen *screen = QGuiApplication::primaryScreen();
	QRect screenGeometry = screen->geometry();
	int x = (screenGeometry.width() - w.width()) / 2;
	int y = (screenGeometry.height() - w.height()) / 2;
	w.move(x, y);

	w.show();
	return a.exec();
}

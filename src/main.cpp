#include <QApplication>
#include <QLocale>
#include <QScreen>
#include <QTranslator>

#include "mainwindow.h"

/*!
 * \file main.cpp
 * \brief Entrance point for the app
 *
 * \author raik
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
  QApplication application(argc, argv);

  QTranslator       translator;
  const QStringList ui_languages = QLocale::system().uiLanguages();
  for (const QString &locale : ui_languages) {
    const QString base_name = "Butcher_Circus_Randomizer_" + QLocale(locale).name();
    if (translator.load(":/i18n/" + base_name)) {
      QApplication::installTranslator(&translator);
      break;
    }
  }

  MainWindow window;
  QScreen   *screen          = QGuiApplication::primaryScreen();
  QRect      screen_geometry = screen->geometry();
  int        x_cord          = (screen_geometry.width() - window.width()) / 2;
  int        y_cord          = (screen_geometry.height() - window.height()) / 2;
  window.move(x_cord, y_cord);
  window.show();
  return QApplication::exec();
}

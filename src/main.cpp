#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QMessageBox>

#include "rungaurd.h"

int main(int argc, char *argv[]) {

  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  static const char ENV_VAR_QT_DEVICE_PIXEL_RATIO[] = "QT_DEVICE_PIXEL_RATIO";
  if (!qEnvironmentVariableIsSet(ENV_VAR_QT_DEVICE_PIXEL_RATIO) &&
      !qEnvironmentVariableIsSet("QT_AUTO_SCREEN_SCALE_FACTOR") &&
      !qEnvironmentVariableIsSet("QT_SCALE_FACTOR") &&
      !qEnvironmentVariableIsSet("QT_SCREEN_SCALE_FACTORS")) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  }

#ifdef QT_DEBUG
  qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--remote-debugging-port=9421");
#else
  qputenv("QTWEBENGINE_CHROMIUM_FLAGS",
          "--disable-logging --single-process --disable-web-security");
#endif

  QApplication a(argc, argv);

  QApplication::setApplicationName("Worddle");
  QApplication::setOrganizationName("org.keshavnrj.ubuntu");
  QApplication::setApplicationVersion(VERSIONSTR);

// allow multiple instances in debug builds
#ifndef QT_DEBUG
  QString appname = QApplication::applicationName();
  RunGuard guard("org.keshavnrj.ubuntu." + appname);
  if (!guard.tryToRun()) {
    QMessageBox::critical(0, appname,
                          "An instance of " + appname + " is already running.");
    return 0;
  }
#endif

  MainWindow w;
  w.show();

  return a.exec();
}

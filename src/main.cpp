#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QMessageBox>

#include "rungaurd.h"

int main(int argc, char *argv[]) {
  /*
   * Add extra arguments on runtime
   */
  char ARG_DISABLE_WEB_SECURITY[] = "--disable-web-security";
  int newArgc = argc + 1 + 1;
  char **newArgv = new char *[newArgc];
  for (int i = 0; i < argc; i++) {
    newArgv[i] = argv[i];
  }
  newArgv[argc] = ARG_DISABLE_WEB_SECURITY;
  newArgv[argc + 1] = nullptr;

#ifdef QT_DEBUG
  qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "23654");
#endif

  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  static const char ENV_VAR_QT_DEVICE_PIXEL_RATIO[] = "QT_DEVICE_PIXEL_RATIO";
  if (!qEnvironmentVariableIsSet(ENV_VAR_QT_DEVICE_PIXEL_RATIO) &&
      !qEnvironmentVariableIsSet("QT_AUTO_SCREEN_SCALE_FACTOR") &&
      !qEnvironmentVariableIsSet("QT_SCALE_FACTOR") &&
      !qEnvironmentVariableIsSet("QT_SCREEN_SCALE_FACTORS")) {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  }

  QApplication a(newArgc, newArgv);

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

  qputenv("QTWEBENGINE_CHROMIUM_FLAGS","--single-process");
  qputenv("QTWEBENGINE_CHROMIUM_FLAGS","--disable-logging");
#ifdef QT_DEBUG
  qputenv("QTWEBENGINE_CHROMIUM_FLAGS","--remote-debugging-port=9421");
#endif

  MainWindow w;
  w.show();

  return a.exec();
}

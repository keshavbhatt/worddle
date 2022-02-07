#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "rateapp.h"

#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  setWindowTitle(QApplication::applicationName());
  setWindowIcon(QIcon(":/icons/app/icon-128.png"));
  setMinimumWidth(350);
  setMinimumHeight(500);

  spinner = new Spinner();

  ui->centralWidget->layout()->setContentsMargins(0, 0, 0, 0);

  webenginePlayerWidget = new WebEnginePlayer(this);

  // init stackWidget
  ui->stackedWidget->setAnimation(QEasingCurve::Type::OutQuart);
  ui->stackedWidget->setSpeed(650);
  connect(ui->stackedWidget, &QStackedWidget::currentChanged, [=](int arg1) {
    Q_UNUSED(arg1);
    if (stackVector.isEmpty() &&
        ui->stackedWidget->currentWidget() != webenginePlayerWidget) {
      qDebug() << "added" << webenginePlayerWidget->objectName()
               << "to stackVector since"
                  " it was empty and home"
                  " was not loaded";
      stackVector.append(webenginePlayerWidget);
    }
  });

  // add to stackWidget
  ui->stackedWidget->addWidget(webenginePlayerWidget);

  RateApp *rateApp = new RateApp(this, "snap://warddle", 5, 5, 1000 * 30);
  rateApp->setWindowTitle(QApplication::applicationName() + " | " +
                          tr("Rate Application"));
  rateApp->setVisible(false);
  rateApp->setWindowFlags(Qt::Dialog);
  rateApp->setAttribute(Qt::WA_DeleteOnClose, true);
  QPoint centerPos = this->geometry().center() - rateApp->geometry().center();
  connect(rateApp, &RateApp::showRateDialog, [=]() {
    if (this->windowState() != Qt::WindowMinimized && this->isVisible() &&
        isActiveWindow()) {
      rateApp->move(centerPos);
      rateApp->show();
    } else {
      rateApp->delayShowEvent();
    }
  });

  restoreGeometry(settings.value("mainWindowGeo").toByteArray());
  restoreState(settings.value("mainWindowState").toByteArray());
}

void MainWindow::closeEvent(QCloseEvent *event) {
  settings.setValue("mainWindowGeo", saveGeometry());
  settings.setValue("mainWindowState", saveState());

  QMainWindow::closeEvent(event);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
  if (event->type() == QEvent::KeyPress) {
    QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
    if (keyEvent->key() == Qt::Key_F5) {
      if (webenginePlayerWidget &&
          ui->stackedWidget->currentWidget() == webenginePlayerWidget)
        webenginePlayerWidget->reset();
    }
  }
  return QMainWindow::eventFilter(obj, event);
}

void MainWindow::switchStackWidget(QWidget *widget, bool addToStackVector) {
  if (addToStackVector) {
    // prevent adding duplicate widget in stackVector
    if (stackVector.isEmpty() == false &&
        ui->stackedWidget->currentWidget() != widget)
      stackVector.append(ui->stackedWidget->currentWidget());
    else if (stackVector.isEmpty()) {
      stackVector.append(ui->stackedWidget->currentWidget());
    }
  }
  ui->stackedWidget->slideInWgt(widget);
}

MainWindow::~MainWindow() { delete ui; }

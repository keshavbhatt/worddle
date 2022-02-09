#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSettings>

#include "spinner.h"
#include "webengineplayer.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void switchStackWidget(QWidget *widget, bool addToStackVector);

protected slots:
    void closeEvent(QCloseEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
    void resizeEvent(QResizeEvent *event);
private:
    Ui::MainWindow *ui;

    Spinner *spinner = nullptr;

    QSettings settings;

    QVector<QWidget*>stackVector;

    WebEnginePlayer *webenginePlayerWidget;


};

#endif // MAINWINDOW_H

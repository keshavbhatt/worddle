#ifndef WEBENGINEPLAYER_H
#define WEBENGINEPLAYER_H

#include "fullscreenwindow.h"

#include <QMainWindow>
#include <QWebEngineFullScreenRequest>
#include <QWebEngineView>
#include "webenginepage.h"

class WebEnginePlayer : public QWidget {
  Q_OBJECT

public:
  explicit WebEnginePlayer(QWidget *parent = nullptr);

signals:
    void loadingStarted();
    void loadingFinished();
public slots:
  void reset();
private slots:
  void fullScreenRequested(QWebEngineFullScreenRequest request);

  void insertStyleSheet(const QString &name, const QString &source, bool immediately);
  void removeStyleSheet(const QString &name, bool immediately);
  void injectMutationObserver();
private:
  QWebEngineView *m_view = nullptr;
  QScopedPointer<FullScreenWindow> m_fullScreenWindow;
};
#endif // WEBENGINEPLAYER_H

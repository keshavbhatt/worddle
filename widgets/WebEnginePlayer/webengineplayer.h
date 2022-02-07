#ifndef WEBENGINEPLAYER_H
#define WEBENGINEPLAYER_H

#include "fullscreenwindow.h"

#include <QMainWindow>
#include <QWebEngineFullScreenRequest>
#include <QWebEngineUrlRequestInfo>
#include <QWebEngineUrlRequestInterceptor>
#include <QWebEngineView>

class WebEnginePlayer : public QWidget {
  Q_OBJECT

public:
  explicit WebEnginePlayer(QWidget *parent = nullptr);

public slots:
  void reset();
private slots:
  void fullScreenRequested(QWebEngineFullScreenRequest request);

private:
  QWebEngineView *m_view = nullptr;
  QScopedPointer<FullScreenWindow> m_fullScreenWindow;
};

class RequestInterceptor : public QWebEngineUrlRequestInterceptor {
  Q_OBJECT

public:
  RequestInterceptor(QObject *parent = nullptr)
      : QWebEngineUrlRequestInterceptor(parent) {}

  void interceptRequest(QWebEngineUrlRequestInfo &info) {

    QString reqUrlStr = info.requestUrl().toString();

    QStringList badUrls = {"googletagmanager.com/gtag/js",
                           "google-analytics.com/g/collect",
                           "dailywordle.com/images/"};
    foreach (QString badUrl, badUrls) {
      if (reqUrlStr.contains(badUrl, Qt::CaseInsensitive))
        info.block(true);
    }
  }
};

#endif // WEBENGINEPLAYER_H

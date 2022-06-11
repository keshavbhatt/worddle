#include <QWebEngineUrlRequestInfo>
#include <QWebEngineUrlRequestInterceptor>

class RequestInterceptor : public QWebEngineUrlRequestInterceptor {
  Q_OBJECT

public:
  RequestInterceptor(QObject *parent = nullptr)
      : QWebEngineUrlRequestInterceptor(parent) {}

  void interceptRequest(QWebEngineUrlRequestInfo &info) {

    QString reqUrlStr = info.requestUrl().toString();

    QStringList badUrls = {"googletagmanager.com",
                           "google-analytics",
                           "dailywordle.com/images/",
                           "nyt.com",
                           "promo",
                           "promotion",
                           "doubleclick.net",
                           "pnytimes.chartbeat.net",
                           "nytimes.com/track"};
    foreach (QString badUrl, badUrls) {
      if (reqUrlStr.contains(badUrl, Qt::CaseInsensitive))
        info.block(true);
    }
  }
};

#include "webengineplayer.h"

#include <QVBoxLayout>
#include <QWebEngineFullScreenRequest>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineView>

WebEnginePlayer::WebEnginePlayer(QWidget *parent)
    : QWidget(parent), m_view(new QWebEngineView(this)) {
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(m_view);
  layout->setContentsMargins(0, 0, 0, 0);
  this->setLayout(layout);

  m_view->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled,
                                   true);
  m_view->page()->profile()->settings()->setAttribute(
      QWebEngineSettings::ShowScrollBars, false);
  m_view->page()->settings()->setAttribute(QWebEngineSettings::ShowScrollBars,
                                           false);
  connect(m_view->page(), &QWebEnginePage::fullScreenRequested, this,
          &WebEnginePlayer::fullScreenRequested);

  // init requestInterceptor to page profile
  RequestInterceptor *interceptor =
      new RequestInterceptor(m_view->page()->profile());
  m_view->page()->profile()->setUrlRequestInterceptor(interceptor);

  this->reset();
}

void WebEnginePlayer::reset() {
  m_view->load(QUrl("https://www.powerlanguage.co.uk/wordle/"));
}

void WebEnginePlayer::fullScreenRequested(QWebEngineFullScreenRequest request) {
  if (request.toggleOn()) {
    if (m_fullScreenWindow)
      return;
    request.accept();
    m_fullScreenWindow.reset(new FullScreenWindow(m_view));
  } else {
    if (!m_fullScreenWindow)
      return;
    request.accept();
    m_fullScreenWindow.reset();
  }
}

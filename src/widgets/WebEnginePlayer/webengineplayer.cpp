#include "webengineplayer.h"
#include "requestinterceptor.h"

#include <QVBoxLayout>
#include <QWebEngineFullScreenRequest>
#include <QWebEngineProfile>
#include <QWebEngineScript>
#include <QWebEngineScriptCollection>
#include <QWebEngineSettings>
#include <QWebEngineView>

WebEnginePlayer::WebEnginePlayer(QWidget *parent)
    : QWidget(parent), m_view(new QWebEngineView(this)) {
  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(m_view);
  layout->setContentsMargins(0, 0, 0, 0);
  this->setLayout(layout);

  this->insertStyleSheet("focus",
                         ":focus {"
                         "outline: 0 !important;"
                         "outline-color: transparent !important;"
                         "outline-style: none !important;}",
                         true);

  m_view->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled,
                                   true);
  m_view->page()->profile()->settings()->setAttribute(
      QWebEngineSettings::ShowScrollBars, false);
  m_view->page()->settings()->setAttribute(QWebEngineSettings::ShowScrollBars,
                                           false);
  m_view->setContextMenuPolicy(Qt::NoContextMenu);
  connect(m_view->page(), &QWebEnginePage::fullScreenRequested, this,
          &WebEnginePlayer::fullScreenRequested);

  // init requestInterceptor to page profile
  RequestInterceptor *interceptor =
      new RequestInterceptor(m_view->page()->profile());
  m_view->page()->profile()->setUrlRequestInterceptor(interceptor);

  connect(m_view, &QWebEngineView::loadStarted, this,
          [=]() { emit loadingStarted(); });

  connect(m_view, &QWebEngineView::loadFinished, this, [=]() {
    QString set_vars = "var setting_button = document.querySelector('body > game-app').shadowRoot.querySelector('#settings-button');";
    QString add_event_listeners = "setting_button.addEventListener('click', remove_menu);";
    QString remove_menu =
        "function remove_menu(){document.querySelector('body > "
        "game-app').shadowRoot.querySelector('#game > game-page > "
        "game-settings').shadowRoot.querySelector('div.sections > "
        "section:nth-child(2)').remove();}";
    QString remove_nav_btn =
        "document.querySelector('game-app').shadowRoot.querySelector('header "
        ".menu-left #nav-button').remove();";
    m_view->page()->runJavaScript(set_vars + add_event_listeners + remove_menu + remove_nav_btn);
    emit loadingFinished();
  });

  this->reset();
}

void WebEnginePlayer::reset() {
  m_view->load(QUrl("https://www.nytimes.com/games/wordle/index.html"));
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

void WebEnginePlayer::insertStyleSheet(const QString &name,
                                       const QString &source,
                                       bool immediately) {
  QWebEngineScript script;
  QString s = QString::fromLatin1("(function() {"
                                  "    css = document.createElement('style');"
                                  "    css.type = 'text/css';"
                                  "    css.id = '%1';"
                                  "    document.head.appendChild(css);"
                                  "    css.innerText = '%2';"
                                  "})()")
                  .arg(name, source.simplified());
  if (immediately)
    m_view->page()->runJavaScript(s, QWebEngineScript::ApplicationWorld);

  script.setName(name);
  script.setSourceCode(s);
  script.setInjectionPoint(QWebEngineScript::DocumentReady);
  script.setRunsOnSubFrames(true);
  script.setWorldId(QWebEngineScript::ApplicationWorld);
  m_view->page()->scripts().insert(script);
}

void WebEnginePlayer::removeStyleSheet(const QString &name, bool immediately) {
  QString s =
      QString::fromLatin1("(function() {"
                          "    var element = document.getElementById('%1');"
                          "    element.outerHTML = '';"
                          "    delete element;"
                          "})()")
          .arg(name);
  if (immediately)
    m_view->page()->runJavaScript(s, QWebEngineScript::ApplicationWorld);

  QWebEngineScript script = m_view->page()->scripts().findScript(name);
  m_view->page()->scripts().remove(script);
}

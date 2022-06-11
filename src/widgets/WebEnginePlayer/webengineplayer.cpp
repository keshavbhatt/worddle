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
  QString pr = R"(<div id='promo'>
  <div class='rule'></div>
  <a href='https://www.snapcraft.io/orion-desktop' class='promo-link'>
    <div class='promo-icon'>
      <svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 48 48' width='42px' height='42px' fill-rule='evenodd' clip-rule='evenodd'>
<path fill='#4caf50' fill-rule='evenodd' d='M23.501,44.125c11.016,0,20-8.984,20-20 c0-11.015-8.984-20-20-20c-11.016,0-20,8.985-20,20C3.501,35.141,12.485,44.125,23.501,44.125z' clip-rule='evenodd'/>
<path fill='#fff' fill-rule='evenodd' d='M43.252,27.114C39.718,25.992,38.055,19.625,34,11l-7,1.077 c1.615,4.905,8.781,16.872,0.728,18.853C20.825,32.722,17.573,20.519,15,14l-8,2l10.178,27.081c1.991,0.67,4.112,1.044,6.323,1.044 c0.982,0,1.941-0.094,2.885-0.232l-4.443-8.376c6.868,1.552,12.308-0.869,12.962-6.203c1.727,2.29,4.089,3.183,6.734,3.172 C42.419,30.807,42.965,29.006,43.252,27.114z' clip-rule='evenodd'/></svg>
    </div>
    <div class='promo-text'>
      <span class='promo-text-primary'>Stream/Download Torrents securely on your Linux Box</span>
      <span class='promo=text-secondary'>with Orion App.</span>
      <span class='promo-cta'>Try Orion Torrent Client</span>
    </div>
    <div class='promo-arrow'>
      <svg width='24' height='24' viewBox='0 0 24 24' fill='none' xmlns='http://www.w3.org/2000/svg'>
        <circle cx='12' cy='12' r='12' fill='var(--svg-arrow-fill)'></circle>
        <path d='M10.4038 6L15.8076 11.4038L10.4038 16.8076' stroke='var(--svg-arrow-stroke)' stroke-width='1.5' stroke-linecap='round'></path>
      </svg>
    </div>
  </a>
</div>)";
  pr = pr.replace("\n", "");

  QString pr2 = R"(<a href='https://www.snapcraft.io/orion-desktop' title='Torrent Client for Linux'><div class='setting'>
        <div class='text'>
          <div class='title'>Orion Torrent Client</div>
        </div>
        <div class='control'>
          <a href='https://www.snapcraft.io/orion-desktop' title='Torrent Client for Linux'><svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 48 48' width='42px' height='42px' fill-rule='evenodd' clip-rule='evenodd'>
<path fill='#4caf50' fill-rule='evenodd' d='M23.501,44.125c11.016,0,20-8.984,20-20 c0-11.015-8.984-20-20-20c-11.016,0-20,8.985-20,20C3.501,35.141,12.485,44.125,23.501,44.125z' clip-rule='evenodd'/>
<path fill='#fff' fill-rule='evenodd' d='M43.252,27.114C39.718,25.992,38.055,19.625,34,11l-7,1.077 c1.615,4.905,8.781,16.872,0.728,18.853C20.825,32.722,17.573,20.519,15,14l-8,2l10.178,27.081c1.991,0.67,4.112,1.044,6.323,1.044 c0.982,0,1.941-0.094,2.885-0.232l-4.443-8.376c6.868,1.552,12.308-0.869,12.962-6.203c1.727,2.29,4.089,3.183,6.734,3.172 C42.419,30.807,42.965,29.006,43.252,27.114z' clip-rule='evenodd'/></svg></a>
        </div>
      </div></a>)";
  pr2 = pr2.replace("\n", "");

  this->insertStyleSheet("focus",
                         ":focus {"
                         "outline: 0 !important;"
                         "outline-color: transparent !important;"
                         "outline-style: none !important;}",
                         true);

  WebEnginePage *page = new WebEnginePage(this);
  m_view->setPage(page);
  m_view->settings()->setAttribute(QWebEngineSettings::FullScreenSupportEnabled,
                                   true);
  m_view->settings()->setAttribute(
      QWebEngineSettings::JavascriptCanAccessClipboard, true);
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
    QString set_vars =
        "var setting_button = document.querySelector('body > "
        "game-app').shadowRoot.querySelector('#settings-button');"
        "var stats_button = document.querySelector('body > game-app')"
        ".shadowRoot.querySelector('#statistics-button');";
    QString add_event_listeners =
        "setting_button.addEventListener('click', remove_menu);"
        "setting_button.addEventListener('click', remove_footnote);"
        "stats_button.addEventListener('click', remove_promo);";
    QString remove_menu =
        QString("function remove_menu(){document.querySelector('body > "
                "game-app').shadowRoot.querySelector('#game > game-page > "
                "game-settings').shadowRoot.querySelector('div.sections > "
                "section:nth-child(2)').outerHTML=\""
                "%1\""
                "}")
            .arg(pr2);
    QString remove_footnote =
        "function remove_footnote(){"
        "document.querySelector('body > "
        "game-app').shadowRoot.querySelector('#game > game-page > "
        "game-settings').shadowRoot.querySelector('#footnote').remove();}";
    QString remove_nav_btn =
        "document.querySelector('game-app').shadowRoot.querySelector('header "
        ".menu-left #nav-button').remove();";
    QString remove_promo =
        QString("function remove_promo(){document.querySelector"
                "('body > game-app').shadowRoot.querySelector"
                "('#game > game-modal > game-stats')."
                "shadowRoot.querySelector('#promo').outerHTML=\""
                "%1\""
                "}")
            .arg(pr);
    m_view->page()->runJavaScript(set_vars + add_event_listeners + remove_menu +
                                  remove_nav_btn + remove_promo +
                                  remove_footnote);
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

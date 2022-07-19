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
  QString pr = R"(
                <hr>
                <a style='text-decoration: none; color: inherit;' href='https://www.snapcraft.io/orion-desktop' class='promo-link'>
                   <div style='float: left;'>
                      <svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 48 48' width='42px' height='42px' fill-rule='evenodd' clip-rule='evenodd'>
                         <path fill='#4caf50' fill-rule='evenodd' d='M23.501,44.125c11.016,0,20-8.984,20-20 c0-11.015-8.984-20-20-20c-11.016,0-20,8.985-20,20C3.501,35.141,12.485,44.125,23.501,44.125z' clip-rule='evenodd'></path>
                         <path fill='#fff' fill-rule='evenodd' d='M43.252,27.114C39.718,25.992,38.055,19.625,34,11l-7,1.077 c1.615,4.905,8.781,16.872,0.728,18.853C20.825,32.722,17.573,20.519,15,14l-8,2l10.178,27.081c1.991,0.67,4.112,1.044,6.323,1.044 c0.982,0,1.941-0.094,2.885-0.232l-4.443-8.376c6.868,1.552,12.308-0.869,12.962-6.203c1.727,2.29,4.089,3.183,6.734,3.172 C42.419,30.807,42.965,29.006,43.252,27.114z' clip-rule='evenodd'></path>
                      </svg>
                   </div>
                   <div style='font-weight: bolder; padding-left: 50px;'>      <span class='promo-text-primary'>Stream/Download Torrents securely on your Linux Box with Orion App. Try Orion Torrent Client</span>    </div>
                </a>
                )";
  pr = pr.replace("\n", "");

  QString pr2 = R"(
                <a style='text-decoration: none; color: inherit;' href='https://www.snapcraft.io/orion-desktop' title='Torrent Client for Linux'>
                    <div class='setting'>
                        <svg style='float: left' xmlns='http://www.w3.org/2000/svg' viewBox='0 0 48 48' width='42px' height='42px' fill-rule='evenodd' clip-rule='evenodd'>
                            <path fill='#4caf50' fill-rule='evenodd' d='M23.501,44.125c11.016,0,20-8.984,20-20 c0-11.015-8.984-20-20-20c-11.016,0-20,8.985-20,20C3.501,35.141,12.485,44.125,23.501,44.125z' clip-rule='evenodd'/>
                            <path fill='#fff' fill-rule='evenodd' d='M43.252,27.114C39.718,25.992,38.055,19.625,34,11l-7,1.077 c1.615,4.905,8.781,16.872,0.728,18.853C20.825,32.722,17.573,20.519,15,14l-8,2l10.178,27.081c1.991,0.67,4.112,1.044,6.323,1.044 c0.982,0,1.941-0.094,2.885-0.232l-4.443-8.376c6.868,1.552,12.308-0.869,12.962-6.203c1.727,2.29,4.089,3.183,6.734,3.172 C42.419,30.807,42.965,29.006,43.252,27.114z' clip-rule='evenodd'/>
                        </svg>
                        <div style='padding-top: 12px;margin-top: 12px; margin-left: 50px; font-weight: bolder;' class='title'>Orion Torrent Client</div>
                    </div>
                </a>
                )";
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
    injectMutationObserver();
    QString set_vars =
        "var setting_button = document.querySelector('#settings-button');"
        "var stats_button = document.querySelector('#statistics-button');"
        "var help_button = document.querySelector('#help-button');";

    QString remove_account_hrefs =
        R"(function remove_account_href(){
            setTimeout( function() {
                document.querySelectorAll("a[href*='myaccount.']").forEach(function (arrayItem) {
                arrayItem.parentNode.style.display = "none";
                });
            }, 100);
        })";

//    // div[class^='Promo-module_promo'
//    QString promo_observer =
//        R"(var pr_remover = function (){ waitForElement("div[class^='Promo-module_promo'").then( () => {
//                                    var container = document.querySelector("div[class^='Promo-module_promo'");
//                                    container.style.display = 'none';
//                                 })};
//                              )";

    QString promo_observer2 =
        R"( var pr_remover = function (){ waitForElement("div[class^='Stats-module_ctaContainer'").then( () => {
                                    var container = document.querySelector("div[class^='Stats-module_ctaContainer'");
                                    container.style.display = 'none';
                                 })};
                              )";

    QString footnote_observer =
        R"( var fn_remover = function (){ waitForElement("div[class*='footnote'").then( () => {
                                    var container = document.querySelector("div[class*='footnote'");
                                    container.style.display = 'none';
                                 })};
                              )";


    QString add_event_listeners =
        "setting_button.addEventListener('click', remove_menu);"
        "setting_button.addEventListener('click', fn_remover);"
        "stats_button.addEventListener('click', remove_promo);"
        "help_button.addEventListener('click', remove_account_href);"
        "stats_button.addEventListener('click', remove_account_href);";

    QString remove_menu =
        QString("function remove_menu(){"
                "setTimeout(function(){"
                "document.querySelector('#wordle-app-game > div:nth-of-type(3) "
                "> div > div:nth-of-type(1) > "
                "section:nth-of-type(2)').outerHTML = \"%1\";"
                "}, 100);"
                "}")
            .arg(pr2);

    QString remove_nav_btn = "document.querySelector('html > body > header > "
                             "div > button').remove();";

    QString remove_promo =
        QString("function remove_promo(){setTimeout( function() {let promo = "
                "document.querySelector"
                "('html > body > div > div:nth-of-type(3) > div > "
                "div:nth-of-type(1) > div:nth-of-type(3)'); const promo_style "
                "= window.getComputedStyle(promo) ; promo.outerHTML=\""
                "%1\""
                "; pr_remover()}, 100)};")
            .arg(pr);

    m_view->page()->runJavaScript(set_vars + promo_observer2 +
                                  footnote_observer + add_event_listeners +
                                  remove_menu + remove_nav_btn + remove_promo +
                                  "; pr_remover(); fn_remover();" + remove_account_hrefs);
    emit loadingFinished();
  });

  this->reset();
}

void WebEnginePlayer::injectMutationObserver() {
  QString js =
      R"(function waitForElement(selector) {
                return new Promise(resolve => {
                    if (document.querySelector(selector)) {
                        return resolve(document.querySelector(selector));
                    }
                    const observer = new MutationObserver(mutations => {
                        if (document.querySelector(selector)) {
                            resolve(document.querySelector(selector));
                            //observer.disconnect();
                        }
                    });
                    observer.observe(document.body, {
                        childList: true,
                        subtree: true
                    });
                });
            })";
  m_view->page()->runJavaScript(js);
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

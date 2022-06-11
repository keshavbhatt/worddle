#include "webenginepage.h"

#include <QDesktopServices>

WebEnginePage::WebEnginePage(QObject *parent)
    : QWebEnginePage{parent}
{

}

bool WebEnginePage::acceptNavigationRequest(const QUrl & url, QWebEnginePage::NavigationType type, bool isMainFrame)
{
    if (type == QWebEnginePage::NavigationTypeLinkClicked)
    {
        QDesktopServices::openUrl(url);
        return false;
    }
    return true;
}


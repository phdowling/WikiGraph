#include "pageloader.h"

PageLoader::PageLoader(QString url, void *item, QObject *parent) :
    QObject(parent)
{
    _url = url;
    _item = item;
    _networkManager = new QNetworkAccessManager(this);
    connect(_networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onPageLoadFinished(QNetworkReply*)));
    _networkManager->get(QNetworkRequest(QUrl(url)));
}

PageLoader::~PageLoader()
{
    _reply->deleteLater();
    _networkManager->deleteLater();
}

void PageLoader::onPageLoadFinished(QNetworkReply *reply)
{
    _reply = reply;
    finished(this);
}

QNetworkReply *PageLoader::getReply() const
{
    return _reply;
}

QString PageLoader::getUrl() const
{
    return _url;
}

void *PageLoader::getItem() const
{
    return _item;
}


#ifndef PAGELOADER_H
#define PAGELOADER_H

#include <QObject>
#include <QString>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class PageLoader : public QObject
{
    Q_OBJECT
public:
    explicit PageLoader(QString url, void *item = 0, QObject *parent = 0);
    virtual ~PageLoader();
    QNetworkReply *getReply() const;
    QString getUrl() const;
    void *getItem() const;

private:
    QNetworkAccessManager *_networkManager;
    QString _url;
    void *_item;
    QNetworkReply *_reply;

signals:
    void finished(PageLoader *pageLoader);

private slots:
    void onPageLoadFinished(QNetworkReply *reply);
};

#endif // PAGELOADER_H

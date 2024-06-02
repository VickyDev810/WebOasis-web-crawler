#ifndef BINGIMAGE_H
#define BINGIMAGE_H

#include <QObject>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>

class BingImage : public QObject
{
    Q_OBJECT

public:
    explicit BingImage(QObject *parent = nullptr);
    void searchImages(const QString &query);

signals:
    void imagesReady(const QStringList &imageUrls);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
};

#endif // BINGIMAGE_H

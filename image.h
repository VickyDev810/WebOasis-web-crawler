#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class Image : public QObject
{
    Q_OBJECT

public:
    Image(QObject *parent = nullptr);
    void searchImages(const QString &query);

signals:
    void imagesReady(const QStringList &imageUrls);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
};

#endif // IMAGE_H

#include "image.h"

Image::Image(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Image::onReplyFinished);
}

void Image::searchImages(const QString &query)
{
    // Construct your Unsplash API URL here

    //To make this project working enter your api key here
    QString apiUrl = "https://api.unsplash.com/search/photos?query=" + query + "&client_id=your api key here&per_page=30";

    // Send a GET request to Unsplash API
    manager->get(QNetworkRequest(QUrl(apiUrl)));
}

void Image::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray data = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QStringList imageUrls;

        if (jsonDoc.isObject())
        {
            QJsonObject jsonObj = jsonDoc.object();
            if (jsonObj.contains("results") && jsonObj["results"].isArray())
            {
                QJsonArray results = jsonObj["results"].toArray();
                for (const QJsonValue &result : results)
                {
                    if (result.isObject())
                    {
                        QJsonObject resultObj = result.toObject();
                        if (resultObj.contains("urls") && resultObj["urls"].isObject())
                        {
                            QJsonObject urlsObj = resultObj["urls"].toObject();
                            if (urlsObj.contains("regular") && urlsObj["regular"].isString())
                            {
                                QString imageUrl = urlsObj["regular"].toString();
                                imageUrls.append(imageUrl);
                            }
                        }
                    }
                }
            }
        }

        emit imagesReady(imageUrls);
    }
    else
    {
        // Handle the error here
    }

    reply->deleteLater();
}

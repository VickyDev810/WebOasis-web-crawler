#include "bingimage.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>
BingImage::BingImage(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &BingImage::onReplyFinished);
}

void BingImage::searchImages(const QString &query)
{
    // Replace "YOUR_BING_API_KEY" with your actual Bing Image Search API key
    QString apiUrl = "https://api.bing.microsoft.com/v7.0/images/search?q=" + query + "&count=30";

    QNetworkRequest request;
    request.setUrl(QUrl(apiUrl));

    //to make this project working enter your api key here

    request.setRawHeader("Ocp-Apim-Subscription-Key", "YOUR API KEY HERE");

    // Send a GET request to Bing Image Search API
    manager->get(request);
}


void BingImage::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray data = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QStringList imageUrls;

        if (jsonDoc.isObject())
        {
            QJsonObject jsonObj = jsonDoc.object();
            if (jsonObj.contains("value") && jsonObj["value"].isArray())
            {
                QJsonArray results = jsonObj["value"].toArray();
                qDebug() << "Total results:" << results.size(); // Debug: Print the total number of results

                for (const QJsonValue &result : results)
                {
                    if (result.isObject())
                    {
                        QJsonObject resultObj = result.toObject();
                        if (resultObj.contains("contentUrl") && resultObj["contentUrl"].isString())
                        {
                            QString imageUrl = resultObj["contentUrl"].toString();
                            imageUrls.append(imageUrl);
                        }
                    }
                }
            }
            else
            {
                qDebug() << "No results found in the 'value' field.";
            }
        }
        else
        {
            qDebug() << "JSON response is not an object.";
        }

        emit imagesReady(imageUrls);
    }
    else
    {
        // Handle the error here
        qDebug() << "Error:" << reply->errorString();
    }
    if (reply->error() == QNetworkReply::NoError)
    {
        // ... (existing code for processing successful response)
    }
    else
    {
        // Handle the error here
        qDebug() << "Error:" << reply->errorString(); // Print the full error message
    }

    reply->deleteLater();
}

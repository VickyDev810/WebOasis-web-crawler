#include "bingtext.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QUrl>

BingText::BingText(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &BingText::onReplyFinished);
}

void BingText::searchText(const QString &query)
{
    // Replace "YOUR_BING_API_KEY" with your actual Bing Search API key
    QString apiUrl = "https://api.cognitive.microsoft.com/bing/v7.0/search?q=" + query;

    QNetworkRequest request;
    request.setUrl(QUrl(apiUrl));
    request.setRawHeader("Ocp-Apim-Subscription-Key", "YOUR_BING_API_KEY");

    // Send a GET request to Bing Search API
    manager->get(request);
}

void BingText::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray data = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QStringList results;

        if (jsonDoc.isObject())
        {
            QJsonObject jsonObj = jsonDoc.object();
            if (jsonObj.contains("webPages") && jsonObj["webPages"].isObject())
            {
                QJsonObject webPagesObj = jsonObj["webPages"].toObject();
                if (webPagesObj.contains("value") && webPagesObj["value"].isArray())
                {
                    QJsonArray valueArray = webPagesObj["value"].toArray();
                    for (const QJsonValue &value : valueArray)
                    {
                        if (value.isObject())
                        {
                            QJsonObject valueObj = value.toObject();
                            if (valueObj.contains("name") && valueObj["name"].isString())
                            {
                                QString resultText = valueObj["name"].toString();
                                results.append(resultText);
                            }
                        }
                    }
                }
            }
        }

        emit textResultsReady(results);
    }
    else
    {
        // Handle the error here
    }

    reply->deleteLater();
}

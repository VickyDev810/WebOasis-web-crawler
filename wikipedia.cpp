#include "wikipedia.h"

Wikipedia::Wikipedia(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &Wikipedia::onReplyFinished);
}

void Wikipedia::searchSummary(const QString &query)
{
    // Construct the Wikipedia API URL for the summary
    QString apiUrl = "https://en.wikipedia.org/api/rest_v1/page/summary/" + query + "?exintro&explaintext";

    // Send a GET request to the Wikipedia API
    manager->get(QNetworkRequest(QUrl(apiUrl)));
}

void Wikipedia::onReplyFinished(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError)
    {
        QByteArray data = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);

        if (jsonDoc.isObject())
        {
            QJsonObject jsonObj = jsonDoc.object();
            if (jsonObj.contains("extract") && jsonObj["extract"].isString())
            {
                QString summary = jsonObj["extract"].toString();
                emit summaryReady(summary);
            }
        }
    }
    else
    {
        // Handle the error here
    }

    reply->deleteLater();
}


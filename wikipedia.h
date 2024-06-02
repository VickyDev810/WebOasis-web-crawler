#ifndef WIKIPEDIA_H
#define WIKIPEDIA_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>

class Wikipedia : public QObject
{
    Q_OBJECT

public:
    Wikipedia(QObject *parent = nullptr);
    void searchSummary(const QString &query);

signals:
    void summaryReady(const QString &summary);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
};

#endif // WIKIPEDIA_H


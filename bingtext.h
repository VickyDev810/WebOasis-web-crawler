#ifndef BINGTEXT_H
#define BINGTEXT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStringList>

class BingText : public QObject
{
    Q_OBJECT

public:
    explicit BingText(QObject *parent = nullptr);
    void searchText(const QString &query);

signals:
    void textResultsReady(const QStringList &results);

private slots:
    void onReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
};

#endif // BINGTEXT_H

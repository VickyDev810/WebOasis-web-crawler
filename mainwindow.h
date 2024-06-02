#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QStringList>
#include <vector>
#include "image.h"
#include "bingimage.h"
#include "wikipedia.h" // Include the Wikipedia class header
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QComboBox>
#include <unordered_set> // Include this line for std::unordered_set
#include <QSet>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void searchImages();
    void searchImagesFromBing();
    void onImagesReady(const QStringList &imageUrls);
    void loadAndDisplayImage(const QStringList &imageUrls);
    void onNetworkReplyFinished(QNetworkReply* reply);
    void onApiSelectionChanged(int index);
    void clearImages();
    void textResultsReady();
    void searchWikipedia();//wikipedia slots
    void handleSummaryReady(const QString &summary);
    void onButtonClicked();
private:
    Ui::MainWindow *ui;
    Image *image;
    BingImage *bingimage;
    Wikipedia *wikipedia; // Declare Wikipedia instance
    std::vector<QLabel*> labelsList;
    int currentImageIndex;
    QHBoxLayout* galleryLayout;
    QWidget* galleryWidget;
    QNetworkAccessManager* networkManager;
    QHash<QLabel*, QString> labelUrlMap;
    std::unordered_set<std::string> searchHistory;

};

#endif // MAINWINDOW_H

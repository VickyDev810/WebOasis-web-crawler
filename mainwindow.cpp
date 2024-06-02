#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "image.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QHBoxLayout>
#include <QEventLoop>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QThread>
#include <QComboBox>
#include <QList>
#include <QSet>
#include <QTextEdit>
#include <QDesktopServices>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentImageIndex(0)
{
    ui->setupUi(this);

    //calling image classes
    image = new Image(this);
    bingimage = new BingImage(this);

    //calling text classes
    wikipedia = new Wikipedia(this);

    // Connect the BingImage's imagesReady signal to MainWindow's onImagesReady slot
    connect(bingimage, &BingImage::imagesReady, this, &MainWindow::onImagesReady);
    connect(image, &Image::imagesReady, this, &MainWindow::onImagesReady);

    //connecting wikipedia
    connect(wikipedia, &Wikipedia::summaryReady, this, &MainWindow::handleSummaryReady);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::searchWikipedia);
    connect(ui->LineEdit, &QLineEdit::returnPressed, this, &MainWindow::searchWikipedia);


    connect(ui->LineEdit, &QLineEdit::returnPressed, this, &MainWindow::searchImages);
    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::searchImages);
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onApiSelectionChanged);
    //Creating Widget to store imaegs
    galleryWidget = new QWidget(this);
    galleryLayout = new QHBoxLayout(galleryWidget);
    galleryLayout->setSpacing(10);

    //creating widgets to show text about query
    QVBoxLayout* textLayout;
    QLabel* wikipediaLabel;

    //constructing text classes


    // Create a scroll area and set its widget
    ui->gallery->setWidgetResizable(true);
    ui->gallery->setWidget(galleryWidget);

    //some testing
//    ui->label->setText("billo bagge balliya di ki kregi baghe baghe billiya di kregi sdsd sdf sdf sdf dsfdsf");
//    ui->label->adjustSize();


    // Create the QNetworkAccessManager instance and connect it here
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &MainWindow::onNetworkReplyFinished);

    ui->LineEdit->setStyleSheet("background: #2C3137;border:none;border-radius:23px;");
    //added some items to combobox
    ui->comboBox->addItem("Unsplash");
    ui->comboBox->addItem("Microsoft Bing Image Search API");
    ui->comboBox_2->addItem("WikiPedia");
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::onApiSelectionChanged(int index)
{
    if (index == 0) {
        // Unsplash API is selected
        searchImages();
    } else if (index == 1) {
        // Microsoft Bing Image Search API is selected
        searchImagesFromBing();
    }
}
void MainWindow::clearImages()
{
    // Remove all child widgets from the gallery layout
    QLayoutItem* item;
    while ((item = galleryLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }

    // Clear the linkLayout as well if needed


    // Reset the current image index
    currentImageIndex = 0;
}

void MainWindow::searchImages()
{
    QString query = ui->LineEdit->text().toLower();
    std::string queryStd = query.toStdString();

    if (searchHistory.find(queryStd) == searchHistory.end()) {
        // Query is not in the search history, add it to the history panel
        QLabel* label = new QLabel(this);
        label->setText(query);
        label->setStyleSheet("padding:15px;color:white; background-color: #202429;");
        label->setFixedHeight(50);
        label->show();
        ui->History->setWidgetResizable(true);
        ui->read->addWidget(label);
        // Update the set with the new query
        searchHistory.insert(queryStd);
    }

    // Rest of your searchImages function
    clearImages();
    image->searchImages(query);
}

void MainWindow::searchImagesFromBing(){
    clearImages();

    QString query = ui->LineEdit->text();
    bingimage->searchImages(query);
}
void MainWindow::onImagesReady(const QStringList &imageUrls)
{

    qDebug() << "Received" << imageUrls.size() << "image URLs.";

    // Download and display images one by one in the order they are received
    loadAndDisplayImage(imageUrls);
}

void MainWindow::loadAndDisplayImage(const QStringList &imageUrls)
{
    if (currentImageIndex < imageUrls.size())
    {
        QString imageUrl = imageUrls.at(currentImageIndex);

        // Debug: Print the URL being processed
        qDebug() << "Processing image URL" << currentImageIndex + 1 << ":" << imageUrl;
        qDebug() << "Image URL: " << imageUrl;



        //adding the functionality to copy on click
//        connect(label, &QLabel::clicked, this, &MainWindow::onLabelClicked);

        QNetworkRequest request(imageUrl);

        // Use a QEventLoop to wait for the network reply to finish
        QEventLoop loop;

        // Send the network request using the existing networkManager
        QNetworkReply* reply = networkManager->get(request);

        // Connect the reply to the event loop
        connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

        loop.exec();

        QPixmap pixmap;
        if (reply->error() == QNetworkReply::NoError)
        {
            pixmap.loadFromData(reply->readAll());
            pixmap = pixmap.scaled(250, 250, Qt::KeepAspectRatio);

            QLabel* label = new QLabel(this);
            label->setFixedSize(250, 250);
            label->setPixmap(pixmap);
            labelUrlMap.insert(label, imageUrl);

            // adding a button to open the desired image to the user

            QPushButton* button = new QPushButton("Open", this);
            button->setFixedSize(80, 30);
            button->setStyleSheet("background: #444654;");
            button->setProperty("url", imageUrl); // Store the URL in the button's property

            connect(button, &QPushButton::clicked, this, &MainWindow::onButtonClicked);
            label->show();

            //adding a layout to store the images and button
            QVBoxLayout* layout = new QVBoxLayout;
            layout->addWidget(label, 0 , Qt::AlignCenter);
            layout->addWidget(button, 0 ,Qt::AlignCenter);

            // Add the QLabel to the galleryLayout
            QWidget* imageWidget = new QWidget(this);
            imageWidget->setLayout(layout);

            galleryLayout->addWidget(imageWidget);

            // Debug: Print a message when an image is successfully displayed
            qDebug() << "Image displayed for URL" << currentImageIndex + 1 << ":" << imageUrl;

            // Clean up the network reply
            reply->deleteLater();

            // Increment the current image index to load the next image
            currentImageIndex++;

            // Call loadAndDisplayImage recursively to load the next image
            loadAndDisplayImage(imageUrls);
        }
        else
        {
            // Handle the case where the image couldn't be loaded
            qDebug() << "Error loading image for URL" << currentImageIndex + 1 << ":" << reply->errorString();
            reply->deleteLater();

            // Move on to the next image
            currentImageIndex++;

            // Call loadAndDisplayImage recursively to load the next image
            loadAndDisplayImage(imageUrls);
        }
    }
    else
    {
        // All images have been loaded
        qDebug() << "All images have been loaded.";
    }
}

void MainWindow::onNetworkReplyFinished(QNetworkReply* reply)
{
    // Handle the completion of network requests here, if needed.
    // This function is called when any network request initiated by the QNetworkAccessManager finishes.
}
void MainWindow::handleSummaryReady(const QString &summary)
{
    // Create a QTextEdit widget to hold the summary
    QTextEdit *summaryWidget = new QTextEdit;
    summaryWidget->setStyleSheet("font-size:20px;");
    // Set the plain text format to preserve line breaks
    summaryWidget->setPlainText(summary);

    // Make the QTextEdit widget read-only
    summaryWidget->setReadOnly(true);

    // Add the summary widget to the scroll area
    ui->scrollArea->setWidgetResizable(true);
    ui->scrollArea->setWidget(summaryWidget);
}



void MainWindow::searchWikipedia()
{
    QString query = ui->LineEdit->text();
    wikipedia->searchSummary(query);
}

void MainWindow::onButtonClicked()
{
    // Get the sender QLabel from the signal
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());

    if (clickedButton)
    {
        QString url = clickedButton->property("url").toString();

        if (!url.isEmpty())
        {
            QDesktopServices::openUrl(QUrl(url));
        }
    }}


//void MainWindow::onLabelClicked()
//{
//    QLabel* clickedLabel = qobject_cast<QLabel*>(sender());

//    if (clickedLabel) {
//        QString labelText = clickedLabel->text();
//        QClipboard* clipboard = QGuiApplication::clipboard();
//        clipboard->setText(labelText);
//    }
//}


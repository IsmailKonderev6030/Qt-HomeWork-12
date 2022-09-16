#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->centralwidget->setFixedSize(1200,400);

    ui->label->setFixedSize(300,250);
    ui->label_2->setFixedSize(300,250);
    ui->label_3->setFixedSize(300,250);

    connect(&downloadConnectImage, &QNetworkAccessManager::finished, this, &MainWindow::showImage);
    connect(&downloadConnectSite, &QNetworkAccessManager::finished, this, &MainWindow::downloadFinishedSite);
    connect(this,SIGNAL(nextImage()),this,SLOT(downloadFinishedImage()));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString searchText = ui->lineEdit->text();

    QString strLink = "https://yandex.ru/images/search?text="+searchText;

    const QUrl url = QUrl(strLink);

    QNetworkRequest request(url);
    downloadConnectSite.clearConnectionCache();
    downloadConnectSite.clearAccessCache();
    downloadConnectSite.get(request);
}

void MainWindow::downloadFinishedSite(QNetworkReply *reply)
{
    QByteArray site = reply->readAll();

    imageLinks = getImageLinks(site);

    emit nextImage();

}


void MainWindow::showImage(QNetworkReply *reply)
{
    static int line = 0;

    QPixmap pm;
    pm.loadFromData(reply->readAll());

    if(line == 0){
        ui->label->setPixmap(pm.scaledToWidth(ui->label->width()));
        line++;
    }
    else if(line == 1){
        ui->label_2->setPixmap(pm.scaledToWidth(ui->label_2->width()));
        line++;
    }
    else{
        ui->label_3->setPixmap(pm.scaledToWidth(ui->label_3->width()));
        line++;
    }

    if(line == 3)
        line=0;
    else
        emit nextImage();
}


QVector<QString> MainWindow::getImageLinks(QByteArray &site)
{
    QVector<QString> imageLinks;
    QString link("");

    QByteArrayView search("src=\"//avatars.mds.yandex.net");

    int countFined = 0;
    int index = 1000; // begin

    while(countFined!=3){
        index = site.indexOf(search,index);
        if(index == -1) break;
        index+=5;
        link = "https:";
        while(site[index]!='\"')
            link+=site[index++];
        imageLinks.push_back(link);
        countFined++;
    }

    return imageLinks;
}

void MainWindow::downloadFinishedImage(){
    static int index = 0;

    QUrl url = QUrl(imageLinks[index++]);
    QNetworkRequest request(url);
    downloadConnectImage.clearAccessCache();
    downloadConnectImage.clearConnectionCache();
    downloadConnectImage.get(request);

    if(index == 3) index = 0;
}

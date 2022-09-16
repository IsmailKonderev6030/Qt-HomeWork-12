#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QLayout>
#include <QPixmap>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QVector>
#include <QString>
#include <QByteArray>
#include <QByteArrayView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QVector<QString> getImageLinks(QByteArray &site);

private slots:
    void on_pushButton_clicked();

    void downloadFinishedImage();
    void downloadFinishedSite(QNetworkReply *reply);
    void showImage(QNetworkReply *reply);

signals:
  void nextImage();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager downloadConnectSite;
    QNetworkAccessManager downloadConnectImage;

    QVector<QString> imageLinks;
};
#endif // MAINWINDOW_H

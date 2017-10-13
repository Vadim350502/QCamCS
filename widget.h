#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtNetwork>
#include <QComboBox>
#include <QMenuBar>
#include <QList>
#include <QCamera>
#include <QCameraInfo>
#include <QCameraImageCapture>
#include <QHostAddress>
#include <QResizeEvent>

class Widget : public QWidget
{
    Q_OBJECT
    QUdpSocket* m_pudp;
    QMenuBar * menuBar;
    QComboBox * cameraList;
    QList<QCameraInfo> cameras;
    QList<QHostAddress> clients;
    qint16 portNum;

public:
    Widget(QWidget *parent = 0);
    void UpdateAvailableCameras();
    ~Widget();
private:
    void SetApplicationPort(qint16 port);
    void ParseDatagram(QDataStream * datagram, QHostAddress * sender);
protected:
    void resizeEvent(QResizeEvent *event);
private slots:

    void slotProcessDatagrams();
    void slotSendDatagram();
};

#endif // WIDGET_H

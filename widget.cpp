#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("WebCamServer");
    setMinimumSize(720, 480);
    m_pudp = new QUdpSocket(this);
    menuBar = new QMenuBar(this);
    cameraList = new QComboBox(menuBar);
    cameraList->addItem("None");
    UpdateAvailableCameras();



    //QTimer* ptimer = new QTimer(this);
    //ptimer->setInterval(500);
    //ptimer->start();
    //connect(ptimer, SIGNAL(timeout()), SLOT(slotSendDatagram()));
}

void Widget::UpdateAvailableCameras(){
    int i = cameraList->count() - 1;
    for(i;i>0;i--)
        cameraList->removeItem(i);
    cameras = QCameraInfo::availableCameras();
      foreach (const QCameraInfo &cameraInfo, cameras) {
          cameraList->addItem(cameraInfo.deviceName());
      }
}

void Widget::SetApplicationPort(qint16 port){
    clients.clear();
    portNum = port;
}

void Widget::resizeEvent(QResizeEvent *event){
    if(event->size() != event->oldSize()){
        menuBar->setGeometry(0, 0, contentsRect().width(), 25);
    }
    QWidget::resizeEvent(event);
}

void Widget::ParseDatagram(QDataStream * datagram, QHostAddress * sender){
    if(!clients.contains(*sender))
        clients.append(*sender);
}

void Widget::slotProcessDatagrams()
{   QHostAddress sender;
    QByteArray baDatagram;
    do {
        baDatagram.resize(m_pudp->pendingDatagramSize());
        m_pudp->readDatagram(baDatagram.data(), baDatagram.size(), &sender);
    } while(m_pudp->hasPendingDatagrams());
    QDataStream in(&baDatagram, QIODevice::ReadOnly);
    in.setVersion(QDataStream::Qt_5_6);
    ParseDatagram(&in, &sender);
}

void Widget::slotSendDatagram()
{
    QByteArray baDatagram;
    QDataStream out(&baDatagram, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_6);

    //out << dt;
    foreach (const QHostAddress &address, clients) {
        m_pudp->writeDatagram(baDatagram, address, portNum);
    }
}

Widget::~Widget()
{

}

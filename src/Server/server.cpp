#include "server.h"
#include "worker.h"



Server::Server(int port, QObject *parent)
    : QTcpServer(parent)
    , m_port(port)
{
    urls << tr("https://www.youtube.com/embed/skq59BfjkyM")
         << tr("https://www.youtube.com/embed/u-ndajHaih8")
         << tr("https://www.youtube.com/embed/vgmRgE1k8Ek&t=2222s");
}

void Server::start()
{
    if (!this->listen(QHostAddress::Any, m_port)) {
        qDebug() << "Could not start server on port" << m_port;
    } else {
        qDebug() << "Listening on port" << m_port;
    }
}

void Server::incomingConnection(qintptr handle)
{
    qDebug() << handle << " connecting...";
    QString url = urls.at(QRandomGenerator::global()->bounded(urls.size()));

    Worker *worker = new Worker(handle, url);
    connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
    worker->start();
}

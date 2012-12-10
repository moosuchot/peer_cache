#ifndef KADEMLIA_CLIENT_HH
#define KADEMLIA_CLIENT_HH

#include "types.hh"
#include "data_server.hh"
#include "request_manager.hh"

#include <QVariantMap>

class QUdpSocket;

class KademliaClient : public DataServer
{
    Q_OBJECT

    public:
        static QStringList SerializeNodes(QList<QNode> nodes);
        static QList<QNode> DeserializeNodeStrings(QStringList node_strings);

        KademliaClient(QNodeAddress bootstrap_addr);
        ~KademliaClient();

    public slots:
        // Scaffold methods for testing kademlia implementation
        void AddFile(QString filename);
        void SearchForFile(QString name);

    protected slots:
        void ReadPendingDatagrams();
        void ProcessDatagram(QNodeAddress addr, QVariantMap message);
        void SendDatagram(QNodeAddress dest, QVariantMap& message);;

        void ProcessNewRequest(int type, quint32 request_id, QNode dest,
            QKey key);
        void SendRequest(QNodeAddress dest, quint32 request_id,
            QVariantMap message);
        void SendPing(QNodeAddress dest, quint32 request_id);
        void SendStore(QNodeAddress dest, quint32 request_id, QKey key);
        void SendFindNode(QNodeAddress dest, quint32 request_id, QNodeId id);
        void SendFindValue(QNodeAddress dest, quint32 request_id, QKey key);

        void SendReply(QNodeAddress dest, quint32 request_id,
            QVariantMap message);
        void ReplyPing(QNodeAddress dest, quint32 request_id);
        // NOTE: ReplyDownload does not exist. Downloads (i.e. replies to store) are
        // handled by the data manager.
        void ReplyFindNode(QNodeAddress dest, quint32 request_id, QNodeId id);
        void ReplyFindValue(QNodeAddress dest, quint32 request_id, QKey key);

    signals:
        void DatagramReady(QNodeAddress, QVariantMap);
        void RequestReady(QNodeAddress, quint32, QVariantMap);
        void ReplyReady(QNodeAddress, quint32, QVariantMap);

        void ResponseReceived(quint32 request_id,
            QNodeList nodes = QNodeList());

    protected:
        const static quint16 kDefaultPort = 42600;

        QNodeId* node_id_;
        QUdpSocket* udp_socket_;
        RequestManager* request_manager_;
};

#endif // KADEMLIA_CLIENT_HH

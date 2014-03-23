#include <QUdpSocket>
#include <QStringList>
#include <iostream>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "Usage: wol [macAddress...]" << std::endl;
        return 1;
    }

    const QString MacAddressPattern = "^([0-9A-Fa-f]{2}[:-]){5}([0-9A-Fa-f]{2})$";
    const int PacketSize = 102;

    for (int i = 1; i < argc; i++) {
        QString macAddressStr = argv[i];
        QRegExp rx(MacAddressPattern);
        if (rx.indexIn(macAddressStr) == -1) {
            std::cout << "Warning: invalid mac address " << macAddressStr.toStdString() << std::endl;
            continue;
        }

        macAddressStr.replace('-', ':');
        QStringList macBytes = macAddressStr.split(':');

        unsigned char packet[PacketSize];
        unsigned char mac[6];

        for (int i = 0; i < 6; i++) {
            mac[i] = (quint8)macBytes[i].toUInt(0, 16);
            packet[i] = 0xFF;
        }

        for (int i = 1; i <= 16; i++) {
            memcpy(&packet[i * 6], &mac, 6 * sizeof(unsigned char));
        }

        //std::cout << QByteArray(reinterpret_cast<const char*>(packet)).toHex().constData() << std::endl;

        QUdpSocket socket;
        socket.writeDatagram(reinterpret_cast<const char*>(packet), PacketSize, QHostAddress::Broadcast, 40000);
        socket.flush();
    }

    return 0;
}

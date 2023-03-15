#include <QCoreApplication>
#include "../TrackerFiles/asterixreader.h"
#include "../TrackerFiles/protocol.h"

int main(int argc, char *argv[])
{
    ывавыа
    QCoreApplication a(argc, argv);
    if(argc != 2) {
        std::cout << "You should write a pcap-file filepath as argument! Try again." << std::endl;
        throw std::exception();
    }
    AsterixReader asterixReader;
    asterixReader.readPcapFileWithTimeIntervals(argv[1], true);

    return a.exec();
}

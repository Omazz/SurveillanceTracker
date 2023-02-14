#include <QCoreApplication>
#include <includes.h>
#include <asterixreader.h>
#include <protocol.h>
#include <test_asterixreader.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTest::qExec(new Test_AsterixReader);


    if(argc != 2) {
        std::cout << "You should write a pcap-file filepath as argument! Try again." << std::endl;
        throw std::exception();
    }

    AsterixReader asterixReader;

    std::list<Asterix34> res34;
    std::list<Asterix48> res48;
    //asterixReader.readPcapFileWithTimeIntervals("/home/leonid/projects/OverviewTracker/Records/record1.pcapng", &res48, &res34, true);
    asterixReader.readPcapFileWithTimeIntervals(argv[1], &res48, &res34, true);

    return a.exec();
}

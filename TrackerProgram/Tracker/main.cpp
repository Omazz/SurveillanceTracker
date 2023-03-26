#include <QCoreApplication>
#include "../TrackerFiles/asterixreader.h"
#include "../TrackerFiles/protocol.h"
#include "AirplaneHandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SettingsTracker::initialization();


    if(argc == 2) {
        std::cout << "You should write a pcap-file filepath as argument! Try again." << std::endl;
        AsterixReader asterixReader;
        asterixReader.readPcapFileWithTimeIntervals(argv[1], true);
    } else {
         AirplaneHandler ah;
    }

    return a.exec();
}

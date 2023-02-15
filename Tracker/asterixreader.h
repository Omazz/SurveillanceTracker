#ifndef ASTERIXREADER_H
#define ASTERIXREADER_H

#include <stdint.h>
#include <pcap.h>
#include <QtCore>
#include <iostream>
#include <protocol.h>

class AsterixReader
{
public:
    AsterixReader();

    /* На вход подаём путь до файла и две ссылки на листы, которые будут заполнены после отработки функции */
    std::pair<std::list<Asterix48>, std::list<Asterix34>> readPcapFile(const char* filePath, bool print = false);

    std::pair<std::list<Asterix48>, std::list<Asterix34>> readPcapFileWithTimeIntervals(const char* filePath, bool print = false);

    Asterix34 parseAsterix34(const uint8_t* data);

    Asterix48 parseAsterix48(const uint8_t* data);

private:
};

#endif // ASTERIXREADER_H

#ifndef ASTERIXREADER_H
#define ASTERIXREADER_H

#include <includes.h>
#include <protocol.h>

class AsterixReader
{
public:
    AsterixReader();

    /* На вход подаём путь до файла и две ссылки на листы, которые будут заполнены после отработки функции */
    void readPcapFile(const char* filePath,
                      std::list<Asterix48>* recordsAsterix48,
                      std::list<Asterix34>* recordsAsterix34,
                      bool print = false);

    void readPcapFileWithTimeIntervals(const char* filePath,
                                       std::list<Asterix48>* recordsAsterix48,
                                       std::list<Asterix34>* recordsAsterix34,
                                       bool print = false);

    Asterix34 parseAsterix34(const uchar* data);

    Asterix48 parseAsterix48(const uchar* data);

private:
};

#endif // ASTERIXREADER_H

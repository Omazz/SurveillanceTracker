#include "asterixreader.h"

AsterixReader::AsterixReader()
{

}


void AsterixReader::readPcapFile(QString filePath) {
    char error_buffer[PCAP_ERRBUF_SIZE];
    pcap_t *handle = pcap_open_offline(filePath.toLocal8Bit().data(), error_buffer);

    while(true) {
        struct pcap_pkthdr packet_header;
        const u_char* packet = pcap_next(handle, &packet_header);

        if (packet == NULL){
            break;
        }


    }
}

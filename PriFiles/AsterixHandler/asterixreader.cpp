#include "asterixreader.h"

AsterixReader::AsterixReader()
{

}


std::pair<std::list<Asterix48>, std::list<Asterix34>> AsterixReader::readPcapFile(const char* filePath, bool print) {
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records;
    char error_buffer[PCAP_ERRBUF_SIZE] = { 0 };
    char check_buffer[PCAP_ERRBUF_SIZE] = { 0 };

    if(strcmp(error_buffer, check_buffer) != 0) {
        std::cout << "You make a mistake in filepath to pcap-file! Try again." << std::endl;
        throw std::exception();
    }

    pcap_t *handle = pcap_open_offline(filePath, error_buffer);

    while(true) {
        struct pcap_pkthdr packet_header;
        const u_char* packet = pcap_next(handle, &packet_header);

        if (packet == NULL){
            break;
        }

        int pos_asterix = -1;
        uint size = packet_header.caplen;
        for(int i = 3 ; i < size; i++) {
            if((packet[size - i] == i + 2) && (packet[size - i - 2] == CAT48 || packet[size - i - 2] == CAT34)) {
                pos_asterix = size - i - 2;
                break;
            }
        }

        if(pos_asterix == -1) {
            continue;
        }

        if(packet[pos_asterix] == CAT48) {
            Asterix48 record48 = parseAsterix48(packet + pos_asterix);
            records.first.push_back(record48);
            if(print) {
                std::cout << std::hex
                         << "\nRecord Asterix48"
                         << "\nFSPEC:" << record48.FSPEC
                         << "\nDataSourceIdentifier:" << record48.DataSourceIdentifier
                         << "\nTimeOfDay:" << record48.TimeOfDay
                         << "\nMeasuredPositionInSlantPolarCoordinates:" << record48.MeasuredPositionInSlantPolarCoordinates << "\n";
            }
        } else if (packet[pos_asterix] == CAT34) {
            Asterix34 record34 = parseAsterix34(packet + pos_asterix);
            records.second.push_back(record34);
            if(print) {
                std::cout << std::hex
                         << "\nRecord Asterix34"
                         << "\nFSPEC:" << record34.FSPEC
                         << "\nDataSourceIdentifier:" << record34.DataSourceIdentifier
                         << "\nTimeOfDay:" << record34.TimeOfDay
                         << "\nMessageType:" << record34.MessageType << "\n";
            }
        }
    }

    return records;
}


std::pair<std::list<Asterix48>, std::list<Asterix34>> AsterixReader::readPcapFileWithTimeIntervals(const char* filePath, bool print) {
    std::pair<std::list<Asterix48>, std::list<Asterix34>> records;
    char error_buffer[PCAP_ERRBUF_SIZE] = { 0 };
    char check_buffer[PCAP_ERRBUF_SIZE] = { 0 };

    pcap_t *handle = pcap_open_offline(filePath, error_buffer);

    if(strcmp(error_buffer, check_buffer) != 0) {
        std::cout << "You make a mistake in filepath to pcap-file! Try again." << std::endl;
        throw std::exception();
    }

    qint64 startSec;
    qint64 startMSec;
    QTime startTime;
    bool firstRecord = true;

    while(true) {
        struct pcap_pkthdr packet_header;
        const u_char* packet = pcap_next(handle, &packet_header);

        if(firstRecord) {
            firstRecord = false;
            startSec = packet_header.ts.tv_sec;
            startMSec = packet_header.ts.tv_usec / 1000;
            startTime = QTime::currentTime();
        }

        qint64 newSec = packet_header.ts.tv_sec;
        qint64 newMSec = packet_header.ts.tv_usec / 1000;
        qint64 dSec = newSec - startSec;
        qint64 dMSec = newMSec - startMSec;
        QTime newTime = startTime.addSecs(dSec).addMSecs(dMSec);

        while (QTime::currentTime() < newTime) {
            qApp->processEvents();
        }


        if (packet == NULL) {
            break;
        }

        int pos_asterix = -1;
        uint size = packet_header.caplen;
        for(int i = 3 ; i < size; i++) {
            if((packet[size - i] == i + 2) && (packet[size - i - 2] == CAT48 || packet[size - i - 2] == CAT34)) {
                pos_asterix = size - i - 2;
                break;
            }
        }

        if(pos_asterix == -1) {
            continue;
        }

        if(packet[pos_asterix] == CAT48) {
            Asterix48 record48 = parseAsterix48(packet + pos_asterix);
            records.first.push_back(record48);
            if(print) {
                std::cout << std::hex
                         << "\nRecord Asterix48"
                         << "\nFSPEC:" << record48.FSPEC
                         << "\nDataSourceIdentifier:" << record48.DataSourceIdentifier
                         << "\nTimeOfDay:" << record48.TimeOfDay
                         << "\nMeasuredPositionInSlantPolarCoordinates:" << record48.MeasuredPositionInSlantPolarCoordinates << "\n";
            }
        } else if (packet[pos_asterix] == CAT34) {
            Asterix34 record34 = parseAsterix34(packet + pos_asterix);
            records.second.push_back(record34);
            if(print) {
                std::cout << std::hex
                         << "\nRecord Asterix34"
                         << "\nFSPEC:" << record34.FSPEC
                         << "\nDataSourceIdentifier:" << record34.DataSourceIdentifier
                         << "\nTimeOfDay:" << record34.TimeOfDay
                         << "\nMessageType:" << record34.MessageType << "\n";
            }
        }
    }

    return records;
}


Asterix34 AsterixReader::parseAsterix34(const uint8_t* data) {
    Asterix34 result;

    result.LEN = (data[1] << 8) | data[2];
    uint16_t position = 3;

    if ((data[position] & 0x1) > 0) {
        result.FSPEC = data[position] | (data[position + 1] << 8);
        position += 2;
    } else {
        result.FSPEC = data[position];
        position++;
    }

    if(result.header34.DataSourceIdentifier_isExist) {
        result.DataSourceIdentifier = (data[position] << 8) | data[position + 1];
        position += 2;
    }

    if(result.header34.MessageType_isExist) {
        result.MessageType = data[position];
        position++;
    }

    if(result.header34.TimeOfDay_isExist) {
        result.TimeOfDay = (data[position] << 16) | (data[position + 1] << 8) | data[position + 2];
        position += 3;
    }

    if(result.header34.SectorNumber_isExist) {
        result.SectorNumber = data[position];
        position++;
    }

    if(result.header34.AntennaRotationPeriod_isExist) {
        result.AntennaRotationPeriod = (data[position] << 8) | data[position + 1];
        position += 2;
    }

    if(result.header34.SystemConfigurationAndStatus_isExist) {
        result.SystemConfigurationAndStatus.PrimarySubfield = data[position];
        position++;
        if((result.SystemConfigurationAndStatus.PrimarySubfield & 0b10000000) > 0) {
            result.SystemConfigurationAndStatus.COM = data[position];
            position++;
        }
        if ((result.SystemConfigurationAndStatus.PrimarySubfield & 0b00010000) > 0) {
            result.SystemConfigurationAndStatus.PSR = data[position];
            position++;
        }
        if ((result.SystemConfigurationAndStatus.PrimarySubfield & 0b00001000) > 0) {
            result.SystemConfigurationAndStatus.SSR = data[position];
            position++;
        }
        if ((result.SystemConfigurationAndStatus.PrimarySubfield & 0b00000100) > 0) {
            result.SystemConfigurationAndStatus.MDS = (data[position] << 8) |data[position + 1];
            position += 2;
        }
    }

    if(result.header34.SystemProcessingMode_isExist) {
        result.SystemProcessingMode.PrimarySubfield = data[position];
        position++;
        if ((result.SystemProcessingMode.PrimarySubfield & 0b10000000) > 0) {
            result.SystemProcessingMode.COM = data[position];
            position++;
        }
        if ((result.SystemProcessingMode.PrimarySubfield & 0b00010000) > 0) {
            result.SystemProcessingMode.PSR = data[position];
            position++;
        }
        if ((result.SystemProcessingMode.PrimarySubfield & 0b00001000) > 0) {
            result.SystemProcessingMode.SSR = data[position];
            position++;
        }
        if ((result.SystemProcessingMode.PrimarySubfield & 0b00000100) > 0) {
            result.SystemProcessingMode.MDS = data[position];
            position++;
        }
    }

    if(result.header34.Block1_End) { // FX
        if(result.header34.TYP_COUNTERs_isExist) {
            result.REP = data[position];
            position++;
            result.TYP_COUNTERs.resize(result.REP);
            for(uint16_t counter = 0; counter < result.REP; ++counter) {
                result.TYP_COUNTERs[counter] = (data[position] << 8) | data[position + 1];
                position += 2;
            }
        }

        if(result.header34.GenericPolarWindow_isExist) {
            result.GenericPolarWindow = (data[position] << 24) | (data[position + 1] << 16)
                                        | (data[position + 2] << 8) | data[position + 3];
            position += 4;
        }

        if(result.header34.DataFilter_isExist) {
            result.DataFilter = data[position];
            position++;
        }

        if(result.header34.PositionOfDataSource_isExist) {
            result.PositionOfDataSource = (data[position] << 24) | (data[position + 1] << 16)
                                          | (data[position + 2] << 8) | data[position + 3];
            position += 4;
        }

        if(result.header34.CollimationError_isExist) {
            result.CollimationError = (data[position] << 8) | data[position + 1];
            position += 2;
        }

        if(result.header34.ReservedExpansionField_isExist) {
            result.ReservedExpansionField = data[position];
            position++;
        }

        if(result.header34.SpecialPurposeField_isExist) {
            result.SpecialPurposeField = data[position];
            position++;
        }
    }

    return result;
}

Asterix48 AsterixReader::parseAsterix48(const uint8_t* data) {
    Asterix48 result;
    result.LEN = (data[1] << 8) | data[2];
    uint16_t position = 3;

    if((data[position] & 0x1) == 0) { // если 1 байт
        result.FSPEC = data[position];
        position++;
    } else if ((data[position + 1] & 0x1) == 0) { // если 2 байта
        result.FSPEC = data[position] | (data[position + 1] << 8);
        position += 2;
    } else if ((data[position + 2] & 0x1) == 0) { // если 3 байта
        result.FSPEC = data[position] | (data[position + 1] << 8) | (data[position + 2] << 16);
        position += 3;
    } else { // если 4 байта
        result.FSPEC = data[position] | (data[position + 1] << 8) | (data[position + 2] << 16) | (data[position + 3] << 24);
        position += 4;
    }

    if(result.header48.DataSourceIdentifier_isExist) {
        result.DataSourceIdentifier = (data[position] << 8) | data[position + 1];
        position += 2;
    }

    if(result.header48.TimeOfDay_isExist) {
        result.TimeOfDay = (data[position] << 16) | (data[position + 1] << 8) | data[position + 2];
        position += 3;
    }

    if(result.header48.TargetReportDescriptor_isExist) {
        while ((data[position] & 0x1) != 0) {
            result.TargetReportDescriptor.push_back(data[position]);
            position++;
        }
        result.TargetReportDescriptor.push_back(data[position]);
        position++;
    }

    if(result.header48.MeasuredPosition_isExist) {
        result.MeasuredPositionInSlantPolarCoordinates = (data[position] << 24) | (data[position + 1] << 16)
                                                         | (data[position + 2] << 8) | data[position + 3];
        position += 4;
    }

    if(result.header48.Mode3ACode_isExist) {
        result.Mode3ACodeInOctalRepresentation = (data[position] << 8) | data[position + 1];
        position += 2;
    }

    if(result.header48.FlightLevel_isExist) {
        result.FlightLevelInBinaryRepresentation = (data[position] << 8) | data[position + 1];
        position += 2;
    }

    if(result.header48.RadarPlotCharacteristics_isExist) {
        while((data[position] & 0x1) != 0) {
            RadarPlotCharacteristicsClass curr;
            curr.PrimarySubfield = data[position];
            position++;
            uint8_t maskSub = 0b10000000;
            int shiftSub = -1;
            if((curr.PrimarySubfield & (maskSub << (++shiftSub))) > 0) {
                curr.SRL = data[position];
                position++;
            }
            if((curr.PrimarySubfield & (maskSub << (++shiftSub))) > 0) {
                curr.SRR = data[position];
                position++;
            }
            if((curr.PrimarySubfield & (maskSub << (++shiftSub))) > 0) {
                curr.SAM = data[position];
                position++;
            }
            if((curr.PrimarySubfield & (maskSub << (++shiftSub))) > 0) {
                curr.PRL = data[position];
                position++;
            }
            if((curr.PrimarySubfield & (maskSub << (++shiftSub))) > 0) {
                curr.PAM = data[position];
                position++;
            }
            if((curr.PrimarySubfield & (maskSub << (++shiftSub))) > 0) {
                curr.RPD = data[position];
                position++;
            }

            if((curr.PrimarySubfield & (maskSub << (++shiftSub))) > 0) {
                curr.APD = data[position];
                position++;
            }
            result.RadarPlotCharacteristics.push_back(curr);
        }

        RadarPlotCharacteristicsClass curr;
        curr.PrimarySubfield = data[position];
        position++;
        uint8_t maskSub = 0b10000000;
        int shiftSub = -1;

        if((curr.PrimarySubfield & (maskSub << (++shiftSub))) > 0) {
            curr.SRL = data[position];
            position++;
        }
        if((curr.PrimarySubfield & (maskSub >> (++shiftSub))) > 0) {
            curr.SRR = data[position];
            position++;
        }
        if((curr.PrimarySubfield & (maskSub >> (++shiftSub))) > 0) {
            curr.SAM = data[position];
            position++;
        }
        if((curr.PrimarySubfield & (maskSub >> (++shiftSub))) > 0) {
            curr.PRL = data[position];
            position++;
        }
        if((curr.PrimarySubfield & (maskSub >> (++shiftSub))) > 0) {
            curr.PAM = data[position];
            position++;
        }
        if((curr.PrimarySubfield & (maskSub >> (++shiftSub))) > 0) {
            curr.RPD = data[position];
            position++;
        }
        if((curr.PrimarySubfield & (maskSub >> (++shiftSub))) > 0) {
            curr.APD = data[position];
            position++;
        }

        result.RadarPlotCharacteristics.push_back(curr);
    }

    if(result.header48.FX1) {
        if(result.header48.AircraftAddress_isExist) {
            result.AircraftAddress = (data[position] << 16) | (data[position + 1] << 8) | data[position + 2];
            position += 3;
        }

        if(result.header48.AircraftIdentification_isExist) {
            uint64_t first = (data[position] << 16) | (data[position + 1] << 8) | (data[position + 2]);
            first = (first << 24) & 0x00FFFFFFFF000000;
            uint32_t second = (data[position + 3] << 16) | (data[position + 4] << 8) | (data[position + 5]);
            result.AircraftIndentification = first | second;
            position += 6;
        }

        if(result.header48.ModeSdata_isExist) {
            result.BDSRegisterDataREP = data[position];
            position++;
            result.BDSRegisterData.resize(result.BDSRegisterDataREP);
            for (uint16_t counter = 0; counter < result.BDSRegisterData.size(); ++counter) {
                result.BDSRegisterData[counter] = (((uint64_t)data[position] << 56) | ((uint64_t)data[position + 1] << 48)
                        | ((uint64_t)data[position + 2] << 40) | ((uint64_t)data[position + 3] << 32)
                        | (data[position + 4] << 24) | (data[position + 5] << 16) | (data[position + 6] << 8)
                        | data[position + 7]);
                position += 8;
            }
        }

        result.PosTrackNumber = position;
        if(result.header48.TrackNumber_isExist) {
            result.TrackNumber = (data[position] << 8) | data[position + 1];
            position += 2;
        }

        if(result.header48.CalculatedPositionInCartesianCoords_isExist) {
            result.CalculatedPositionInCartesianCoordiantes = (data[position] << 24) | (data[position + 1] << 16) | (data[position + 2] << 8) | data[position + 3];
            position += 4;
        }

        if(result.header48.CalculatedTrackVelocityInPolarRepresentation_isExist) {
            result.CalculatedTrackVelocityInPolarRepresentation = (data[position] << 24) | (data[position + 1] << 16) | (data[position + 2] << 8) | data[position + 3];
            position += 4;
        }

        if(result.header48.TrackStatus_isExist) {
            while((data[position] & 0x1) != 0) {
                result.TrackStatus.push_back(data[position]);
                position++;
            }
            result.TrackStatus.push_back(data[position]);
            position++;
        }

        if(result.header48.FX2) {
            if(result.header48.TrackQuality_isExist) {
                result.TrackQuality = (data[position] << 24) | (data[position + 1] << 16) | (data[position + 2] << 8) | data[position + 3];
                position += 4;
            }

            if(result.header48.WarningConditionsAndTargetClassification_isExist) {
                while((data[position] & 0x1) != 0) {
                    result.WarningConditionsAndTargetClassification.push_back(data[position]);
                    position++;
                }
                result.WarningConditionsAndTargetClassification.push_back(data[position]);
                position++;
            }

            if(result.header48.Mode3ACodeConfidenceIndicator_isExist) {
                result.Mode3ACodeConfidenceIndicator = (data[position] << 8) | data[position + 1];
                position += 2;
            }

            if(result.header48.ModeCCodeAndConfidenceIndicator_isExist) {
                result.ModeCCodeAndConfidenceIndicator = (data[position] << 24) | (data[position + 1] << 16) | (data[position + 2] << 8) | data[position + 3];
                position += 4;
            }

            if(result.header48.HeightMeasuredBy3dRadar_isExist) {
                result.HeightMeasuredBy3dRadar = (data[position] << 8) | data[position + 1];
                position += 2;
            }

            if(result.header48.RadialDopplerSpeed_isExist) {
                result.RadialDopplerSpeed.PrimarySubfield = data[position];
                position++;

                if((result.RadialDopplerSpeed.PrimarySubfield & 0b10000000) > 0) {
                    result.RadialDopplerSpeed.CalculatedDopplerSpeed = (data[position] << 8) | data[position + 1];
                    position += 2;
                }

                if((result.RadialDopplerSpeed.PrimarySubfield & 0b01000000) > 0) {
                    result.RadialDopplerSpeed.REP = data[position];
                    position++;
                    result.RadialDopplerSpeed.RawDopplerSpeed.resize(result.RadialDopplerSpeed.REP);
                    for(uint16_t counter = 0; counter < result.RadialDopplerSpeed.RawDopplerSpeed.size(); ++counter) {
                        result.RadialDopplerSpeed.RawDopplerSpeed[counter] = ((data[position] << 40) | (data[position + 1] << 32)
                                                                        | (data[position + 2] << 24) | (data[position + 3] << 16)
                                                                        | (data[position + 4] << 8) | data[position + 5]);
                        position += 6;
                    }
                }
            }

            if(result.header48.FlightStatus_isExist) {
                result.FlightStatus = (data[position] << 8) | data[position + 1];
                position += 2;
            }

            if(result.header48.FX3) {
                result.ACASResolutionAdvisoryReport = 0;
                if(result.header48.ACASResolutionAdvisoryReport_isExist) {
                    result.ACASResolutionAdvisoryReport = (((uint64_t)data[position] << 48) | ((uint64_t)data[position + 1] << 40)
                                                              | ((uint64_t)data[position + 2] << 32) | (data[position + 3] << 24)
                                                  | (data[position + 4] << 16) | (data[position + 5] << 8) | data[position + 6]);
                    position += 7;
                }

                if(result.header48.Mode1CodeInOctalRepresentation_isExist) {
                    result.Mode1CodeInOctalRepresentation = data[position];
                    position++;
                }

                if(result.header48.Mode2CodeInOctalRepresentation_isExist) {
                    result.Mode2CodeInOctalRepresentation = (data[position] << 8) | data[position + 1];
                    position += 2;
                }

                if(result.header48.Mode1CodeConfidenceIndicator_isExist) {
                    result.Mode1CodeConfidenceIndicator = data[position];
                    position++;
                }

                if(result.header48.Mode2CodeConfidenceIndicator_isExist) {
                    result.Mode2CodeConfidenceIndicator = (data[position] << 8) | data[position + 1];
                    position += 2;
                }

                if(result.header48.ReservedExpansionField_isExist) {
                    result.ReservedExpansionField = data[position];
                    position++;
                }

                if(result.header48.SpecialPurposeField_isExist) {
                    result.SpecialPurposeField = data[position];
                    position++;
                }
            }
        }
    }

    result.Data = QByteArray((const char*)(data), result.LEN);
    return result;
}

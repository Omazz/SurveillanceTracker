#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>
#include <list>
#include <vector>
#include <QByteArray>

const static uint8_t OK = 200;
const static uint8_t ERROR = 220;

const static uint8_t CAT48 = 0x30;
const static uint8_t CAT34 = 0x22;

/*  Asterix34 --  протокол для передачи служебных сообщений монорадара,
 *  таких как SectorCrossed, NORTH MARKER и других */

struct SystemConfigurationAndStatusClass {
    uint8_t PrimarySubfield = 0;
    uint8_t COM = 0;
    uint8_t PSR = 0;
    uint8_t SSR = 0;
    uint16_t MDS = 0;
};

struct SystemProcessingModeClass {
    uint8_t PrimarySubfield = 0;
    uint8_t COM = 0;
    uint8_t PSR = 0;
    uint8_t SSR = 0;
    uint8_t MDS = 0;
};

struct Asterix34 {
    uint16_t FSPEC = 0;
    uint16_t LEN = 0;
    uint16_t DataSourceIdentifier = 0;
    uint8_t MessageType = 0;
    uint32_t TimeOfDay = 0;
    uint8_t SectorNumber = 0;
    uint16_t AntennaRotationPeriod = 0;
    SystemConfigurationAndStatusClass SystemConfigurationAndStatus;
    SystemProcessingModeClass SystemProcessingMode;
    uint8_t REP = 0;
    std::vector<uint16_t> TYP_COUNTERs;
    uint64_t GenericPolarWindow = 0;
    uint8_t DataFilter = 0;
    uint64_t PositionOfDataSource = 0;
    uint16_t CollimationError = 0;
    uint8_t ReservedExpansionField = 0;
    uint8_t SpecialPurposeField = 0;
};


/*  Asterix48 --  протокол для передачи служебных сообщений монорадара,
 *  таких как SectorCrossed, NORTH MARKER и других */


struct RadarPlotCharacteristicsClass {
    uint8_t PrimarySubfield = 0;
    uint8_t SRL = 0;
    uint8_t SRR = 0;
    uint8_t SAM = 0;
    uint8_t PRL = 0;
    uint8_t PAM = 0;
    uint8_t RPD = 0;
    uint8_t APD = 0;
};

struct RadialDopplerSpeedClass {
    uint8_t PrimarySubfield = 0;
    uint16_t CalculatedDopplerSpeed = 0;
    uint8_t REP = 0;
    std::vector<uint64_t> RawDopplerSpeed;
};

struct Asterix48 {
    uint32_t FSPEC = 0;
    uint16_t LEN = 0;
    uint16_t DataSourceIdentifier = 0;
    std::list<uint8_t> TargetReportDescriptor; // 1+
    uint32_t TimeOfDay = 0; // 24 бита
    uint32_t MeasuredPositionInSlantPolarCoordinates = 0;
    bool polarCoordsIsExist = false;
    uint16_t Mode3ACodeInOctalRepresentation = 0;
    uint16_t FlightLevelInBinaryRepresentation = 0;
    std::list<RadarPlotCharacteristicsClass> RadarPlotCharacteristics; // 1+1+
    uint32_t AircraftAddress = 0; //24
    uint64_t AircraftIndentification = 0; //48
    uint8_t BDSRegisterDataREP = 0;
    std::vector<uint64_t> BDSRegisterData;
    uint16_t TrackNumber = 0;
    uint32_t CalculatedPositionInCartesianCoordiantes = 0;
    bool cartesianCoordsIsExist = false;
    uint32_t CalculatedTrackVelocityInPolarRepresentation = 0;
    std::list<uint8_t> TrackStatus;
    uint32_t TrackQuality = 0;
    std::list<uint8_t> WarningConditionsAndTargetClassification;
    uint16_t Mode3ACodeConfidenceIndicator = 0;
    uint32_t ModeCCodeAndConfidenceIndicator = 0;
    uint16_t HeightMeasuredBy3dRadar = 0;
    RadialDopplerSpeedClass RadialDopplerSpeed;
    uint16_t FlightStatus = 0; // Communication / ACAS Capability and Flight Status
    uint64_t ACASResolutionAdvisoryReport = 0;
    uint8_t Mode1CodeInOctalRepresentation = 0;
    uint16_t Mode2CodeInOctalRepresentation = 0;
    uint8_t Mode1CodeConfidenceIndicator = 0;
    uint16_t Mode2CodeConfidenceIndicator = 0;
    uint8_t ReservedExpansionField = 0;
    uint8_t SpecialPurposeField = 0;

    QByteArray Data;
    int PosTrackNumber = -1;
};

#endif // PROTOCOL_H

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

#pragma pack(push, 1)

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

struct Header34 {    
    bool Block1_End                             : 1;
    bool SystemProcessingMode_isExist           : 1;
    bool SystemConfigurationAndStatus_isExist   : 1;
    bool AntennaRotationPeriod_isExist          : 1;
    bool SectorNumber_isExist                   : 1;
    bool TimeOfDay_isExist                      : 1;
    bool MessageType_isExist                    : 1;
    bool DataSourceIdentifier_isExist           : 1;
    bool Block2_End                             : 1;
    bool SpecialPurposeField_isExist            : 1;
    bool ReservedExpansionField_isExist         : 1;
    bool CollimationError_isExist               : 1;
    bool PositionOfDataSource_isExist           : 1;
    bool DataFilter_isExist                     : 1;
    bool GenericPolarWindow_isExist             : 1;
    bool TYP_COUNTERs_isExist                   : 1;
};

struct Asterix34 {
    union {
        uint16_t FSPEC = 0;
        Header34 header34;
    };
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

struct Header48 {
    bool FX1                                                    : 1;
    bool RadarPlotCharacteristics_isExist                       : 1;
    bool FlightLevel_isExist                                    : 1;
    bool Mode3ACode_isExist                                     : 1;
    bool MeasuredPosition_isExist                               : 1;
    bool TimeOfDay_isExist                                      : 1;
    bool TargetReportDescriptor_isExist                         : 1;
    bool DataSourceIdentifier_isExist                           : 1;
    bool FX2                                                    : 1;
    bool TrackStatus_isExist                                    : 1;
    bool CalculatedTrackVelocityInPolarRepresentation_isExist   : 1;
    bool CalculatedPositionInCartesianCoords_isExist            : 1;
    bool TrackNumber_isExist                                    : 1;
    bool ModeSdata_isExist                                      : 1;
    bool AircraftIdentification_isExist                         : 1;
    bool AircraftAddress_isExist                                : 1;
    bool FX3                                                    : 1;
    bool FlightStatus_isExist                                   : 1;
    bool RadialDopplerSpeed_isExist                             : 1;
    bool HeightMeasuredBy3dRadar_isExist                        : 1;
    bool ModeCCodeAndConfidenceIndicator_isExist                : 1;
    bool Mode3ACodeConfidenceIndicator_isExist                  : 1;
    bool WarningConditionsAndTargetClassification_isExist       : 1;
    bool TrackQuality_isExist                                   : 1;
    bool FX4                                                    : 1;
    bool ReservedExpansionField_isExist                         : 1;
    bool SpecialPurposeField_isExist                            : 1;
    bool Mode2CodeConfidenceIndicator_isExist                   : 1;
    bool Mode1CodeConfidenceIndicator_isExist                   : 1;
    bool Mode2CodeInOctalRepresentation_isExist                 : 1;
    bool Mode1CodeInOctalRepresentation_isExist                 : 1;
    bool ACASResolutionAdvisoryReport_isExist                   : 1;
};

struct Asterix48 {
    union {
        uint32_t FSPEC = 0;
        Header48 header48;
    };
    uint16_t LEN = 0;
    uint16_t DataSourceIdentifier = 0;
    std::list<uint8_t> TargetReportDescriptor; // 1+
    uint32_t TimeOfDay = 0; // 24 бита
    uint32_t MeasuredPositionInSlantPolarCoordinates = 0;
    uint16_t Mode3ACodeInOctalRepresentation = 0;
    uint16_t FlightLevelInBinaryRepresentation = 0;
    std::list<RadarPlotCharacteristicsClass> RadarPlotCharacteristics; // 1+1+
    uint32_t AircraftAddress = 0; //24
    uint64_t AircraftIndentification = 0; //48
    uint8_t BDSRegisterDataREP = 0;
    std::vector<uint64_t> BDSRegisterData;
    uint16_t TrackNumber = 0;
    uint32_t CalculatedPositionInCartesianCoordiantes = 0;
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
    uint8_t SpecialPurposeField = 0;
    uint8_t ReservedExpansionField = 0;

    QByteArray Data;
    int PosTrackNumber = -1;
};

#pragma pack(pop)


#endif // PROTOCOL_H

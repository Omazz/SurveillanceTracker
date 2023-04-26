#include "RadarWindow.h"
#include "ui_RadarWindow.h"
#include <QRegExpValidator>

RadarWindow::RadarWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RadarWindow)
{
    ui->setupUi(this);

    _udpSocket = new QUdpSocket(this);

    setFixedSize(width(), height());
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->LE_ipAddress->setValidator(ipValidator);
}

RadarWindow::~RadarWindow()
{
    delete ui;
}


void RadarWindow::on_PB_start_clicked() {
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open pcap"), "../../Records", tr("pcap (*.pcapng)"));

    if(filePath.isEmpty()) {
        return;
    }

    QHostAddress address = QHostAddress(ui->LE_ipAddress->text());
    int port = ui->LE_port->text().toInt();


    ui->TB_log->clear();
    ui->PB_start->setEnabled(false);
    ui->TB_log->append("Start sending records!");

    char error_buffer[PCAP_ERRBUF_SIZE] = { 0 };

    pcap_t *handle = pcap_open_offline(filePath.toStdString().c_str(), error_buffer);
    int totalSize = 0;
    while(true) {
        struct pcap_pkthdr packet_header;
        const u_char* packet = pcap_next(handle, &packet_header);

        if(packet == NULL) {
            break;
        }
        totalSize++;
    }

    handle = pcap_open_offline(filePath.toStdString().c_str(), error_buffer);
    qint64 startSec;
    qint64 startMSec;
    QTime startTime;
    bool firstRecord = true;
    int counter = 0;
    while(_isWorking) {
        struct pcap_pkthdr packet_header;
        const u_char* packet = pcap_next(handle, &packet_header);
        counter++;

        ui->PB_progressBar->setValue((qreal) (counter / (qreal) totalSize) * 100.0);
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


        if (packet == NULL){
            break;
        }

        QString stringToPrint;
        int pos_asterix = -1;
        uint size = packet_header.caplen;
        for(int i = 3 ; i < size; i++) {
            if((packet[size - i] == i + 2) && (packet[size - i - 2] == 48 || packet[size - i - 2] == 34)) {
                pos_asterix = size - i - 2;
                break;
            }
        }

        if(pos_asterix == -1) {
            continue;
        }
        stringToPrint.push_back(QString::number(counter).append(") Record: "));
        stringToPrint.push_back(QString("0x%1 ").arg(packet[pos_asterix]));

        if(_udpSocket->writeDatagram((const char*) (packet + pos_asterix), packet_header.caplen - pos_asterix, address, port)) {
            ui->TB_log->append(stringToPrint);

        } else {
            ui->TB_log->append("Error to send message!");
        }
    }

    ui->PB_progressBar->setValue(100);
    ui->TB_log->append("End of pcap-file!");
    ui->PB_start->setEnabled(true);
    return;
}

void RadarWindow::closeEvent(QCloseEvent* event)
{
    Q_UNUSED(event)
    //_udpSocket->close();
    _isWorking = false;
}

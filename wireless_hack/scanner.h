#ifndef SCANNER_H
#define SCANNER_H

#define BUFSIZE 65535
#define IEEE80211_SSID 0x0

#include <pcap.h>
#include <iostream>
#include <string.h>
#include <QThread>
#include <time.h>
#include <cstddef>

#include <QWidget>
#include <QTreeWidgetItem>
#include <QList>
#include <QDebug>
#include <QTimer>
#include <QProcess>
#include <QFileDialog>
#include <QObject>
#include <QString>
using namespace std;

#pragma pack(push, 1)
typedef struct MyClass {
    u_char src[18];
    u_char dest[18];
    u_char bss[18];
    char timestamp[100];
    char ssid[256];
    u_int enc;
} MC ;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct beacon_frame {
    u_int16_t frame_control;
    u_int16_t duration;
    u_char dest[6];
    u_char src[6];
    u_char bss[6];
    u_int16_t seq_control;
} BF_header ;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct ieee80211_wlan_management {
    u_int64_t timestamp;
    u_int16_t beacon_interval;
    u_int16_t capabilities_information;
} IEEE80211 ;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct AP_Info {
    QString AP;
    u_int8_t STA_Count;
    u_int8_t Channel;
    QString ENC;
    u_int Datas;
    int Signal;
    QString BSSID;
} Info ;
#pragma pack(pop)

enum scannerColumns
{
    SCANNER_COLUMN_AP,
    SCANNER_COLUMN_STA_COUNT,
    SCANNER_COLUMN_CHANNEL,
    SCANNER_COLUMN_ENCRYPT,
    SCANNER_COLUMN_DATAS,
    SCANNER_COLUMN_BSSID
};

class Scanner : public QObject
{
    Q_OBJECT
private:
    MC *myclass;
    IEEE80211 *ieee;
    BF_header *bf_header;
    pcap_pkthdr *pkthdr;
    const u_char *data;
    char errBuf[256];
    char dev[256];
    pcap_t *handle;

public:
    explicit Scanner(QObject *parent = 0);

    ~Scanner();         // Destructor

    void startScan();
    void stopScan();
signals:
    void captured(Info);

public slots:
    void doStart();
};


class ChangeChannel : public QThread {
protected:
    virtual void run(){
        char cmd[30] = {0};
        int i=1;
        while(!isFinished()){
            sprintf(cmd, "iwconfig wlan0 channel %d", i);
            sleep(2);
            system(cmd);
            printf("[*][*] Channel Changed To : < %2d >\n", i++);
            if(i == 14) i = 1;
        }
    }
};

#endif // SCANNER_H

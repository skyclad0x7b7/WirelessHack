#ifndef SCANNER_H
#define SCANNER_H

#define BUFSIZE 65535
#define IEEE80211_SSID 0x0

#include <pcap.h>
#include <iostream>
#include <string.h>
#include <QThread>
#include <time.h>

#include <QWidget>
#include <QTreeWidgetItem>
#include <QThread>
#include <QList>
#include <QDebug>
#include <QTimer>
#include <QProcess>
#include <QFileDialog>
#include <QObject>

using namespace std;

#pragma pack(push, 1)
typedef struct MyClass {
    u_char src[18];
    u_char dest[18];
    u_char bss[18];
    char timestamp[100];

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
    void captured();

public slots:
    void doStart();
};

#endif // SCANNER_H

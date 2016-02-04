#include "scanner.h"


Scanner::Scanner(QObject *parent) : QObject(parent)
{
    myclass = new MC();
    ieee = new IEEE80211;
    bf_header = new BF_header;
    strcpy(this->dev, "wlan1");
    this->handle = pcap_open_live(this->dev, BUFSIZE, 1, 10, this->errBuf);
    if(handle == NULL){
        fprintf(stderr, "Couldn't open device %s : %s\n", "wlan0", this->dev, errBuf);
        printf("%s\n", this->dev);
        exit(1);
    }
    cout << "[*] Scanner Created" << endl;
}


/*Scanner::Scanner(char *device)
{
    myclass = new MC();
    ieee = new IEEE80211;
    bf_header = new BF_header;
    strcpy(this->dev, device);
    this->handle = pcap_open_live(this->dev, BUFSIZE, 1, 10, this->errBuf);
    if(handle == NULL){
        fprintf(stderr, "Couldn't open device %s : %s\n", "wlan0", this->dev, errBuf);
        printf("%s\n", this->dev);
        exit(1);
    }
    cout << "[*] Scanner Created" << endl;
}*/

Scanner::~Scanner()
{
    delete myclass;
    delete ieee;
    delete bf_header;
}

void Scanner::startScan()
{
    cout << "Start" << endl;
}

void Scanner::stopScan()
{

}

void Scanner::doStart()
{
    while(true)
    {
        Info info;
        int res = pcap_next_ex(handle, &pkthdr, &data);
        if (res == 0) continue;
        else if (res < 0) break;
        data += *(u_int16_t *)(data+2);          // Skip Radiotap Header
        memcpy(bf_header, data, 0x18);           // Beacon Frame Header
        data += 0x18;
        memcpy(ieee, data, 12);                  // Fixed 802.11 Header
        data += 12;
        int len2 = pkthdr->caplen - (18+0x18+12);
        const u_char *data2 = data;
        myclass->enc = 0;

        while((data2 - data) < len2)
        {
            if(data2[0] == IEEE80211_SSID){
                strcpy(myclass->ssid, (const char *)data2+2);
                myclass->ssid[data2[1]] = 0;
                if(strlen(myclass->ssid) == 0) {
                    strcpy(myclass->ssid, "Broadcast");
                }
                if(data2[1] == 0) data2 += 2;
                else data2 += data2[1];
                continue;
            }
            if(data2[0] == 48) { // wpa2
                myclass->enc += 2;
                data2 += data2[1]+2;
                continue;
            }
            if(data2[0] == (char)221) {
                int j=0;
                for(j=0; j<data2[1]; j++) if(!memcmp(data2+2+j, "\x00\x50\xf2\x01\x01\x00", 6)) {
                        myclass->enc += 1;
                        break;
                    }
                data2 += data2[1] + 2;
                continue;
            }
            data2 += data2[1] + 2;
        }

        switch(myclass->enc)
        {
        case 0:
            info.ENC = QString("None");
            break;
        case 1:
            info.ENC = QString("WPA");
            break;
        case 2:
            info.ENC = QString("WPA2");
            break;
        case 3:
            info.ENC = QString("WPA/WPA2");
            break;
        default:
            info.ENC = QString("????");
            break;
        }

        sprintf((char *)myclass->bss, "%02X:%02X:%02X:%02X:%02X:%02X", bf_header->bss[0], bf_header->bss[1], bf_header->bss[2], bf_header->bss[3], bf_header->bss[4], bf_header->bss[5]);
        //sprintf((char *)myclass->dest, "%02X:%02X:%02X:%02X:%02X:%02X", bf_header->dest[0], bf_header->dest[1], bf_header->dest[2], bf_header->dest[3], bf_header->dest[4], bf_header->dest[5]);
        //sprintf((char *)myclass->src, "%02X:%02X:%02X:%02X:%02X:%02X", bf_header->src[0], bf_header->src[1], bf_header->src[2], bf_header->src[3], bf_header->src[4], bf_header->src[5]);

        info.BSSID = QString((char *)myclass->bss);
        info.AP = QString((char *)myclass->ssid);
        //time_t tm;
        //time(&tm);

        //cout << "[*] SRC : " << myclass->src << "    DEST : " << myclass->dest << "    BSS : " << myclass->bss;
        //printf("    Time : %u\n", tm);

        //printf("Channel : %d AP : ", info.Channel);
        cout << qPrintable(info.AP) << " : " << qPrintable(info.BSSID) << " ENC : " << qPrintable(info.ENC);
        printf("%d\n", myclass->enc);
        emit captured();
    }
}

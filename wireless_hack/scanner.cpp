#include "scanner.h"

Scanner::Scanner()
{
    strcpy(this->dev, "wlan0");
    this->handle = pcap_open_live(this->dev, BUFSIZE, 1, 10, this->errBuf);
    if(handle == NULL){
        fprintf(stderr, "Couldn't open device %s : %s\n", "wlan0", this->dev, errBuf);
        printf("%s\n", this->dev);
        exit(1);
    }
}

Scanner::Scanner(char *device)
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
}

Scanner::~Scanner()
{
    delete myclass;
    delete ieee;
    delete bf_header;
}

void Scanner::run()
{
    while(!isFinished())
    {
        int res = pcap_next_ex(handle, &pkthdr, &data);
        if (res == 0) continue;
        else if (res < 0) break;
        data += *(u_int16_t *)(data+2);          // Skip Radiotap Header
        memcpy(bf_header, data, 0x18);           // Beacon Frame Header
        data += 0x18;
        memcpy(ieee, data, 12);                  // Fixed 802.11 Header
        data += 12;
        /*int len2 = pkthdr->caplen - (18+0x18+12);
        const u_char *data2 = data;
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
            if(data2[1] == 0) data2 += 2;
            else data2 += data2[1];
        }
        cout << "[*] SSID : " << myclass->ssid << endl;*/

        sprintf((char *)myclass->bss, "%02X:%02X:%02X:%02X:%02X:%02X", bf_header->bss[0], bf_header->bss[1], bf_header->bss[2], bf_header->bss[3], bf_header->bss[4], bf_header->bss[5]);
        sprintf((char *)myclass->dest, "%02X:%02X:%02X:%02X:%02X:%02X", bf_header->dest[0], bf_header->dest[1], bf_header->dest[2], bf_header->dest[3], bf_header->dest[4], bf_header->dest[5]);
        sprintf((char *)myclass->src, "%02X:%02X:%02X:%02X:%02X:%02X", bf_header->src[0], bf_header->src[1], bf_header->src[2], bf_header->src[3], bf_header->src[4], bf_header->src[5]);

        time_t tm;
        time(&tm);

        cout << "[*] SRC : " << myclass->src << "    DEST : " << myclass->dest << "    BSS : " << myclass->bss;
        printf("    Time : %u\n", tm);
    }
}

void Scanner::startScan()
{
    cout << "Start" << endl;
    this->start();
}

void Scanner::stopScan()
{
    this->terminate();
    this->wait();
}

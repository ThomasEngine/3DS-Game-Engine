#include "systems/network_system.h"
#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define WLAN_COMM_ID 0x48434745 //Random hex string for a room has to be hardcoded for now
#define DATA_CHANNEL 1
#define AMOUNT_OF_PLAYERS 2

//TODO Error messages the right way prinf doesnt do much

namespace net{
    static const char* PASSPHRASE = "3dsengine-uds-test-v1"; // Password for the room
    static bool connected;
    static bool isHostFlag;
    static udsBindContext bindctx;
    static u32 tick = 0;
    static u8 recvbuf[UDS_DATAFRAME_MAXSIZE];

    void init() {
        connected = false;
        isHostFlag = false;

        Result ret = udsInit(0x3000, NULL);
        if (R_FAILED(ret))
        {
            printf("udsInit faalde: 0x%08lX\n", ret);
            return;
        }
    }

    void exit() {
        if (!connected) {
            udsExit();
            return;
        }

        udsUnbind(&bindctx);
        if (isHostFlag) {
            udsDestroyNetwork();
        }
        else {
            udsDisconnectNetwork();
        }
        connected = false;
        udsExit();
    }

    bool host_game() {
        udsNetworkStruct networkStruct;
        Result ret;

        memset(&networkStruct, 0, sizeof(networkStruct));
        udsGenerateDefaultNetworkStruct(&networkStruct, WLAN_COMM_ID, 0, AMOUNT_OF_PLAYERS);

        ret = udsCreateNetwork(&networkStruct, PASSPHRASE, strlen(PASSPHRASE) + 1,
            &bindctx, DATA_CHANNEL, UDS_DEFAULT_RECVBUFSIZE);

        if (R_FAILED(ret))
        {
            printf("udsCreateNetwork faalde: 0x%08lX\n", ret);
            return false;
        }

        printf("Netwerk gehost. Wachten op een speler...\n");
        isHostFlag = true;
        connected = true;
        return true;
    }

    bool find_and_join_host() {
        void* scanbuf;
        size_t scanbuf_size = 0x4000;
        udsNetworkScanInfo* networks = NULL;
        size_t total_networks = 0;
        Result ret;
        int tries;

        scanbuf = malloc(scanbuf_size);
        if (!scanbuf)
        {
            printf("Kon geen geheugen reserveren voor scan.\n");
            return false;
        }

        printf("Scannen naar netwerken...\n");

        for (tries = 0; tries < 10; tries++)
        {
            total_networks = 0;
            ret = udsScanBeacons(scanbuf, scanbuf_size, &networks, &total_networks,
                WLAN_COMM_ID, 0, NULL, false);

            if (R_FAILED(ret))
            {
                printf("udsScanBeacons faalde: 0x%08lX\n", ret);
                free(scanbuf);
                return false;
            }

            if (total_networks > 0)
                break;

            svcSleepThread(500ULL * 1000 * 1000);
        }

        if (total_networks == 0)
        {
            printf("Geen host gevonden. Is de host actief?\n");
            free(scanbuf);
            return false;
        }

        printf("Host gevonden! Verbinden...\n");

        ret = udsConnectNetwork(&networks[0].network, PASSPHRASE, strlen(PASSPHRASE) + 1,
            &bindctx, UDS_BROADCAST_NETWORKNODEID,
            UDSCONTYPE_Client, DATA_CHANNEL, UDS_DEFAULT_RECVBUFSIZE);

        free(scanbuf);

        if (R_FAILED(ret))
        {
            printf("udsConnectNetwork faalde: 0x%08lX\n", ret);
            return false;
        }

        printf("Verbonden met host!\n");
        isHostFlag = false;
        connected = true;
        return true;
    }

    void update() {
        if (!is_connected())
            return;

        size_t actual_size;
        u16 srcNodeID;

        do {
            actual_size = 0;
            Result pret = udsPullPacket(&bindctx, recvbuf, sizeof(recvbuf),
                &actual_size, &srcNodeID);

            if (R_FAILED(pret)) {
                // TODO: loggen via eigen debug-systeem ipv printf
                break;
            }

            if (actual_size > 0) {
                // TODO: hier komt straks handlePacket(recvbuf, actual_size, srcNodeID)
                // dat het PacketType uitleest en de juiste component bijwerkt
            }
        } while (actual_size > 0);

        tick++;

        if (tick % 30 == 0) {
            char msg[32];
            snprintf(msg, sizeof(msg), "tick:%lu", (unsigned long)tick);

            Result sret = udsSendTo(UDS_BROADCAST_NETWORKNODEID, DATA_CHANNEL,
                0, msg, strlen(msg) + 1);

            if (R_FAILED(sret)) {
                // TODO: loggen via eigen debug-systeem ipv printf
            }
        }


    }

    bool is_connected(){
        return connected;
    }

    bool is_host(){
        return isHostFlag;
    }
}
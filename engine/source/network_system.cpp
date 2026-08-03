#include "systems/network_system.h"
#include <3ds.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define WLAN_COMM_ID 0x48434745 //Willekeurige hex getal voor rooom dit moet hardcoded
#define DATA_CHANNEL 1
#define AMOUNT_OF_PLAYERS 2

static const char* PASSPHRASE = "3dsengine-uds-test-v1"; // soort wachtwoord dat gekozen moet worden

//TODO foutmeldingen goed teruggeven printf doet hier niet zo veel

void NetworkSystem::networkInit() {
    connected = false;
    isHostFlag = false;

    Result ret = udsInit(0x3000, NULL);
    if (R_FAILED(ret))
    {
        printf("udsInit faalde: 0x%08lX\n", ret);
        return;
    }
}

void NetworkSystem::networkShutdown() {
    if (!connected) {
        udsExit();
        return;
    }

    udsUnbind(&bindctx);
    if (isHostFlag) {
        udsDestroyNetwork();
    } else {
        udsDisconnectNetwork();
    }
    connected = false;
    udsExit();
}

bool NetworkSystem::hostGame() {
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

bool NetworkSystem::findAndJoinGame() {
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

void NetworkSystem::update() {
    if (!isConnected())
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

bool NetworkSystem::isConnected() const {
    return connected;
}

bool NetworkSystem::isHost() const {
    return isHostFlag;
}
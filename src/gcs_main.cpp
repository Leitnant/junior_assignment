#include "gcs.h"
#include <lnl/lnl.h>

#include <iostream>

using namespace std;

enum Action {
    ARM = 1,
    DISARM = 2,
    GOTO = 3
};

int main() {
    lnl::initialize();
    printf("[GCS]: Starting GCS...\n");
    GCS gcs(4499);

    int choice;
    float x, y, alt;
    while (1) {
        if (gcs.isConnected()){
            printf("\n[GCS]: ACTION       [n]\n");
            printf("[GCS]: Arm          [1]\n");
            printf("[GCS]: Disarm       [2]\n");
            printf("[GCS]: Goto         [3]\n");
            printf("[GCS]: Choose acton: ");
            cin >> choice;
            if (choice == ARM){
                printf("[GCS]: Arming drone...\n");
                gcs.send_arm();
            } else if (choice == DISARM) {
                printf("[GCS]: Disarming drone...\n");
                gcs.send_disarm();
            } else if (choice == GOTO){
                printf("[GCS]: Enter X:   ");
                cin >> x;
                printf("[GCS]: Enter Y:   ");
                cin >> y;
                printf("[GCS]: ENTER ALT: ");
                cin >> alt;
                printf("[GCS]: Sending target...\n");
                gcs.send_goto(x, y, alt);
            } else {
                printf("[GCS]: Invalid input!\n");
            }
        }
    }

    return 0;
}
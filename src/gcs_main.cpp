#include "gcs.h"

#include <iostream>
#include <string>

using namespace std;

enum Action {
    ARM = 1,
    DISARM = 2,
    GOTO = 3
};

int main() {
    printf("[GCS]: Starting GCS...\n");
    GCS gcs(4499);
    
    gcs.startDisplayLoop();

    string input;
    int choice;
    float x, y, alt;
    while (1) {
        printf("\n[GCS]: ACTION        [n]\n");
        printf(  "[GCS]: Arm           [1]\n");
        printf(  "[GCS]: Disarm        [2]\n");
        printf(  "[GCS]: Goto          [3]\n");
        printf(  "[GCS]: Choose action: ");

        cin >> input;
        try {
            choice = stoi(input);
        } catch (invalid_argument& e) {
            printf("[GCS]: Invalid input! Please enter a number.\n");
            continue;
        }

        switch (choice){
            case ARM:
                printf("[GCS]: Arming drone...\n");
                gcs.sendArm();
                break;
            case DISARM:
                printf("[GCS]: Disarming drone...\n");
                gcs.sendDisarm();
                break;
            case GOTO:
                printf("[GCS]: Enter X:   ");
                cin >> x;
                printf("[GCS]: Enter Y:   ");
                cin >> y;
                printf("[GCS]: ENTER ALT: ");
                cin >> alt;
                printf("[GCS]: Sending target...\n");
                gcs.sendGoTo(x, y, alt);
                break;
            default:
                printf("[GCS]: Invalid input! Number doesn't match a command.\n");
                break;
        }
    }
    gcs.stopDisplayLoop();
    return 0;
}
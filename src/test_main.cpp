#include "new_udp_client.h"
#include "new_udp_server.h"
#include <thread>

using namespace std;

int main() {
    int clientPort = 4498;
    int serverPort = 4499;
    string serverIP = "127.0.0.1";
    UDPClient client = UDPClient(clientPort, serverIP, serverPort);
    this_thread::sleep_for(15ms);

    UDPServer server = UDPServer(serverPort, serverIP, clientPort);
    this_thread::sleep_for(15ms);

    while (1) {
        server.sendArm();
        this_thread::sleep_for(500ms);
        mavlink_command_long_t command = client.receiveCommand();
        printf("cmd: %d\n", (int)command.command);
        printf("cnf: %d\n", (int)command.confirmation);
        printf("pr1: %d\n", (int)command.param1);
        printf("pr2: %d\n", (int)command.param2);
        printf("pr3: %d\n", (int)command.param3);
        printf("pr4: %d\n", (int)command.param4);
        printf("pr5: %d\n", (int)command.param5);
        printf("pr6: %d\n", (int)command.param6);
        printf("pr7: %d\n", (int)command.param7);
        printf("tgc: %d\n", (int)command.target_component);
        printf("tgs: %d\n", (int)command.target_system);
        this_thread::sleep_for(500ms);
    }
}
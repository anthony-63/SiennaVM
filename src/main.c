#include <stdio.h>
#include "databus.h"
#include "bankeddatabus.h"
int main() {
    sienna_databus_t databus;
    sienna_databus_init(&databus);
    
    printf("-----------Testing databus-----------\n");
    printf("Setting the number 0x10 at address 0x4123...");
    sienna_databus_send(&databus, 0x10, 0x4123);
    printf("Success\n");
    printf("Setting the number 0x1D at address 0x2314...");
    sienna_databus_send(&databus, 0x1D, 0x2314);
    printf("Success\n");
    printf("Fetching at address 0x2314...%04x\n", sienna_databus_fetch(&databus, 0x2314));
    printf("Fetching at address 0x4123...%04x\n", sienna_databus_fetch(&databus, 0x4123));
    printf("Cleaning up....\n");
    sienna_databus_cleanup(&databus);

    printf("\n-----------Testing Banked Databus-----------\n");
    sienna_bankeddatabus_t banked_data_bus;
    sienna_bankeddatabus_init(&banked_data_bus, 16);
    printf("Setting the number 0x10 at address 0x0011 in bank 1...");
    sienna_bankeddatabus_send(&banked_data_bus, 1, 0x10, 0x0011);
    printf("Success\n");
    printf("Setting the number 0x51 at address 0x0011 in bank 2...");
    sienna_bankeddatabus_send(&banked_data_bus, 2, 0x51, 0x0011);
    printf("Success\n");
    printf("Fetching at address 0x0011 in bank 2...%04x\n", sienna_bankeddatabus_fetch(&banked_data_bus, 2, 0x0011));
    printf("Fetching at address 0x0011 in bank 1...%04x\n", sienna_bankeddatabus_fetch(&banked_data_bus, 1, 0x0011));
    printf("Cleaning up....\n");
    sienna_bankeddatabus_cleanup(&banked_data_bus);
    return 0;
}
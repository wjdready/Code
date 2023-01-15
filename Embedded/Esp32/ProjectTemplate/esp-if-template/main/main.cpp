#include <stdio.h>
#include <iostream>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "func.h"
#include "comp.h"
#include "comp2.h"


extern "C" void app_main()
{   
    printf("Hello World\n");
    printHello();
    comprint();
    comprint2();

    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }

    std::cout << "Restarting now." << std::endl;
    esp_restart();
}

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "gpio_lib.h"

#define PD01    SUNXI_GPD(0)
#define PD1    SUNXI_GPD(1)
#define PD2    SUNXI_GPD(2)
#define PD3    SUNXI_GPD(3)
#define PD4    SUNXI_GPD(4)
#define MISO    SUNXI_GPE(3)
#define MOSI    SUNXI_GPE(2)
#define SCK     SUNXI_GPE(1)
#define CS      SUNXI_GPE(0)

int main()
{
    // int runtime=1;
    char input[100];
    char bcode[] = "8941100314517";

    if(SETUP_OK!=sunxi_gpio_init()){
        printf("Failed to initialize GPIO\n");
        return -1;
    }
    if(SETUP_OK!=sunxi_gpio_set_cfgpin(PD01,OUTPUT)){
        printf("Failed to config GPIO pin\n");
        return -1;
    }

    while (1){//unlimited loop
        scanf ("%[^\n]%*c", input);
        // int i;
        //printf("%d\n",strcmp(input, bcode));
        int tt;
        int match = 1;
        for (tt=0;tt<10;tt++){
            if(input[tt] != bcode[tt]){
                match = 0;
            }
        }
        if (match == 1){
            // for(i=0;i<runtime;i++){
                if(sunxi_gpio_output(PD01,HIGH)){
                    printf("Failed to set GPIO pin value\n");
                    return -1;
                }
                usleep(500000);
                if(sunxi_gpio_output(PD01,LOW)){
                    printf("Failed to set GPIO pin value\n");
                return -1;
                }
            usleep(500000);
            // }
        }//if match
    }//while
    sunxi_gpio_cleanup();
    return 0;
}





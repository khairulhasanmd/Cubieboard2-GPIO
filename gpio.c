#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
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

struct string {
  char *ptr;
  size_t len;
};

void init_string(struct string *s) {
  s->len = 0;
  s->ptr = malloc(s->len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "malloc() failed\n");
    exit(EXIT_FAILURE);
  }
  s->ptr[0] = '\0';
}

size_t writefunc(void *ptr, size_t size, size_t nmemb, struct string *s)
{
  s->len = 0;
  size_t new_len = s->len + size*nmemb;
  s->ptr = realloc(s->ptr, new_len+1);
  if (s->ptr == NULL) {
    fprintf(stderr, "realloc() failed\n");
    exit(EXIT_FAILURE);
  }
  memcpy(s->ptr+s->len, ptr, size*nmemb);
  s->ptr[new_len] = '\0';
  s->len = new_len;

  return size*nmemb;
}

int main(void)
{
    CURL *curl;
    CURLcode res;
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
    curl = curl_easy_init();
    if(curl) {
        char URL[100];
        struct string s;
        init_string(&s);

        while (1){//unlimited loop
            scanf ("%[^\n]%*c", input);
            // int i;
            //printf("%d\n",strcmp(input, bcode));
            sprintf(URL,"http://192.168.0.106/index.php?barcode=%s",input);
  
            curl_easy_setopt(curl, CURLOPT_URL, URL);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &s);
            res = curl_easy_perform(curl);
            //printf("%s\n", s.ptr);
            printf("%c", s.ptr[0]);
	    if (s.ptr[0] == '1'){
                if(sunxi_gpio_output(PD01,HIGH)){
                    printf("Failed to set GPIO pin value\n");
                    return -1;
                }
                usleep(500000);
                if(sunxi_gpio_output(PD01,LOW)){
                    printf("Failed to set GPIO pin value\n");
                return -1;
                }
                // usleep(500000);
                //s.ptr = NULL;
		//s.len = 0;
                //free(s.ptr);
                //init_string(&s);
            }//if match
            
        }//while     

        /* always cleanup */
        free(s.ptr);
        curl_easy_cleanup(curl);
        sunxi_gpio_cleanup();
    }
    return 0;
}

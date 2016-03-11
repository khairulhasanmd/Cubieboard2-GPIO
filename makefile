run: gpio.c
	if [ -e gpio ]; then rm gpio; fi;
	gcc gpio_lib.c -c
	gcc gpio.c -c
	#gcc gpio.o gpio_lib.o -o gpio
	gcc -Wall gpio.c -lcurl -lexpat -L/usr/local/ssl/lib -lssl -lcrypto -lm -o gpio
	./gpio

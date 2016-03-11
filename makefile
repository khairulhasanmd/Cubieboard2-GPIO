run: gpio.c
	gcc gpio_lib.c -c
	gcc gpio.c -c
	gcc gpio.o gpio_lib.o -o gpio
	cp gpio /bin/gpio
	gpio

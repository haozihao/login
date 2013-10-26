TARGET=login14
CROSS_PATH=/usr/local/arm/4.3.2/arm-linux
helloworld:$(TARGET).c
	arm-linux-gcc -o $(TARGET) $(TARGET).c \
        -L$(CROSS_PATH)/lib -L /root/minigui/sources/sqlite-arm/lib/\
        -I$(CROSS_PATH)/include -I /root/minigui/sources/sqlite-arm/include/ \
        -lpng -ljpeg -lminigui -lz -lpthread -lm -lts -lmgext 
clean:
	rm $(TARGET)*.o -rf


OBJS1	=	server_main.o server_net.o server_command.o server_system.o
OBJS2	=	client_main.o client_net.o client_command.o client_win.o client_system.o
TARGET1	=	server
TARGET2	=	client
CFLAGS	=	-c -DNDEBUG

.c.o:
	gcc $(CFLAGS) $<

all: $(TARGET1) $(TARGET2)

$(TARGET1):	$(OBJS1)
	gcc -o $(TARGET1) $(OBJS1) -lm -lSDL2
$(TARGET2):	$(OBJS2)
	gcc -o $(TARGET2) $(OBJS2) -lm -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_gfx -lSDL2_mixer -lcwiimote -D_ENABLE_TILT -D_ENABLE_FORCE$(LDFLAGS)
clean:
	rm *.o $(TARGET1) $(TARGET2)

server_main.o: server_main.c server_common.h server_func.h common.h server_system.h
server_net.o: server_net.c server_common.h server_func.h common.h server_system.h
server_command.o: server_command.c server_common.h server_func.h common.h server_system.h 
server_system.o: server_system.c server_func.h server_common.h server_system.h common.h 
client_main.o: client_main.c client_func.h common.h system.h
client_net.o: client_net.c client_func.h common.h system.h
client_command.o: client_command.c client_func.h common.h system.h
client_win.o: client_win.c client_func.h common.h system.h
client_system.o: client_system.c client_func.h common.h system.h

all: serv client

serv: serv.c
	gcc $< -o $@ `pkg-config --cflags --libs dbus-1`
client: client.c
	gcc $< -o $@ `pkg-config --cflags --libs dbus-1`


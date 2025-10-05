CC = gcc
CFLAGS = -g -O0 -std=c99
PKGCONFIG = `pkg-config --cflags --libs sdl`
LIBS = -lm

TARGET = chip8
SRCS = chip8-implementation.c sdl-basecode-derivation.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS) $(PKGCONFIG) $(LIBS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) core
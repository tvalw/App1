# Nombre del ejecutable
TARGET = app1

# Archivos fuente
SRCS = main.c metricas.c

# Archivos objeto
OBJS = $(SRCS:.c=.o)

# Compilador
CC = gcc

# Flags de compilación
CFLAGS = -Wall -Wextra -std=c99

# Build por defecto
all: $(TARGET)

# Enlazado final
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Limpieza de .o y binario
clean:
	rm -f $(OBJS) $(TARGET)

# Ejecutar con CSV y métricas de prueba
run: $(TARGET)
	./$(TARGET) ventas.csv pms pls dms dlsp apo ims



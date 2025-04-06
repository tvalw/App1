#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "metricas.h"  // Librería que define funciones para métricas
#include "structs.h"   // Librería que contiene la definición de la estructura Venta

// Máximo número de órdenes a leer
#define MAX_ORDERS 1000

// Función para leer un archivo CSV y cargar los datos de ventas
int leer_csv(const char *filename, Venta **ventas) {
    FILE *file = fopen(filename, "r");  // Abre el archivo para lectura
    if (!file) {  // Si no se puede abrir el archivo, muestra un mensaje de error
        printf("Error: No se pudo abrir el archivo %s\n", filename);
        return 0;  // Retorna 0 en caso de error
    }

    char line[1024];  // Buffer para almacenar cada línea del archivo
    int count = 0;  // Contador de registros leídos
    fgets(line, sizeof(line), file); // Salta la primera línea (encabezado del CSV)

    // Asignar memoria dinámica para el arreglo de ventas
    *ventas = malloc(MAX_ORDERS * sizeof(Venta));  
    if (*ventas == NULL) {
        printf("Error: No se pudo asignar memoria para las ventas\n");
        fclose(file);
        return 0;  // Retorna 0 si no se puede asignar memoria
    }

    // Lee las líneas del archivo y procesa cada una
    while (fgets(line, sizeof(line), file) && count < MAX_ORDERS) {
        Venta v;  // Estructura para almacenar cada venta
        char *ptr = line;  // Puntero para recorrer la línea
        char *field;  // Puntero para almacenar los campos individuales
        int col = 0;  // Contador de columnas (campos) en cada línea

        // Bucle que recorre las columnas de cada línea
        while (col < 11) {
            if (*ptr == '"') {  // Si encontramos un campo entre comillas
                ptr++;  // Saltamos la comilla inicial
                field = ptr;
                while (*ptr && !(*ptr == '"' && *(ptr + 1) == ',')) ptr++;  // Busca el final del campo
                *ptr = '\0';  // Termina la cadena en el final del campo
                ptr += 2;  // Salta la coma y la comilla final
            } else {  // Si el campo no está entre comillas
                field = ptr;
                while (*ptr && *ptr != ',') ptr++;  // Busca la coma que separa el campo
                if (*ptr) *ptr++ = '\0';  // Termina el campo y mueve el puntero a la siguiente columna
            }

            // Asigna el valor del campo correspondiente a la estructura 'Venta'
            switch (col) {
                case 0: v.pizza_id = atoi(field); break;
                case 1: v.order_id = atoi(field); break;
                case 2: strncpy(v.pizza_name_id, field, 100); break;
                case 3: v.quantity = atoi(field); break;
                case 4: strncpy(v.order_date, field, 20); break;
                case 5: strncpy(v.order_time, field, 20); break;
                case 6: v.unit_price = atof(field); break;
                case 7: v.total_price = atof(field); break;
                case 8: strncpy(v.pizza_size, field, 5); break;
                case 9: strncpy(v.pizza_category, field, 50); break;
                case 10: strncpy(v.pizza_ingredients, field, 200); break;
            }

            col++;  // Pasa al siguiente campo
        }

        // Procesa el último campo 'pizza_name' fuera del bucle porque no termina con una coma
        if (*ptr == '"') {
            ptr++;
            field = ptr;
            while (*ptr && *ptr != '"') ptr++;  // Busca el final del campo entre comillas
        } else {
            field = ptr;
            while (*ptr && *ptr != '\n') ptr++;  // Busca el final del campo
        }
        *ptr = '\0';  // Termina la cadena de texto
        strncpy(v.pizza_name, field, 100);  // Asigna el valor al campo 'pizza_name'

        (*ventas)[count++] = v;  // Guarda la venta en el arreglo
    }

    fclose(file);  // Cierra el archivo
    return count;  // Retorna la cantidad de registros leídos
}

// Función para liberar la memoria de ventas
void liberar_ventas(Venta *ventas) {
    free(ventas);  // Libera la memoria asignada dinámicamente para el arreglo de ventas
}

// Función que calcula la pizza más vendida
char *pms(int *size, Venta *ventas) {
    // El código de esta función se mantiene igual
    // ...
    // Retorna el resultado
}

// Función que calcula la pizza menos vendida
char *pls(int *size, Venta *ventas) {
    // El código de esta función se mantiene igual
    // ...
    // Retorna el resultado
}

// Función que calcula el día con más ventas en dinero
char* dms(int* size, Venta* ventas) {
    // El código de esta función se mantiene igual
    // ...
    // Retorna el resultado
}

// Fecha con menos ventas en dinero
char* dls(int* size, Venta* ventas) {
    // El código de esta función se mantiene igual
    // ...
    // Retorna el resultado
}

// Función para generar las métricas solicitadas
void generar_metricas(int *size, Venta *ventas, const char **metricas, int cantidad_metricas) {
    for (int i = 0; i < cantidad_metricas; i++) {
        // Compara y llama a la función correspondiente para cada métrica
        if (strcmp(metricas[i], "pms") == 0) {
            printf("%s\n", pms(size, ventas));  // Pizza más vendida
        } else if (strcmp(metricas[i], "pls") == 0) {
            printf("%s\n", pls(size, ventas));  // Pizza menos vendida
        } else if (strcmp(metricas[i], "dms") == 0) {
            char *r = dms(size, ventas);
            printf("%s\n", r);  // Fecha con más ventas en dinero
            free(r);  // Libera la memoria asignada
        } else if (strcmp(metricas[i], "dls") == 0) {
            char *r = dls(size, ventas);
            printf("%s\n", r);  // Fecha con menos ventas en dinero
            free(r);  // Libera la memoria asignada
        } else if (strcmp(metricas[i], "dmsp") == 0) {
            printf("%s\n", dmsp(size, ventas));  // Fecha con más pizzas vendidas
        } else if (strcmp(metricas[i], "dlsp") == 0) {
            printf("%s\n", dlsp(size, ventas));  // Fecha con menos pizzas vendidas
        } else if (strcmp(metricas[i], "apo") == 0) {
            char *r = apo(size, ventas);
            printf("%s\n", r);  // Promedio de pizzas por orden
            free(r);  // Libera la memoria asignada
        } else if (strcmp(metricas[i], "apd") == 0) {
            char *r = apd(size, ventas);
            printf("%s\n", r);  // Promedio de pizzas por día
            free(r);  // Libera la memoria asignada
        } else if (strcmp(metricas[i], "ims") == 0) {
            char *r = ims(size, ventas);
            printf("%s\n", r);  // Ingrediente más vendido
            free(r);  // Libera la memoria asignada
        } else if (strcmp(metricas[i], "hp") == 0) {
            char *r = hp(size, ventas);
            printf("%s\n", r);  // Conteo de pizzas por categoría
            free(r);  // Libera la memoria asignada
        } else {
            printf("Métrica desconocida: %s\n", metricas[i]);  // Si la métrica no es reconocida
        }
    }
}

int main() {
    Venta *ventas = NULL;  // Apuntador a las ventas
    int total_ventas = leer_csv("ventas.csv", &ventas);  // Llamada a la función de lectura

    if (total_ventas > 0) {
        // Aquí puedes usar las funciones que operan sobre 'ventas'

        // Ejemplo de cómo generar métricas
        const char *metricas[] = {"pms", "pls", "dms", "dls"};
        generar_metricas(&total_ventas, ventas, metricas, 4);

        // Liberar la memoria dinámica
        liberar_ventas(ventas);
    }

    return 0;
}


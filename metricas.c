#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "metricas.h"  // Librería que define funciones para métricas
#include "structs.h"   // Librería que contiene la definición de la estructura Venta

#define MAX_ORDERS 1000  // Máximo número de órdenes a leer

// Función para leer un archivo CSV y cargar los datos de ventas
int leer_csv(const char *filename, Venta ventas[]) {
    FILE *file = fopen(filename, "r");  // Abre el archivo para lectura
    if (!file) {  // Si no se puede abrir el archivo, muestra un mensaje de error
        printf("Error: No se pudo abrir el archivo %s\n", filename);
        return 0;  // Retorna 0 en caso de error
    }

    char line[1024];  // Buffer para almacenar cada línea del archivo
    int count = 0;  // Contador de registros leídos
    fgets(line, sizeof(line), file); // Salta la primera línea (encabezado del CSV)

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

        ventas[count++] = v;  // Guarda la venta en el arreglo
    }

    fclose(file);  // Cierra el archivo
    return count;  // Retorna la cantidad de registros leídos
}

// Función que calcula la pizza más vendida
char *pms(int *size, Venta *ventas) {
    int max_count = 0;  // Contador máximo de unidades vendidas
    char* best_seller = NULL;  // Nombre de la pizza más vendida
    int sales[MAX_ORDERS] = {0};  // Arreglo para almacenar las ventas por pizza
    static char resultado[150];  // Buffer para almacenar el resultado

    // Acumula las ventas por pizza
    for (int i = 0; i < *size; i++) {
        int found = 0;
        for (int j = 0; j < i; j++) {
            if (strcmp(ventas[i].pizza_name, ventas[j].pizza_name) == 0) {
                sales[j] += ventas[i].quantity;
                found = 1;
                break;
            }
        }
        if (!found) sales[i] = ventas[i].quantity;  // Si no se encuentra, se asigna la cantidad inicial
    }

    // Busca la pizza más vendida
    for (int i = 0; i < *size; i++) {
        if (sales[i] > max_count) {
            max_count = sales[i];
            best_seller = ventas[i].pizza_name;  // Asigna el nombre de la pizza más vendida
        }
    }

    // Formatea el resultado
    snprintf(resultado, sizeof(resultado), "Pizza más vendida: %s (%d unidades)", best_seller, max_count);
    return resultado;  // Retorna el resultado
}

// Función que calcula la pizza menos vendida
char *pls(int *size, Venta *ventas) {
    int min_count = 999999;  // Inicializa el contador con un valor muy alto
    char* worst_seller = NULL;  // Nombre de la pizza menos vendida
    static char resultado[150];  // Buffer para almacenar el resultado

    // Acumula las ventas por pizza
    for (int i = 0; i < *size; i++) {
        int sum = 0;
        for (int j = 0; j < *size; j++) {
            if (strcmp(ventas[i].pizza_name, ventas[j].pizza_name) == 0) {
                sum += ventas[j].quantity;  // Suma las cantidades de la misma pizza
            }
        }
        if (sum < min_count) {  // Si encontramos una pizza con menos ventas
            min_count = sum;
            worst_seller = ventas[i].pizza_name;  // Asigna el nombre de la pizza menos vendida
        }
    }

    // Formatea el resultado
    snprintf(resultado, sizeof(resultado), "Pizza menos vendida: %s (%d unidades)", worst_seller, min_count);
    return resultado;  // Retorna el resultado
}

// Función que calcula el día con más ventas en dinero
char* dms(int* size, Venta* ventas) {
    float max_revenue = 0;  // Inicializa el máximo de ventas en dinero
    char mejor_fecha[20] = "";  // Fecha con más ventas
    // Recorre las ventas acumulando las ventas por fecha
    for (int i = 0; i < *size; i++) {
        float total = 0;  // Total de ventas para cada fecha
        for (int j = 0; j < *size; j++) {
            if (strcmp(ventas[i].order_date, ventas[j].order_date) == 0) {
                total += ventas[j].total_price;  // Suma el total por fecha
            }
        }

        // Si encontramos una fecha con más ventas, la actualizamos
        if (total > max_revenue) {
            max_revenue = total;
            strncpy(mejor_fecha, ventas[i].order_date, 20);
            mejor_fecha[19] = '\0';  // Asegura que la cadena esté terminada
        }
    }

    // Crea un nuevo string para el resultado
    char* resultado = malloc(100);  
    if (resultado == NULL) return NULL;  // Verifica si se pudo asignar memoria

    // Formatea el resultado
    sprintf(resultado, "Fecha con más ventas (dinero): %s - $%.2f", mejor_fecha, max_revenue);
    return resultado;  // Retorna el resultado
}

// Fecha con menos ventas en dinero
char* dls(int* size, Venta* ventas) {
    float min_revenue = -1;  // Inicializa el mínimo de ventas en dinero
    char peor_fecha[20] = "";  // Fecha con menos ventas
    // Recorre las ventas acumulando las ventas por fecha
    for (int i = 0; i < *size; i++) {
        float total = 0;  // Total de ventas para cada fecha
        for (int j = 0; j < *size; j++) {
            if (strcmp(ventas[i].order_date, ventas[j].order_date) == 0) {
                total += ventas[j].total_price;  // Suma el total por fecha
            }
        }

        // Si encontramos una fecha con menos ventas, la actualizamos
        if (min_revenue == -1 || total < min_revenue) {
            min_revenue = total;
            strncpy(peor_fecha, ventas[i].order_date, 20);
            peor_fecha[19] = '\0';  // Asegura que la cadena esté terminada
        }
    }

    // Crea un nuevo string para el resultado
    char* resultado = malloc(100);  
    if (resultado == NULL) return NULL;  // Verifica si se pudo asignar memoria

    // Formatea el resultado
    sprintf(resultado, "Fecha con menos ventas (dinero): %s - $%.2f", peor_fecha, min_revenue);
    return resultado;  // Retorna el resultado
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "metricas.h"
#include "structs.h"


// Fecha con menos ventas en dinero
char* dls(int* size, Venta* ventas) {
    float min_revenue = -1;
    char peor_fecha[20] = "";

    for (int i = 0; i < *size; i++) {
        float total = 0;

        for (int j = 0; j < *size; j++) {
            if (strcmp(ventas[i].order_date, ventas[j].order_date) == 0) {
                total += ventas[j].total_price;
            }
        }

        // Verifica si es la primera vez o si encontramos un total menor
        if (min_revenue == -1 || total < min_revenue) {
            min_revenue = total;
            strncpy(peor_fecha, ventas[i].order_date, 20);
            peor_fecha[19] = '\0';
        }
    }

    char* resultado = malloc(100);
    if (resultado == NULL) return NULL;

    sprintf(resultado, "Fecha con menos ventas (dinero): %s - $%.2f", peor_fecha, min_revenue);
    return resultado;
}












// Promedio de pizzas por orden
char* apo(int* size, Venta* ventas) {
    int total_pizzas = 0;
    int total_ordenes = 0;
    int ultimo_order_id = -1;

    for (int i = 0; i < *size; i++) {
        total_pizzas += ventas[i].quantity;
        if (ventas[i].order_id != ultimo_order_id) {
            total_ordenes++;
            ultimo_order_id = ventas[i].order_id;
        }
    }

    float promedio = (float)total_pizzas / total_ordenes;

    char* resultado = malloc(100);
    sprintf(resultado, "Promedio de pizzas por orden: %.2f", promedio);
    return resultado;
}

// Promedio de pizzas por día
char* apd(int* size, Venta* ventas) {
    int total_pizzas = 0;
    int total_dias = 0;
    char fechas_unicas[*size][20];

    for (int i = 0; i < *size; i++) {
        total_pizzas += ventas[i].quantity;

        int encontrada = 0;
        for (int j = 0; j < total_dias; j++) {
            if (strcmp(fechas_unicas[j], ventas[i].order_date) == 0) {
                encontrada = 1;
                break;
            }
        }

        if (!encontrada) {
            strcpy(fechas_unicas[total_dias], ventas[i].order_date);
            total_dias++;
        }
    }

    float promedio = (float)total_pizzas / total_dias;
    char* resultado = malloc(100);
    sprintf(resultado, "Promedio de pizzas por día: %.2f", promedio);
    return resultado;
}

// Ingrediente más vendido
typedef struct {
    char nombre[100];
    int cantidad;
} IngredienteCount;

char* ims(int* size, Venta* ventas) {
    IngredienteCount ingredientes[500];
    int total_ingredientes = 0;

    for (int i = 0; i < *size; i++) {
        char* ingredientes_str = strdup(ventas[i].pizza_ingredients);
        char* token = strtok(ingredientes_str, ",");

        while (token != NULL) {
            while (*token == ' ') token++;

            int encontrado = 0;
            for (int j = 0; j < total_ingredientes; j++) {
                if (strcmp(ingredientes[j].nombre, token) == 0) {
                    ingredientes[j].cantidad += ventas[i].quantity;
                    encontrado = 1;
                    break;
                }
            }

            if (!encontrado) {
                strcpy(ingredientes[total_ingredientes].nombre, token);
                ingredientes[total_ingredientes].cantidad = ventas[i].quantity;
                total_ingredientes++;
            }

            token = strtok(NULL, ",");
        }

        free(ingredientes_str);
    }

    int max_cantidad = 0;
    char* mas_vendido = malloc(100);
    for (int i = 0; i < total_ingredientes; i++) {
        if (ingredientes[i].cantidad > max_cantidad) {
            max_cantidad = ingredientes[i].cantidad;
            strcpy(mas_vendido, ingredientes[i].nombre);
        }
    }

    char* resultado = malloc(150);
    sprintf(resultado, "Ingrediente más vendido: %s (%d unidades)", mas_vendido, max_cantidad);
    free(mas_vendido);
    return resultado;
}


// Conteo de pizzas por categoría
// Estructura auxiliar para contar categorías
typedef struct {
    char categoria[50];
    int cantidad;
} CategoriaCount;

char* hp(int* size, Venta* orders) {
    if (orders == NULL || *size == 0) {
        char* mensaje = malloc(50);
        sprintf(mensaje, "No hay datos disponibles.");
        return mensaje;
    }

    CategoriaCount categorias[20]; // asumiendo pocas categorías distintas
    int total_categorias = 0;

    for (int i = 0; i < *size; i++) {
        int encontrada = 0;

        for (int j = 0; j < total_categorias; j++) {
            if (strcmp(categorias[j].categoria, orders[i].pizza_category) == 0) {
                categorias[j].cantidad += orders[i].quantity;
                encontrada = 1;
                break;
            }
        }

        if (!encontrada) {
            strcpy(categorias[total_categorias].categoria, orders[i].pizza_category);
            categorias[total_categorias].cantidad = orders[i].quantity;
            total_categorias++;
        }
    }

    // Crear resultado
    char* resultado = malloc(300);
    resultado[0] = '\0'; // Inicializar string

    for (int i = 0; i < total_categorias; i++) {
        char linea[100];
        sprintf(linea, "%s: %d\n", categorias[i].categoria, categorias[i].cantidad);
        strcat(resultado, linea);
    }

    return resultado;
}

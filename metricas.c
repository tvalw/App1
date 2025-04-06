#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "metricas.h"
#include "structs.h"

#define MAX_ORDERS 1000

int leer_csv(const char *filename, Venta ventas[]) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: No se pudo abrir el archivo %s\n", filename);
        return 0;
    }

    char line[1024];
    int count = 0;
    fgets(line, sizeof(line), file); // saltar encabezado

    while (fgets(line, sizeof(line), file) && count < MAX_ORDERS) {
        Venta v;
        char *ptr = line;
        char *field;
        int col = 0;

        while (col < 11) {
            if (*ptr == '"') {
                // Campo entre comillas
                ptr++;
                field = ptr;
                while (*ptr && !(*ptr == '"' && *(ptr + 1) == ',')) ptr++;
                *ptr = '\0';
                ptr += 2; // Saltar '",'
            } else {
                // Campo sin comillas
                field = ptr;
                while (*ptr && *ptr != ',') ptr++;
                if (*ptr) *ptr++ = '\0';
            }

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

            col++;
        }

        // El último campo: pizza_name (fuera del while porque no termina en coma)
        if (*ptr == '"') {
            ptr++;
            field = ptr;
            while (*ptr && *ptr != '"') ptr++;
        } else {
            field = ptr;
            while (*ptr && *ptr != '\n') ptr++;
        }
        *ptr = '\0';
        strncpy(v.pizza_name, field, 100);

        ventas[count++] = v;
    }

    fclose(file);
    return count;
}


char *pms(int *size, Venta *ventas) {
    int max_count = 0;
    char* best_seller = NULL;
    int sales[MAX_ORDERS] = {0};
    static char resultado[150];  

    for (int i = 0; i < *size; i++) {
        int found = 0;
        for (int j = 0; j < i; j++) {
            // Compara correctamente los nombres de pizza
            if (strcmp(ventas[i].pizza_name, ventas[j].pizza_name) == 0) {
                sales[j] += ventas[i].quantity;
                found = 1;
                break;
            }
        }
        if (!found) sales[i] = ventas[i].quantity;
    }

    for (int i = 0; i < *size; i++) {
        if (sales[i] > max_count) {
            max_count = sales[i];
            best_seller = ventas[i].pizza_name;  // Asignamos el nombre de la pizza
        }
    }

    snprintf(resultado, sizeof(resultado), "Pizza más vendida: %s (%d unidades)", best_seller, max_count);
    return resultado;
}


char *pls(int *size, Venta *ventas) {
    int min_count = 999999;
    char* worst_seller = NULL;
    static char resultado[150]; 

    for (int i = 0; i < *size; i++) {
        int sum = 0;
        for (int j = 0; j < *size; j++) {
            // Compara correctamente los nombres de pizza
            if (strcmp(ventas[i].pizza_name, ventas[j].pizza_name) == 0) {
                sum += ventas[j].quantity;
            }
        }
        if (sum < min_count) {
            min_count = sum;
            worst_seller = ventas[i].pizza_name;  // Asignamos el nombre de la pizza
        }
    }

    snprintf(resultado, sizeof(resultado), "Pizza menos vendida: %s (%d unidades)", worst_seller, min_count);
    return resultado;
}


char* dms(int* size, Venta* ventas) {
    float max_revenue = 0;
    char mejor_fecha[20] = "";

    for (int i = 0; i < *size; i++) {
        float total = 0;

        for (int j = 0; j < *size; j++) {
            if (strcmp(ventas[i].order_date, ventas[j].order_date) == 0) {
                total += ventas[j].total_price;
            }
        }

        if (total > max_revenue) {
            max_revenue = total;
            strncpy(mejor_fecha, ventas[i].order_date, 20);
            mejor_fecha[19] = '\0'; // Asegurar null terminator
        }
    }

    char* resultado = malloc(100);
    if (resultado == NULL) return NULL;

    sprintf(resultado, "Fecha con más ventas (dinero): %s - $%.2f", mejor_fecha, max_revenue);
    return resultado;
}


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

// Fecha con más pizzas vendidas
char* dmsp(int *size, Venta *ventas) {
    // Asumimos un máximo de 365 días distintos
    typedef struct {
        char fecha[20];
        int cantidad;
    } DiaVentas;

    DiaVentas dias[365];
    int num_dias = 0;

    for (int i = 0; i < *size; i++) {
        int encontrado = 0;
        for (int j = 0; j < num_dias; j++) {
            if (strcmp(dias[j].fecha, ventas[i].order_date) == 0) {
                dias[j].cantidad += ventas[i].quantity;
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            strcpy(dias[num_dias].fecha, ventas[i].order_date);
            dias[num_dias].cantidad = ventas[i].quantity;
            num_dias++;
        }
    }

    // Buscar la fecha con más ventas
    int max = 0;
    int indice_max = 0;
    for (int i = 0; i < num_dias; i++) {
        if (dias[i].cantidad > max) {
            max = dias[i].cantidad;
            indice_max = i;
        }
    }

    // Crear string de salida
    static char resultado[100];
    sprintf(resultado, "Fecha con más pizzas: %s (%d pizzas)", dias[indice_max].fecha, dias[indice_max].cantidad);
    return resultado;
}


// Fecha con menos pizzas vendidas
char* dlsp(int *size, Venta *ventas) {
    // Estructura auxiliar para acumular ventas por fecha
    typedef struct {
        char fecha[20];
        int cantidad;
    } DiaVentas;

    DiaVentas dias[365];
    int num_dias = 0;

    for (int i = 0; i < *size; i++) {
        int encontrado = 0;
        for (int j = 0; j < num_dias; j++) {
            if (strcmp(dias[j].fecha, ventas[i].order_date) == 0) {
                dias[j].cantidad += ventas[i].quantity;
                encontrado = 1;
                break;
            }
        }
        if (!encontrado) {
            strcpy(dias[num_dias].fecha, ventas[i].order_date);
            dias[num_dias].cantidad = ventas[i].quantity;
            num_dias++;
        }
    }

    // Buscar la fecha con menos ventas
    int min = dias[0].cantidad;
    int indice_min = 0;
    for (int i = 1; i < num_dias; i++) {
        if (dias[i].cantidad < min) {
            min = dias[i].cantidad;
            indice_min = i;
        }
    }

    // Crear string de salida
    static char resultado[100];
    sprintf(resultado, "Fecha con menos pizzas: %s (%d pizzas)", dias[indice_min].fecha, dias[indice_min].cantidad);
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

void generar_metricas(int *size, Venta *ventas, const char **metricas, int cantidad_metricas) {
    for (int i = 0; i < cantidad_metricas; i++) {
        if (strcmp(metricas[i], "pms") == 0) {
            printf("%s\n", pms(size, ventas));
        } else if (strcmp(metricas[i], "pls") == 0) {
            printf("%s\n", pls(size, ventas));
        } else if (strcmp(metricas[i], "dms") == 0) {
            char *r = dms(size, ventas);
            printf("%s\n", r);
            free(r);
        } else if (strcmp(metricas[i], "dls") == 0) {
            char *r = dls(size, ventas);
            printf("%s\n", r);
            free(r);
        } else if (strcmp(metricas[i], "dmsp") == 0) {
            printf("%s\n", dmsp(size, ventas));
        } else if (strcmp(metricas[i], "dlsp") == 0) {
            printf("%s\n", dlsp(size, ventas));
        } else if (strcmp(metricas[i], "apo") == 0) {
            char *r = apo(size, ventas);
            printf("%s\n", r);
            free(r);
        } else if (strcmp(metricas[i], "apd") == 0) {
            char *r = apd(size, ventas);
            printf("%s\n", r);
            free(r);
        } else if (strcmp(metricas[i], "ims") == 0) {
            char *r = ims(size, ventas);
            printf("%s\n", r);
            free(r);
        } else if (strcmp(metricas[i], "hp") == 0) {
            char *r = hp(size, ventas);
            printf("%s\n", r);
            free(r);
        } else {
            printf("Métrica desconocida: %s\n", metricas[i]);
        }
    }
}

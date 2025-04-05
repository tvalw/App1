#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "metricas.h"
#include "structs.h"

// Definir tipo de función para métricas
typedef char* (*MetricaFunction)(int*, Venta*);

// Estructura para asociar el nombre de una métrica con su función correspondiente
typedef struct {
    const char* nombre;
    MetricaFunction funcion;
} Metrica;

// Menú de ayuda
void mostrarMenu() {
    printf("\nUso del programa:\n");
    printf("./app1 <archivo.csv> <métrica1> <métrica2> ...\n");
    printf("\nMétricas disponibles:\n");
    printf("  pms   : Pizza más vendida\n");
    printf("  pls   : Pizza menos vendida\n");
    printf("  dms   : Fecha con más ventas (dinero)\n");
    printf("  dls   : Fecha con menos ventas (dinero)\n");
    printf("  dmsp  : Fecha con más pizzas vendidas\n");
    printf("  dlsp  : Fecha con menos pizzas vendidas\n");
    printf("  apo   : Promedio de pizzas por orden\n");
    printf("  apd   : Promedio de pizzas por día\n");
    printf("  ims   : Ingrediente más vendido\n");
    printf("  hp    : Cantidad de pizzas por categoría\n");
    printf("\nEjemplo de uso:\n");
    printf("  ./app1 ventas.csv pms apd hp\n\n");
}

int main(int argc, char *argv[]) {   // Si no se proporcionan suficientes argumentos, muestra el menú de ayuda y termina
    if (argc < 3) {
        mostrarMenu();
        return 1;
    }

    Venta ventas[1000]; // Arreglo para almacenar hasta 1000 ventas
    int size = leer_csv(argv[1], ventas); // Lee el archivo CSV pasado como argumento y llena el arreglo 'ventas'

    if (size == 0) {
        printf("Error: no se pudieron cargar los datos.\n");
        return 1;
    }

    // Lista de métricas disponibles
    Metrica metricas[] = {
        {"pms", pms},
        {"pls", pls},
        {"dms", dms},
        {"dls", dls},
        {"dmsp", dmsp},
        {"dlsp", dlsp},
        {"apo", apo},
        {"apd", apd},
        {"ims", ims},
        {"hp", hp},
    };

    int num_metricas = sizeof(metricas) / sizeof(metricas[0]); // Calcula cuántas métricas hay en la lista

    // Ejecutar todas las métricas solicitadas
    for (int i = 2; i < argc; i++) {
        int encontrada = 0;
        for (int j = 0; j < num_metricas; j++) {
            if (strcmp(argv[i], metricas[j].nombre) == 0) {
                char* resultado = metricas[j].funcion(&size, ventas);
                printf("Resultado de %s:\n%s\n\n", argv[i], resultado);
                encontrada = 1;
                break;
            }
        }
        if (!encontrada) { // Si no se encontró la métrica, muestra un mensaje de error
            printf("Métrica '%s' no reconocida.\n", argv[i]);
        }
    }

    return 0;
}

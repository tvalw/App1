#include <stdio.h>
#include <string.h>
#include "metricas.h"
#include "structs.h"

void mostrarMenu() {
    printf("\nUso del programa:\n");
    printf("./app1 <archivo.csv> <métrica>\n");
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
    printf("  ./app1 ventas.csv pms\n\n");
}
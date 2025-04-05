#ifndef METRICAS_H
#define METRICAS_H

#include "structs.h"

// Función que lee el archivo CSV y llena el arreglo de ventas
int leer_csv(const char *filename, Venta ventas[]);

// Función que calcula el promedio de pizzas por orden
char *apo(int *size, Venta *ventas);

// Función que calcula el promedio de pizzas por día
char *apd(int *size, Venta *ventas);

// Función que devuelve el ingrediente más vendido
char *ims(int *size, Venta *ventas);

// Función para Conteo de pizzas por categoría
char* hp(int *size, Venta *ventas);

#endif // METRICAS_H

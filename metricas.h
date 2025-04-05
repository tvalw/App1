#ifndef METRICAS_H
#define METRICAS_H

#include "structs.h"

// Función que lee el archivo CSV y llena el arreglo de ventas
int leer_csv(const char *filename, Venta ventas[]);

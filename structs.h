#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
    int pizza_id;
    int order_id;
    char pizza_name_id[100];
    int quantity;
    char order_date[20];
    char order_time[20];
    float unit_price;
    float total_price;
    char pizza_size[5];
    char pizza_category[50];
    char pizza_ingredients[200];
    char pizza_name[100];
} Venta;

// Definir tipo de función para métricas
typedef char* (*MetricaFunction)(int*, Venta*);

// Estructura para asociar el nombre de una métrica con su función correspondiente
typedef struct {
    const char* nombre;
    MetricaFunction funcion;
} Metrica;


#endif


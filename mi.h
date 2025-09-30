 
#ifndef MI_H
#define MI_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// creo un nuevo tipo de dato llamado tproducto es una estructura global que representa un archivo.
typedef struct tproducto {
    int codigo;
    char descripcion[50];
    float precio;
    int stock;
} tproducto;

typedef struct {
    int factura;
    int codigo;
    float precio;
    int cantidad;
} rVta;

//Prototipos de funciones

//limpia el buffer.
void limpiarBuffer(void);

//controla que sea un numero entero.
int esNumero(const char* cadena);

//controla que sea un numero flotante.
int esFlotante(const char* cadena);

//devuelvo un entero,funcion utilizada en qsort en la funcion ordeno_descripcion.
int compa(const void* a, const void* b);

/*recibe un arreglo de Tproducto y la cantidad de productos leidos del archivo,"Productos.dat" y muestra en pantalla los productos ordenado por su descripcion.*/
void ordeno_descripcion(tproducto* pt, int n);

/*recibe un arreglo de Tproducto y la cantidad de productos leidos del archivo,Productos.dat" y muestra en pantalla el registro de tproducto ordenado por codigo.*/
void ordeno_codigo(tproducto* pt, int n);

//devuelvo un entero,funcion utilizada en qsort en la funcion ordeno_codigo. 
int comparo(const void* a, const void* b);

/*devuelve s o n para la eliminacion de un producto! esta funcion es utilizada en la funcion Eliminar.*/
char borrar(tproducto* producto);

//graba las ventas en archivo "ventas.dat".
void grabarVenta(int* x, tproducto* producto, int* n);

//muestra todas las ventas que estan almacenada en el archivo "ventas.dat" 
void mostrarventa();

//busca una factura por su codigo.
void nfactura();

//devuelve verdadero o falso 1 o 0 si es que existe un producto la utilizo en la funcion agregar para controlar que no existan * dos productos con el mismo codigo.
int existe(int* x);

//Muestra un mensaje por pantalla y espera a que se presione una tecla.
void continuar();

//Muestra el producto y el total a pagar de la compra realizada descuenta el stock.
void factura();

//Agrega nuevo registro al final del archivo.
void Agregar();

//Muestra el listado completo del archivo, si no se puede abrir retorno 1.
void Listado();

//Permite consultar un producto por su codigo si es igual lo muestra por pantalla.
void Consultar();

//Borra un registro completo, hace una copia en otro archivo llamado "prodaux.dat" luego se elimina el archivo.
void Eliminar();

//Modifica el precio o el stock de un producto.
void Modificar();

#endif

 

#include <stdio.h>
#include <stdlib.h>
#include "mi.h"  

 

int main() {
    char opcion;

    do {
        printf("|----------------------------------------|\n");
        printf("|   SISTEMA DE VENTAS - TIENDA v1.0      |\n");
        printf("|----------------------------------------|\n");
        printf("| 1 - Cargar productos al archivo        |\n");
        printf("| 2 - Lista de los productos             |\n");
        printf("| 3 - Consulta de un producto por codigo |\n");
        printf("| 4 - Modificación de precio o stock     |\n");
        printf("| 5 - Facturar                           |\n");
        printf("| 6 - Eliminar productos                 |\n");
        printf("| 7 - Listado de facturas                |\n");
        printf("| 8 - Buscar factura por su codigo       |\n");
        printf("| 9 - Finalizar                          |\n");
        printf("|----------------------------------------|\n");
        printf("Ingrese el numero de opcion: ");

        opcion = getchar();         // Lee la opción
        limpiarBuffer();            // Limpia el buffer

        switch (opcion) {
        case '1': Agregar(); break;
        case '2': Listado(); break;
        case '3': Consultar(); break;
        case '4': Modificar(); break;
        case '5': factura(); break;
        case '6': Eliminar(); break;
        case '7': mostrarventa(); break;
        case '8': nfactura(); break;
        case '9':
            printf("\nFinalizando el programa. ¡Hasta luego!\n");
            break;
        default:
            printf("\n Opcion incorrecta: '%c'. Intente nuevamente.\n\n", opcion);
            break;
        }

    } while (opcion != '9');

    return 0;
}

#define _CRT_SECURE_NO_WARNINGS
#include "mi.h"

//FUNCIONES


void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int esNumero(const char* cadena) {
    int i = 0;

    // Si la cadena está vacía, no es un número
    if (cadena[0] == '\0' || cadena[0] == '\n')
        return 0;

    // Si comienza con signo negativo, avanzar al siguiente carácter
    if (cadena[0] == '-') {
        i = 1;
        // Si solo hay un signo sin número, no es válido
        if (cadena[1] == '\0' || cadena[1] == '\n')
            return 0;
    }

    for (; cadena[i] != '\0' && cadena[i] != '\n'; i++) {
        if (!isdigit((unsigned char)cadena[i])) {
            return 0;
        }
    }

    return 1;
}

int esFlotante(const char* cadena) {
    int i = 0, punto = 0;

    if (cadena[0] == '-' || cadena[0] == '+') i++;

    int digitoEncontrado = 0;

    for (; cadena[i] != '\0' && cadena[i] != '\n'; i++) {
        if (isdigit((unsigned char)cadena[i])) {
            digitoEncontrado = 1;
        }
        else if (cadena[i] == '.') {
            if (punto) return 0;
            punto = 1;
        }
        else {
            return 0;
        }
    }

    return digitoEncontrado;
}



//Muestra un mensaje por pantalla y espera a que se presione una tecla, la uso al final de las funciones
void continuar() {
    printf("\nPresione una tecla para continuar...");
    getchar();
}


//carga de dato de tipo producto al archivo binario, si no se puede abrir dicho archivo retorno un 1. 
void Agregar() {
    char s = 's';
    int i = 0;   
    int count = 0;
    char valor[100];
    tproducto producto;
    FILE* archivo = fopen("Productos.dat", "ab");//abro el archivo para agregar.
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo.\n");
        exit(1);
    }

    //grabo registro de tproducto,pido los datos al usuario para poder cargar los campos.
    while (s == 's') {
        // Leer y validar código de barra
        while (1) {
            
            printf("\nIngrese el código de barra del producto: ");
            fgets(valor, sizeof(valor), stdin);
            valor[strcspn(valor, "\n")] = 0;
            int codigoTemp = atoi(valor); //convierto el valor ingresado a un entero.

            //controlo que sea un numero valido.
            if (esNumero(valor) && codigoTemp >= 0) {
                sscanf(valor, "%d", &producto.codigo);
                 
                if (existe(&producto.codigo) || (codigoTemp == count)) {
                    printf("Error: el producto ya existe.\n");
                    continue;
                }
                count = codigoTemp;//guardo en count el valor del codigo de barra para controlar que no vuelva a escribir otro producto con el mismo codigo ya que no cierro el archivo para volver a consultar si existe el ncodigo.
                break;
            }
            else {
                printf("Error: debe ingresar un numero entero valido.\n");
            }
        }

        //Leer nombre
        printf("Ingrese el nombre del producto: ");
        fgets(producto.descripcion, sizeof(producto.descripcion), stdin);
        producto.descripcion[strcspn(producto.descripcion, "\n")] = '\0';

        //Leer y validar precio
        while (1) {
            printf("Ingrese el precio: ");
            fgets(valor, sizeof(valor), stdin);
            valor[strcspn(valor, "\n")] = 0;
            float precioTemp = atof(valor);//convierto el valor ingresado a un float.

            //control de un numero valido.
            if (esFlotante(valor) && precioTemp >= 0.0f) {
                sscanf(valor, "%f", &producto.precio);
                break;
            }
            else {
                printf("Error: debe ingresar un numero valido (puede incluir decimales).\n");
            }
        }

        //Leer y validar stock
        while (1) {
            printf("Ingrese el stock: ");
            fgets(valor, sizeof(valor), stdin);
            valor[strcspn(valor, "\n")] = 0;
            int stockTemp = atoi(valor);//convierto el valor ingresado a un entero.

            //control de un numero valido.
            if (esNumero(valor) && stockTemp >= 0) {
                sscanf(valor, "%d", &producto.stock);
                break;
            }
            else {
                printf("Error: debe ingresar un numero entero valido.\n");
            }
        }

        //Guardo los datos ingresados en archivo.
        fwrite(&producto, sizeof(tproducto), 1, archivo);
        i++;
         
        //Pregunta si desea continuar
        printf(" ¿Agregar más productos? [s/n]: ");
        
        s = tolower(getchar());
        limpiarBuffer();
         
         
         
    }

    printf("\nSe agregaron [%i] productos al archivo!\n", i);
    fclose(archivo);
    continuar();
    system("cls");
}

//funcion que muestra el listado completo, si no se puede abrir muestro un mensaje y retorno 1.
void Listado() {
    FILE* archivo = fopen("Productos.dat", "rb");//abro el archivo en modo lectura.
    if (archivo == NULL) {
        printf("ERROR!!! El archivo no existe!\n");
        continuar();
        return;
    }

    tproducto producto, * tp = malloc(sizeof(tproducto) * 1000);
    int i = 0, resp;

    while (fread(&producto, sizeof(tproducto), 1, archivo)) {
        tp[i++] = producto;
    }

    printf("%-8s | %-20s | %-10s | %-6s\n", "Codigo", "Descripcion", "Precio", "Stock");

    for (int j = 0; j < i; j++) {
        printf("%-8d | %-20s | %-10.2f | %-6d\n", tp[j].codigo, tp[j].descripcion, tp[j].precio, tp[j].stock);
    }

    printf("\nIngrese 1 para ordenar por codigo o 2 por descripcion: ");
    scanf("%d", &resp);
    limpiarBuffer();

    if (resp == 1)
        ordeno_codigo(tp, i);
    else
        ordeno_descripcion(tp, i);

    free(tp);
    fclose(archivo);//cierro el archivo
    continuar();
    system("cls");
}

//funcion para consultar un producto por su codigo, si no se puede abrir el achivo retrono 1,si no existe el registro ingresado por teclado muestro un msj por pantalla.
void Consultar() {
    char s = 's';
    int codi, existe = 0;    //existe lo utiliso como bandera para saber si se encontro un producto con el mismo codigo que se ingreso por teclado
    tproducto producto;

    while (s == 's') {
        FILE* archivo = fopen("Productos.dat", "rb");
        if (archivo == NULL)
            exit(1);

        printf("\nIngrese el codigo de barra del producto a consultar: ");
        scanf("%d", &codi);
        limpiarBuffer();

        while (fread(&producto, sizeof(tproducto), 1, archivo)) {
            //comparo el codigo que ingreso por teclado con el codigo de los registro si lo encuentra lo muestra ponde la bandera en 1 salgo del while con break.
            if (codi == producto.codigo) {
                printf("|| %d %s $%.2f stock [%d]\n", producto.codigo, producto.descripcion, producto.precio, producto.stock);
                existe = 1;
                printf("\nConsultar otro Producto? [s/n]: ");
                s = getchar();
                limpiarBuffer();
                break;
            }
        }

        if (!existe) {
            printf("\nERROR!!! No existe un producto con el codigo: ||%d\n", codi);
            s = 'n';
        }

        fclose(archivo);
    }

    continuar();
    system("cls");
}


//funcion que modifica el precio de un producto y el stock almacenado en el archivo Productos.dat
void Modificar() {
    int codi, existe;
    char s = 's';
    char valor[100];
    int t;
    tproducto producto;

    while (s == 's') {
        FILE* archivo = fopen("Productos.dat", "r+b");//abro el archivo en modo lectura y escritura.
        if (archivo == NULL)
            exit(1);

        printf("\nIngrese el codigo de barra del producto a modificar: ");
        scanf("%d", &codi);
       
        limpiarBuffer();

        existe = 0;

        while (fread(&producto, sizeof(tproducto), 1, archivo)) {

            //Si encuentro el producto muestro por pantalla y pido el precio para modificarlo.
            if (codi == producto.codigo) {
                printf("|| %d %s $%.2f stock [%d]\n", producto.codigo, producto.descripcion, producto.precio, producto.stock);

             while(1){
                printf("\nModificar el precio? [s/n]: ");
                s = getchar();
                limpiarBuffer();

                if (s == 's') {
                    printf("Nuevo precio: ");
                    fgets(valor, sizeof(valor), stdin);
                    valor[strcspn(valor, "\n")] = 0;
                    float precioTemp = atof(valor);

                    if (esFlotante(valor) && precioTemp >= 0.0f) {
                        sscanf(valor, "%f", &producto.precio);
                        break;

                    }
                    else {
                        printf("Error: debe ingresar un numero valido (puede incluir decimales).\n");
                        continue;
                    }
                 } 
                  
                 break;
                   
             }

             while (1) {
                 printf("Agregar stock? [s/n]: ");
                 s = getchar();
                 limpiarBuffer();

                 if (s == 's') {
                     printf("Cantidad a agregar: ");
                     fgets(valor, sizeof(valor), stdin);
                     valor[strcspn(valor, "\n")] = 0;
                     int stockTemp = atoi(valor);

                     if (esNumero(valor) && stockTemp >= 0) {
                         //sscanf(valor, "%d", &producto.stock);
                         producto.stock += stockTemp;
                          
                         break;
                     }
                     else {
                         printf("Error: debe ingresar un número entero válido.\n");
                         continue;
                     }     
                 }
                 break;
             }      
                 
                fseek(archivo, -(long)sizeof(tproducto), SEEK_CUR);
                fwrite(&producto, sizeof(tproducto), 1, archivo);
                existe = 1; //cambio el estado de la bandera y salgo del while con la instrucion break.
                break;
            }
        }

        fclose(archivo);

        if (!existe)
            printf("\nERROR! No existe producto con el codigo: ||%d\n", codi);

        printf("\nModificar otro? [s/n]: ");
        s = getchar();
        limpiarBuffer();
    }

    continuar();
    system("cls");
}


//Muestra los productos ingresado a vender,muestra el producto y el total por cada porducto y el total de la sumatoria de los producos!
void factura() {
    int x, codi, fac, fa;
    char c = 's';
    char valor[100];
    float total = 0, aux = 0;
    tproducto producto;

    while (1) {
        printf("Numero de factura: ");
        fgets(valor, sizeof(valor), stdin);
        valor[strcspn(valor, "\n")] = 0;
        int codigofac = atoi(valor);

        if (esNumero(valor) && codigofac>= 0) {
            sscanf(valor, "%d", &fac);
            break;
        }  
        else {
            printf("Error: debe ingresar un numero entero valido.\n");
        }
    }
      
    //limpiarBuffer();

    while (c == 's') {
        FILE* archivo = fopen("Productos.dat", "r+b");
        if (archivo == NULL)
            exit(1);

        while (1) {
            printf("\nCodigo de barra: ");
            fgets(valor, sizeof(valor), stdin);
            valor[strcspn(valor, "\n")] = 0;
            int codigoTemp = atoi(valor);

            if (esNumero(valor) && codigoTemp >= 0) {
                sscanf(valor, "%d", &codi);
                break;
            }
            else {
                printf("Error: debe ingresar un numero entero valido.\n");
            }
             
            limpiarBuffer();

        }
        int existe = 0;

        while (fread(&producto, sizeof(tproducto), 1, archivo)) {
            if (codi == producto.codigo) {
                printf("|| %d %s $%.2f stock [%d]\n", producto.codigo, producto.descripcion, producto.precio, producto.stock);

                while (1) {
                    printf("Cantidad: ");
                    fgets(valor, sizeof(valor), stdin);
                    valor[strcspn(valor, "\n")] = 0;
                    int codigocan = atoi(valor);

                    if (esNumero(valor) && codigocan>= 0) {
                        sscanf(valor, "%d", &x);
                        if (producto.stock == 0 || x > producto.stock) {
                            printf("ERROR: stock insuficiente.\n");
                            fclose(archivo);
                            producto.precio = 0;
                             
                        }
                         break;
                    }
                    else {
                        printf("Error: debe ingresar un numero entero valido.\n");
                    }


                     
                    limpiarBuffer();

                    
                }
                producto.stock -= x;
                grabarVenta(&fac, &producto, &x);

                fseek(archivo, -(long)sizeof(tproducto), SEEK_CUR);
                fwrite(&producto, sizeof(tproducto), 1, archivo);

                existe = 1;
                break;
            }
        }

        fclose(archivo);

        if (!existe)
            printf("ERROR: producto no encontrado.\n");
        else {
                total = producto.precio * x;
                aux += total;

                printf("Subtotal: $%.2f - %s\n", total, producto.descripcion);
            }

        printf("Agregar más productos? [s/n]: ");
        c = getchar();
        limpiarBuffer();
    }

    printf("\nTOTAL A PAGAR: $%.2f\n", aux);
    continuar();
    system("cls");
}


//Elimina un registro de tipo tproducto del archivo utiliza un archivo auxiliar para hacer la copia.
void Eliminar() {
    char s = 's';
    int x;
    tproducto producto;

    while (s == 's') {
        FILE* f = fopen("Productos.dat", "rb");
        FILE* faux = fopen("prodaux.dat", "wb");
        if (!f || !faux) exit(1);

        printf("\nCodigo de barra a borrar: ");
        scanf("%d", &x);
        limpiarBuffer();

        while (fread(&producto, sizeof(tproducto), 1, f)) {
            if (producto.codigo == x && borrar(&producto) == 'n')
                fwrite(&producto, sizeof(tproducto), 1, faux);
            else if (producto.codigo != x)
                fwrite(&producto, sizeof(tproducto), 1, faux);
        }

        fclose(f);
        fclose(faux);

        f = fopen("Productos.dat", "wb");
        faux = fopen("prodaux.dat", "rb");

        while (fread(&producto, sizeof(tproducto), 1, faux))
            fwrite(&producto, sizeof(tproducto), 1, f);

        fclose(f);
        fclose(faux);
        remove("prodaux.dat");

        printf("Borrar otro producto? [s/n]: ");
        s = getchar();
        limpiarBuffer();
    }

    continuar();
    system("cls");
}


//Graba las ventas en archivo ventas.dat recibe 3 parametros de la funcion factura, recibe el numero de factura,el producto y la cantidad.
void grabarVenta(int* x, tproducto* producto, int* n) {
    rVta vta;
    FILE* archivo = fopen("ventas.dat", "ab");
    if (!archivo) exit(1);

    vta.factura = *x;
    vta.codigo = producto->codigo;
    vta.precio = producto->precio;
    vta.cantidad = *n;

    fwrite(&vta, sizeof(rVta), 1, archivo);
    fclose(archivo);
}


//Muestras las ventas que existen en el archivo ventas.dat.
void mostrarventa() {
    rVta vta;
    FILE* archivo = fopen("ventas.dat", "rb");
    if (!archivo) {
        printf("No hay archivo de ventas\n");
        continuar();
        return;
    }

    printf("Factura | Codigo | Precio | Cantidad\n");

    while (fread(&vta, sizeof(rVta), 1, archivo)) {
        printf("%d\t%d\t%.2f\t%d\n", vta.factura, vta.codigo, vta.precio, vta.cantidad);
    }

    fclose(archivo);
    continuar();
    system("cls");
}
//busca una factura y la muestra en pantalla y tambien muestra el total de la factura  por medio de una suma, si es que existe la factura ingresada  por teclado 
void nfactura() {
     
        rVta vta;
        int x = 0;
        float r, t = 0;
        FILE* p = fopen("ventas.dat", "rb");   
        if (p == NULL) {
            printf("ERROR al abrir el archivo\n");
            exit(1);
        }

        printf("Ingrese número de factura: ");
        scanf("%i", &x);
        limpiarBuffer();  

        while (fread(&vta, sizeof(rVta), 1, p) == 1) {
            if (x == vta.factura) {
                printf("\nFactura: %d | Código: %d\nPrecio: $%.2f | Cantidad: %d\n",
                    vta.factura, vta.codigo, vta.precio, vta.cantidad);
                r = vta.precio * vta.cantidad;
                t += r;
            }
        }

        if (t == 0) {
            printf("No se encontró la factura número %d\n", x);
        }
        else {
            printf("\nEl Total de la factura es: $%.2f\n", t);
        }

        fclose(p);
        continuar();
        system("cls"); // En Linux o Mac usá: system("clear");
    }

 
//devuelve verdadero o falso si es que existe un producto
//La utilizo en la funcion agregar si me retorna 1 quiere decir que existe el producto
//de esa manera no permito agregar al archivo dos productos con el mismo codigo.
int existe(int* x) {
    FILE* archivo = fopen("Productos.dat", "rb");
    tproducto producto;

    if (archivo) {
        while (fread(&producto, sizeof(tproducto), 1, archivo)) {
            if (*x == producto.codigo) {
                printf("Producto ya existe: ||%d %s\n", producto.codigo, producto.descripcion);
                fclose(archivo);
                return 1;
            }
        }
        fclose(archivo);
    }

    return 0;
}

//funcion que recibe como parametro un producto de la funcion Eliminar lo muestra por pantalla
//devuelve s o n a la funcion Eliminar para saber si desea eliminar el producto ingresado. 	  
char borrar(tproducto* producto) {
    char s;
    printf("Producto encontrado: ||%d %s\n", producto->codigo, producto->descripcion);
    printf("Eliminar este producto?  [s/n]: ");
    s = getchar();
    limpiarBuffer();
    return s;
}

//Comparador por descripcion.
int compa(const void* a, const void* b) {
    tproducto* p1 = (tproducto*)a;
    tproducto* p2 = (tproducto*)b;
    return strcmp(p1->descripcion, p2->descripcion);
}

//Comparador por codigo.
int comparo(const void* a, const void* b) {
    tproducto* p1 = (tproducto*)a;
    tproducto* p2 = (tproducto*)b;
    return p1->codigo - p2->codigo;
}

//Ordenar productos por descripcion.
void ordeno_descripcion(tproducto* pt, int n) {
    qsort(pt, n, sizeof(tproducto), compa);
    printf("\nOrdenado por descripcion:\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t%s\t%.2f\t%d\n", pt[i].codigo, pt[i].descripcion, pt[i].precio, pt[i].stock);
    }
}

//Ordenar productos por codigo.
void ordeno_codigo(tproducto* pt, int n) {
    qsort(pt, n, sizeof(tproducto), comparo);
    printf("\nOrdenado por codigo:\n");

    for (int i = 0; i < n; i++) {
        printf("%d\t%s\t%.2f\t%d\n", pt[i].codigo, pt[i].descripcion, pt[i].precio, pt[i].stock);
    }
}

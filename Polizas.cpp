//***************DIRECTIVAS DEL PREPROCESADOR********************

#include <iostream>
#include <conio.h>
#include <locale.h>
#include <string.h>
#include <stdio.h>
#define ASEGURADOS "Asegurados.bak"
#define INCIDENTES "Incidentes.bak"
#define AUX "Auxiliar.bak"
#define PROCESADOS "Procesados.bak"
#define CONTADOR "Contador.bak"
#define DIM 2000

using namespace std;

//**************************ESTRUCTURAS**************************

struct Poliza {
    int nropoliza;
    int cantincidentes;
    char dni[11];
    char nombre[20], apellido[20];
    char patente[10];
    char cuotaaldia;
    bool activa=false;
};

struct Incidente {
    int nroPoliza;
    int codigo;
    char fechaHora [15];
    char dniAsegurado [11];
    char dniOtroConductor[11];
    char calle [20];
    int altura;
};


//******************PROTOTIPO DE SUBPROGRAMAS********************

char menuDeOpciones();
int contadorPolizas();
void mostrarPoliza(Poliza);
int validarPoliza(int);
void levantarPolizas();           //opción 1
void cargarPoliza();              //opción 2
void desactivarPoliza();          //opción 3
void buscarPorNroPoliza();        //opción 4
void buscarPorDNI();              //opción 5
void listarPolizas();             //opción 6
void crearIncidente();            //opción 7
void procesarIncidentes();        //opción 7
void finalizarJornada();          //opción 8


//*******************SUBPROGRAMA PRINCIPAL***********************

int main() {
    setlocale(LC_ALL, "Spanish");

    char opcion;
    do {
        opcion = menuDeOpciones();
    } while (opcion != 27);

  return 0;
}


//******************DEFINICIÓN DE SUBPROGRAMAS******************

char menuDeOpciones() {
    char opcion;

    cout << "\t--- MENU DE OPCIONES ---\n" << endl;
    cout << "1 - Mostrar información de las pólizas" << endl;
    cout << "2 - Cargar una nueva póliza" << endl;
    cout << "3 - Desactivar una póliza existente" << endl;
    cout << "4 - Buscar por N° de Póliza" << endl;
    cout << "5 - Buscar por N° de DNI" << endl;
    cout << "6 - Listar todas las pólizas activas ordenadas por cantidad de incidentes descendente" << endl;
    cout << "7 - Ingresar incidente" << endl;
    cout << "8 - Finalizar jornada (Borrar las pólizas inactivas)" << endl;
    cout << "Esc - Salir\n\n" << endl;

    do {
        opcion = getch();
    } while (opcion!=27 && !(opcion>='1' && opcion<='9'));

    switch(opcion) {
        case '1':
            cout << "\t--- MOSTRAR INFORMACIÓN DE LAS PÓLIZAS --- " << endl;
            levantarPolizas();
            break;
        case '2':
            cout << "\t--- CARGA DE NUEVA PÓLIZA --- " << endl;
            cargarPoliza();
            break;
        case '3':
            cout << "\t--- DESACTIVAR PÓLIZA --- " << endl;
            desactivarPoliza();
            break;
        case '4':
            cout << "\t--- BÚSQUEDA POR NÚMERO DE PÓLIZA --- " << endl;
            buscarPorNroPoliza();
            break;
        case '5':
            cout << "\t--- BÚSQUEDA POR NÚMERO DE DNI --- " << endl;
            buscarPorDNI();
            break;
        case '6':
            cout << "\t--- LISTAR POLIZAS POR CANTIDAD DE INCIDENTES DESCENDENTE --- " << endl;
            listarPolizas();
            break;
        case '7':
            cout << "\t--- CREAR UN INCIDENTE ---  " << endl;
            crearIncidente();
            procesarIncidentes();
            break;
        case '8':
            cout << "\t--- FINALIZAR JORNADA ---" << endl;
            finalizarJornada();
            break;
    }
    if(opcion!=27){
        cout << "\nPresione una tecla para continuar" << endl;
        getch();
    }
    system("cls");

    return opcion;
}

//Opción 1 - Muestra información de las pólizas cargadas.
void levantarPolizas(){
    FILE *asegurados;
    Poliza p;
    asegurados=fopen(ASEGURADOS,"rb");
    int cantPolizas=0, cantCuotasDeudoras=0, cantActivos=0, cantInactivos=0;

    while(fread(&p,sizeof(Poliza),1,asegurados)){
        cantPolizas++;
        if(p.cuotaaldia=='f'||p.cuotaaldia=='F')
            cantCuotasDeudoras++;
        if(p.activa)
            cantActivos++;
        else
            cantInactivos++;
    }

    cout << "Cantidad de Polizas: " << cantPolizas<<endl;
    cout << "Cantidad de Activas: " << cantActivos<<endl;
    cout << "Cantidad de Inactivas: " << cantInactivos<<endl;
    cout << "Cantidad de Polizas impagas: " << cantCuotasDeudoras<<endl;

    fclose(asegurados);
}


//Opción 2 - Ingresar una nueva póliza
void cargarPoliza() {
    FILE *asegurados;
    Poliza p;
    bool cargaexitosa=false;

    if ((asegurados = fopen(ASEGURADOS,"ab"))) {

        p.nropoliza=contadorPolizas();

        p.cantincidentes=0;

        fflush(stdin);
        cout << "DNI (Sin puntos): ";
        cin.getline(p.dni,11,'\n');

        fflush(stdin);
        cout << "Nombre: ";
        cin.getline(p.nombre,20,'\n');

        fflush(stdin);
        cout << "Apellido: ";
        cin.getline(p.apellido,20,'\n');

        fflush(stdin);
        cout << "Patente: ";
        cin.getline(p.patente,10,'\n');

        cout << "Cuota al día (Ingrese V o F): ";
        do {
            p.cuotaaldia=getch();
        } while(p.cuotaaldia!='V' && p.cuotaaldia!='F' && p.cuotaaldia!='v' && p.cuotaaldia!='f');
        cout << p.cuotaaldia;

        p.activa=true;

        fwrite(&p, sizeof(Poliza), 1, asegurados);
        cargaexitosa = true;
        fclose(asegurados);
    }

    if(cargaexitosa)
        cout << "\n\nSe han guardado los datos exitosamente." << endl;
    else
        cout << "\n\nNo se han podido guardar los datos." << endl;
}


//Opción 3 - Desctivar póliza
void desactivarPoliza() {
    FILE *asegurados;
    int encontrado=0;
    int nropoliza;
    Poliza p;

    if ((asegurados = fopen (ASEGURADOS,"rb+"))) {
        cout << "Ingrese el número de póliza que desea desactivar: ";
        cin >> nropoliza;
        while (!encontrado && fread (&p, sizeof(Poliza), 1, asegurados)) {
            if (nropoliza == p.nropoliza) {
                encontrado = 1;
                p.activa = false;
                fseek (asegurados, (-1)*sizeof(Poliza), SEEK_CUR);
                fwrite (&p, sizeof(Poliza), 1, asegurados);
            }
        }
        if (!encontrado)
            cout << "No se encontro la póliza a borrar." << endl;
        else if (encontrado && !p.activa)
            cout << "\nLa póliza número " << nropoliza << " se ha desactivado satisfactoriamente." << endl;
        fclose(asegurados);
    } else {
        cout << "Error al intentar abrir el archivo" << endl;
    }
}

//Opción 4 - Búsqueda secuencial por número de póliza
void buscarPorNroPoliza() {
    FILE *asegurados;
    int polizabuscada;
    int encontrado=0;
    Poliza p;

    if ((asegurados = fopen(ASEGURADOS, "rb"))) {
        cout << "\nIngrese el N° de póliza que desea buscar: ";
        cin >> polizabuscada;
        cout << endl;

        while ((fread (&p, sizeof(Poliza), 1, asegurados) && encontrado==0)) {
            if (polizabuscada == p.nropoliza) {
                mostrarPoliza(p);
                encontrado = 1;
            }
        }
        if (!encontrado)
            cout << "No se encontró la póliza buscada.\n" << endl;
        fclose(asegurados);
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }
}


//Opción 4 - Búsqueda secuencial por número de DNI
void buscarPorDNI() {
    FILE *asegurados;
    char dnibuscado[11];
    int encontrado=0;
    Poliza p;

    if ((asegurados = fopen(ASEGURADOS,"rb"))) {
        fflush(stdin);
        cout << "\nIngrese el DNI que desea buscar: ";
        cin.getline(dnibuscado,11,'\n');
        cout << endl;

        while(fread(&p,sizeof(Poliza),1,asegurados)) {
            if (strcmp(dnibuscado, p.dni)== 0) {
                mostrarPoliza(p);
                encontrado = 1;
            }
        }
        if (!encontrado)
            cout << "No se encontró la póliza buscada.\n" << endl;
        fclose(asegurados);
    } else {
        cout << "No se pudo abrir el archivo." << endl;
    }
}


//Subprograma usado en opción 6 - Burbujeo de pólizas para ordenarlas descendentemente por cantidad de incidentes.
void burbujeo(Poliza arr[], int n){
    int i, j;
    Poliza aux;
    for (i = 0; i < n-1; i++)
        for (j = 0; j < n-1-i; j++)
            if ((arr[j].cantincidentes < arr[j+1].cantincidentes)) {
                aux = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = aux;
            }
}

//Subprograma usado en opción 6 - Muestra los datos de la póliza e índice enviados por parámetro.
void imprimirArreglo(Poliza arr[], int n){
    int i;
    for (i = 0; i < n; i++){
        cout << "Número de Poliza: " << arr[i].nropoliza << endl;
        cout << "Cantidad de Incidentes: " << arr[i].cantincidentes <<endl;
        cout << "DNI: " << arr[i].dni << endl;
        cout << "Nombre: " << arr[i].nombre << endl;
        cout << "Apellido: " << arr[i].apellido << endl;
        cout << "Patente: " << arr[i].patente << endl;
        cout << "Cuota al día: " << ((arr[i].cuotaaldia=='v' || arr[i].cuotaaldia=='V') ? "Si" : "No") << endl;
        cout << "Activo: " << (arr[i].activa ? "Si" : "No") << endl;
        cout << " " << endl;
    }
}

//Opción 6 - Lista todas las pólizas activas ordenadas por cantidad de incidentes descendente
void listarPolizas(){
    FILE *asegurados;
    Poliza p, arr[DIM];
    int i = 0;
    if ((asegurados = fopen(ASEGURADOS,"rb"))) {
        while(fread(&p,sizeof(Poliza),1,asegurados)){
            arr[i] = p;
            i++;
        }
    burbujeo(arr,i);
    imprimirArreglo(arr,i);
    fclose(asegurados);

    } else
        cout << "No se pudo abrir el archivo" << endl;
}

//Opción 7 - Crea un incidente en el número de póliza que se ingrese.
void crearIncidente(){
    FILE *incidentes;
    Incidente i;
    bool cargaExitosa = false;
    char opcion = 0;
    char archivo [30] = INCIDENTES;
    int polizaIngresada;
    cout << "Número de Póliza: ";
    cin >> polizaIngresada;
    fflush(stdin);
    int polizaValida = validarPoliza(polizaIngresada);
    if(polizaValida==1) {
        cout << "La Poliza ingresada no esta asegurada" << endl;
        return;
    } else if(polizaValida == 2) {
        cout << "No hay polizas aseguradas " << endl;
        return;
    }
    
    if ((incidentes = fopen(archivo,"ab"))){
        i.nroPoliza = polizaIngresada;
        system("cls");
        cout << "    --- INGRESAR UN INCIDENTE --- "<< endl;
        cout << "Número de póliza: " << polizaIngresada;

        cout << "\nCódigo de Incidente: ";
        cin >> i.codigo;

        fflush(stdin);
        cout << "Fecha y hora (AAAAMMDDHH:MM) : ";
        cin.getline(i.fechaHora,15,'\n');

        fflush(stdin);
        cout << "DNI del Asegurado (Sin puntos): ";
        cin.getline(i.dniAsegurado,11,'\n');

        fflush(stdin);
        cout << "DNI del otro Conductor (Sin puntos): ";
        cin.getline(i.dniOtroConductor,11,'\n');

        fflush(stdin);
        cout << "Calle: ";
        cin.getline(i.calle,20,'\n');

        fflush(stdin);
        cout << "Altura: ";
        cin >> i.altura;

        fwrite(&i, sizeof(Incidente), 1, incidentes);
        cargaExitosa = true;
    } else
        cout << "No se pudo abrir el archivo Incidentes.bak" << endl;

    if(!cargaExitosa)
        cout << "No se pudo cargar el incidente" << endl;
    fclose(incidentes);
}


//Subprograma usado en procesarIncidentes() - Búsqueda binaria
bool actualizacionDePoliza(int nroPoliza,Poliza arr[], int tam){
    FILE *asegurados;
    int sup=tam-1;
    bool encontro = false;
    int j = 0;
    int medio;
    while(!encontro && j<=sup && nroPoliza<=arr[tam-1].nropoliza ) {
        medio=(sup+j)/2;
        if (nroPoliza == arr[medio].nropoliza){
            arr[medio].cantincidentes++;
            encontro = true;
        }else{
            if(nroPoliza<arr[medio].nropoliza)
                sup=medio-1;
            else
                j=medio+1;
        }
    }

    if((asegurados=fopen(ASEGURADOS,"rb+"))){
        fseek(asegurados,0,SEEK_SET);
        fwrite(arr, sizeof(Poliza)*tam, 1, asegurados);
        fclose(asegurados);

        return true;
    }else
        return false;
}

//Subprograma usado en procesarIncidentes() - Guarda en el archivo Procesados.bak el historial de incidentes.
void asentarIncidente(Incidente i){
    FILE *procesados;
    if((procesados = fopen(PROCESADOS,"ab")))
        fwrite(&i, sizeof(Incidente), 1, procesados);
    else 
        cout << "No se pudo abrir el archivo Procesados.bak" << endl;
    fclose(procesados);
}

//Opción 7 - Actualiza el archivo de pólizas con los incidentes ingresados
void procesarIncidentes(){
    FILE *asegurados;
    Poliza p, arr[DIM];
    bool actualizado=false;
    int k = 0;
    if ((asegurados = fopen(ASEGURADOS,"rb+"))) {
        while(fread(&p,sizeof(Poliza),1,asegurados)){
            arr[k] = p;
            k++;
        }
    }else{
         cout << "No se pudo abrir el archivo Asegurados.bak" << endl;
         return;
    }
    fclose(asegurados);
    
    FILE *incidentes;
    char archivo [30] = INCIDENTES;
    Incidente i;
    if ((incidentes = fopen(archivo,"rb"))){
        while(fread(&i,sizeof(Incidente),1,incidentes)){
            if(actualizacionDePoliza(i.nroPoliza,arr, k)){
                asentarIncidente(i);
                actualizado=true;
            }else
                cout<<"Error al actualizar incidentes."<<endl;
        }
    }else
        cout << "No hay incidentes para procesar" << endl;

    if(actualizado)
        cout<<"El proceso de incidentes finalizó correctamente"<<endl;

    fclose(incidentes);
    remove(archivo);
}


//Opción 8 - Sobreescribe el archivo Asegurados.bak y conserva sólo las pólizas activas
void finalizarJornada(){
    FILE *auxiliar;
    FILE *asegurados;
    Poliza p;
    auxiliar=fopen(AUX,"wb");

    if((asegurados=fopen(ASEGURADOS,"rb+"))){
        while(fread(&p,sizeof(Poliza),1,asegurados))
            if(p.activa)
                fwrite(&p,sizeof(Poliza),1,auxiliar);
        fclose(asegurados);
    } else
        cout << "No se pudo abrir el archivo Asegurados.bak" << endl;

    fclose(auxiliar);

    if (!remove(ASEGURADOS)) {
        if(!rename(AUX,ASEGURADOS))
            cout << "La jornada se ha finalizado correctamente" << endl;
        else
            cout << "Error al actualizar el archivo" << endl;
    }
    else
        cout << "Error al actualizar el archivo" << endl;
}

//Permite enumerar las pólizas de forma creciente desde el 1 sin que se repitan aunque sean borradas
int contadorPolizas () {
    FILE *contador=fopen(CONTADOR,"rb+");
    unsigned int polizasTotales;

    if (!contador)
        fopen(CONTADOR,"wb");

    contador=fopen(CONTADOR,"rb+");
    if(ftell(contador)==0)
        polizasTotales=1;

    if (fread (&polizasTotales, sizeof(unsigned int), 1, contador)){
        fseek (contador, -sizeof(unsigned int), SEEK_END);
        polizasTotales++;
    }

    fwrite(&polizasTotales, sizeof(unsigned int), 1, contador);

    fclose(contador);
    return polizasTotales;
}

//Muestra los datos de la póliza enviada por parámetro
void mostrarPoliza (Poliza p) {
    cout << "\t****** Datos de la póliza *******\n" << endl;
    cout << "Número de póliza: " << p.nropoliza << endl;
    cout << "Cantidad de incidentes: " << p.cantincidentes << endl;
    cout << "DNI: " << p.dni << endl;
    cout << "Nombre: " << p.nombre << endl;
    cout << "Apellido: " << p.apellido<< endl;
    cout << "Patente: " << p.patente<< endl;
    cout << "Cuota al día: " << ((p.cuotaaldia=='v' || p.cuotaaldia=='V') ? "Si" : "No") << endl;
    cout << "Activo: " << (p.activa ? "Si" : "No") << endl;
    cout << endl;
}

//Valida si el número de póliza enviada por parámetro existe en el archivo.
//Devuelve 0 si encuentra la póliza, 1 si el archivo existe y la póliza no se encontró, 2 si el archivo no existe o no se pudo abrir.
int validarPoliza(int polizaIngresada){
    bool polizaValida=false;
    FILE *asegurados;
    Poliza p;
    if((asegurados = fopen(ASEGURADOS, "rb"))){
        while(((fread (&p, sizeof(Poliza), 1, asegurados) && !polizaValida))){
            if(p.nropoliza == polizaIngresada){
                polizaValida = true;
                cout << endl;
                fclose(asegurados);
                return 0;
            }
        }
        fclose(asegurados);
        return 1;
    }
    else
        return 2;
}

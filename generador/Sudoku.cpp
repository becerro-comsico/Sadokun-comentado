#include <iostream>
#include <array>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include "Sudoku.hpp"

using namespace std;

//                  MÉTODOS PÚBLICOS DE LA CLASE

Sudoku::Sudoku()
{
    resetearTabla();
}

// Mostrar tabla permite imprimir la tabla generada.
void Sudoku::mostrarTabla()
{
    for(int i = 0; i < N; i++)
    {
        for(int j=0; j < N; j++)
        {
            cout<< this->tabla[i][j];
            if((j+1) % MAX_COL == 0)
                cout<< " ";
        }
        if((i+1) % MAX_FIL == 0)
            cout<< endl;
        cout << endl;
    }
}

// Mostrar tabla permite imprimir la tabla solucionada.
void Sudoku::mostrarTablaSol()
{
    cout << endl << "---------------" << endl << endl;
    for(int i = 0; i < N; i++)
    {
        for(int j=0; j < N; j++)
        {
            cout<< this->tablaSol[i][j];
            if((j+1) % MAX_COL == 0)
                cout<< " ";
        }
        if((i+1) % MAX_FIL == 0)
            cout<< endl;
        cout << endl;
    }
    cout << "---------------" << endl;
}

// Generar tabla genera una tabla con espacios vacíos dependiendo de la dificultad
void Sudoku::generarTabla(int dific)
{
    int vacMin, vacMax; // Número de casillas vacias en la tabla del Sudoku

    // Establece los valores mínimos y máximos de casillas vacías en función de la dificultad elegida
    switch(dific)
    {
        case 0:
            vacMin = 5;
            vacMax = 100;
            break;
        case 1:
            vacMin = 14;
            vacMax = 34;
            break;
        case 2:
            vacMin = 34;
            vacMax = 54;
            break;
        case 3:
            vacMin = 54;
            vacMax = 74;
            break;
    }

    // Genera la tabla del sudoku hasta que se cumpla la condición de tener un número de casillas vacías
    // comprendido entre vacMin y vacMax y que el sudoku tenga una y sólo una solución
    do
    {
        // Crea una tabla solución completa y la copia en la tabla actual
        crearTablaSol();
        copy(begin(this->tablaSol), end(this->tablaSol), begin(this->tabla));

        // Genera los espacios vacios y comprueba que tenga una y sólo una solución
    }while(!generarVacios(vacMin, vacMax));
    // While(no se generen los vacios correctamente) 
}



//                  MÉTODOS PRIVADOS DE LA CLASE

// Crea una tabla ya solucionada y la almacena en tablaSol
void Sudoku::crearTablaSol()
{
    resetearTabla();
    llenarDiagonales(this->tablaSol);
    resolverSudoku(this->tablaSol);
}

// Vacía tanto tabla, como tablaSol
void Sudoku::resetearTabla()
{
    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            this->tabla[i][j] = VACIO;

    for(int i = 0; i < N; i++)
        for(int j = 0; j < N; j++)
            this->tablaSol[i][j] = VACIO;
}

/*
    Resuelve el sudoku por medio del algoritmo de Backtracking

    Parametros: matriz de la tabla del sudoku
*/
bool Sudoku::resolverSudoku(array<array<char, N>, N> tabla)
{
    int fil, col;

    // Si no hay casillas vacias, entonces la tabla está resuelta
    if(!HallarVacio(tabla, fil, col)){
        // Copia la tabla resuelta a la variable tablaSol de la clase Sudoku
        copy(begin(tabla), end(tabla), begin(this->tablaSol));
        // Retorna verdadero ya que la tabla está resuelta
        return true;
    }

    // Revisa cada caracter disponible en el arreglo caracts
    for(char car : caracts)
    {
        // Verifica si es seguro asignar el caracter a la casilla actual
        if(esSeguro(tabla, fil, col, car))
        {
            // Asigna el caracter a la casilla
            tabla[fil][col] = car;

            // Continúa la resolución recursivamente
            if(resolverSudoku(tabla))
                // Si la tabla está resuelta, retorna verdadero
                return true;

            // Si no es seguro asignar el caracter, se retrocede y se vuelve a intentar con otro
            tabla[fil][col] = VACIO;
        }
    }

    // Si no se encuentra solución con los caracteres disponibles, retorna falso
    return false;
}

/*
    Vacía casillas aleatorias, comprobando que el Sudoku tenga una y tan solo una solución.

    Parametros: mínimo y máximo de casillas vacias
    Retorno: falso si falla, verdadero si se realiza correctamente
*/
bool Sudoku::generarVacios(int vacMin, int vacMax)
{
    // contador de espacios vacios
    int contVac = 0;
    // randPos es un array que describe posiciones aleatorias dentro del sudoku
    array<int, N*N> randPos;
    genRandPos(randPos);

    // Recorre las posiciones aleatorias del sudoku hasta `vacMax`
    for(int i = 0; i < vacMax; i++)
    {
        // x e y son la fila y columna correspondientes a la posición aleatoria i
        int x = randPos[i] / 12;
        int y = randPos[i] % 12;
        int aux = this->tabla[x][y]; // Guarda el valor actual de la casilla

        // Vacía la casilla
        this->tabla[x][y] = VACIO;

        // nSol almacena el número de soluciones posibles después de vaciar la casilla
        int nSol = 0;
        contarSoluciones(this->tabla, nSol);

        // Si el número de soluciones no es 1, significa que la casilla vaciada hace que el sudoku sea inválido
        // por lo que se debe dejar el valor original de la casilla
        if(nSol != 1)
            this->tabla[x][y] = aux;
        else // Si el número de soluciones es 1, significa que la casilla vaciada es válida
            contVac++; // por lo que se incrementa el contador de espacios vacios
    }

    // Si el contador de espacios vacios es menor que el mínimo especificado, se devuelve falso
    if(contVac < vacMin)
        return false;
    else // Si el contador de espacios vacios es mayor o igual que el mínimo especificado, se devuelve verdadero
        return true;
}

//                  FUNCIONES DE AYUDA

// Función para encontrar la próxima casilla vacía en la tabla del sudoku.
// Recibe como argumento la tabla del sudoku, y dos enteros fil y col que serán usados para almacenar la fila y columna de la casilla vacía.
bool HallarVacio(array<array<char, N>, N> tabla, int &fil, int &col)
{
    // Recorre cada casilla de la tabla
    for(fil = 0; fil < N; fil++)
        for(col = 0; col < N; col++)
            // Si encuentra una casilla vacía, devuelve verdadero y almacena su posición en fil y col
            if(tabla[fil][col] == VACIO)
                return true;
    // Si no encuentra ninguna casilla vacía, devuelve falso
    return false;
}

bool UsadoFil(array<array<char, N>, N> tabla, int fil, char car)
{
    for(int col = 0; col < N; col++)
        if(tabla[fil][col] == car)
            return true;
    return false;
}

bool UsadoCol(array<array<char, N>, N> tabla, int col, char car)
{
    for(int fil = 0; fil < N; fil++)
        if(tabla[fil][col] == car)
            return true;
    return false;
}

bool UsadoCaj(array<array< char, N>, N> tabla, int inicioFila, int inicioCol, char car)
{
    for(int fil = 0; fil < MAX_FIL; fil++)
        for(int col = 0; col < MAX_COL; col++)
            if(tabla[inicioFila + fil][inicioCol + col] == car)
                return true;
    return false;
}

// Devuelve un valor booleano (verdadero, falso) dependiendo de si es posible usar el caracter en la casilla
bool esSeguro(array<array<char, N>, N> tabla, int fil, int col, char car)
{
    return !UsadoFil(tabla, fil, car)
           && !UsadoCol(tabla, col, car)
           && !UsadoCaj(tabla, fil - fil % MAX_FIL, col - col % MAX_COL, car)
           && tabla[fil][col] == VACIO;
}

// Llena los cuadrados 3x4 que no comparten filas ni columnas, por lo tanto no pueden generar un Sudoku invalido.
void llenarDiagonales(array<array<char, N>, N> &tabla) {
    // Variables para almacenar el inicio de cada uno de los cuadrados de 3x4
    int inicCol, inicFil;
    // Crea una copia de la lista de caracteres disponibles
    array<char, N> randCaracts;
    copy(begin(caracts), end(caracts), begin(randCaracts));

    // Itera sobre la cantidad de cuadrados 3x4 posicionados diagonalmente el uno del otro
    for(int x = 0; x < 3; x++) {
        // Calcular el punto de inicio de cada cuadrado
        inicFil = 3 * x;
        inicCol = 4 * x;

        // Hacer que la lista de caracteres disponibles sea aleatoria
        random_shuffle(begin(randCaracts), end(randCaracts), genRand);

        // Iterar sobre cada fila y columna de cada cuadrado
        for(int i = 0; i < 3; i++)
            for(int j = 0; j < 4; j++)
                // Asignar un caracter no usado antes en el cuadrado actual
                tabla[inicFil + i][inicCol + j] = randCaracts[i*4 + j];
    }
}

// Cuenta la cantidad de soluciones de una tabla por medio del algoritmo de Backtracking.
void contarSoluciones(array<array<char, N>, N> tabla, int &cont)
{
    int fil, col;

    // Si no hay más casillas vacías, aumentamos el contador de soluciones
    if(!HallarVacio(tabla, fil, col)){
        cont++;
        return ;
    }

    // Iteramos a través de cada caracter en caracts
    for(int i = 0; i < 9 && cont < 2; i++)
    {
        // Si el caracter en la posición actual es seguro para ser insertado
        if(esSeguro(tabla, fil, col, caracts[i]))
        {
            // Insertamos el caracter
            tabla[fil][col] = caracts[i];
            // Llamamos recursivamente a la función para comprobar la siguiente casilla
            contarSoluciones(tabla, cont);
        }

        // Si no es seguro, volvemos a dejar la casilla en blanco
        tabla[fil][col] = VACIO;
    }
}

// Función para generar un número aleatorio
int genRand(int limiteMax)
{
    // Inicializamos la semilla del generador de números aleatorios con la función time(0)
    srand(time(0));
    // Generamos un número aleatorio en el rango [0, limiteMax) y lo retornamos
    return rand() % limiteMax;
}

// Función para generar un array de posiciones aleatorias en el sudoku
void genRandPos(array<int, N*N> &pos)
{
    // Llenamos el array con los números del 0 al 143
    for(int i = 0; i < N*N; i++)
        pos[i] = i;
    // Desordenamos el array con la función random_shuffle, usando genRand como función de comparación
    random_shuffle(begin(pos), end(pos), genRand);
}
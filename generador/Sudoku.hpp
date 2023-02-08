#include <array>

using namespace std;

const int N = 12;
const int MAX_FIL = 3;
const int MAX_COL = 4;
const char VACIO = '0';
const array<char, N> caracts = {'1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C'};

// Clase Sudoku:

class Sudoku
{
public:
    Sudoku();
    void mostrarTabla();
    void mostrarTablaSol();
    void generarTabla(int dific);

private:
    array< array< char, N>, N > tabla;
    array< array< char, N>, N > tablaSol;

    void resetearTabla();
    bool generarVacios(int vacMin, int vacMax);
    void crearTablaSol();
    bool resolverSudoku(array<array< char, N>, N> tabla);
};

// Funciones de ayuda:

bool HallarVacio(array<array<char, N>, N> tabla, int &fil, int &col);
bool UsadoFil(array<array<char, N>, N> tabla, int fil, char car);
bool UsadoCol(array<array<char, N>, N> tabla, int col, char car);
bool UsadoCaj(array<array<char, N>, N> tabla, int inicioFila, int inicioCol, char car);
bool esSeguro(array<array<char, N>, N> tabla, int fil, int col, char car);
void llenarDiagonales(array<array<char, N>, N> &tabla);
void contarSoluciones(array<array<char, N>, N> tabla, int &cont);
int genRand(int limiteMax);
void genRandPos(array<int, N * N> &pos);

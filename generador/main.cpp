#include <iostream>
#include "Sudoku.hpp"

using namespace std;

void generarTabla(Sudoku &sudo);
bool correctoTabla(Sudoku &sudo);

int main()
{
    int opt = 0;
    Sudoku sudo = Sudoku();
    do
    {
        cout << endl;
        sudo.mostrarTabla();
        cout << endl;
        cout << "1. Generar Sudoku" << endl;
        cout << "2. Mostrar solucion" << endl;
        cout << "3. Salir" << endl;
        cin >> opt;

        switch(opt)
        {
            case 1:
                generarTabla(sudo);
                break;
            case 2:
                sudo.mostrarTablaSol();
                break;
            case 3:
                break;
            default:
                cout << "Opción incorrecta";
        }
    }while(opt != 3);
}

void generarTabla(Sudoku &sudo)
{
    int dif;
    do{
        cout << endl << "\t--DIFICULTADES--" << endl;
        cout << "0. Aleatoria: cualquier sudoku" << endl;
        cout << "1. Fácil: de 110 a 130 pistas" << endl;
        cout << "2. Medio: de 90 a 110 pistas" << endl;
        cout << "3. Dificil: de 70 a 90 pistas" << endl;
        cin >> dif;
    }while(!(dif >= 0 && dif <= 3));
    sudo.generarTabla(dif);
}


#include "heder.h"

using namespace std;

//заряд частиц внутри области
const double Q_in = 1;
//масса частиц внутри области
const double M_in = 1;
//заряд частиц на границе
const double Q_wall = 1.2;
//масса частиц на границе
const double M_wall =  1;

int  Grid(int n_wall1, int n_wall2, int n_inside, char type, double h, double Time);

int main()
{
	setlocale( LC_ALL,"Russian" );
    //количетсво точек внешней границы
    int n_wall1 = 80;
    //количество точек внутренней границы
    int n_wall2 = 20;
    //количетсво точек внутри области
    int n_inside = 160;

	int num_points = Grid(n_wall1, n_wall2, n_inside, 'r',10E-4,100000);

	cout << "Количество оставшихся точек сетки "<<num_points << endl;
	return 0;
	 
}
int  Grid(int n_wall1, int n_wall2, int n_inside, char type, double h, double Time)
{

    //общее количетсво точек границы
    int n_wall = n_wall1 + n_wall2;
    //массив частиц на границе
    particle *wall; 	 wall = new particle[n_wall];
    //массив частиц внутри области
    particle *inside; 	 inside = new particle[n_inside];
    particle wall_ex, in_ex;
    // параметры частиц, которые поместим на границе
    wall_ex.q = Q_wall;
    wall_ex.m = M_wall;
    in_ex.q = Q_in;
    in_ex.m = M_wall;
    bool wall2 = false;
    if (n_wall > 0)
        wall2 = true;
    // посчитаем сколько точек осталось внутри области
    int count_in = 0;
    // инициализируем частицы на границе и внутри области
    Initialisation(wall, inside, n_wall1, n_inside, wall_ex, in_ex, 's', n_wall2);
    // начальное распределение точек внутри области
    Print_Result(inside, n_inside, "Init_inside.dat");
    // распределение точек на границе
    Print_Result(wall, n_wall, "Init_wall.dat");

    Runge_Kutta4(inside, wall, h, n_inside, n_wall, Time,'s',wall2);
    // переписываем все точки вместе ( граница + внутри области)
    ofstream Fout("all.dat");


    for (int i = 0; i < n_inside; i++) {
        if (inside[i].in){
            Fout << inside[i].point.x << " " << inside[i].point.y << "\n";
            count_in++;
        }
    }

    for (int i = 0; i < n_wall; i++)
        Fout << wall[i].point.x << " " << wall[i].point.y << "\n";
    Fout.close();

    return count_in + n_wall;
}

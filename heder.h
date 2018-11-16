

#ifndef heder_H_
#define heder_H_
#include <cmath>
#include <fstream>
#include <iostream>

using namespace std;
const double K = 30;
const double R = 1;


const double Pi = 4 * atan( 1 );
 
const double  C = 0.1;
const double  C_wall = 0.1;

double random(double min, double max) {
	return (double)(rand()) / RAND_MAX * (max - min) + min;
}

struct vector {
	double x; 
	double y; 
	vector() { x = 0; y = 0; }
	vector(double a, double b) { x = a; y = b; }
    
     vector& operator = (const vector &v){
        x = v.x;
        y = v.y;
        return *this;
    }

	vector operator + (const vector v){
		return vector( x + v.x, y + v.y );
	}
	vector operator * (const double v){
		return vector( v * x, v * y );
	}

	vector operator / (const double v){
		return vector( x / v, y / v );
	}

};



struct particle {

    double m; // масса частицы
    double q; // заряд частицы

    vector point;
    vector speed;

    bool in;

    particle() {
        m = 0;
        q = 0;
        in = true;
    }

    particle(double a, double b, vector p, vector v) {
        m = a;
        q = b;
        point = p;
        speed = v;
        in = true;
    }

    particle &operator=(const particle &p) {
        m = p.m;
        q = p.q;
        point = p.point;
        speed = p.speed;
        in = p.in;
        return *this;
    }

    particle operator+(const particle &p) {
        return particle(m + p.m, q + p.q, point + p.point, speed + p.speed);
    }
};


int sign(double A){
	return (int) ( A / abs( A ) );
}


void Print_Result( particle*
p, int n, char* filename ){
	ofstream F(filename);
	for ( int i = 0; i < n; i ++ )   
		F<<p[i].point.x<<" "<<p[i].point.y<<"\n";
	F.close();
}

// Считаем силу действующую на частицы
void Forse(particle*in, particle*wall, vector*F, int n, int m, int i, int K){
	
	vector Kulon_in; 
	vector Kulon_wall; 
	vector  friction;
    //расстояние между частицами
	double r = 0;
	// на случай, если частицы очень приблизились друг к другу
	const double  Epsilon = 10E-3;
	const double  Fmax = 1000;
	// Считаем силу Кулоновского взаимодействия между подвижными частицами
	for (int j = 0; j < n; j++){
		if (j == i) continue; 
			// растояние между часицами
			r = sqrt( ( in[i].point.x - in[j].point.x ) * ( in[i].point.x - in[j].point.x ) + ( in[i].point.y - in[j].point.y ) * ( in[i].point.y - in[j].point.y ) );
			// (это для того, чтобы частицы сильно не разлеатались при приближениии друг другу
			if (r > Epsilon) {
				Kulon_in.x +=  - C * in[i].q * in[j].q * ( in[j].point.x - in[i].point.x ) / abs( pow( r,3 ) );
				Kulon_in.y +=  - C * in[i].q * in[j].q * ( in[j].point.y - in[i].point.y ) / abs( pow( r,3 ) );
			}
			else {
				Kulon_in.x += Fmax * sign( - in[j].point.x + in[i].point.x );
				Kulon_in.y += Fmax * sign( - in[j].point.y + in[i].point.y );
			}
	}
	// Сила взаимодействия частиц с частицами на границе
	for (int j = 0; j < m; j ++){
		r = sqrt( ( in[i].point.x - wall[j].point.x ) * ( in[i].point.x - wall[j].point.x ) + ( in[i].point.y - wall[j].point.y ) * ( in[i].point.y - wall[j].point.y ) );
		if (r > Epsilon){
			Kulon_wall.x += - C_wall*in[i].q * wall[j].q * ( - in[i].point.x + wall[j].point.x ) / abs( pow( r,3 ) );
			Kulon_wall.y += - C_wall*in[i].q * wall[j].q * ( - in[i].point.y + wall[j].point.y ) / abs( pow( r,3 ) );
		}
		else {
			Kulon_wall.x += sign( in[i].point.x - wall[j].point.x ) * Fmax;
			Kulon_wall.y += sign( in[i].point.y - wall[j].point.y ) * Fmax;
		}
	}
	// Сила сопротивления
	friction.x = - K * in[i].speed.x;
	friction.y = - K * in[i].speed.y;
	// Суммарные проекции сил действующих на частицу
	(*F).x = Kulon_in.x + Kulon_wall.x + friction.x;
    (*F).y = Kulon_in.y + Kulon_wall.y + friction.y;
		 	
}

void Initialisation(particle*wall, particle*inside, int n_wall1, int n_inside, particle wall_ex, particle in_ex, char type, int n_wall2)
{
	// пределы в которых будут задаваться точки внутри области
	double max = R - 0.2 ;
	double min = 0.2;



	// задаем заряды и массы на границе
	for (int i = 0; i < n_wall1 + n_wall2; i++) {
		wall[i].q = wall_ex.q;
		wall[i].m = wall_ex.m;
	}
	// круглая или квадратная граница
	switch (type) {
	case 'r': {
		for (int i = 0; i < n_wall1; i++) {
			wall[i].point.x = R + R * cos(i * 2 * Pi / (n_wall1));
			wall[i].point.y =  R + R * sin(i * 2 * Pi / (n_wall1));
		}
		break;
	}
	case 's': {
		int k = n_wall1 / 4; 
		double h = R / k;
		for (int i = 0; i < k; i++) {
			wall[i].point.x = h * i; 
			wall[i].point.y = 0;
			wall[i + k].point.x = R;
			wall[i + k].point.y = h * i;
			wall[i + 2 * k].point.x = R - h * i;
			wall[i + 2 * k].point.y = R;
			wall[i + 3 * k].point.x = 0;
			wall[i + 3 * k].point.y = R - h * i;
		}
		break;
	}
	}
	
	if (n_wall2 > 0) {
        for (int i = 0; i < n_wall2; i++) {
            wall[i + n_wall1].point.x =  R/2 + (R / 4) * cos(i * 2 * Pi / (n_wall2));
            wall[i + n_wall1].point.y =  R/2 + (R / 4) * sin(i * 2 * Pi / (n_wall2));
            wall[i + n_wall1].q = 1;
            wall[i + n_wall1].q = 1;
        }
        for (int i = 0; i < n_inside / 4; i++) {
            //inside[i].point.x = random(R / 2 + R / 4  + 0.01, min);
            //inside[i].point.y = random(R / 2 +R / 4 + 0.01, min);
            inside[i].point.x = random(   R/2 - R/4 - 0.05 ,  0.05 );
            inside[i].point.y = random(  R / 2 - R/4- 0.05,  0.05);
            inside[i].q = in_ex.q ;
            inside[i].m = in_ex.m;
            //inside[i + n_inside / 2].point.x = random(0.01, R / 4  - 0.01);
            //inside[i + n_inside / 2].point.y = random(0.01, R / 4  - 0.01);
            inside[i + n_inside / 4].point.x = random( R - 0.05, R/2+R/4 + 0.05);
            inside[i + n_inside / 4].point.y = random( R/2 - R/4 - 0.05,   0.05);
            inside[i + n_inside / 4].q = in_ex.q;
            inside[i + n_inside / 4].m = in_ex.m;

            inside[i + n_inside / 2].point.x = random( R - 0.05, R/2+R/4 + 0.05);
            inside[i + n_inside / 2].point.y = random( R - 0.05, R/2+R/4 + 0.05);
            inside[i + n_inside / 2].q = in_ex.q;
            inside[i + n_inside / 2].m = in_ex.m;

            inside[i + 3 * n_inside / 4].point.x = random(   R/2 - R/4 - 0.05 ,  0.05 );
            inside[i + 3 * n_inside / 4].point.y = random(  R  - 0.05,  R/2 + R/4 + 0.05);
            inside[i + 3 * n_inside / 4].q = in_ex.q;
            inside[i + 3 * n_inside / 4].m = in_ex.m;
        }
    }
	else // если нет точек на внутренней границе
		for (int i = 0; i < n_inside; i++) {
			//inside[i].point.x = random(max,min);
			//inside[i].point.y = random(max,min);
            inside[i].point.x = random(3*R/2,R/2);
            inside[i].point.y = random(3*R/2,R/2);
			inside[i].q = in_ex.q;
			inside[i].m = in_ex.m;
		}
		
}

 void Runge_Kutta4( particle* dot, particle* wall,   double h,  int inside, int bound, int iter,char type, bool wall2 ) {

	vector* k; k = new vector[5];
	vector* k_v; k_v = new vector[5];
	particle *middot; middot = new particle[inside];
	particle *newdot; newdot = new particle[inside];
	vector F;

	for (int i = 0; i < inside; i ++)
		middot[i] = dot[i];

	for (int i = 0; i < iter; i ++)
	{
		for (int j = 0; j < inside; j ++){

			Forse( dot, wall, &F, inside, bound, j, K);
			k[0] = dot[j].speed;
			k_v[0] = F;
			middot[j].point = dot[j].point + k[0] * (h / 2);
			middot[j].speed  =  dot[j].speed + k_v[0] * (h / 2);

			Forse(middot, wall, &F, inside, bound, j,K);
			k[1] = middot[j].speed;
			k_v[1] = F;
			middot[j].point  = dot[j].point + k[1] * (h / 2);
			middot[j].speed = dot[j].speed + k_v[1] * (h / 2);
				
			Forse(middot, wall, &F, inside,  bound, j, K );
			k[2] = middot[j].speed; 
			k_v[2] = F;
			middot[j].point = dot[j].point + k[2] * h;
			middot[j].speed = dot[j].speed + k_v[2] * h;
				
			Forse(middot, wall, &F, inside,  bound, j, K);
			k[3] = middot[j].speed;
			k_v[3] = F;
			k[4] = (k[0] + k[1] * 2 + k[2] * 2 + k[3]) / 6;
			k_v[4] = (k_v[0] + k_v[1] * 2  + k_v[2] * 2 + k_v[3]) / 6;
			newdot[j].point = dot[j].point + k[4] * h;
			newdot[j].speed = dot[j].speed + k_v[4] * h;
			
		}
        // для выкидывания точек вылетевших из области
		switch (type)
		{
		case 's':
		{
			for (int k = 0; k < inside; k++)
			{
				if (newdot[k].point.x > R || newdot[k].point.y > R || newdot[k].point.x < 0 || newdot[k].point.y < 0) {
					newdot[k].q = 0; newdot[k].m = 0; newdot[k].speed.x = 0; newdot[k].speed.y = 0; newdot[k].in = false;
				}
				if (wall2)
                    if ((newdot[k].point.x -R/2) * (newdot[k].point.x-R/2) + (newdot[k].point.y-R/2) * (newdot[k].point.y-R/2) <= R / 4 * R / 4) {
					newdot[k].q = 0; newdot[k].m = 0; newdot[k].speed.x = 0; newdot[k].speed.y = 0; newdot[k].point.x = 100; newdot[k].point.y = 100;
					newdot[k].in = false;
				}

				dot[k] = newdot[k];
				 
			}
			break;
		}
		case 'r':
		{
			for (int k = 0; k < inside; k++)
			{
				if ((newdot[k].point.x -R) * (newdot[k].point.x -R) + (newdot[k].point.y-R ) * (newdot[k].point.y-R ) >=  R*R) {
					newdot[k].q = 0; newdot[k].m = 0; newdot[k].speed.x = 0; newdot[k].speed.y = 0; newdot[k].in = false;
				}
				/*if(n_wall2>0)
                    if ((newdot[k].point.x ) * (newdot[k].point.x ) + (newdot[k].point.y ) * (newdot[k].point.y ) <= R / 4 * R / 4) {
					newdot[k].q = 0; newdot[k].m = 0; newdot[k].speed.x = 0; newdot[k].speed.y = 0; newdot[k].point.x = 100; newdot[k].point.y = 100;
					newdot[k].in = false;
				    }*/
				dot[k] = newdot[k];
			}
			break; 
		}

		default:
			break;
		}

			
	}
 }
  

#endif //Print_result_H_

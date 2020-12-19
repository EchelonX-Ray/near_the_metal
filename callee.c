#include "clib.h"
#include "clib_string.h"
#include "clib_math.h"
#include "syscalls.h"

float rtod(float r);
float dtor(float r);

signed int main(unsigned int argc, char* argv[], char* envp[]) {
	//dprintf(2, "TraceA %d %f\n", -10, -0.46455);
	//return 0;
	
	/*
	long double D = 10000000000.3;
	double d = 10000000000.3;
	float f = 10000000000.3;
	
	dprintf(1, "aa%Lfbb\n", D);
	dprintf(1, "cc%fdd\n", d);
	dprintf(1, "ee%fff\n", f);
	dprintf(1, "gg%dhh\n", 10);
	dprintf(1, "ii%sjj\n", "Hello, World!");
	
	fprintf(1, "aa%Lfbb\n", D);
	fprintf(1, "cc%fdd\n", d);
	fprintf(1, "ee%fff\n", f);
	fprintf(1, "gg%dhh\n", 10);
	fprintf(1, "ii%sjj\n", "Hello, World!");
	
	printf("aa%Lfbb\n", D);
	printf("cc%fdd\n", d);
	printf("ee%fff\n", f);
	printf("gg%dhh\n", 10);
	printf("ii%sjj\n", "Hello, World!");
	*/
	
	/*
	float f0 = 20.0;
	printf("f0: %f, sqrtf(f0): %f, sqrt(f0): %f\n", f0, sqrtf(f0), sqrt(f0));
	double d0 = 20.0;
	printf("f0: %f, sqrtf(f0): %f, sqrt(f0): %f\n", d0, sqrtf(d0), sqrt(d0));
	d0 = f0;
	printf("f0: %f, sqrtf(f0): %f, sqrt(f0): %f\n", d0, sqrtf(d0), sqrt(d0));
	
	return 0;
	*/
	
	float deg_angle = -720.0;
	
	while (deg_angle <= 721.0) {
		printf("deg_angle: %f, ", deg_angle);
		printf("rad_angle: %f, ", dtor(deg_angle));
		printf("cosf: %f, ", cosf(dtor(deg_angle)));
		printf("cos: %f\n", cos(dtor(deg_angle)));
		deg_angle += 15.0;
	}
	
	printf("\n");
	
	double num;
	double res;
	
	num = 1.0;
	res = sqrt(num);
	printf("sqrt(%f): %f\n", num, res);
	num = 0.5;
	res = sqrt(num);
	printf("sqrt(%f): %f\n", num, res);
	num = 0.25;
	res = sqrt(num);
	printf("sqrt(%f): %f\n", num, res);
	num = 0.125;
	res = sqrt(num);
	printf("sqrt(%f): %f\n", num, res);
	num = 0.0;
	res = sqrt(num);
	printf("sqrt(%f): %f\n", num, res);
	
	return 0;
}

float rtod(float r) {
	return (r * 180.0) / M_PI;
}

float dtor(float d) {
	return (d * M_PI) / 180.0;
}

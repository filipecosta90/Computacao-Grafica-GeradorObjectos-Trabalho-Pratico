#include <iostream>
#include <fstream>

void plano(float largura, float comprimento){

	FILE *f;
	f = fopen("plano.3d", "w");

	fprintf(f, "%f %f %f", comprimento / 2, 0.0, largura / 2);
	fprintf(f, "%f %f %f", comprimento / 2, 0.0, -largura / 2);
	fprintf(f, "%f %f %f", -comprimento / 2, 0.0, largura / 2);


	fprintf(f, "%f %f %f", comprimento / 2, 0.0, largura / 2);
	fprintf(f, "%f %f %f", -comprimento / 2, 0.0, -largura / 2);
	fprintf(f, "%f %f %f", -comprimento / 2, 0.0, largura / 2);

	fclose(f);
}

int main(){
	plano(10.0, 10.0);
}
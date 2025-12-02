#include "captura.h"


void clear_buffer(void)
{
	char c;
	while( (c = getchar()) != '\n' && c !=EOF);
}

void remover_salto(char *cadena)
{
	int i = -1;
	while(cadena[++i]!='\n' && cadena[i]!='\0');
	cadena[i] = '\0';
}
int inputEntero(char *mensaje,int *valor)
{
	int resultado;
	printf("%s",mensaje);
	resultado = scanf("%d",valor);
	clear_buffer();
	return resultado;
}

int inputFloat(char *mensaje,float *valor)
{
	int resultado;
	printf("%s",mensaje);
	resultado = scanf("%f",valor);
	clear_buffer();
	return resultado;
}
int inputDouble(char *mensaje,double *valor)
{	
	int resultado;
	printf("%s",mensaje);
	resultado = scanf("%lf",valor);
	clear_buffer();
	return resultado;
}
int inputChar(char *mensaje,char *valor)
{	
	int resultado;
	printf("%s",mensaje);
	resultado = scanf("%c",valor);
	clear_buffer();
	return resultado;	
}

int inputCadena(char *mensaje,char *cadena,int longitud)
{	
	char *res;
	int resultado;
	if( longitud >= 2 )
		cadena[longitud-2] = '\n';	
	else 
		return 0;
	printf("%s",mensaje);
	res = fgets(cadena,longitud,stdin);
	if(cadena[longitud-2] != '\n')
		clear_buffer();	
	remover_salto(cadena);
	resultado = (res) ? 1:0;
	return resultado;	
}
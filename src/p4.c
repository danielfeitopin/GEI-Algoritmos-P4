#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <math.h>
#include <string.h>

#define TAM_MAX 1000

typedef int ** matriz;

void dijkstra(matriz grafo, matriz distancias, int tam) {
	int n, i, j, min, v=0;
	int *noVisitados = malloc(tam*sizeof(int));
	
	for (n=0; n<tam; n++) {
		for (i=0; i<tam; i++) {
			noVisitados[i] = 1;
			distancias[n][i] = grafo[n][i];
		}
		noVisitados[n] = 0;
		for(i=0; i<tam-2; i++) {
			min = TAM_MAX;
			for (j=0; j<tam; j++) {
				if(noVisitados[j] == 1) {
					if(distancias[n][j] < min){
						min = distancias[n][j];
						v = j;
					}	
				}
			}
			noVisitados[v]=0;
			for (j=0; j<tam ; j++){
				if (noVisitados[j] ==1){
					if (distancias [n][j] > (distancias [n][v]+ grafo [v][j]))
						distancias [n][j] = distancias [n][v] + grafo[v][j];
				}
			}	
		}
	}
	free(noVisitados);
}


matriz crearMatriz(int n) {
	int i;
	matriz aux;
	
	if ((aux = malloc(n*sizeof(int *))) == NULL)
		return NULL;
	for (i=0; i<n; i++)
		if ((aux[i] = malloc(n*sizeof(int))) == NULL)
			return NULL;
	return aux;
}

/* Inicializacion pseudoaleatoria [1..TAM_MAX] de un grafo completo
no dirigido con n nodos, representado por su matriz de adayencia */
void iniMatriz(matriz m, int n) {
	int i, j;
	for (i=0; i<n; i++)
		for (j=i+1; j<n; j++)
			m[i][j] = rand() % TAM_MAX + 1;
	for (i=0; i<n; i++)
		for (j=0; j<=i; j++)
			if (i==j)
				m[i][j] = 0;
			else
				m[i][j] = m[j][i];
}

void liberarMatriz(matriz m, int n) {
	int i;
	for (i=0; i<n; i++)
		free(m[i]);
	free(m);
}

void matriz1 (matriz m){
	
	m[0][0]=0;	m[0][1]=1;	m[0][2]=8;	m[0][3]=4;	m[0][4]=7;
	m[1][0]=1;	m[1][1]=0;	m[1][2]=2;	m[1][3]=6;	m[1][4]=5;
	m[2][0]=8;	m[2][1]=2;	m[2][2]=0;	m[2][3]=9;	m[2][4]=5;
	m[3][0]=4;	m[3][1]=6;	m[3][2]=9;	m[3][3]=0;	m[3][4]=3;
	m[4][0]=7;	m[4][1]=5;	m[4][2]=5;	m[4][3]=3;	m[4][4]=0;
	
}

void matriz2 (matriz m){
	
	m[0][0]=0;	m[0][1]=1;	m[0][2]=4;	m[0][3]=7;	
	m[1][0]=1;	m[1][1]=0;	m[1][2]=2;	m[1][3]=8;	
	m[2][0]=4;	m[2][1]=2;	m[2][2]=0;	m[2][3]=3;	
	m[3][0]=7;	m[3][1]=8;	m[3][2]=3;	m[3][3]=0;
	
}

void listarMatriz(matriz m,int tam) {
	int i,j;
	for(i=0;i<tam;i++){
		for(j=0;j<tam;j++){
		printf("%4d ", m[i][j]);
		}
		putchar('\n');
	}
}

void test1(void){
	
	matriz distancias,grafo;;

	grafo = crearMatriz(5);
	distancias = crearMatriz(5);
	matriz1(grafo);
	
	printf("Matriz adyacente\n");
	listarMatriz(grafo,5);
	printf("\n");
	
	printf("Distancias\n");
	dijkstra(grafo,distancias,5);
	listarMatriz(distancias,5);
	
	liberarMatriz(distancias,5);
	liberarMatriz(grafo,5);
	
}

void test2(void){
	
	matriz distancias,grafo;;
	
	grafo = crearMatriz(4);
	distancias = crearMatriz(4);
	matriz2(grafo);
	
	printf("Matriz adyacente\n");
	listarMatriz(grafo,4);
	printf("\n");
	
	printf("Distancias\n");
	dijkstra(grafo,distancias,4);
	listarMatriz(distancias,4);
	
	liberarMatriz(distancias,4);
	liberarMatriz(grafo,4);
		
}


void inicializar_semilla() {
	srand(time(NULL));
}


double microsegundos() { /* obtiene la hora del sistema en microsegundos */
	struct timeval t;
	if (gettimeofday(&t, NULL) < 0 )
		return 0.0;
	return (t.tv_usec + t.tv_sec * 1000000.0);
}


//Cotas
double cota_sub(int n) {return 8*pow(n,2);}
double cota_aj(int n) {return pow(n,2.8);}
double cota_sob(int n) {return pow(n,3);}


void mostrar_tabla (void algoritmo(),void orden(),
					double cota1(),double cota2(),double cota3()) {
	double t1, t2, taux1, taux2, tiempo; //Variables de tiempo
	matriz matrix, distancias;
	int capacidad, k;
	matrix = crearMatriz(TAM_MAX);
	distancias = crearMatriz(TAM_MAX);
	printf("   (n)\t\tt(n)\t   t(n)/f(n)\t  t(n)/g(n)\t t(n)/h(n)\n");
	for (capacidad = 10; capacidad <= TAM_MAX; capacidad*=2) {
		orden(matrix, capacidad);
		t1 = microsegundos();
		algoritmo(matrix, distancias, capacidad);
		t2 = microsegundos();
		tiempo = t2 - t1;
		if (tiempo < 500) {
			printf("*");
			
			taux1 = microsegundos();
			for (k = 0; k < 1000; k++) {
				orden(matrix, capacidad);
			}
			taux2 = microsegundos();
						
			t1 = microsegundos();
			for (k = 0; k < 1000; k++) {
				orden(matrix, capacidad);
				algoritmo(matrix, distancias, capacidad);
			}
			t2 = microsegundos();
			tiempo= (t2-t1-(taux2-taux1))/1000;
		}else printf(" ");
	 	printf("%7d    ",capacidad);
	 	printf("%12.3f   ",tiempo);
	 	printf("%.10f   ",tiempo/cota1(capacidad));
	 	printf("%.10f   ",tiempo/cota2(capacidad));
	 	printf("%.10f\n",tiempo/cota3(capacidad));
	}
	liberarMatriz(distancias, TAM_MAX);
	liberarMatriz(matrix, TAM_MAX);
}

int main() {
	inicializar_semilla();
	
	//Tests
	printf("Test de funcionamiento:\n");
	test1();
	printf("\n");
	test2();
	
	printf("\nAnálisis de la complejidad de Dijkstra:\n");
	printf("Cota Subestimada:  f(n)= 8*n^2\n");
	printf("Cota Ajustada:     g(n)= n^2.8\n");
	printf("Cota Sobrestimada: h(n)= n^3\n\n");
	mostrar_tabla(dijkstra,iniMatriz,cota_sub,cota_aj,cota_sob);
}

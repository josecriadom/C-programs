		/***********************************************
		*      Jose Criado Miguel   --  71043194N      *
		*    David Hernando Lorenzo  --  71039175H     *
		************************************************/


/*-----------------------------------------------------------------------------------------------------
--------------------------                     ENNCIADO                     ---------------------------
-------------------------------------------------------------------------------------------------------

Se trata de hacer un programa para dibujar el perfil que se verı́a en el horizonte mirando
desde determinado punto de la Tierra, a altura Ho, y sabiendo que hay una colección de picos
en la dirección a la que se mira representado por cuaternas (A,D,B,H), siendo A el ángulo
respecto a la dirección en que se mira, D la distancia, B la longitud de la base y H la altura,
suponiendo que el pico es un triángulo isósceles. Por simplicidad, para resolver el ejercicio se
supondrá que la Tierra es plana.
En la lı́nea de comandos se ejecutará el programa con una orden similar a
./horizonte entrada.txt salida.txt
siendo entrada.txt y salida.txt dos ficheros con los datos de entrada y de salida respectivamente.
El fichero de entrada debe contener un primer renglón con la altura desde la que mira el
observador, Ho, y un renglón para cada pico con las 4 cantidades citadas arriba: A, D, B y
H, siendo todos ellos números reales, empleando las unidades que prefiera el alumno, pero la
misma para todas las longitudes, ya sean metros, Km, etc.
El fichero de salida debe contener una serie de renglones de tipo I, A, H, siendo I el ı́ndice
del pico en la lista de entrada (I=1 el primero, I=2 el segundo, etc.), A el ángulo en el que
se ve su máxima altura, y H dicha altura (medidas en las mismas unidades que los datos de
entrada). Los picos que sean tapados por otros en toda su extensión no deben figurar en la
lista de salida, y los que sean troceados en varias partes por los picos que los tapen, sólo deben
figurar una vez, con su altura máxima.

------------------------------------------------------------------------------------------------------
------------------------------------------------------------------------------------------------------*/


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define MIN(x,y) (x < y ? x : y)
#define MAX(x,y) (x > y ? x : y)

typedef struct
  {
     int pos;
     double x[3];
     double y[3];
  } COORDENADAS;
  
typedef struct
  {
     int pos;
     double angulo;
     double altura;
  } PICOS;



//------------------------------------------------------------------------------------------------------
void obtiene_datos(char *fich, double *ho, double *a, double *d, double *b, double *h, int *cant){
  FILE *fp;
  char caracter, *buffer, *trozo=NULL;
  int i=0, j;
  buffer = (char *)malloc(sizeof (char));
  
  //abrimos el fichero
  fp = fopen(fich, "r");
  if(fp==NULL)
  {
    printf("Error al abrir el fichero de lectura\n");
    exit(0);
  }else{
    *cant=0;
    
    //leemos caracter a caracter y contamos los saltos de linea
    while((caracter = fgetc(fp)) != EOF)
    {
      if(i>0) buffer = (char *) realloc(buffer, i+1); //aumentamos el tamaño del bufer a la vez que vamos leyendo caracteres
      if(caracter=='\n') *cant=*cant+1; 
      buffer[i]=caracter; //guardamos los caracteres en un array
      i++;
    }
    *cant=*cant-1;    
    
    //imprimir
    printf("Contenido del archivo de origen\n");
    for(j=0;j<(i-1);j++)
      printf("%c",buffer[j]);
    printf("\n\n");
    
    i=0;
    j=1;
    //separamos las posibles palabras en trozos
    printf("\n----------------DATOS OPTENIDOS--------------\n");
    trozo = strtok( buffer, " \n");
    *ho = atof(trozo);
    printf("ho = %f\n\n", *ho);
    
    
    while(i!=*cant) {
      trozo = strtok( NULL, " \n");
      
      if(j==1){
        a[i] = atof(trozo);
        printf("a[%d] = %f\n", i, a[i]);
        j++;
      } else if(j==2){
        d[i] = atof(trozo);
        printf("d[%d] = %f\n", i, d[i]);
        j++;
      } else if(j==3){
        b[i] = atof(trozo);
        printf("b[%d] = %f\n", i, b[i]);
        j++;
      } else if(j==4){
        h[i] = atof(trozo);
        printf("h[%d] = %f\n\n", i, h[i]);
        j=1;
        i++;
      }
    }
  }
  
  fclose(fp);
  free(buffer);
}



//------------------------------------------------------------------------------------------------------
void quicksort(int *in, double *a, double *d, double *b, double *h, int izq, int der){
int i, j;
double aux, pivote;
int auxIn;

	i = izq;
	j = der;
	pivote = d[(izq+der)/2];
	
	while(i<=j){
		while(d[i]<pivote){
			i++;
		}
	
		while(d[j]>pivote){
			j--;
		}
		
		if(i<=j){
			//modificamos el array a ordenar
			aux=d[i];
			d[i]=d[j];
			d[j]=aux;
			
			//Modificamos tambien los demás arrays a la vez
			auxIn=in[i];
			in[i]=in[j];
			in[j]=auxIn;
			
			aux=a[i];
			a[i]=a[j];
			a[j]=aux;
			
			aux=b[i];
			b[i]=b[j];
			b[j]=aux;
			
			aux=h[i];
			h[i]=h[j];
			h[j]=aux;
			
			
			i++;
			j--;
		}
	}

	if(izq<j){
		quicksort(in, a, d, b, h, izq, j);
	}
	
	if(der>i){
		quicksort(in, a, d, b, h, i, der);
	}
}



//------------------------------------------------------------------------------------------------------
void devuelve_datos(char *fich, PICOS *picos, int cant){
  FILE *fp;
  int j;
  
  //abrimos el fichero
  fp = fopen(fich, "w+");
  if(fp==NULL)
  {
    printf("Error al abrir el fichero de escritura\n");
    exit(0);
  }else{ 
    for(j=0;j<cant;j++){
      fprintf(fp, "%d", picos[j].pos);
      fputc(' ', fp);
      fprintf(fp, "%g", picos[j].angulo);
      fputc(' ', fp);
      fprintf(fp, "%g", picos[j].altura);
      fputc('\n', fp);
    }
  }
  fclose(fp);
}



//------------------------------------------------------------------------------------------------------
//hay que tener en cuenta el el plano donde prollectamos está constantemente a 1m de nosotros, por lo cual, visto desde arriba, es una circunferencia de 1m de radio
void prollectar(COORDENADAS *coord, double ho, double *a, double *d, double *b, double *h, int *pos, int cant){
  int i;
  double newAngulo, PI=3.141592;
  
  for(i=0;i<cant;i++){
    //introducimos la posicion
    coord[i].pos = pos[i];
  
    //calculamos el pico
      //esta la coordenada y, osea la altura
      //la formula sería tangente(arcotangente((alturaMontaña-alturaObservador)/distancia)/distancia_al_plano)
      //como la distancia al plano es 1m no hay que ponerlo. Arcotang con tangente se anulan
      coord[i].y[0] = ((h[i]-ho)/d[i]);
      
      //cordenada x
      //la formula es distancia, que sería 1m por el ángulo en radianes.
      coord[i].x[0] = PI*a[i];
      
    //calculamos la coordenada y de las bases. Son la misma para las dos
      //la formula sería tangente(arcotangente((-alturaObservador)/distancia)/distancia_al_plano)
      //como la distancia al plano es 1m no hay que ponerlo. Arcotang con tangente se anulan
      coord[i].y[1] = (-ho)/d[i];
      coord[i].y[2] = coord[i].y[1];
      
      //coordenada x 
      //la formula sería la misma que la de el pico pero modificando el angulo, antes hay que optener el angulo 
      //que se crea al girar la cabeza desde el centro de la montaña a uno de sus lados
      newAngulo = atan(b[i]/(2*d[i]));
        //base izquierda
        coord[i].x[1] = PI*(a[i]-newAngulo);
        //base derecha
        coord[i].x[2] = PI*(a[i]+newAngulo);
  }
}




//------------------------------------------------------------------------------------------------------
void mostrar_plano(COORDENADAS *coord, int n){
  int i;
  
  printf("----------------------Coordenadas en el plano--------------------------\n");
  for(i=0;i<n;i++){
    printf("La montaña %d esta situada en:\n\tPico(%f,%f)\n\tBaseI(%f,%f)\n\tBaseD(%f,%f)\n", 
    coord[i].pos, coord[i].x[0], coord[i].y[0], coord[i].x[1], coord[i].y[1], coord[i].x[2], coord[i].y[2]);
  }
}




//------------------------------------------------------------------------------------------------------
//esta funcion comprueba que el punto dado esé dentro del area creada por un poligono dado por puntos.
//en este caso el poligono dado es un triangulo por lo que tiene 3 puntos
int esta_dentro(COORDENADAS montana, double *pico){
  int counter = 0;
  int i;
  double xinters;
  double p1[2], p2[2];

  p1[0] = montana.x[0];
  p1[1] = montana.y[0];
  for (i=1;i<=3;i++) {
    p2[0] = montana.x[i % 3];
    p2[1] = montana.y[i % 3];
    if (pico[1] > MIN(p1[1],p2[1])) {
    if (pico[1] <= MAX(p1[1],p2[1])) {
      if (pico[0] <= MAX(p1[0],p2[0])) {
        if (p1[1] != p2[1]) {
          xinters = (pico[1]-p1[1])*(p2[0]-p1[0])/(p2[1]-p1[1])+p1[0];
            if (p1[0] == p2[0] || pico[0] <= xinters)
              counter++;
            }
          }
       } 
    }
    p1[0] = p2[0];
    p1[1] = p2[1];
  }

  if (counter % 2 == 0)
    return(0);//Fuera
  else
    return(1);//Dentro
}

//------------------------------------------------------------------------------------------------------
//ordena los puntos por altura
void ordenar(COORDENADAS coord){
  double aux;

  if(coord.y[2]>coord.y[1] && coord.y[2]>coord.y[0]){
    aux=coord.x[0];
    coord.x[0]=coord.x[2];
    coord.x[2]=aux;
    
    aux=coord.y[0];
    coord.y[0]=coord.y[2];
    coord.y[2]=aux;
  } else {
    if(coord.y[1]>coord.y[0] && coord.y[1]>coord.y[2]){
      aux=coord.x[0];
      coord.x[0]=coord.x[1];
      coord.x[1]=aux;
      
      aux=coord.y[0];
      coord.y[0]=coord.y[1];
      coord.y[1]=aux;
    }
  }


}


//------------------------------------------------------------------------------------------------------
//devuelve 1 si algun pico se ve y 0 si la montaña queda visible
//n1 es la montaña de atras y n2 la de alante, la cual tapa a la anterior
//si se generan dos montañas guardamos la segunda en picoAux en el caso de que no exista una o la que exista sea mas baja
//existeAux nos indica si ya existe una montaña auxiliar
int nueva_vista(int n1, int n2, COORDENADAS *coord, int *existeAux, COORDENADAS picoAux){
  double x, y, x1, y1, x2, y2, x3, y3, x4, y4, xyAux;
  int i, j, contAuxD=0, contAuxI=0;
  COORDENADAS aux[2]; //el primero es para la posible montaña de la izq y otro para la de la der
  
  
  for(i=0;i<3;i++){
    x1 = coord[n1].x[i];
    y1 = coord[n1].y[i];
      
    if(i==2){
      x2 = coord[n1].x[0];
      y2 = coord[n1].y[0];
    } else {
      x2 = coord[n1].x[i+1];
      y2 = coord[n1].y[i+1];
    }
    
    for(j=1;j<3;j++){
      x3 = coord[n2].x[0];
      y3 = coord[n2].y[0];
  
      x4 = coord[n2].x[j];
      y4 = coord[n2].y[j];
      
      x = (x1 - ((-x1 + x2) * y1)/(-y1 + y2) + ((-x1 + x2) * y3)/(-y1 + y2) - ((-x1 + x2) * x3 * (-y3 + y4))/((-x3 + x4) * (-y1 + y2)))/(1 - ((-x1 + x2) * (-y3 + y4))/((-x3 + x4) * (-y1 + y2)));
      y = ((y4 - y3) * (x - x3))/(x4 - x3) + y3 - y1;
      
      //comprobamos que alguno de los puntos no sea el pico
      if(x!=coord[n1].x[0] && y!=coord[n1].y[0]){
        //comprobamos que los puntos esten contenido en la montaña que está tapada
        if(((x>=x1 && x<=x2) || (x>=x2 && x<=x1)) && ((y>=y1 && y<=y2) || (y>=y2 && y<=y1))){
          //si el punto es del lado izq
          if(x<coord[n1].x[0]){
            aux[0].x[contAuxI] = x;
            aux[0].y[contAuxI] = y;
            contAuxI++;
          } else { //lado derecho
            aux[1].x[contAuxD] = x;
            aux[1].y[contAuxD] = y;
            contAuxD++;
          }
        }
      } else {
        return 1;
      }
    }
  }
  
  //si no se han creado alguna montaña
  if(contAuxI==0 && contAuxD==0){
    return 0; //no se ve la montaña
  } else { //si se ha creado alguna montaña
    //si se han creado 2 montañas
    if(contAuxI!=0 && contAuxD!=0){
      //dordenamos las dos
      ordenar(aux[0]);
      ordenar(aux[1]);
      
      //comparamos cual es mas alta
      if(aux[0].y[0]>aux[1].y[0]){ //la izquierda
        memcpy(&coord[n1].x,&aux[0].x,sizeof(aux[0].x));
        memcpy(&coord[n1].y,&aux[0].y,sizeof(aux[0].y));
        
        //comprobamos si no hay auxiliar. y si no la hay guardamos la otra montaña
        if(*existeAux == 0){
          memcpy(&picoAux.x,&aux[1].x,sizeof(aux[0].x));
          memcpy(&picoAux.y,&aux[1].y,sizeof(aux[0].y));
          *existeAux = 1;
        }
      } else { //la derecha
        memcpy(&coord[n1].x,&aux[1].x,sizeof(aux[1].x));
        memcpy(&coord[n1].y,&aux[1].y,sizeof(aux[1].y));
        
          //comprobamos si no hay auxiliar. y si no la hay guardamos la otra montaña
        if(*existeAux == 0){
          memcpy(&picoAux.x,&aux[0].x,sizeof(aux[0].x));
          memcpy(&picoAux.y,&aux[0].y,sizeof(aux[0].y));
          *existeAux = 1;
        }
      }
      
    } else { //si se ha creado solo una montaña
      //existe la montaña izquierda
      if(contAuxI!=0){
        memcpy(&coord[n1].x,&aux[0].x,sizeof(aux[0].x));
        memcpy(&coord[n1].y,&aux[0].y,sizeof(aux[0].y));
      } else { //existe la montaña derecha
        memcpy(&coord[n1].x,&aux[1].x,sizeof(aux[1].x));
        memcpy(&coord[n1].y,&aux[1].y,sizeof(aux[1].y));
      }
    }
  }
  return 1;

}


//------------------------------------------------------------------------------------------------------
int picos_visibles(int n, COORDENADAS *coord, PICOS *picos, double ho, double *d){
  int i, j, cantidad = 0, visible, dentro;
  double pico[2];
  COORDENADAS picoAux, aux;//por si al tapar una montaña se genera otra guardamos el otro trozo. Suponemos que no se generarán mas de dos trozos en casos reles
  int existeAux;
  
  //recorremos todas las montañas desde la segunda mas cercana hasta la mas lejana
  //nos saltamos la mas cercana ya que no la tapa ninguna
  for(i=1;i<n;++i){
    //reseteamos los valores
    existeAux = 0;
    visible = 1;
    
    do{      
      //guardamos el pico, guardamos el mas alto en la posicion 0
      if(existeAux == 1 && coord[i].y[0]<picoAux.y[0]){
	      pico[0] = picoAux.x[0];
	      pico[1] = picoAux.y[0];
	      //intercambiamos el pico auxiliar con el principal
	      memcpy(&aux.x,&coord[i].x,sizeof(coord[i].x));
	      memcpy(&aux.y,&coord[i].y,sizeof(coord[i].y));
	      
	      memcpy(&coord[i].x,&picoAux.x,sizeof(picoAux.x));
	      memcpy(&coord[i].y,&picoAux.y,sizeof(picoAux.y));
	      
	      memcpy(&picoAux.x,&aux.x,sizeof(aux.x));
	      memcpy(&picoAux.y,&aux.y,sizeof(aux.y));
      } else {
	      pico[0] = coord[i].x[0];
	      pico[1] = coord[i].y[0];
      }
         
      for(j=0;j<i;j++){
        //comprobamos si se tapa el pico
	    dentro = esta_dentro(coord[j], pico); 

	    if(dentro == 1){
	      //aqui buscariamos el nuevo pico para esta montaña
	      visible = nueva_vista(i, j, coord, &existeAux, picoAux);
	  
	      //si no es visible y hay auxiliar probamos con la auxiliar
	      if(visible == 0 && existeAux == 1){
	        existeAux = 0;
	        memcpy(&coord[j].x,&picoAux.x,sizeof(picoAux.x));
	        memcpy(&coord[j].y,&picoAux.y,sizeof(picoAux.y));
	        visible = nueva_vista(i, j, coord, &existeAux, picoAux);
	      }
	    }
      }
    }while(dentro == 1 && (visible == 0 && existeAux == 1));

    
    //guardamos los picos
    if(dentro == 0){
      cantidad++;//sumamos 1 al numero de montañas
      picos[cantidad].pos = coord[i].pos; 
      picos[cantidad].angulo = coord[i].x[0]/3.141592; 
      picos[cantidad].altura = (coord[i].y[0]*d[i])+ho; 
    } else {
      if(visible == 1){
        cantidad++;//sumamos 1 al numero de montañas
        picos[cantidad].pos = coord[i].pos;
        picos[cantidad].angulo = pico[0]/3.141592;
        picos[cantidad].altura = (pico[1]*d[i])+ho;
      }
    }
    
  }
  return cantidad;
}




//------------------------------------------------------------------------------------------------------
int main(int argc, char *argv[])
{
  int j;
  
  //farametros del fichero de entrada-salida, suponemos un máximo de 10 montañas
  double ho, a[10], d[10], b[10], h[10]; 
  int *i;
  int n; //n es el numero de picos visibles
  int cantidad; //esta es la cantidad de montañas que hay

  
  //comprobamos que se han introducido los argumentos que se necesitan
  if(argc!=3)
  {
    printf("ERROR nº de argumentos incorrecto\n");
    return 1;
  }
  
  //leemos el archivo de entrada para obtener los datos de las montañas
  obtiene_datos(argv[1], &ho, a, d, b, h, &cantidad);
  
  //resevamos memoria y creamos el array de indices
  i = (int *)malloc(cantidad*sizeof (int));
  for(j=0;j<cantidad;j++)
    i[j]=j+1;
    
  //ordenamos las montañas por distancia
  quicksort(i, a, d, b, h, 0, cantidad-1);
  
  
  //reservamos memoria para coordenadas
  //coordenadas = (COORDENADAS *)malloc (cantidad*sizeof(COORDENADAS *));
  
  //creamos coordenadas
  COORDENADAS coordenadas[cantidad];
  
  
  //en este modulo prollectaremos los tres vertices de las montañas a un plano a 1m en modo de coordenadas
  prollectar(coordenadas, ho, a, d, b, h, i, cantidad);
  
  //liberamos el espacio de i
  free(i);
  
  //mostramos el plano
  mostrar_plano(coordenadas, cantidad);
  

  //reservamos memoria para picos
  PICOS picos[cantidad];
  
  //guardamos los valores de la primera montaña
  picos[0].pos = coordenadas[0].pos; 
  picos[0].angulo = a[0];
  picos[0].altura = h[0];
  
  //funcion voraz---------------------------------
  n = picos_visibles(cantidad, coordenadas, picos, ho, d);
  
  //mostramos la solucion que se va a escribir en el documento de salida
  printf("\n\n----------------------Solucion--------------------------\n");
  for(j=0;j<=n;j++){
    printf("La montaña %d esta situada en:\n\tangulo(%f)\n\taltura(%f)\n", 
    picos[j].pos, picos[j].angulo, picos[j].altura);
  }
  
  
  devuelve_datos(argv[2], picos, n+1);

}

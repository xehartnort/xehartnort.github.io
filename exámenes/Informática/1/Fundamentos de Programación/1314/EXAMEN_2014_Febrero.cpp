#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
using namespace std;

/*
	Resoluci�n examen Febrero 2014.
	Se incluyen clases y m�todos que no se ped�an en el examen.
*/

class Imagen{
private:
	vector < vector<double> > pixels;
public:
	// Prec: Todas las filas tiene el mismo n�mero de columnas
	Imagen (vector < vector <double> > pixels_imagen)
		:pixels(pixels_imagen)
	{
		// Si se desea, se podr�a comprobar la precondici�n para forzar
		// que todas las filas tengan el mismo n�mero de columnas
	}
	
	// Prec: fil y col en el rango adecuado
	double Pixel(unsigned int fil, unsigned int col){
		return pixels.at(fil).at(col);
	}

	unsigned int TotalFilas(){
		return pixels.size();
	}

	unsigned int TotalColumnas(){
		if (TotalFilas() == 0)
			return 0;
		else
			return pixels.at(0).size();
	}


	bool EsPixelCorrecto(unsigned int fil, unsigned int col){
		/*
		B�SICO:
			Como necesitamos comprobar que un pixel es correcto en varios sitios,
			definimos un m�todo que se encargue de dicha tarea.
			Lo hemos puesto como un m�todo porque el criterio de ser pixel correcto
			o no podr�a cambiar en el futuro. De esta forma, s�lo hay que modificar
			la l�nea correspondiente en este m�todo. 
		*/
		return pixels.at(fil).at(col) >= 0;
	}

	// Calcula la media de los vecinos correctos.
	// Si todos son incorrectos, devuelve 0
	double MediaVecinosCorrectos(unsigned int fil, unsigned int col){
		/*
		B�SICO:

		Para recorrer los vecinos, NO haremos algo de la forma siguiente:

			if (pixels.at(fil-1).at(col-1) >= 0){
				suma_vecinos = suma_vecinos + pixels.at(fil-1).at(col-1);
				total_vecinos++;
			}
			if (pixels.at(fil-1).at(col) >= 0){
				suma_vecinos = suma_vecinos + pixels.at(fil-1).at(col);
				total_vecinos++;
			}
			if (pixels.at(fil-1).at(col+1) >= 0){
				suma_vecinos = suma_vecinos + pixels.at(fil-1).at(col+1);
				total_vecinos++;
			}
			......

		Ya que violar�amos el principio de Una �nica Vez (se repite c�digo)
		*/

		/*
		AVANZADO:
			Hemos optado por no generar ninguna excepci�n en este m�todo.

			Si nos pasan como par�metros valores de fil y col incorrectos,
			el m�todo at del vector privado lanzar� la excepci�n out_of_range
			que corresponde a �ndice fuera del rango.
			Nota: La clase de excepci�n out_of_range hereda (es un tipo particular) de logic_error

			De esta forma tendr�amos:
				main --> Imagen.MediaVecinosCorrectos --> vector.at --> genera excepci�n que se recoge en main
			Nota: Recordad lo visto en clase sobre el mecanismo RAII de liberaci�n de memoria en la pila seguido por C++

			Ahora bien, si estuvi�semos trabajando con im�genes muy grandes, 
			convendr�a acceder con [] para que el acceso fuese m�s r�pido. 
			En este caso, al haber puesto p�blico el m�todo MediaVecinosCorrectos, 
			al principio de �ste deber�amos garantizar que fil y col est�n en los rangos adecuados
			con el if correspondiente. En caso de que no se satisficiese la precondici�n,
			lanzar�amos una excepci�n logic_error (o out_of_range)

			Para aumentar a�n m�s la eficiencia, podr�amos definir un m�todo privado que 
			no realizase la comprobaci�n de la precondici�n e hiciese los c�mputos deseados.
			El m�todo p�blico MediaVecinosCorrectos llamar�a al privado comprobando antes 
			la precondici�n y lanzando la excepci�n en el caso de que no se satisficiese.
		*/

		/*
		Algoritmo:
			Recorremos los pixels de las tres filas y columnas que hay alrededor 
			del pixel que han pasado exceptuando el propio pixel.
			Si es un pixel correcto lo contabilizamos para la media aritm�tica.

		Nota:
			Para excluir del c�mputo de la media el propio pixel, 
			debemos realizar la comprobaci�n i != j
			En el examen se pide que se calcule la media de los vecinos
			de aquellos pixeles que son incorrectos.
			Por tanto, cuando i == j, el pixel es incorrecto; por lo tanto 
			podr�amos suprimir la comprobaci�n if (i != j)
			ya que posteriormente se realiza la comprobaci�n if (EsPixelCorrecto(i, j))

			Sin embargo, es mejor mantener la comprobaci�n if (i != j)
			ya que el m�todo MediaVecinosCorrectos podr�a ser llamado
			en cualquier otra situaci�n, como por ejemplo, el c�mputo
			de la media de los vecinos de un pixel que S� sea correcto.
		*/

		double suma_vecinos = 0.0;
		int total_vecinos = 0;

		for (unsigned int i = fil - 1; i <= fil + 1; i++)
			for (unsigned int j = col - 1; j <= col + 1; j++)
				if (i != j)				
					if (EsPixelCorrecto(i, j)){
						suma_vecinos = suma_vecinos + pixels.at(i).at(j);
						total_vecinos++;
					}
	
		if (total_vecinos == 0)
			return 0;
		else
			return suma_vecinos / total_vecinos;
	}

	// Devuelve una imagen suavizada, es decir, sustiyendo cada pixel por
	// el promedio de sus vecinos correctos -> Mean Filter for Smoothing
	Imagen Suavizada(){
		/*
		B�SICO: 
			No devolvemos un vector < vector <double> > sino otro objeto de la clase Imagen,
			para que luego podamos usarlo en cualquier otro sitio como cualquier otra imagen.
		*/
		/*
		Algoritmo:
			Copiamos todos los pixels originales en pixels_suavizada
			Recorremos todos los pixels de la copia, sustituyendo cada pixel incorrecto
			por la media de sus vecinos originales
		*/
		/*
		AVANZADO:
			Hemos copiado toda la imagen original en la suavizada y luego modificamos los
			valores incorrectos. En el caso de que la imagen fuese muy grande y tuvi�semos
			usualmente muchos valores incorrectos, se podr�a evitar tener que copiar todos los datos
			al principio, por lo que deber�amos ir almacenando los valores (corregidos o no) uno a uno.
		*/	

		vector < vector<double> > pixels_suavizada (pixels);
		unsigned int filas_utilizadas, columnas_utilizadas;

		filas_utilizadas = TotalFilas();

		if (filas_utilizadas != 0){                    // Si la imagen est� vac�a, devolvemos una imagen vac�a
			columnas_utilizadas = TotalColumnas();

			unsigned int ultima_fila = filas_utilizadas - 2;
			unsigned int ultima_colu = columnas_utilizadas - 2;
			
			for (unsigned int i=1; i <= ultima_fila; i++)
				for (unsigned int j=1; j <= ultima_colu; j++)
					if (! EsPixelCorrecto(i, j))
						pixels_suavizada.at(i).at(j) = MediaVecinosCorrectos(i,j);
		}

		Imagen suavizada(pixels_suavizada);

		return suavizada;
	}

	// Aqu� ir�an todos los m�todos u operaciones que quisi�ramos hacer con una imagen.
};

class LectorImagen{
private:
	unsigned int numero_filas, numero_columnas;
public:
	LectorImagen(unsigned int numero_filas_imagen, unsigned int numero_columnas_imagen)
		:numero_filas(numero_filas_imagen), numero_columnas(numero_columnas_imagen)
	{
	}

	Imagen Lee(){
		vector < vector<double> > pixels;
		vector <double> fila;
		double pixel;

		/*
			Creamos un �nico objeto fila fuera del bucle y lo vamos reutilizando
			en cada iteraci�n, ejecutando cada vez el m�todo clear.
		*/

		fila.reserve(numero_columnas);
		pixels.reserve(numero_filas * numero_columnas);

		for (unsigned int i=0; i<numero_filas; i++){
			for (unsigned int j=0; j<numero_columnas; j++){
				cin >> pixel;
				fila.push_back(pixel);
			}

			pixels.push_back(fila);
			fila.clear();
		}

		Imagen leida(pixels);

		return leida;
	}
};

class ImpresorImagen{
private:
	string inicio, final, intermedio, entre_lineas;
public:
	ImpresorImagen(string cadena_inicio, string cadena_final, string cadena_intermedio, string cadena_entre_lineas)
		:inicio(cadena_inicio), final(cadena_final), intermedio(cadena_intermedio), entre_lineas(cadena_entre_lineas)
	{
	}

	void Imprime(Imagen imagen_a_imprimir){
		int total_filas = imagen_a_imprimir.TotalFilas(),
			 penultimo_columna = imagen_a_imprimir.TotalColumnas() - 1;

		for (int f=0; f<total_filas; f++){
			cout << inicio;

			for (int c=0; c<= penultimo_columna; c++)
				cout << imagen_a_imprimir.Pixel(f,c) << intermedio;
			
			cout << imagen_a_imprimir.Pixel(f,penultimo_columna) << final;
			cout << entre_lineas;
		}
   }
};

class MiVectorCaracteres{
private:
	static const int MAX = 100;
	char vector_privado[MAX]; 
	unsigned int total_utilizados;
public:
	MiVectorCaracteres(string cadena){
		int tope = cadena.size();

		for (int i=0; i<tope; i++)
			vector_privado[i] = cadena[i];

		total_utilizados = tope;
	}

	unsigned int TotalUtilizados(){
		return total_utilizados;
	}

	char Elemento(unsigned int indice){
		return vector_privado[indice];
	}

	void Reemplazar(unsigned int pos_inicio, unsigned int numero_posiciones_a_quitar, MiVectorCaracteres a_insertar){
		/*
		B�SICO:

		Llamemos diferencia_componentes
		a la diferencia entre las posiciones a quitar y el tama�o del vector a insertar 

		Si el vector a insertar es m�s peque�o que el n�mero de componentes a quitar
			es decir, si diferencia_componentes > 0 
			hay que acortar el vector => 
			Desplazar  hacia la izquierda todas las componentes que hay entre
			(pos_inicio + numero_posiciones_a_quitar) y el final	
		else
			hay que agrandar el vector =>
			Desplazar hacia la derecha todas las componentes que hay entre
			(pos_inicio + numero_posiciones_a_quitar) y el final	

		Finalmente, volcaremos el vector a insertar en las posiciones indicadas.

		�C�mo realizamos los desplazamientos?
		
		Opci�n Ineficiente:
			Para acortar el vector: 
			Repetir para todas y cada una de las componentes -i- que haya que eliminar
				Desplazar a la izquierda una posici�n todas las componentes que hay a la derecha de -i-

			Para agrandar el vector:
			Repetir para todas y cada una de las componentes -i- que haya que insertar nuevas
				Desplazar a la derecha una posici�n todas las componentes que hay a la derecha de -i-
		
		Esta opci�n es ineficiente tal y como se indic� en los ejercicios
		Eliminar May�sculas y Eliminar Exceso de Blancos de la Relaci�n de Problemas IV.
		En su lugar, debemos ir colocando directamente cada componente en la situaci�n que le corresponde.
		Nos quedar�a:
		
		Algoritmo:
			diferencia_componentes = diferencia entre las posiciones a quitar y el tama�o del vector a insertar
			Si (diferencia_componentes > 0) => Hay que acortar el vector
				Trasladamos a la izda las componentes que hay 
				por encima de pos_inicio + numero_posiciones_a_quitar
			else
				Trasladamos a la derecha las componentes que hay 
				por encima de pos_inicio + numero_posiciones_a_quitar	
			
			Volcamos el vector a insertar en el "hueco" que ha quedado
		*/
		/*

		int lectura, escritura, tamanio_a_insertar, diferencia_componentes;

		tamanio_a_insertar = a_insertar.TotalUtilizados();

		if (tamanio_a_insertar + total_utilizados > MAX)
			throw logic_error ("No hay componentes suficientes");
		else if (numero_posiciones_a_quitar > total_utilizados - pos_inicio)
			throw logic_error ("Se han especificado m�s componentes a eliminar de las que hay");
		else if (pos_inicio > total_utilizados)
			throw logic_error ("Posici�n de inicio inv�lida");
		else{
			diferencia_componentes = numero_posiciones_a_quitar - tamanio_a_insertar;

			if ( diferencia_componentes > 0 ){     // Hay que acortar el vector
				escritura = pos_inicio + tamanio_a_insertar;

				for (lectura =  pos_inicio + numero_posiciones_a_quitar; 
					  lectura < total_utilizados;
					  lectura++){

						vector_privado[escritura] = vector_privado[lectura];
						escritura++;
				}
			}
			else{   
				escritura = total_utilizados - diferencia_componentes - 1;  // diferencia < 0 => escritura > total_utilizados
				int tope_izda = pos_inicio + numero_posiciones_a_quitar - 1;

				for (lectura =  total_utilizados - 1; 
					  lectura > tope_izda ;
					  lectura--){

						vector_privado[escritura] = vector_privado[lectura];
						escritura--;
				}
			}
		
			for (int i=0; i<tamanio_a_insertar; i++)
				vector_privado[pos_inicio + i] = a_insertar.Elemento(i);

			total_utilizados = total_utilizados - diferencia_componentes;
		}
		*/

		/*
		Podemos unificar el c�digo anterior en un �nico bucle usando una variable gen�rica
		avance_siguiente que valdr� +1 o -1 dependiendo de si el avance es hacia delante
		o hacia atr�s. 
		De esta forma, si tuvi�semos que realizar cualquier tipo de procesamiento,
		s�lo tendr�amos que a�adir instrucciones en un �nico sitio.
		*/
		
		unsigned int lectura, escritura,  tamanio_a_insertar, total_utilizados;
		int avance_siguiente, diferencia_componentes, numero_componentes_a_desplazar;

		tamanio_a_insertar = a_insertar.TotalUtilizados();

		if (tamanio_a_insertar + total_utilizados > MAX)
			throw logic_error ("No hay componentes suficientes");
		else if (numero_posiciones_a_quitar > total_utilizados - pos_inicio)
			throw logic_error ("Se han especificado m�s componentes a eliminar de las que hay");
		else if (pos_inicio > total_utilizados)
			throw logic_error ("Posici�n de inicio inv�lida");
		else{
			diferencia_componentes = numero_posiciones_a_quitar - tamanio_a_insertar;
			 
			if ( diferencia_componentes > 0 ){   // Hay que acortar el vector
				avance_siguiente = 1;
				lectura   = pos_inicio + numero_posiciones_a_quitar;
				escritura = pos_inicio + tamanio_a_insertar;
			}
			else{
				avance_siguiente = -1;
				lectura   = total_utilizados - 1;
				escritura = total_utilizados - 1 - diferencia_componentes;
			}

			numero_componentes_a_desplazar = total_utilizados - numero_posiciones_a_quitar - pos_inicio;

			for (int cuenta=0; cuenta < numero_componentes_a_desplazar; cuenta++){
				vector_privado[escritura] = vector_privado[lectura];
				lectura   = lectura   + avance_siguiente;
				escritura = escritura + avance_siguiente;
			}

			for (unsigned int i=0; i<tamanio_a_insertar; i++)
				vector_privado[pos_inicio + i] = a_insertar.Elemento(i);
		}

		total_utilizados = total_utilizados - diferencia_componentes;	// Si la diferencia es negativa, habr� m�s componentes
	}
};


class MiVectorCaracteres_STL_privado{
private:
	vector <char> vector_privado; 	
public:
	MiVectorCaracteres_STL_privado(string cadena){
		int tope = cadena.size();
		vector_privado.reserve(tope);

		for (int i=0; i<tope; i++)
			vector_privado.push_back(cadena[i]);
	}

	unsigned int TotalUtilizados(){
		return vector_privado.size();
	}

	char Elemento(unsigned int indice){
		return vector_privado.at(indice);
	}

	void Reemplazar(unsigned int pos_inicio, unsigned int numero_posiciones_a_quitar, MiVectorCaracteres_STL_privado a_insertar){
		/*
		B�SICO:
			Si en vez de usar un vector cl�sico como dato miembro se usa un vector de la STL,
			la implementaci�n es muy similar pero tenemos que tener cuidado de no acceder a
			componentes que no est�n siendo utilizadas, pues saltar�a una excepci�n.
			Tambi�n tenemos que acortar o agrandar el vector, para lo que usaremos el m�todo resize.
			Todas las l�neas del programa ser�an iguales salvo que:
			- Si hay que acortar el vector, haremos un resize DESPU�S de haber trasladado las componentes a la izquierda
			- Si hay que agrandar el vector, haremos un resize ANTES de trasladar las componentes a la derecha

			Otra alternativa muy f�cil y r�pida de implementar ser�a:

				Usar un vector local auxiliar: copia.

				Volcar las componentes del original que hay antes de pos_inicio en copia.
				Volcar el nuevo en copia
				Volcar las componentes del original que hay por encima de pos_inicio + numero_posiciones_a_quitar en copia
				Asignar copia al original

			Esta soluci�n tiene el inconveniente de que duplica la memoria necesaria, aunque no es tan grave
			como la soluci�n del algoritmo con desplazamientos repetidos.
		*/
		
		unsigned int lectura, escritura,  tamanio_a_insertar, total_utilizados;
		int avance_siguiente, diferencia_componentes, numero_componentes_a_desplazar;

		total_utilizados = vector_privado.size();
		tamanio_a_insertar = a_insertar.TotalUtilizados();

		if (numero_posiciones_a_quitar > total_utilizados - pos_inicio)
			throw logic_error ("Se han especificado m�s componentes a eliminar de las que hay");
		else if (pos_inicio > total_utilizados)
			throw logic_error ("Posici�n de inicio inv�lida");
		else{
			diferencia_componentes = numero_posiciones_a_quitar - tamanio_a_insertar;
			 
			if ( diferencia_componentes > 0 ){   // Hay que acortar el vector
				avance_siguiente = 1;
				lectura   = pos_inicio + numero_posiciones_a_quitar;
				escritura = pos_inicio + tamanio_a_insertar;
			}
			else{
				avance_siguiente = -1;
				lectura   = total_utilizados - 1;
				escritura = total_utilizados - 1 - diferencia_componentes;
				vector_privado.resize(total_utilizados - diferencia_componentes);  // <<<<<<<<<<<<<<<<
			}

			numero_componentes_a_desplazar = total_utilizados - numero_posiciones_a_quitar - pos_inicio;

			for (int cuenta=0; cuenta < numero_componentes_a_desplazar; cuenta++){
				vector_privado[escritura] = vector_privado[lectura];
				lectura   = lectura   + avance_siguiente;
				escritura = escritura + avance_siguiente;
			}

			for (unsigned int i=0; i<tamanio_a_insertar; i++)
				vector_privado[pos_inicio + i] = a_insertar.Elemento(i);
		}

		if ( diferencia_componentes > 0 )
			vector_privado.resize(total_utilizados - diferencia_componentes);	     // <<<<<<<<<<<<<<<<
	}
};


class ListadoEquipos{
private:
	vector <string> nombres;
public:
	ListadoEquipos(vector <string> nombres_equipos)
		:nombres(nombres_equipos){
	}

	unsigned int TotalUtilizados(){
		return nombres.size();
	}

	string Nombre(int indice){
		return nombres.at(indice);
	}

	// Forma todos los posibles emparejamientos de dos equipos
	vector <vector <string> > Emparejamientos(){
		/*
		Algoritmo:
			Recorrer todo el vector de equipos
				Por cada uno de ellos, emparejarlo con cada uno de 
				los equipos	que hay a su derecha.
		*/
		/*
		B�SICO:
			Cada una de las parejas se representa con un vector <string>
			Tambi�n podr�amos haber usado una clase Pareja con dos datos miembros:
			PrimerEquipo, SegundoEquipo.
		*/
		/*
		AVANZADO:
			Necesitamos un objeto pareja.
			Podr�amos haberlo declarado dentro del bucle, pero entonces, cada vez que entrase
			al bucle, tendr�a que crear un objeto nuevo, lo cual ser�a ineficiente.
			Mejor si construimos un �nico objeto fuera del bucle y lo vamos "limpiando"
			en cada iteraci�n.
		*/

		vector <vector <string> > emparejamientos;
		vector <string> pareja;
		int ultimo;
		
		ultimo = nombres.size() - 1;

		for (int inicio=0; inicio < ultimo ; inicio++){
			pareja.push_back(nombres[inicio]);

			for (int j=inicio + 1; j <= ultimo; j++){
				pareja.push_back(nombres[j]);
				emparejamientos.push_back(pareja);
				pareja.pop_back();
			}

			pareja.clear();
		}

		return emparejamientos;
	}
};


int NumeroCuadrados(double x_esq_inf_izda, double y_esq_inf_izda, double x_esq_sup_dcha, double y_esq_sup_dcha, double area_minima){
	/*
	B�SICO:
		- En vez de las coordenadas, tambi�n podr�a interpretarse del enunciado que se puede pasar el 
		  �rea de un cuadrado, es decir:

				int NumeroCuadrados(double area, double area_minima)
		  
		  Lo que NO ser�a correcto ser�a pasar coordenadas Y �rea, ya que, entonces, podr�amos haber pasado
		  como par�metros actuales un �rea que no se correspondiese con las coordenadas.

		- El �rea m�nima no puede ser un dato local, pues entonces la funci�n s�lo permitir�a trabajar
		  con dicho valor local (�nico para todas las llamadas)

		- Tambi�n se podr�an haber pasado como par�metros dos objetos de la clase Punto2D en vez de las cuatro coordenadas por separado.
	*/

	double base, altura, area;

	base	 = x_esq_sup_dcha - x_esq_inf_izda;
	altura = y_esq_sup_dcha - y_esq_inf_izda;
	area   = base * altura;

	if (area < area_minima)
		return 0;
	else
		return 1 + NumeroCuadrados(x_esq_inf_izda, y_esq_inf_izda, x_esq_sup_dcha - base/2.0, y_esq_sup_dcha - altura/2.0, area_minima);
}

class ImpresorVectorString{
private:
	string inicio, final, intermedio;
public:
	ImpresorVectorString(string cadena_inicio, string cadena_final, string cadena_intermedio)
		:inicio(cadena_inicio), final(cadena_final), intermedio(cadena_intermedio)
	{
	}

	void Imprime(vector <string> conjuntos_cadenas){
		int tope = conjuntos_cadenas.size() - 1;

		cout << inicio;

      for (int i=0; i<tope; i++)
         cout << conjuntos_cadenas[i] << intermedio;

      cout << conjuntos_cadenas[tope] << final;
	}
};

int main(){
	/*
	LectorImagen lector_imagenes(5,6);
	ImpresorImagen impresor_imagenes("{","}"," ","\n");

	Imagen una_imagen(lector_imagenes.Lee());
	Imagen imagen_suavizada(una_imagen.Suavizada());

	cout << "\nImagen original:\n";
	impresor_imagenes.Imprime(una_imagen);

	cout << "\n\nImagen suavizada:\n";
	impresor_imagenes.Imprime(imagen_suavizada);
	*/
	
	
	MiVectorCaracteres_STL_privado palabra("123456");
   MiVectorCaracteres_STL_privado a_insertar("ABCDEFG");

	// palabra.Reemplazar(6,0,a_insertar);
	palabra.Reemplazar(1,2,a_insertar);

	int tope = palabra.TotalUtilizados();

	for (int i=0; i<tope; i++)
		cout << palabra.Elemento(i);
	
	cout << "|"; 
	
	
	/*
	vector <string> nombres_equipos;

	//nombres_equipos.push_back("1 R. Madrid");
	//nombres_equipos.push_back("2 At. Granada");
	//nombres_equipos.push_back("3 Barcelona CF");
   //nombres_equipos.push_back("4 At. Bilbao");

	nombres_equipos.push_back("1");
	nombres_equipos.push_back("2");
	nombres_equipos.push_back("3");
    nombres_equipos.push_back("4");
	nombres_equipos.push_back("5");
	nombres_equipos.push_back("6");


	ListadoEquipos listado(nombres_equipos);

	vector <vector <string>> emparejamientos;

	// emparejamientos = listado.Emparejamientos();
	emparejamientos = listado.Combinaciones();

	ImpresorVectorString impresor_vector_string("{", "}", ",");

	int total_emparejamientos = emparejamientos.size();
	
	for (int i=0; i<total_emparejamientos; i++){
		impresor_vector_string.Imprime(emparejamientos.at(i));
		cout << "\n";
	}

	cout << "\nTotal elementos = " << emparejamientos.size();
	*/

	cout << "\n\n";
	system("pause");
}

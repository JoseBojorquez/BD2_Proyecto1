# BD2_Proyecto1
## Integrantes:
- José Bojorquez
- Sebastian Tenorio
- Carlos Flores
- José Osnayo
## Introducción
Para el presente proyecto se implementaron estrategias de organización de archivos para el manejo de un sistema de gestión de base de datos. Bajo esta premisa, implementamos un parser SQL y elegimos tres métodos de organización de archivos: Sequential File, ISAM y Extendible Hashing. 
## Dominio de datos
Dentro del proyecto, trabajaremos con los siguientes 2 datasets definidos para la carga de archivos .csv:\
### Credit Card Fraud Detection Dataset 2023
#### Descripción de las variables 

| **Columna** | **Descripción**                                                                 |
|-------------|---------------------------------------------------------------------------------|
| **id**      | Identificador único para cada transacción                                       |
| **V1-V28**  | Features anónimas que representan varios atributos de transacción (por ejemplo, hora, ubicación, etc.) |
| **Amount**  | El monto de la transacción                                                      |
| **Class**   | Etiqueta binaria que indica si la transacción es fraudulenta (1) o no (0)       |

\
*Source:* https://www.kaggle.com/datasets/nelgiriyewithana/credit-card-fraud-detection-dataset-2023 
### 500,000+ US Homes Data (For Sale Properties) 
#### Descripción de las variables (12 de 28)
| **Columna**      | **Descripción**                                       |
|------------------|-------------------------------------------------------|
| **id**           | Identificador único de la propiedad.                  |
| **address**      | Dirección de la propiedad.                            |
| **city**         | Ciudad donde se encuentra la propiedad.               |
| **state**        | Estado donde se encuentra la propiedad.               |
| **zip**          | Código postal de la propiedad.                        |
| **price**        | Precio de venta de la propiedad.                      |
| **beds**         | Número de habitaciones.                               |
| **baths**        | Número de baños.                                     |
| **area**         | Área en pies cuadrados.                              |
| **property_type**| Tipo de propiedad (e.g., casa, apartamento).         |
| **sale_type**    | Tipo de venta (e.g., venta normal, subasta).         |

\
*Source:* https://www.kaggle.com/datasets/polartech/500000-us-homes-data-for-sale-properties 

## Resultados esperados
Se espera que las técnicas de indexación elegidas tengan una notoria diferencia de desempeño entre sí dependiendo de las operaciones a realizar dentro del manejo de archivos.

## Funciones implementadas
```c++
Record search(T key);
vector <Record> rangeSearch(T begin-key, T end-key);
void insert(Record registro);
void remove(T key);
```
## Organización de Archivos
Las estrategias usadas son las siguientes:
+ Sequential File
+ ISAM
+ Extendible Hashing

### Sequential File
![seqFile](/Imagenes/seqFile.png) \
**Insert:** Hace inserciones dependiendo de la posición donde debe ser ingresado el nuevo registro. Si el espacio está libre, inserta en el archivo principal. De otro modo, se inserta en el espacio auxiliar, manteniendo actualizados los punteros de cada uno de los registros. \
\
**Find:** Deberá buscar primero en el archivo principal y en caso de no encontrarlo se moverá a buscarlo en el espacio auxiliar. \
\
**Remove:** Se usa los punteros para saltar el registro eliminado y en la reconstrucción del archivo se eliminan completamente. \
\
**RangeSearch:** Se busca la primera key inicial y a partir de ahí se itera hasta llegar al último registro que sea menor o igual a la key final. \
\
**Rebuild:** Reconstruye el archivo una vez se haya alcanzado el tope de inserciones en el archivo auxiliar. \

### ISAM
![Isam](/Imagenes/isam.png) \
**Insert:** Escribe el registro en el archivo de datos y actualiza los índices primarios y terciarios. Lo cual implica leer los índices existentes y reorganizarlos si es necesario\
\
**Find:** Deberá buscar buscar en el índice terciario y obtener el bloque del índice secundario. Luego, buscar en el índice secundario para obtener el bloque del índice primario y finalmente buscar en el índice primario para obtener la posición del registro en el archivo de datos \
\
**Remove:** Busca y retira de la hoja en el archivo de datos. Si la página está vacía, hace de-allocate. \
\
**RangeSearch:** Comienza en la raíz y utiliza la key de comparaciones para ir hasta la hoja. Una vez ahí, recoge todos los archivos mayores o iguales a la key begin y cuando acaba regresa para buscar más registros en la hoja siguiente \
\
### Extendible Hashing
![extendibleHashing](/Imagenes/extendibleHashing.png) \
**Insert:** Sea D la profundidad global del índice, aplica la función hash sobre la clave de búsqueda y obtener la secuencia de bits con longitud D. Después hace coincidir esa secuencia con una entrada en el directorio y se dirige al bucket correspondiente para encontrar el registro. \
\
**Find:** Se usa de manera parecida la secuencia D-bit para dirigirse al bucket en cuestión. Si no encuentra el bucket, procede a crear uno nuevo. Si encontró un bucket y no esta lleno, proceder a insertar. Si el bucket está lleno, divide el bucket y reinserta todos los registros actualizando el directorio. \
\
**Remove:** Localiza el buffer respectivo y remueve el registro. Si el bucket queda vacío, puede ser descartado dentro de los buckets manejados. Si dos buckets tienen pocos elementos y tienen el mismo prefijo en la profundidad local anterior (D-1), se procede a mezclar ambos buckets. \

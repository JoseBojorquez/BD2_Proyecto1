# BD2_Proyecto1
## Integrantes:
- Sebastian Tenorio
- Carlos Flores
- José Osnayo
## Introducción
Para el presente proyecto se implementaron estrategias de organización de archivos para el manejo de un sistema de gestión de base de datos. Bajo esta premisa, implementamos un parser SQL y elegimos tres métodos de organización de archivos: Sequential File, ISAM y Extendible Hashing. 
## Dominio de datos
Dentro del proyecto, trabajaremos con los siguientes 2 datasets definidos para la carga de archivos .csv:
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
#### Insert
La función `insert` maneja la adición de un nuevo registro a la tabla hash. Aquí está un resumen de su proceso:

1. **Generación del Hash**: La función comienza generando un valor hash para la clave del registro.
2. **Coincidencia de Índice**: Luego encuentra el registro de índice adecuado que coincide con este valor hash.
3. **Carga del Bucket**: El bucket correspondiente se carga desde el archivo.
4. **Manejo de Bucket Lleno**:
    - **Bucket Lleno pero Profundidad < Profundidad Máxima**: Si el bucket está lleno y su profundidad local es menor que la profundidad máxima, el bucket se divide. La función incrementa la profundidad local del bucket y reasigna los registros en dos nuevos buckets según sus valores hash. Se crea un nuevo registro de índice para el nuevo bucket.
    - **Bucket Lleno y Profundidad >= Profundidad Máxima**: Si la profundidad local del bucket ya está en el máximo, maneja el desbordamiento creando páginas de desbordamiento si es necesario e inserta el registro en la página de desbordamiento adecuada.
5. **Bucket No Lleno**: Si el bucket no está lleno, simplemente se añade el registro a él.
6. **Guardar Cambios**: El bucket (y cualquier nuevo bucket o páginas de desbordamiento) se guarda de nuevo en el archivo, y la función completa la inserción.

#### Remove
La función `remove` elimina un registro con una clave específica de la tabla hash. El proceso incluye:

1. **Generación del Hash**: Se genera un hash para la clave dada.
2. **Coincidencia de Índice**: Se encuentra el registro de índice que coincide.
3. **Recorrido del Bucket**: Se carga el bucket correspondiente y se busca el registro a eliminar.
4. **Páginas de Desbordamiento**: Se recorren las páginas de desbordamiento y se eliminan todas las apariciones del registro.
5. **Guardar Cambios**: Los buckets modificados se guardan de nuevo en el archivo.

#### Search
La función `search` recupera los registros que coinciden con una clave dada. El proceso incluye:

1. **Generación del Hash**: Se genera un hash para la clave dada.
2. **Coincidencia de Índice**: Se encuentra el registro de índice que coincide.
3. **Recorrido del Bucket**: Se carga el bucket correspondiente y se busca el registro.
4. **Páginas de Desbordamiento**: Se recorren las páginas de desbordamiento para encontrar todos los registros coincidentes.
5. **Devolver Resultados**: Se devuelve un vector de todos los registros que coinciden con la clave.

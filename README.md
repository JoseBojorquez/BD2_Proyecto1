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

### ISAM

### Extendible Hashing

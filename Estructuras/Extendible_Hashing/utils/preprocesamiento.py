# EXPERIMENTACION HECHO EN DEEP NOTE, ESTO SOLO ES EL CODIGO USADO PARA GENERAR LOS DATASETS_WITHCOMMAND
# https://deepnote.com/workspace/carlosflores-74ceb7d2-1a96-4831-a748-8a3f48b322f3/project/preprocesamiento-ec9d7bb7-5bf6-46f0-b39f-6a7f9891453a/notebook/Notebook%201-5cdd2043eeed4d6bbb68d6cef35b19d7
import pandas as pd
import os

def filter_csv(input_file, output_file_temp, output_file_final, num_rows):
    # Leer el archivo CSV con el parámetro delimitador y sin tratar comillas como delimitadores
    df = pd.read_csv(input_file, delimiter=',', quoting=0, nrows=num_rows)

    # Procesar la columna 'address' para quitar las comas dentro de esa columna
    df['address'] = df['address'].str.replace(',', '')

    # Seleccionar las columnas a mantener
    columns_to_keep = [
        "property_id", "address", "city", "state", "postcode",
        "price", "property_type", "property_status"
    ]
    filtered_df = df[columns_to_keep]

    # Guardar el DataFrame filtrado en un archivo temporal
    filtered_df.to_csv(output_file_temp, index=False, quoting=1, quotechar='"')

    print(f"Filtered CSV saved as {output_file_temp}")

    # Generar comandos para cada fila del archivo filtrado
    generate_commands(output_file_temp, output_file_final)


def generate_commands(input_file, output_file_temp):
    # Leer el archivo CSV sin la primera fila (encabezados)
    df = pd.read_csv(input_file, header=None, skiprows=1)

    # Iterar sobre cada fila y construir la cadena deseada
    with open(output_file_temp, 'w') as f:
        for index, row in df.iterrows():
            data_string = ','.join([f'"{value}"' for value in row])
            f.write(f"./extHash HomesRecord add {data_string}\n")

    print(f"Commands generated and saved as {output_file_temp}")


if __name__ == "__main__":
    input_file = '600K_US_Housing_Properties.csv'
    output_file_temp = 'HomesData600kfiltered_temp.csv'
    output_file_final = 'HomesData600kwith_command.csv'

    # Definir el número de filas a filtrar
    num_rows_to_filter = 600000  # Cambia este valor según sea necesario

    filter_csv(input_file, output_file_temp, output_file_final, num_rows_to_filter)

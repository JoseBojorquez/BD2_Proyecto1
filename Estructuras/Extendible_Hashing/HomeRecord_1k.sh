#!/bin/bash
# chmod +x HomeRecord_1k.sh
# Compilar el programa
g++ main.cpp -o extHash

# Verificar si la compilación fue exitosa
if [ $? -ne 0 ]; then
    echo "Error: Compilation failed."
    exit 1
fi

# Nombre del archivo CSV con los comandos
input_file="dataset/HomesData1kwith_command.csv"

# Iterar sobre cada línea del archivo CSV
while IFS= read -r line; do
    # Ejecutar la línea como un comando
    eval "$line"
done < "$input_file"

import sys

'''
replace_last_occurrence
Función utilizada para reemplazar la última o n-ésima aparición (desde el final) de una subcadena dentro de una cadena.
original_string: Cadena original
target_substring: Subcadena que se desea reemplazar
replacement_substring: Subcadena nueva que reemplazará a la anterior
occurrence: Selecciona cuál ocurrencia (desde el final) se desea reemplazar
'''
def replace_last_occurrence(original_string, target_substring, replacement_substring, occurrence):
    parts = original_string.rsplit(target_substring, occurrence)
    return replacement_substring.join(parts)

'''
El siguiente bloque de código se utiliza para abrir el archivo de destino
y reemplazar una subcadena con el contenido de otro archivo.
'''

target_file = open(sys.argv[1], "r")
target_content = target_file.read()
target_file.close()

# Abre el archivo que contiene el contenido a inyectar (payload)
payload_file = open(sys.argv[2], "r")
payload_content = payload_file.read()

# Realiza el reemplazo de la última ocurrencia de la línea que contiene "exit 0"
# por el contenido del payload
modified_content = replace_last_occurrence(target_content, 'exit 0', payload_content, 1)

# Escribe el contenido modificado en el archivo original
output_file = open(sys.argv[1], "w")
output_file.seek(0)
output_file.write(modified_content)
output_file.truncate()
output_file.close()

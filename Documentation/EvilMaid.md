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

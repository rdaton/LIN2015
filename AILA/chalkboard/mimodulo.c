#include <linux/module.h> /* Requerido por todos los módulos */
#include <linux/kernel.h> /* Definición de KERN_INFO */
MODULE_LICENSE("GPL"); /* Licencia del módulo */
/* Función que se invoca cuando se carga el módulo en el kernel */
int modulo_lin_init(void)
{
printk(KERN_INFO "Modulo LIN cargado. Hola kernel.\n");
return 0;
}
/* Devolver 0 para indicar una carga correcta del módulo */

/* Función que se invoca cuando se descarga el módulo del kernel */
void modulo_lin_clean(void)
{
printk(KERN_INFO "Modulo LIN descargado. Adios kernel.\n");
}
/* Declaración de funciones init y cleanup */
module_init(modulo_lin_init);
module_exit(modulo_lin_clean);

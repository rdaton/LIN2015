//fuente: https://phoxis.org/2013/09/05/finding-overall-and-per-core-cpu-utilization/

/* Show per core CPU utilization of the system
 * This is a part of the post https://phoxis.org/2013/09/05/finding-overall-and-per-core-cpu-utilization
 */
#include <stdio.h>
#include <unistd.h>
#define BUF_MAX 1024
#define MAX_CPU 128
#include <string.h>


int read_fields (FILE *fp, unsigned long long int *fields)
{
  int retval;
  char buffer[BUF_MAX];
 
 
  if (!fgets (buffer, BUF_MAX, fp))
  { perror ("Error"); }
  /* line starts with c and a string. This is to handle cpu, cpu[0-9]+ */
  retval = sscanf (buffer, "c%*s %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu %Lu",
                            &fields[0], 
                            &fields[1], 
                            &fields[2], 
                            &fields[3], 
                            &fields[4], 
                            &fields[5], 
                            &fields[6], 
                            &fields[7], 
                            &fields[8], 
                            &fields[9]); 
  if (retval == 0)
  { return -1; }
  if (retval < 4) /* Atleast 4 fields is to be read */
  {
    fprintf (stderr, "Error reading /proc/stat cpu field\n");
    return 0;
  }
  return 1;
}

char* dameColor(double unPorcentaje)
{
	char* verde="0x001100";	
	char* amarillo="0x111100";
	char* rojo="0x110000";	
	char salida[24];
	
	if (unPorcentaje < 50)
		strcpy(salida,verde);
	else 
	if (unPorcentaje < 90)
		strcpy(salida, amarillo);
	else
		strcpy(salida,rojo);
	
	
}

void imprimirStick(double* ochoPorcentajes)
{
	char cadenas[8][24];
	char salida[200];
	int i=0;	
	for (i=0;i<8;i++)
	{			

		char numLed[2];	
		snprintf(numLed,sizeof(numLed),"%d",i);
		strcpy(cadenas[i],numLed);		
		strcat(cadenas[i],":");
		strcat(cadenas[i],dameColor(ochoPorcentajes[i]));		
		strcat(cadenas[i],",");		
	}
	
	for (i=0;i<8;i++)
	{
			printf(cadenas[i]);
			strcat(salida,cadenas[i]);
			printf("\n");
	}
	
	printf (salida);
	
	
}

int main (void)
{
  double ochoPorcentajes[8] ;
  FILE *fp;
  unsigned long long int fields[10], total_tick[MAX_CPU], total_tick_old[MAX_CPU], idle[MAX_CPU], idle_old[MAX_CPU], del_total_tick[MAX_CPU], del_idle[MAX_CPU];
  int update_cycle = 0, i, cpus = 0, count;
  double percent_usage;
 
 
  fp = fopen ("/proc/stat", "r");
  if (fp == NULL)
  {
    perror ("Error");
  }
 
 
  while (read_fields (fp, fields) != -1)
  {
    for (i=0, total_tick[cpus] = 0; i<10; i++)
    { total_tick[cpus] += fields[i]; }
    idle[cpus] = fields[3]; /* idle ticks index */
    cpus++;
  }
 
  while (1)
  {
    sleep (1);
    fseek (fp, 0, SEEK_SET);
    fflush (fp);
    printf ("[Update cycle %d]\n", update_cycle); 
    for (count = 0; count < cpus; count++)
    {
      total_tick_old[count] = total_tick[count];
      idle_old[count] = idle[count];
     
      if (!read_fields (fp, fields))
      { return 0; }
 
      for (i=0, total_tick[count] = 0; i<10; i++)
      { total_tick[count] += fields[i]; }
      idle[count] = fields[3];
 
      del_total_tick[count] = total_tick[count] - total_tick_old[count];
      del_idle[count] = idle[count] - idle_old[count];
 
      percent_usage = ((del_total_tick[count] - del_idle[count]) / (double) del_total_tick[count]) * 100;
      ochoPorcentajes[count]=percent_usage;
      if (count == 0)
      { printf ("Total CPU Usage: %3.2lf%%\n", percent_usage); }
      else
      { printf ("\tCPU%d Usage: %3.2lf%%\n", count - 1, percent_usage); }
    }
    imprimirStick(ochoPorcentajes);
    update_cycle++;
    printf ("\n");
  }
 
  /* Ctrl + C quit, therefore this will not be reached. We rely on the kernel to close this file */
  fclose (fp);
  
  return 0;
}

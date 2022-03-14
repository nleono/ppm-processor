#include <error.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include "ppm.h"
#include "ppm-operations.h"

extern int errno;
extern char *optarg;
extern int opterr, optind;

#define OP_I 'i'
#define OP_O 'o'
#define OP_R 'r'
#define OP_H 'h'
#define OP_V 'v'
#define OP_B 'b'
#define OP_N 'n'
#define HELP '?'

#define OPTION_STRING "i:o:b:nrhv?"

static void usage(char *argv[], int code){
  printf("MODO DE USO: %-12s -i [ARCHIVO PPM ENTRADA] -o [ARCHIVO PPM DE SALIDA] [OPCION] ...\n", argv[0]);
  
  printf("\nOPCIONES: \n");
  printf(" -i <INPUT PPM FILE>        archivo origen. OPCION REQUERIDA\n");
  printf(" -o <OUTPUT PPM FILE>       archivo salida. OPCION REQUERIDA\n");
  printf(" -r                         rota la imagen 90º hacia la derecha\n"); 
  printf(" -n                         genera el negativo de la imagen\n"); 
  printf(" -h                         espeja horizontalmente la imagen\n");
  printf(" -v                         espeja verticalmente la imagen\n");
  printf(" -b <NUM>                   genera desenfoque con radio NUM\n");
  printf(" -?                         muestra esta ayuda y finaliza\n\n");

  exit(code);
}

/* Main program */

int main(int argc, char* argv[]) {
  FILE *f, *op;
  t_ppm p, p_op;

  char *input_filename, *output_filename;
  unsigned char ratio;
  
  int opt;
  int i_count = 0, o_count = 0;
  input_filename = output_filename = (char *) NULL;
  

  while ((opt = getopt(argc, argv, OPTION_STRING)) != -1) {
    switch (opt)
    {
      case OP_I:        
        input_filename = optarg;
        if (input_filename == NULL){
          error(1, errno, "Missing input file name"); 
        }
        if (i_count != 0){
          error(1, errno, "Too many input files" );  
        }
        /* Opening files and creating ppm struct */
        if ((f = fopen(input_filename, "rb")) == NULL) {
          error(1,errno, "Error opening file %s", input_filename);
        }      
        p = ppm_from_file(f);
        i_count++;
        break;
      case OP_O:
        if (i_count != 1){
          error(1, errno, "Error. An input file must be specified as the first argument");  
        }
        output_filename = optarg;
        if (output_filename == NULL){
          error(1, errno, "Missing output file name"); 
        }
        if (o_count != 0){
          error(1, errno, "Too many output files" );  
        }
        if ((op = fopen(output_filename, "wb")) == NULL) {
          error(1,errno, "Error opening file %s", output_filename);
        }
        o_count++;
        break;
      case OP_R:
        if (i_count != 1){
          error(1, errno, "Error. An input file must be specified as the first argument");  
        }
        p_op = ppm_operation_rotate90(p);
        ppm_free(p);
        p = p_op;
        break;
      case OP_V:
        if (i_count != 1){
          error(1, errno, "Error. An input file must be specified as the first argument");  
        }
        p_op = ppm_operation_flip_vertical(p);
        ppm_free(p);
        p = p_op;
        break;
      case OP_H:
        if (i_count != 1){        
          error(1, errno, "Error. An input file must be specified as the first argument");  
        }
        p_op = ppm_operation_flip_horizontal(p);
        ppm_free(p);
        p = p_op;
        break;
      case OP_B:
        if (i_count != 1){ 
          error(1, errno, "Error. An input file must be specified as the first argument");  
        }
        //ratio = (unsigned char) strtol(optarg, NULL, 10);
        if (sscanf(optarg, "%hhu", &ratio) != 1){
          ppm_free(p);
          fclose(f);
          if (o_count == 1) fclose(op);
          error(1, errno, "Error. Ratio must be a value between 0 and 255");
        }
        p_op = ppm_operation_blur(p, ratio);
        ppm_free(p);
        p = p_op;
        break;
      case OP_N:
        if (i_count != 1){ 
          error(1, errno, "Error. An input file must be specified as the first argument");  
        }
        p_op = ppm_operation_negative(p);
        ppm_free(p);
        p = p_op;
        break;
      case HELP:
        usage(argv, EXIT_SUCCESS);
        break;
      default:
        error(1, errno, "Invalid option");
        break;
    }
  }

  if (optind != argc) {
    if (i_count == 1){
      fclose(f);
      ppm_free(p);
    }
    if (o_count == 1){
      fclose(op);
    } 
    error(1,errno, "Invalid arguments");
  }

  if (o_count != 1){
    ppm_free(p);
    fclose(f);
    error(1, errno, "Error. No output file specified");
  }  

  /* Result ppm image saved in output file */
  ppm_save(p, op);

  /* Input and output file closed and PPM struct must free its memory */
  fclose(op);
  fclose(f);
  
  ppm_free(p);
  
  exit(EXIT_SUCCESS);
}

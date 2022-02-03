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

static int parse_args(int argc, char *argv[], 
                      bool *r_flag, bool *h_flag, bool *v_flag, bool *b_flag, bool *n_flag,
                      char **in_name, char **out_name, unsigned char *ratio){
  int opt;
  int i_count = 0, o_count = 0;
  *in_name = NULL;
  *out_name = NULL;

  while ((opt = getopt(argc, argv, OPTION_STRING)) != -1) {
    switch (opt)
    {
    case OP_I:
      i_count++;    
      *in_name = optarg;
      break;
    case OP_O:
      o_count++;
      *out_name = optarg;
      break;
    case OP_R:
      *r_flag= true;
      break;
    case OP_V:
      *v_flag = true;
      break;
    case OP_H:
      *h_flag = true;
      break;
    case OP_B:
      *b_flag = true;
      //if (strtol(optarg, NULL, 10) < 0 || > 255) --> habria que chequear esto?
      *ratio = (unsigned char) strtol(optarg, NULL, 10);
      break;
    case OP_N:
      *n_flag = true;
      break;
    case HELP:
      usage(argv, EXIT_SUCCESS);
      break;
    default:
      return (int) false;
      break;
    }
  }
  
  if (i_count != 1 || o_count != 1) return (int) false;         // error(1,0, "Error in input/output arguments");
  if (in_name == NULL || out_name == NULL) return (int) false;  // error(1,0, "Missing input/output filenames");
  if (*b_flag && ratio == NULL) return (int) false;             // error(1,0, "Missing blur radio value");
  if (optind != argc) return (int) false;                       // error(1,0, "Invalid arguments");
  return (int) true;
}


/* Main program */

int main(int argc, char* argv[]) {
  FILE *f, *op;
  char *input_filename, *output_filename;
  t_ppm p, p_op;
  bool h_flag, v_flag, r_flag, b_flag, n_flag;
  unsigned char ratio;
  h_flag = v_flag = r_flag = b_flag = n_flag = false;

  if (!parse_args(argc, argv, &r_flag, &h_flag, &v_flag, &b_flag, &n_flag, &input_filename, &output_filename, &ratio)){
    usage(argv, EXIT_FAILURE);
  }

  /* Opening files and creating ppm struct */
  if ((f = fopen(input_filename, "rb")) == NULL) {
    error(1,errno, "Error opening file %s", input_filename);
  }
  p = ppm_from_file(f);

  if ((op = fopen(output_filename, "wb")) == NULL) {
    error(1,errno, "Error opening file %s", output_filename);
  }

  /* PPM processing*/
  p_op = ppm_copy(p);
  if (r_flag) p_op = ppm_operation_rotate90(p_op);
  if (n_flag) p_op = ppm_operation_negative(p_op);
  if (h_flag) p_op = ppm_operation_flip_horizontal(p_op);
  if (v_flag) p_op = ppm_operation_flip_vertical(p_op);
  if (b_flag) p_op = ppm_operation_blur(p_op, ratio);
  
  ppm_save(p_op, op);

  /* Output file closed and PPM struct must free its memory */
  fclose(op);
  ppm_free(p_op);

  /* Input file closed and PPM struct must free its memory */
  ppm_free(p);
  fclose(f);
  exit(EXIT_SUCCESS);
}

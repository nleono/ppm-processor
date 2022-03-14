#include "ppm.h"

t_ppm ppm_copy(t_ppm p){
  t_ppm new = ppm_create(p.height, p.width, p.depth);
  for (int i =0; i< p.height; i++){
    for (int j=0; j<p.width; j++){
      new.pixels[i][j] = p.pixels[i][j];
    }
  }
  return new;
}

t_ppm ppm_operation_rotate90(t_ppm p){
  t_ppm new = ppm_create(p.width, p.height, p.depth);
  unsigned int row,col,new_col;
  for (row=0; row < p.height; row++){
    new_col = p.height - row -1;
    for (col=0; col < p.width; col++){
	    new.pixels[col][new_col]= p.pixels[row][col];
    }
  }
  return new;
}


t_ppm ppm_operation_flip_horizontal(t_ppm p){
  t_ppm new = ppm_create(p.height, p.width, p.depth);
  unsigned int row,col;
  for (row=0; row < p.height; row++){
    for (col=0; col < p.width; col++){
      new.pixels[row][col]= p.pixels[row][p.width-col-1];
    }
  }
  return new;
}

t_ppm ppm_operation_flip_vertical(t_ppm p){
  t_ppm new = ppm_create(p.height, p.width, p.depth);
  unsigned int col, row;
  for (row=0; row < p.height; row++){
    for (col=0; col < p.width; col++){
      new.pixels[row][col] = p.pixels[p.height-row-1][col];
    }
  }
  return new;
}

int max(int a, int b){
  if (a>b) return a;
  return b;
}

int min(int a, int b){
  if (a<b) return a;
  return b;
}

t_ppm ppm_operation_blur(t_ppm p, unsigned char ratio){
  t_ppm new = ppm_create(p.height, p.width, p.depth);
  unsigned int row, col, i, j;
  unsigned int red, blue, green, pixel_count;
  unsigned int row_start, row_end, col_start, col_end;
  for (row=0; row < p.height; row++){
    row_start = max(row-ratio, 0);
    row_end = min(row+ratio, p.height-1);
    for (col=0; col<p.width; col++){
      col_start = max(col-ratio, 0);
      col_end = min(col+ratio, p.width-1);
      red = green = blue = pixel_count = 0;

      for (i=row_start; i<=row_end; i++){
        for (j=col_start; j<=col_end; j++){
          pixel_count++;
          red += p.pixels[i][j].red;
          green += p.pixels[i][j].green;
          blue += p.pixels[i][j].blue;
        }
      }
      new.pixels[row][col].red= red / pixel_count;
      new.pixels[row][col].green= green / pixel_count;
      new.pixels[row][col].blue= blue / pixel_count;
    }
  }
  return new;
}

void ppm_invert_colours(t_ppm_pixel *p){
  p->red=255-p->red;
  p->green=255-p->green;
  p->blue=255-p->blue;
}

t_ppm ppm_operation_negative(t_ppm p){
  unsigned int row,col; 
  t_ppm new = ppm_create(p.height, p.width, p.depth);
  for(row=0; row < p.height; row++) for(col=0; col < p.width ; col++){
    new.pixels[row][col] = p.pixels[row][col];
    ppm_invert_colours(&new.pixels[row][col]);
  }
  return new;
}


#ifndef __PPM_OPERATIONS_H
#define __PPM_OPERATIONS_H

t_ppm ppm_copy(t_ppm );
t_ppm ppm_operation_rotate90(t_ppm );
t_ppm ppm_operation_flip_horizontal(t_ppm );
t_ppm ppm_operation_flip_vertical(t_ppm );
t_ppm ppm_operation_negative(t_ppm );
t_ppm ppm_operation_blur(t_ppm , unsigned char);
//t_ppm ppm_operation_blur(t_ppm);
#endif


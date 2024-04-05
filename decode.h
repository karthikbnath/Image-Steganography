#ifndef DECODE_H
#define DECODE_H

#include "types.h" 
typedef struct _DecodeInfo
{
	/* Stego Image Info */
	char *stego_image_fname;
	FILE *fptr_stego_image;

	/* Decoded file  Info */
	char *output_fname;
	int secret_file_extn_size;
	char *secret_file_extention;
	int secret_file_size;
	FILE *fptr_output_fname;
} DecodeInfo;


/* Read and validate Encode args from argv */
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

/* Perform the decoding */
Status do_decoding(DecodeInfo *decInfo);

/* Get File pointers for i/p and o/p files */
Status open_files_decode(DecodeInfo *decInfo);

/* Store Magic String */
Status decode_magic_string(int size, DecodeInfo *decInfo);

/* Decode secret file extenstion */
Status decode_secret_file_extn(DecodeInfo *decInfo);

/* Decode secret file data*/
Status decode_secret_file_data(DecodeInfo *decInfo);

/* Decode function*/
Status decode_data_from_image(int size, FILE *fptr_stego_image, FILE *fptr_decoded);

/* Decode a byte from LSB of image data array */ 
Status decode_byte_from_lsb(int size, char *image_buffer);

/* Decode extension size and file size */
Status decode_size(FILE *fptr_stego_image);

/* Closing the files*/
Status dec_close_files(DecodeInfo *decInfo);
#endif

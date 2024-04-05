#ifndef ENCODE_H
#define ENCODE_H

#include "types.h" // Contains user defined types

/* 
 * Structure to store information required for
 * encoding secret file to source Image
 * Info about output and intermediate data is
 * also stored
 */

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _EncodeInfo
{  
    //beautiful.bmp file
    /* Source Image info */
    char *src_image_fname; //file name
    FILE *fptr_src_image; // Source file .bmp
    uint image_capacity; //.bmp size
    uint bits_per_pixel;
    char image_data[MAX_IMAGE_BUF_SIZE];
    
    //Secret.txt file
    /* Secret File Info */
    char *secret_fname; //file name
    FILE *fptr_secret; //To access data in secret.txt
    char extn_secret_file[MAX_FILE_SUFFIX]; //To store the extention .txt or .c or .sh
    char secret_data[MAX_SECRET_BUF_SIZE]; // To store Password(Name)
    long size_secret_file; //Store size of secret file
    
    //Output.bmp file
    /* Stego Image Info */
    char *stego_image_fname; //To store output filename
    FILE *fptr_stego_image; // To access output file

} EncodeInfo; //Datatype for structure


/* Encoding function prototype */

/* Check operation type */
OperationType check_operation_type(char *argv[]); //Check encoding or decoding

/* Read and validate Encode args from argv */
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo); 

/* Perform the encoding */
Status do_encoding(EncodeInfo *encInfo); 

/* Get File pointers for i/p and o/p files */
Status open_files(EncodeInfo *encInfo); //check beautiful.bmp is empty or not

/* check capacity */
Status check_capacity(EncodeInfo *encInfo); // Check File size grater than all other data

/* Get image size */
uint get_image_size_for_bmp(FILE *fptr_image); //beautiful.bmp file size

/* Get file size */
uint get_file_size(FILE *fptr); //Secret.txt size

/* Copy bmp image header */
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image); //Copy the header file(First 54 bytes)

/* Store Magic String */
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo); //Password

/* Encode secret file extenstion */
Status encode_secret_file_extn(char *file_extn, EncodeInfo *encInfo); //Encode extention of secret file

/* Encode secret file size */
Status encode_secret_file_size(int file_size, EncodeInfo *encInfo); // Ecncode size of secret file

/* Encode secret file data*/
Status encode_secret_file_data(EncodeInfo *encInfo); 

/* Encode function, which does the real encoding */
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image,EncodeInfo *encInfo);

/* Encode a byte into LSB of image data array */
Status encode_byte_to_lsb(char data, char *image_buffer); 

//Encode file extention Size
Status encode_size(int size , FILE *fptr_src_image, FILE *fptr_stego_image); 

Status encode_size_to_lsb(char *buffer, int size);
/* Copy remaining image bytes from src to stego image after encoding */
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest);//To copy remaining data form source to output file

#endif

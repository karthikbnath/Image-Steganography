#include <stdio.h>
#include <string.h>
#include<stdlib.h>
#include "types.h"
#include "decode.h"
#include "common.h"

int num = 0, temp = 0, l = 0;
//Function to do the decoding process
Status do_decoding(DecodeInfo *decInfo)  
{
	printf("Files opened\n");
	if(open_files_decode(decInfo) == e_success) 
	{
		rewind(decInfo -> fptr_stego_image);
		fseek(decInfo -> fptr_stego_image, 54, SEEK_SET);

		//printf("Decoding the magic string\n");
		if(decode_magic_string(strlen(MAGIC_STRING), decInfo) == e_success)
		{
			printf("\nDecoding the file extension size\n");

			if(decode_size(decInfo->fptr_stego_image) == e_success)
			{
				decInfo->secret_file_extn_size = num;    
			//	printf("Size of extension is: %d bytes\n",num);
				printf("Decoding the file extension\n");
				if(decode_secret_file_extn(decInfo) == e_success)
				{
					if ( decInfo->output_fname != NULL )  
						decInfo->fptr_output_fname = fopen(decInfo->output_fname, "w");  
					else
					{
						decInfo->output_fname = "output.txt"; 

						printf("Default file created : %s\n", decInfo->output_fname);
						decInfo->fptr_output_fname = fopen(decInfo->output_fname, "w");   
					}
					printf("Decoding the secret file size\n");
					if(decode_size(decInfo -> fptr_stego_image) == e_success)
					{
						decInfo->secret_file_size = num;    
						//printf("Size of secret file is: %d bytes\n",num);
						printf("Decoding the file\n");
						if(decode_secret_file_data(decInfo) == e_success)
						{
							printf("Decoded the file successfully\n");
						}
						else

						{
							printf("Failed to decode the secret file data\n");
							return e_failure;    
						}
					}
					else
					{
						printf("Failed to decode the secret file size\n");
						return e_failure;    
					}
				}
				else
				{
					printf("Failed to decode the secret file extension\n");
					return e_failure;    
				}
			}
			else
			{
				printf("Failed to decode size of extension\n");
				return e_failure;    
			}
		}
		else
		{
			printf("Failed to decode magic string\n");
			return e_failure; 
		}
	}
	else
	{
		printf("Failed to open the file\n");
		return e_failure;     
	}
	return e_success;     
}

char ch, arr[sizeof(MAGIC_STRING) + 4];    

//Function to open stego image file
Status open_files_decode(DecodeInfo *decInfo)   
{
	decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "r");//Open in read mode

	if (decInfo->fptr_stego_image == NULL)   
	{
		perror("fopen");  
		fprintf(stderr, "ERROR: Unable to open file %s\n", decInfo->stego_image_fname);

		return e_failure;
	}

	return e_success;    
}

//Function to read and validate CLA
Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo) 
{
    // Check if stego image file is provided and has .bmp extension
	if (argv[2] != NULL && strstr(argv[2], ".bmp") != NULL ) 
		decInfo->stego_image_fname = argv[2];    
	else
	{
		printf("Please enter the .bmp file\n");
		return e_failure;
	}

	// Check if output file is provided and has .txt or .c extension
	if(argv[3] != NULL)
	{
		if(strstr(argv[3],".txt") != NULL || strstr(argv[3],".c") != NULL)  
		{
			decInfo->output_fname = argv[3];    
		}
		else
		{   
			printf("Output file must be of type '.txt' or '.c'\n");
			return e_failure;
		}   
	}
	else
		decInfo->output_fname = NULL;
	return e_success;    
}

// Function to decode magic string
Status decode_magic_string(int size, DecodeInfo *decInfo) 
{
	printf("The magic string: ");
	if(decode_data_from_image(strlen(MAGIC_STRING), decInfo -> fptr_stego_image, decInfo -> fptr_output_fname) == e_success)
		arr[l] = '\0';
	l = 0;
	if(strcmp(arr, MAGIC_STRING) == 0 )   // Check if decoded magic string matches   
		return e_success;                      
	else
		return e_failure;                 
}

//Function to decode data from image
Status decode_data_from_image(int size, FILE *fptr_stego_image, FILE *fptr_output_fname) 
{
	char image_buffer[8];
	for(int j = 0 ; j < size ; j++)      
	{
		fread(image_buffer, 1, 8, fptr_stego_image);  
		decode_byte_from_lsb(size, image_buffer);     
		if(temp == 1)                                
			fputc(ch, fptr_output_fname);//Write decoded character to output file         
	}

	return e_success;         
}

//Function to decode byte from lsb
Status decode_byte_from_lsb(int size, char *image_buffer) 
{
	int i;
	ch = 0 ;
	ch = ch | (image_buffer[0] & 0x01);// Extract LSB of first byte
	for( i = 1 ; i < 8 ; i++ )
	{
		ch = ch << 1;                       
		ch |= (image_buffer[i] & 0x01);// Extract LSB of the rest
	}

	if(temp == 0)                  
	{
		printf("%c", ch);
		arr[l] = ch;       
		l++;                    
	}
}

//Functiom to decode the size of file
Status decode_size(FILE *fptr_stego_image) 
{
	char image_buffer[32];
	fread(image_buffer, 1, 32, fptr_stego_image);    
	num |= (image_buffer[0] & 0x01);// Extract LSB of first byte
	for(int i = 0 ; i < 32 ; i++)
	{
		num = num << 1 ;                   
		num |= (image_buffer[i] & 0x01);   
	}
	return e_success; 	           
}

//Function to decode the size of file extension
Status decode_secret_file_extn(DecodeInfo *decInfo) 
{
	//printf("The decoded file extention is: ");
	if(decode_data_from_image(num, decInfo -> fptr_stego_image, decInfo -> fptr_output_fname) == e_success)
		return e_success;    
}

//Function to decode the secret file data
Status decode_secret_file_data(DecodeInfo *decInfo) 
{
	temp++;
	rewind(decInfo->fptr_output_fname); // Rewind output file pointer
	if(decode_data_from_image(num, decInfo -> fptr_stego_image, decInfo -> fptr_output_fname) == e_success)
		return e_success;   
}


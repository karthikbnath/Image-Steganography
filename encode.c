#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "common.h"

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
	uint width, height;
	// Seek to 18th byte
	fseek(fptr_image, 18, SEEK_SET);

	// Read the width (an int)
	fread(&width, sizeof(int), 1, fptr_image);

	// Read the height (an int)
	fread(&height, sizeof(int), 1, fptr_image);

	// Return image capacity
	return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

// Function to open necessary files for encoding
Status open_files(EncodeInfo *encInfo)
{
	// Open source image file for reading
	encInfo->fptr_src_image = fopen(encInfo->src_image_fname, "r");
	if (encInfo->fptr_src_image == NULL)
	{
		perror("fopen"); 
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->src_image_fname); 

		return e_failure; 
	}

	// Open secret file for reading
	encInfo->fptr_secret = fopen(encInfo->secret_fname, "r");
	if (encInfo->fptr_secret == NULL)
	{
		perror("fopen"); 
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->secret_fname); 

		return e_failure; 
	}

	// Open stego image file 
	encInfo->fptr_stego_image = fopen(encInfo->stego_image_fname, "w");
	if (encInfo->fptr_stego_image == NULL)
	{
		perror("fopen"); 
		fprintf(stderr, "ERROR: Unable to open file %s\n", encInfo->stego_image_fname); 

		return e_failure; 
	}
	return e_success;
}

// Function to read and validate command line arguments for encoding
Status read_and_validate_encode_args(char *argv[], EncodeInfo *encInfo)
{
	// Check if the source file is a .bmp file
	if (strcmp((strstr(argv[2], ".")), ".bmp") == 0)
	{
		encInfo->src_image_fname = argv[2]; 
	}
	else
	{
		return e_failure; 
	}

	// Check if the secret file has an extension
	if (strchr(argv[3], '.') != NULL)
	{
		encInfo->secret_fname = argv[3]; 
		strcpy(encInfo->extn_secret_file, strchr(argv[3], '.')); 
	}
	else
	{
		printf("Error : The secret file doesn't have an extension\n");
		return e_failure; 
	}

	// Check if the stego image file is provided
	if (argv[4] != NULL)
	{
		// Check if it is a .bmp file
		if (strstr(argv[4], ".bmp") != NULL)
		{
			encInfo->stego_image_fname = argv[4]; 
		}
	}
	else
	{
		encInfo->stego_image_fname = "stego.bmp"; // Store the default stego image file name
	}

	return e_success; 
}

//Function to start the encoding process
Status do_encoding(EncodeInfo *encInfo)
{
	printf("Starting encoding\n");

	if(open_files(encInfo) == e_success)   
	{
		printf("File opened\n");

		if(check_capacity(encInfo) == e_success)
		{
			printf("Checked capacity\n");

			if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
			{
				printf("Copied bmp header file\n");

				if(encode_magic_string(MAGIC_STRING,encInfo) ==e_success )
				{
					printf("Encoded magic string\n");

					if(encode_size(strlen(strstr(encInfo->secret_fname , ".")) ,encInfo->fptr_src_image,encInfo-> fptr_stego_image )== e_success)			      
					{
						printf("Encoded secret file extension size\n");
						if(encode_secret_file_extn(strstr(encInfo->secret_fname,"."),encInfo)==e_success)
						{
							printf("Encoded secret file extension\n");
							if(encode_secret_file_size(encInfo->size_secret_file,encInfo)== e_success)
							{
								printf("Encoded secret file size successfully\n");
								if(encode_secret_file_data(encInfo) == e_success)
								{
									printf("Encoded secret file data\n");
									if(copy_remaining_img_data(encInfo->fptr_src_image,encInfo->fptr_stego_image) == e_success)
									{
										printf("Copied the remaining bytes\n");
									}
									else
									{
										printf("Copying the remaining bytes failed\n");
										return e_failure;
									}
								}
								else
								{  
									printf("Secret file data encoding failed\n");
									return e_failure;
								}
							}
							else
							{
								printf("Secret file size encoding failed\n");
								return e_failure;
							}
						}
						else
						{
							printf("Secret file extension encoding failed\n");
							return e_failure;
						}
					}
					else
					{
						printf("Secret file extension size failed\n");
						return e_failure;
					}
				}
				else
				{
					printf("Encoding magic string failed\n");
					return e_failure;
				}
			}
			else
			{
				printf("Failed to copy bmp header file\n");
				return e_failure;
			}
		}
		else
		{
			printf("Failed to check capacity\n");
			return e_failure;
		}
	}
	else
	{
		printf("File could not be opened\n");
		return e_failure;
	}
	return e_success;
}

//Function to check the capacity
Status check_capacity(EncodeInfo *encInfo)
{ 
	encInfo->image_capacity = get_image_size_for_bmp(encInfo->fptr_src_image); //get the size of stego image
	encInfo->size_secret_file  = get_file_size(encInfo->fptr_secret);// size of secret file
	if((encInfo->image_capacity -54) > ((strlen(MAGIC_STRING ) + (strlen(encInfo->extn_secret_file)) + 4 + 4 + encInfo->size_secret_file) * 8 )) //calculating and checking total size required to hide
	{
		return e_success;
	}
	else
	{
		return e_failure;
	}
}

//Function to get the file size
uint get_file_size(FILE *fptr) 
{
	fseek(fptr , 0 , SEEK_END);//move pointer to end of file
	return ftell(fptr);//get current position of pointer
}

//Funciton to copy bmp header 
Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image)
{
	char str[54];
	fseek(fptr_src_image , 0 , SEEK_SET); //moving ptr to beginning
	fread(str, 54 , 1 , fptr_src_image);//read the first 54 bytes
	fwrite(str, 54 , 1 , fptr_dest_image);
	return e_success; 
}

//Function to encode magic string
Status encode_magic_string(char *magic_string, EncodeInfo *encInfo)
{
	encode_data_to_image(magic_string, strlen(magic_string), encInfo->fptr_src_image,encInfo ->fptr_stego_image,encInfo);
	return e_success;
}

//Function to encode data to the image
Status encode_data_to_image(char *data, int size, FILE *fptr_src_image, FILE *fptr_stego_image,EncodeInfo *encInfo)
{
	int i;
	for(i = 0; i < size; i++)
	{
		fread(encInfo->image_data , 8 , 1 ,fptr_src_image); // Read 8 bytes of image data
		encode_byte_to_lsb(data[i] ,encInfo->image_data);
		fwrite(encInfo->image_data, 8 ,1 ,fptr_stego_image);
	}
	return e_success;
}

//Function to encode byte to the lsb
Status encode_byte_to_lsb(char data, char *image_buffer)
{
	unsigned int mask = 1 << 7;
	for(int i = 0; i < 8; i++)
	{
		image_buffer[i] = ((image_buffer[i] & 0xfe) | (data & mask) >> (7-i));// Encode the LSB of the data into the image buffer
		mask = mask >> 1;
	}
}


//Function to encode size of the file
Status encode_size(int size,FILE *fptr_src_image,FILE *fptr_stego_image)
{
	char str[32];
	fread(str , 32 ,1 ,fptr_src_image);
	encode_size_to_lsb(str,size);
	fwrite(str ,32 , 1 ,fptr_stego_image);
	return e_success;
}

//Function to encode size to the lsb
Status encode_size_to_lsb(char *image_buffer ,int size)
{
	unsigned int mask;
	mask = 1 << 31;
	for(int i = 0; i < 32; i++)
	{
		image_buffer[i] = ((image_buffer[i] & 0xfe) | (size & mask) >> (31-i));// Encode the bits of the file size into the image buffer
		mask = mask >> 1;
	}

}

//Function to encode secret file extension
Status encode_secret_file_extn(char *file_extn,EncodeInfo *encInfo)
{
	encode_data_to_image(file_extn,strlen(file_extn),encInfo->fptr_src_image,encInfo->fptr_stego_image,encInfo);
	return e_success;
}

//Function to encode secret file size 
Status encode_secret_file_size(int file_size,EncodeInfo *encInfo)
{

	char str[32];
	fread( str, 32 ,1 ,encInfo->fptr_src_image );
	encode_size_to_lsb(str,file_size);
	fwrite(str ,32 , 1 ,encInfo->fptr_stego_image);
	return e_success;
}

//Function to encode secret file data
Status encode_secret_file_data(EncodeInfo *encInfo)
{
	char str[encInfo->size_secret_file];
	fseek(encInfo->fptr_secret ,0 ,SEEK_SET); // Move to the beginning
	fread(str ,encInfo->size_secret_file,1,encInfo->fptr_secret);
	encode_data_to_image(str,encInfo->size_secret_file,encInfo->fptr_src_image,encInfo->fptr_stego_image,encInfo);
	return e_success;
}

//Function to copy any remaining image data
Status copy_remaining_img_data(FILE *fptr_src, FILE *fptr_dest)
{
	char ch;
	// Read and write image data until the end of file
	while(fread(&ch,1,1,fptr_src) > 0)
	{
		fwrite(&ch,1,1,fptr_dest);
	}
	return e_success;
}


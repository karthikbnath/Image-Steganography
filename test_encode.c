/*
Name : Karthik B 
Date : 24-03-24
Description : Project
 */
#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "types.h"
#include "decode.h" 
#include "common.h"
int main(int argc , char *argv[])
{
    if ( argc <=2 )
    {
	printf("ERROR:Insufficient command line arguments\n");
    }
    else{
	if( check_operation_type(argv) == e_encode )
	{
		printf("---Selected encoding---\n");
		EncodeInfo encInfo;
		if(read_and_validate_encode_args(argv , &encInfo) == e_success)
		{
			printf("Read and validated sucessfully\n");
			if(do_encoding(&encInfo) == e_success)
			{
				printf("---Encoded successfully completed---\n");
			}
			else
			{
				printf("Encoding failed\n");
			}
		}
		else
		{
			printf("Read and validate failed\n");
		}
	}
	else if( check_operation_type(argv) == e_decode )
	{
		printf("---Selected decoding---\n"); 
		DecodeInfo decInfo;
		if(read_and_validate_decode_args(argv, &decInfo) == e_success)
		{
			printf("Read and validated successfully\n");
			if(do_decoding(&decInfo) == e_success)
			{
				printf("---Decoded successfully completed---\n");
			}
			else
			{
				printf("Decoding failed\n");
			}
		} 
		else
		{
			printf("Read and validate failed\n");
			return 1;
		}
	}
    }
	/*else
	{
		printf("Invalid input\nFor encoding :\n ./a.out -e secret.txt beautiful.bmp\nFor decoding :\n ./a.out -d stego.bmp\n");
	}

	return 0;*/
} 
OperationType check_operation_type(char *argv[])
{
	if(strcmp(argv[1] , "-e") == 0)
	{
		return e_encode;
	}
	else if(strcmp(argv[1] , "-d") == 0)
	{
		return e_decode;
	}
	else
	{
		return e_unsupported;
	}
}

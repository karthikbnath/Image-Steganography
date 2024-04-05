The concealment of information within harmless messages using the LSB method to enhance data security. 
This project was implemented using C programming.

Compiler used : GCC (Compile as : gcc *.c )

Usage (To be given via command line)
To encode : ./a.out -e bmpfile.bmp secret_file.extension output_file.bmp*
To decode : ./a.out -d encoded_image.bmp output_file_name**

NOTE: * The output file name is optional. In case of no input, default filename will be created by the program.
** The output file name only need to be passed if needed. The extension will be automatically added by the
program based on data decoded from the image.

test_encode.c : Contains the main functions and calls respective functions to carryout process selected by user.
types.h : Contains user-defined data-types for function return type and process selection.
encode.h : Contains function and structre declerations required for encoding process.
encode.c : Contains functions definitions for carrying out encoding process.
decode.h : Contains function and structre declerations required for decoding process.
decode.c : Contains functions definitions for carrying out decoding process.
common.h : Contains the magic string as macro to check if the file is encoded or not.
secret.txt : Additional sample text file containing the secret message to be encoded.
beautiful.bmp : Sample bmp file.

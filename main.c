/* main.c -- argument parsing and main logic of the compiler
 *
 * Copyright (C) 2017 Alex White <internetunexplorer@gmail.com>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */ 

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#include "error.c"
#include "argparse.c"

int main(int argc, char **argv) {
	
	char **input_file_paths; //Paths of input files
	char  *output_file_path; //Path of output file
	int    input_file_count; //Number of input files
	
	int do_preprocess = 1;   //Preprocess files?
	int do_compile    = 1;   //Compile files to asm?
	int do_assemble   = 1;   //Assemble files?
	int do_link       = 1;   //Link files?

	char *msg = "Usage: pcc [options] <files>";	
	const argparse_opt arg_opts[] = {
		{"output", "output-file", 'o', 1, "Put the garbage somewhere specific"},
		{"verbose", "verbose", 'v', 0, "Be loud and obnoxious"},
		{"unsafe-math", "unsafe-math", '\0', 0, "Go fast (and probably segfault)"},
		{"quiet", NULL, 'q', 0, "Silence is golden"},
		{"pedantic", "", 'p', 0, "Be annoying about stuff"}

	};
	int arg_optc = sizeof(arg_opts) / sizeof(arg_opts[0]);
	argparse_printhelp(msg, arg_optc, (argparse_opt*) arg_opts); 
}

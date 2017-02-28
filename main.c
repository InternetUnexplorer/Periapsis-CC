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

int main(int argc, char **argv) {
	
	char **input_file_paths; //Paths of input files
	char  *output_file_path; //Path of output file
	int    input_file_count; //Number of input files
	
	int do_preprocess = 1;   //Preprocess files?
	int do_compile    = 1;   //Compile files to asm?
	int do_assemble   = 1;   //Assemble files?
	int do_link       = 1;   //Link files?
	
	int accept_opts   = 1;   //Accepting options (or just files)?
	int continue_err  = 1;   //Continue after argument parsing?
	int i;

	input_file_paths = (char**) malloc((argc - 1) * sizeof(char*));
	output_file_path = "a.out";

	for(i = 1; i < argc; i++) {
		char *arg = argv[i];
		if(accept_opts && !strncmp("--", arg, 2)) {
			if(!strcmp("--", arg)) {
				accept_opts = 0;
			} else { 
				continue_err = errmsg(0, "Unknown argument ‘%s’", arg);
			}
			continue;
		}
		if(!strncmp("-o", arg, 2)) {
			if(!strcmp("-o", arg)) {
				if(++i == argc) {
					continue_err = errmsg(0, "-o: Filename expected");
				} else {
					output_file_path = argv[i];
				}
			} else {
				output_file_path = arg + 2 * sizeof(char*);
			}
			continue;
		}
		char *path = realpath(arg, NULL);
		if(path == NULL) {
			errmsg(errno, arg);
			continue_err = 0;
		} else {
			input_file_paths[input_file_count++] = path;
		}	
	}
	
	for(i = 0; i < input_file_count; i++)
		free(input_file_paths[i]);
}

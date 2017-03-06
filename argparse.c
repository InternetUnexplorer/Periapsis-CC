/* argparse.c -- argument parsing
 *
 * Copyright (C) 2017 Alex White <internetunexplorer@gmail.com>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

#ifndef ARGPARSE_C_
#define ARGPARSE_C_

#include <stdio.h>

//An option that can be passed via the CLI. Used for parsing and printing help.
typedef struct {
	char *name;  //"output" (used to identify option later)
	char *opt_l; //"output-file" (NULL if none)
	char opt_s;  //'o' ('\0' or ' ' if none)
	int  arg;    //Does this option take an argument?
	char *desc;  //"Specifies an output file" (used when printing help)
} argparse_opt;

//One for each argparse_opt. Returned by the parser in one argparse_result.
typedef struct {
	char *name;  //Name defined in argparse_opt
	int  found; //Was this option found?
	char *arg;  //Value of the argument, if any
} argparse_opt_result;

//Result of the argument parsing. Returned by the parser.
typedef struct {
	int  error;      //Was an error encountered when parsing?
	char *error_msg; //Error message, if any
	argparse_opt_result *opt_results; //Option results (see argparse_opt_result)
	int opt_result_count; //Number of option results
	char **argv;     //List of arguments not related to options (e.g. filenames) (must be freed)
	int  argc;       //Number of arguments not related to options
} argparse_result;

argparse_result* argparse_parse(int argc, char **args, int optc, argparse_opt *opts) {
	return;
}

//Pretty-prints help for an argparse_opt array with optc elements.
void argparse_printhelp(char *msg, int optc, argparse_opt *opts) {
	int opts_w = 0, optl_w = 0; //Max width of opt_s and opt_l columns
	int i;
	if(msg)
		printf("%s\n", msg);
	for(i = 0; i < optc; i++) {
		if(opts[i].opt_s && !opts_w)
			opts_w = 1;
		if(opts[i].opt_l && strlen(opts[i].opt_l) > optl_w)
			optl_w = strlen(opts[i].opt_l);
	}
	for(i = 0; i < optc; i++) {
		if(opts[i].opt_s)
			printf("-%c, ", opts[i].opt_s);
		else if(opts_w)
			printf("    ");
		if(opts[i].opt_l && opts[i].opt_l[0])
			printf("--%-*s ", optl_w, opts[i].opt_l);
		else if(optl_w)
			printf("  %*s ", optl_w, "");
		printf("%s\n", opts[i].desc ? opts[i].desc : "");
	}
}

#endif

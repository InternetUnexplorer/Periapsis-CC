/* argp.h -- command line argument parsing
 *
 * Copyright (C) 2017 Alex White <internetunexplorer@gmail.com>
 *
 * This software may be modified and distributed under the terms
 * of the MIT license.  See the LICENSE file for details.
 */

/* Represents an option that can be passed on the command line.
 *   lopt - Long option
 *   sopt - Short option
 *   arg  - Option takes an argument?
 *   desc - Option description
 * Either lopt or sopt can be omitted. Omitting both signals the
 * end of the option list.
 */
struct argp_opt {
	char *lopt;
	char  sopt;
	int   arg;
	char *desc;
}

/* Represents an option that was found during argp_parse.
 *   lopt - Long option (same value as argp_opt)
 *   sopt - Short option (same value as argp_opt)
 *   arg  - Argument for this option
 * lopt and sopt are the same as those defined in argp_opt.
 */
struct arp_res_opt {
	char *lopt;
	char  sopt;
	char *arg;
}

/* pam_econf.c -- routines to parse configuration files with libeconf */

#if defined (USE_ECONF)
#include <stdio.h>
#include <security/_pam_macros.h>
#include "pam_econf.h"

econf_err pam_econf_readconfig(econf_file **key_file,
			       const char *usr_conf_dir,
			       const char *etc_conf_dir,
			       const char *config_name,
			       const char *config_suffix,
			       const char *delim,
			       const char *comment,
			       bool (*callback)(const char *filename, const void *data),
			       const void *callback_data)
{
    econf_err ret = ECONF_SUCCESS;
#ifdef HAVE_ECONF_READCONFIG
    /* Entries in director /run will be also parsed like it is described in           */
    /* https://uapi-group.org/specifications/specs/configuration_files_specification/ */
    D(("Read configuration from directory %s /run and %s", etc_conf_dir, usr_conf_dir));
    char *parsing_dirs = NULL;
    if (asprintf(&parsing_dirs, "PARSING_DIRS=%s:/run:%s", usr_conf_dir, etc_conf_dir) < 0) {
        ret = ECONF_NOMEM;
        parsing_dirs = NULL;
    }
    if (ret == ECONF_SUCCESS)
        ret = econf_newKeyFile_with_options(key_file, parsing_dirs);
    if (ret == ECONF_SUCCESS)
        ret = econf_readConfigWithCallback(key_file,
					   NULL,
					   usr_conf_dir,
					   config_name,
					   config_suffix,
					   delim,
					   comment,
					   callback, callback_data);
    free(parsing_dirs);
#else
    D(("Read configuration from directory %s and %s", etc_conf_dir, usr_conf_dir));
    ret = econf_readDirsWithCallback(key_file,
				     usr_conf_dir,
				     etc_conf_dir,
				     config_name,
				     config_suffix,
				     delim,
				     comment,
				     callback, callback_data);
#endif
    return ret;
}

#endif /* USE_ECONF */

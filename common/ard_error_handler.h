/*****************************************************************************
FILE: ard_error_handler.h
  
PURPOSE: Contains ARD error handling related defines and structures

PROJECT:  Land Satellites Data System Science Research and Development (LSRD)
at the USGS EROS

LICENSE TYPE:  NASA Open Source Agreement Version 1.3

NOTES:
*****************************************************************************/

#ifndef ARD_ERROR_HANDLER_H_
#define ARD_ERROR_HANDLER_H_

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "ard_common.h"

/* Prototypes */
void ard_error_handler
(
    bool error_flag,  /* I: true for errors, false for warnings */
    char *module,     /* I: calling module name */
    char *errmsg      /* I: error/warning message to be printed, without
                            ending EOL */
);

#endif

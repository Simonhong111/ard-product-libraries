/*****************************************************************************
FILE: ard_error_handler.c
  
PURPOSE: Contains functions for ARD error handling.

PROJECT:  Land Satellites Data System Science Research and Development (LSRD)
at the USGS EROS

LICENSE TYPE:  NASA Open Source Agreement Version 1.3

NOTES:
*****************************************************************************/

#include "ard_error_handler.h"

/******************************************************************************
MODULE:  ard_error_handler

PURPOSE:  Prints the error/warning message.

RETURN VALUE:
Type = None

NOTES:
******************************************************************************/
void ard_error_handler
(
    bool error_flag,  /* I: true for errors, false for warnings */
    char *module,     /* I: calling module name */
    char *errmsg      /* I: error/warning message to be printed, without
                            ending EOL */
)
{
    if (error_flag)
        printf ("Error: %s : %s\n\n", module, errmsg);
    else
        printf ("Warning: %s : %s\n", module, errmsg);
}

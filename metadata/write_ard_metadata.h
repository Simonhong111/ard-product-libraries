/*****************************************************************************
FILE: write_ard_metadata.h
  
PURPOSE: Contains ARD metadata write related prototypes and defines

PROJECT:  Land Satellites Data System Science Research and Development (LSRD)
at the USGS EROS

LICENSE TYPE:  NASA Open Source Agreement Version 1.3

NOTES:
*****************************************************************************/

#ifndef WRITE_ARD_METADATA_H
#define WRITE_ARD_METADATA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ard_error_handler.h"
#include "ard_metadata.h"

/* Defines */
/* maximum number of characters per line in the XML file */
#define MAX_LINE_SIZE 1024

/* Prototypes */
void write_ard_proj_metadata
(
    Ard_proj_meta_t *proj_info, /* I: pointer to the projection information */
    FILE *fptr               /* I: file pointer to the open XML metadata file */
);

void write_ard_band_metadata
(
    int nbands,              /* I: number of bands to be written */
    Ard_band_meta_t *bmeta,  /* I: pointer to the array of either tile or
                                   scene band metadata */
    FILE *fptr,              /* I: file pointer to the open XML metadata file */
    bool skip_bands_cntnr    /* I: skip writing the opening and closing bands
                                   container information <bands> and </bands>,
                                   in the event the bands are going to be
                                   appended to */
);

int write_ard_metadata
(
    Ard_meta_t *ard_meta,      /* I: input ARD metadata structure to be written
                                     to XML */
    char *xml_file             /* I: name of the XML metadata file to be
                                     written to or overwritten */
);

#endif

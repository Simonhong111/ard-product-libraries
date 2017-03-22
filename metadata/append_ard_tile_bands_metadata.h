/*****************************************************************************
FILE: append_ard_tile_bands_metadata.h
  
PURPOSE: Contains ARD metadata bands append related prototypes and defines

PROJECT:  Land Satellites Data System Science Research and Development (LSRD)
at the USGS EROS

LICENSE TYPE:  NASA Open Source Agreement Version 1.3

NOTES:
*****************************************************************************/

#ifndef APPEND_ARD_TILE_BANDS_METADATA_H
#define APPEND_ARD_TILE_BANDS_METADATA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "ard_error_handler.h"
#include "ard_metadata.h"


int append_tile_bands_ard_metadata
(
    Ard_meta_t *ard_meta,      /* I: original ARD metadata structure to be
                                     appended to */
    int nbands_append,         /* I: number of bands to be appended */
    Ard_band_meta_t *bmeta,    /* I: pointer to the array of tile bands
                                     to be appended to the XML metadata */
    char *xml_file             /* I: name of the XML metadata file to be
                                     written to or overwritten */
);

#endif

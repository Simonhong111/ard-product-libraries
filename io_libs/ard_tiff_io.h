/*****************************************************************************
FILE: tiff_io.h
  
PURPOSE: Contains Tiff input/output related defines and structures

PROJECT:  Land Satellites Data System Science Research and Development (LSRD)
at the USGS EROS

LICENSE TYPE:  NASA Open Source Agreement Version 1.3

NOTES:
*****************************************************************************/

#ifndef TIFF_IO_H
#define TIFF_IO_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "tiffio.h"
#include "xtiffio.h"
#include "geotiffio.h"
#include "ard_metadata.h"
#include "parse_ard_metadata.h"
#include "ard_error_handler.h"

/* Defines */
typedef enum {
  ARD_TIFF_READ_FORMAT,
  ARD_TIFF_WRITE_FORMAT,
  ARD_TIFF_READ_WRITE_FORMAT,
} Ard_tiff_format_t;

/* Prototypes */
int ard_set_geotiff_datum
(
    GTIF *gtif_fptr,    /* I: GeoTiff file pointer */
    int datum_type,     /* I: datum type (see ARD_* in gctp_defines.h */
    char *citation      /* I/O: string for geo citation tag (updated) */
);

int ard_set_geotiff_tags
(
    TIFF *tiff_fptr,             /* I: pointer to Tiff file */
    Ard_band_meta_t *bmeta,      /* I: band metadata */
    Ard_proj_meta_t *proj_info   /* I: global projection information */
);

void ard_set_tiff_tags
(
    TIFF *tif,        /* I: pointer to Tiff file */
    int data_type,    /* I: data type of this band (see ARD_* in
                            ard_metadata.h) */
    int nlines,       /* I: number of lines in image */
    int nsamps,       /* I: number of samples in image */
    int t_nlines,     /* I: number of lines per tile */
    int t_nsamps      /* I: number of samples per tile */
);

TIFF *ard_open_tiff
(
    char *tiff_file,     /* I: name of the input Tiff file to be opened */
    char *access_type    /* I: string for the access type for reading the
                               input file; use the ard_tiff_format array at
                               the top of this file */
);

void ard_close_tiff
(
    TIFF *tiff_fptr    /* I: pointer to Tiff file to be closed */
);

int ard_write_tiff
(
    TIFF *tif,       /* I: pointer to the Tiff file */
    int data_type,   /* I: data type of the array to be written (see
                           Ard_data_type in ard_metadata.h) */
    int nlines,      /* I: number of lines to write to the file */
    int nsamps,      /* I: number of samples to write to the file */
    void *img_buf    /* I: array of nlines * nsamps * size to be written to the
                           Tiff file */
);

int ard_read_tiff
(
    TIFF *tif_fptr,  /* I: pointer to the Tiff file */
    int data_type,   /* I: data type of the array to be read (see
                           Ard_data_type in ard_metadata.h) */
    int nlines,      /* I: number of lines to read from the file */
    int nsamps,      /* I: number of samples to read from the file */
    void *img_buf    /* O: array of nlines * nsamps * size to be read from the
                           Tiff file (sufficient space should already have
                           been allocated) */
);

#endif

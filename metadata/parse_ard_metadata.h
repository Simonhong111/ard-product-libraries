/*****************************************************************************
FILE: parse_ard_metadata.h
  
PURPOSE: Contains prototypes for parsing the ESPA internal metadata

PROJECT:  Land Satellites Data System Science Research and Development (LSRD)
at the USGS EROS

LICENSE TYPE:  NASA Open Source Agreement Version 1.3

NOTES:
*****************************************************************************/

#ifndef PARSE_ARD_METADATA_H
#define PARSE_ARD_METADATA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlschemastypes.h>
#include "ard_error_handler.h"
#include "ard_metadata.h"
#include "meta_stack.h"

int add_global_metadata_proj_info_albers
(
    xmlNode *a_node,            /* I: pointer to the element node to process */
    Ard_proj_meta_t *proj_info  /* I: projection info structure */
);

int add_global_metadata_proj_info_ps
(
    xmlNode *a_node,            /* I: pointer to the element node to process */
    Ard_proj_meta_t *proj_info  /* I: projection info structure */
);

int add_global_metadata_proj_info_sin
(
    xmlNode *a_node,            /* I: pointer to the element node to process */
    Ard_proj_meta_t *proj_info  /* I: projection info structure */
);

int add_global_metadata_proj_info_utm
(
    xmlNode *a_node,            /* I: pointer to the element node to process */
    Ard_proj_meta_t *proj_info  /* I: projection info structure */
);

int add_global_metadata_proj_info
(
    xmlNode *a_node,            /* I: pointer to the element node to process */
    Ard_proj_meta_t *proj_info  /* I: projection info structure */
);

int add_global_metadata_bounding_coords
(
    xmlNode *a_node,            /* I: pointer to the element node to process */
    double bounding_coords[4]   /* O: bounding coordinates */
);

int add_global_scene_metadata
(
    xmlNode *a_node,            /* I: pointer to the element node to process */
    Ard_global_scene_meta_t *gmeta /* I: scene-based global metadata struct */
);

int add_global_tile_metadata
(
    xmlNode *a_node,            /* I: pointer to the element node to process */
    Ard_global_tile_meta_t *gmeta /* I: tile-based global metadata struct */
);

int add_band_metadata_bitmap_description
(
    xmlNode *a_node,            /* I/O: pointer to the element node to
                                        process */
    Ard_band_meta_t *bmeta      /* I: band metadata structure for current
                                      band in the bands structure */
);

int add_band_metadata_class_values
(
    xmlNode *a_node,            /* I/O: pointer to the element node to
                                        process */
    Ard_band_meta_t *bmeta      /* I: band metadata structure for current
                                      band in the bands structure */
);

int add_band_metadata
(
    xmlNode *a_node,            /* I: pointer to the element node to process */
    Ard_band_meta_t *bmeta      /* I: band metadata structure for current
                                      band in the bands structure */
);

int parse_xml_into_struct
(
    xmlNode *a_node,           /* I: pointer to the current node */
    Ard_meta_t *ard_meta,      /* I: ARD metadata structure to be filled */
    int *top_of_stack,         /* I: pointer to top of the stack */
    char **stack               /* I: stack to use for parsing */
);

int parse_ard_metadata
(
    char *metafile,       /* I: input metadata file or URL */
    Ard_meta_t *ard_meta  /* I: input ARD metadata structure which has been
                                initialized via init_metadata_struct */
);

#endif

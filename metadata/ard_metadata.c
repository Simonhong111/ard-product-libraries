/*****************************************************************************
FILE: ard_metadata.c
  
PURPOSE: Contains functions for reading/writing/appending the ARD metadata
files along with inializing/freeing memory in the metadata structures.

PROJECT:  Land Satellites Data System Science Research and Development (LSRD)
at the USGS EROS

LICENSE TYPE:  NASA Open Source Agreement Version 1.3

NOTES:
  1. The XML metadata format parsed or written via this library follows the
     ARD metadata format.
     The schema for the ARD metadata format is available via the ARD DFCB or
     http://espa.cr.usgs.gov/schema/ard/ard_internal_metadata_v1_0.xsd.
  2. This code relies on the libxml2 library developed for the Gnome project.
*****************************************************************************/
#include <sys/stat.h>
#include "ard_metadata.h"

/******************************************************************************
MODULE:  validate_xml_file

PURPOSE:  Validates the specified ARD XML file with the specified schema file.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           XML does not validate against the specified schema
SUCCESS         XML validates

NOTES:
******************************************************************************/
int validate_ard_xml_file
(
    char *meta_file           /* I: name of metadata file to be validated */
)
{
    char FUNC_NAME[] = "validate_ard_xml_file";   /* function name */
    char errmsg[STR_SIZE];        /* error message */
    char *schema_file = NULL;     /* name of schema file or URL to be validated
                                     against */
    int status;                   /* return status */
    xmlDocPtr doc = NULL;         /* resulting document tree */
    xmlSchemaPtr schema = NULL;   /* pointer to the schema */
    xmlSchemaParserCtxtPtr ctxt = NULL;  /* parser context for the schema */
    xmlSchemaValidCtxtPtr valid_ctxt = NULL;  /* pointer to validate from the
                                                 schema */
    struct stat statbuf;          /* buffer for the file stat function */

    /* Get the ARD schema environment variable which specifies the location
       of the XML schema to be used */
    schema_file = getenv ("ARD_SCHEMA");
    if (schema_file == NULL)
    {  /* ARD schema environment variable wasn't defined. Try the version in
          /usr/local... */
        schema_file = LOCAL_ARD_SCHEMA;
        if (stat (schema_file, &statbuf) == -1)
        {  /* /usr/local ARD schema file doesn't exist.  Try the version on
              the ARD http site... */
            schema_file = ARD_SCHEMA;
        }
    }
printf ("DEBUG: Using schema_file for validation ... %s\n", schema_file);

    /* Set up the schema parser and parse the schema file/URL */
    xmlLineNumbersDefault (1);
    ctxt = xmlSchemaNewParserCtxt (schema_file);
    xmlSchemaSetParserErrors (ctxt, (xmlSchemaValidityErrorFunc) fprintf,
        (xmlSchemaValidityWarningFunc) fprintf, stderr);
    schema = xmlSchemaParse (ctxt);

    /* Free the schema parser context */
    xmlSchemaFreeParserCtxt (ctxt);

    /* Load the XML file and parse it to the document tree */
    doc = xmlReadFile (meta_file, NULL, 0);
    if (doc == NULL)
    {
        sprintf (errmsg, "Could not parse %s", meta_file);
        ard_error_handler (true, FUNC_NAME, errmsg);
        sprintf (errmsg, "Possible schema file not found.  ARD_SCHEMA "
            "environment variable isn't defined.  The first default schema "
            "location of %s doesn't exist.  And the second default location of "
            "%s was used as the last default.", LOCAL_ARD_SCHEMA, ARD_SCHEMA);
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (ERROR);
    }

    /* Identify the schema file as the validation source */
    valid_ctxt = xmlSchemaNewValidCtxt (schema);
    xmlSchemaSetValidErrors (valid_ctxt, (xmlSchemaValidityErrorFunc) fprintf,
        (xmlSchemaValidityWarningFunc) fprintf, stderr);

    /* Validate the XML metadata against the schema */
    status = xmlSchemaValidateDoc (valid_ctxt, doc);
    if (status > 0)
    {
        sprintf (errmsg, "%s fails to validate", meta_file);
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (ERROR);
    }
    else if (status != 0)
    {
        sprintf (errmsg, "%s validation generated an internal error",
            meta_file);
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (ERROR);
    }

    /* Free the resources and clean up the memory */
    xmlSchemaFreeValidCtxt (valid_ctxt);
    xmlFreeDoc (doc);
    if (schema != NULL)
        xmlSchemaFree (schema);
    xmlSchemaCleanupTypes();
    xmlCleanupParser();   /* cleanup the XML library */
    xmlMemoryDump();      /* for debugging */

    /* Successful completion */
    return (SUCCESS);
}


/******************************************************************************
MODULE:  init_ard_metadata_struct

PURPOSE:  Initializes the ARD metadata structure, particularly the pointers
within each sub-structure.  Assigns field values to fill to make it easier to
detect if the values were parsed from the input metadata file or assigned by
the user.

RETURN VALUE:
Type = None

NOTES:
******************************************************************************/
void init_ard_metadata_struct
(
    Ard_meta_t *ard_meta    /* I: pointer to ARD metadata structure to be
                                  initialized */
)
{
    int i;                       /* looping variable */
    Ard_tile_meta_t *tile_meta = &ard_meta->tile_meta;
                                 /* pointer to the tile metadata structure */
    Ard_global_tile_meta_t *tile_gmeta = &tile_meta->tile_global;
                                 /* pointer to tile global metadata structure */
    Ard_scene_meta_t *scene_meta = NULL; /* ptr to scene metadata structure */
    Ard_global_scene_meta_t *scene_gmeta = NULL;
                                 /* ptr to scene global metadata structure */

    /* Initialze the number of bands for the tile-based metadata */
    tile_meta->nbands = 0;
    tile_meta->band = NULL;

    /* Initialize the tile-based global metadata values to fill for use by the
       write metadata routines */
    strcpy (tile_gmeta->data_provider, ARD_STRING_META_FILL);
    strcpy (tile_gmeta->acquisition_date, ARD_STRING_META_FILL);
    strcpy (tile_gmeta->product_id, ARD_STRING_META_FILL);
    strcpy (tile_gmeta->production_date, ARD_STRING_META_FILL);
    tile_gmeta->bounding_coords[0] = ARD_FLOAT_META_FILL;
    tile_gmeta->bounding_coords[1] = ARD_FLOAT_META_FILL;
    tile_gmeta->bounding_coords[2] = ARD_FLOAT_META_FILL;
    tile_gmeta->bounding_coords[3] = ARD_FLOAT_META_FILL;
    tile_gmeta->proj_info.proj_type = ARD_INT_META_FILL;
    tile_gmeta->proj_info.datum_type = ARD_NODATUM;
    tile_gmeta->orientation_angle = ARD_FLOAT_META_FILL;
    tile_gmeta->htile = ARD_INT_META_FILL;
    tile_gmeta->vtile = ARD_INT_META_FILL;
    tile_gmeta->scene_count = ARD_INT_META_FILL;
    tile_gmeta->cloud_cover = ARD_FLOAT_META_FILL;
    tile_gmeta->cloud_shadow = ARD_FLOAT_META_FILL;
    tile_gmeta->snow_ice = ARD_FLOAT_META_FILL;
    tile_gmeta->fill = ARD_FLOAT_META_FILL;

    /* Initialize the number of scenes */
    ard_meta->nscenes = 0;

    /* Loop through each scene (MAX_TOTAL_SCENES) and initialize the fields */
    for (i = 0; i < MAX_TOTAL_SCENES; i++)
    {
        /* Initialize the bands */
        scene_meta = &ard_meta->scene_meta[i];
        scene_meta->nbands = 0;
        scene_meta->band = NULL;
        scene_gmeta = &scene_meta->scene_global;

        /* Initialize the scene-based global metadata values to fill for use by
           the write metadata routines */
        strcpy (scene_gmeta->data_provider, ARD_STRING_META_FILL);
        strcpy (scene_gmeta->satellite, ARD_STRING_META_FILL);
        strcpy (scene_gmeta->instrument, ARD_STRING_META_FILL);
        strcpy (scene_gmeta->acquisition_date, ARD_STRING_META_FILL);
        strcpy (scene_gmeta->scene_center_time, ARD_STRING_META_FILL);
        strcpy (scene_gmeta->level1_production_date, ARD_STRING_META_FILL);
        scene_gmeta->wrs_system = ARD_INT_META_FILL;
        scene_gmeta->wrs_path = ARD_INT_META_FILL;
        scene_gmeta->wrs_row = ARD_INT_META_FILL;
        strcpy (scene_gmeta->request_id, ARD_STRING_META_FILL);
        strcpy (scene_gmeta->scene_id, ARD_STRING_META_FILL);
        strcpy (scene_gmeta->product_id, ARD_STRING_META_FILL);
        scene_gmeta->elevation_src = ARD_INT_META_FILL;
        scene_gmeta->sensor_mode = ARD_INT_META_FILL;
        scene_gmeta->ephemeris_type = ARD_INT_META_FILL;
        strcpy (scene_gmeta->cpf_name, ARD_STRING_META_FILL);
        strcpy (scene_gmeta->lpgs_metadata_file, ARD_STRING_META_FILL);
        scene_gmeta->geometric_rmse_model = ARD_FLOAT_META_FILL;
        scene_gmeta->geometric_rmse_model_x = ARD_FLOAT_META_FILL;
        scene_gmeta->geometric_rmse_model_y = ARD_FLOAT_META_FILL;
    }
}


/******************************************************************************
MODULE:  allocate_band_metadata

PURPOSE:  Allocates memory in the ARD scene or tile metadata structure for
nbands.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error allocating memory for the nbands
SUCCESS         Successfully allocated memory

NOTES:
  1. Initializes the bitmap_description and class_values for each band to NULL
     and sets the nbits, nclass, ncover to 0.
******************************************************************************/
int allocate_band_metadata
(
    Ard_tile_meta_t *tile_meta,   /* I: pointer to tile-specific metadata,
                                        NULL if initializing scene metadata */
    Ard_scene_meta_t *scene_meta, /* I: pointer to scene-specific metadata,
                                        NULL if initializing tile metadata */
    int nbands     /* I: number of bands to allocate for the tile-specific or
                         scene-specific band field */
)
{
    char FUNC_NAME[] = "allocate_band_metadata";   /* function name */
    char errmsg[STR_SIZE];          /* error message */
    Ard_band_meta_t *bmeta = NULL;  /* pointer to array of bands metadata in
                                       the tile-specific or scene-specific
                                       metadata structure */
    int i;                          /* looping variable */

    /* Allocate the number of bands to nbands and the associated pointers */
    if (tile_meta != NULL)
    {
        tile_meta->nbands = nbands;
        tile_meta->band = calloc (nbands, sizeof (Ard_band_meta_t));
        if (tile_meta->band == NULL)
        {
            sprintf (errmsg, "Allocating ARD tile-specific band metadata for "
                "%d bands", nbands);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
        bmeta = tile_meta->band;
    }
    else if (scene_meta != NULL)
    {
        scene_meta->nbands = nbands;
        scene_meta->band = calloc (nbands, sizeof (Ard_band_meta_t));
        if (scene_meta->band == NULL)
        {
            sprintf (errmsg, "Allocating ARD scene-specific band metadata for "
                "%d bands", nbands);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
        bmeta = scene_meta->band;
    }

    /* Set the nbits, nclass, ncover fields in the band metadata to 0 for each
       band and initialize the pointers to NULL.  Initialize the other fields to
       fill to make it easy to distinguish if they were populated by reading
       an input metadata file or assigned directly. */
    for (i = 0; i < nbands; i++)
    {
        bmeta[i].nbits = 0;
        bmeta[i].bitmap_description = NULL;
        bmeta[i].nclass = 0;
        bmeta[i].class_values = NULL;

        strcpy (bmeta[i].product, ARD_STRING_META_FILL);
        strcpy (bmeta[i].source, ARD_STRING_META_FILL);
        strcpy (bmeta[i].name, ARD_STRING_META_FILL);
        strcpy (bmeta[i].category, ARD_STRING_META_FILL);
        bmeta[i].data_type = ARD_UINT8;
        bmeta[i].nlines = ARD_INT_META_FILL;
        bmeta[i].nsamps = ARD_INT_META_FILL;
        bmeta[i].fill_value = ARD_INT_META_FILL;
        bmeta[i].saturate_value = ARD_INT_META_FILL;
        bmeta[i].scale_factor = ARD_FLOAT_META_FILL;
        bmeta[i].add_offset = ARD_FLOAT_META_FILL;
        bmeta[i].resample_method = ARD_NONE;
        strcpy (bmeta[i].short_name, ARD_STRING_META_FILL);
        strcpy (bmeta[i].long_name, ARD_STRING_META_FILL);
        strcpy (bmeta[i].file_name, ARD_STRING_META_FILL);
        bmeta[i].pixel_size[0] = bmeta[i].pixel_size[1] = ARD_FLOAT_META_FILL;
        strcpy (bmeta[i].pixel_units, ARD_STRING_META_FILL);
        strcpy (bmeta[i].data_units, ARD_STRING_META_FILL);
        bmeta[i].valid_range[0] = bmeta[i].valid_range[1] =
            ARD_FLOAT_META_FILL;
        strcpy (bmeta[i].app_version, ARD_STRING_META_FILL);
        strcpy (bmeta[i].production_date, ARD_STRING_META_FILL);
    }

    return (SUCCESS);
}


/******************************************************************************
MODULE:  allocate_class_metadata

PURPOSE:  Allocates memory in the ARD band metadata structure for nclasses.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error allocating memory for nclasses
SUCCESS         Successfully allocated memory

NOTES:
******************************************************************************/
int allocate_class_metadata
(
    Ard_band_meta_t *band_meta,   /* I: pointer to band metadata structure */
    int nclass                    /* I: number of classes to allocate for the
                                        band metadata */
)
{
    char FUNC_NAME[] = "allocate_class_metadata";   /* function name */
    char errmsg[STR_SIZE];        /* error message */

    /* Allocate the number of classes to nclass and the associated class_values
       pointer */
    band_meta->nclass = nclass;
    band_meta->class_values = calloc (nclass, sizeof (Ard_class_t));
    if (band_meta->class_values == NULL)
    {
        sprintf (errmsg, "Allocating ARD band metadata for %d nclasses",
            nclass);
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (ERROR);
    }

    return (SUCCESS);
}


/******************************************************************************
MODULE:  allocate_bitmap_metadata

PURPOSE:  Allocates memory in the ARD band metadata structure for nbits.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error allocating memory for nbits
SUCCESS         Successfully allocated memory

NOTES:
******************************************************************************/
int allocate_bitmap_metadata
(
    Ard_band_meta_t *band_meta,   /* I: pointer to band metadata structure */
    int nbits                     /* I: number of bits to allocate for the
                                        bitmap metadata */
)
{
    char FUNC_NAME[] = "allocate_bitmap_metadata";   /* function name */
    char errmsg[STR_SIZE];        /* error message */
    int i;                        /* looping variable */

    /* Allocate the number of bits to nbits and the associated bitmap pointer */
    band_meta->nbits = nbits;
    band_meta->bitmap_description = calloc (nbits, sizeof (char *));
    if (band_meta->bitmap_description == NULL)
    {
        sprintf (errmsg, "Allocating ARD bitmap description");
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (ERROR);
    }

    for (i = 0; i < nbits; i++)
    {
        band_meta->bitmap_description[i] = calloc (STR_SIZE, sizeof (char));
        if (band_meta->bitmap_description[i] == NULL)
        {
            sprintf (errmsg, "Allocating ARD band metadata for %d nbits",
                nbits);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }

    return (SUCCESS);
}


/******************************************************************************
MODULE:  free_ard_band_metadata

PURPOSE:  Frees memory in the ARD bands metadata

RETURN VALUE: N/A

NOTES:
******************************************************************************/
void free_ard_band_metadata
(
    int nbands,            /* I: number of bands in the metadata file */
    Ard_band_meta_t *band  /* I/O: array of band metadata to be freed */
)
{
    int i, b;       /* looping variables */

    /* Free the pointers in band metadata */
    for (i = 0; i < nbands; i++)
    {
        if (band[i].nbits > 0)
        {
            for (b = 0; b < band[i].nbits; b++)
                free (band[i].bitmap_description[b]);
            free (band[i].bitmap_description);
        }

        free (band[i].class_values);
    }
    free (band);
}


/******************************************************************************
MODULE:  free_ard_metadata

PURPOSE:  Frees memory in the ARD tile and scene metadata structures.

RETURN VALUE: N/A

NOTES:
******************************************************************************/
void free_ard_metadata
(
    Ard_meta_t *ard_meta   /* I: pointer to ARD metadata structure */
)
{
    Ard_tile_meta_t *tmeta = NULL;   /* pointer to the tile metadata */
    Ard_scene_meta_t *smeta = NULL;  /* pointer to the scene metadata */

    /* Free the pointers in the tile-specific band metadata */
    tmeta = &ard_meta->tile_meta;
    free_ard_band_metadata (tmeta->nbands, tmeta->band);

    /* Free the pointers in the scene-specific band metadata */
    smeta = ard_meta->scene_meta;
    free_ard_band_metadata (smeta->nbands, smeta->band);
}


/******************************************************************************
MODULE:  print_element_names

PURPOSE:  Print the information for the elements in the document tree,
starting at the node provided.

RETURN VALUE:  N/A

NOTES:
  1. Prints to stdout.
******************************************************************************/
void print_element_names
(
    xmlNode *a_node   /* I: pointer to the current node in the tree to start
                            printing */
)
{
    xmlNode *cur_node = NULL;   /* pointer to the current node */

    /* Start at the input node and traverse the tree, visiting all the children
       and siblings */
    for (cur_node = a_node; cur_node;
         cur_node = xmlNextElementSibling (cur_node))
    {
        /* Only print the ELEMENT node types */
        if (cur_node->type == XML_ELEMENT_NODE) 
        {
            /* Print out the name of the element */
            xmlAttrPtr attr;     /* pointer to the element attributes */
            printf ("node type: Element, name: %s", cur_node->name);

            /* Print out the namespace info as well */
            xmlNsPtr ns = cur_node->nsDef;
            while (ns != 0)
            {
                printf (" with namespace: %s %p\n", ns->href, ns->prefix);
                ns = ns->next;
            }
            printf("\n");

            /* Print out the attribute properties for this element */
            for (attr = cur_node->properties; attr != NULL; attr = attr->next)
            {
                xmlChar *v = xmlGetProp (cur_node, attr->name);
                if (attr->ns != NULL)
                {
                    if (attr->ns->prefix != NULL)
                    {
                        printf (" with namespace: %s %p\n", attr->ns->href,
                            attr->ns->prefix);
                    }
                    else
                    {
                        printf (" with namespace: %s\n", attr->ns->href);
                    }
                }
                printf (" @%s=%s ", attr->name, v);
                xmlFree (v);
            }
            printf ("\n");
        }
        else if (cur_node->type == XML_TEXT_NODE) 
        {
            /* Print out the text for the element */
            printf ("   node type: Text, content: %s\n", cur_node->content);
        }

        print_element_names (cur_node->children);
    }
}


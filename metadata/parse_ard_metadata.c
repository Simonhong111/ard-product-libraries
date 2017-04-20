/*****************************************************************************
FILE: parse_ard_metadata.c
  
PURPOSE: Contains functions for parsing the ARD metadata files.

PROJECT:  Land Satellites Data System Science Research and Development (LSRD)
at the USGS EROS

LICENSE TYPE:  NASA Open Source Agreement Version 1.3

NOTES:
  1. The XML metadata format parsed or written via this library follows the
     ARD metadata format found in ARD DFCB.
     The schema for the ARD  metadata format is available at
     http://espa.cr.usgs.gov/schema/ard/ard_tile_metadata_v1_0.xsd.
  2. This code relies on the libxml2 library developed for the Gnome project.
*****************************************************************************/

#include "ard_metadata.h"
#include "parse_ard_metadata.h"

/******************************************************************************
MODULE:  add_global_ard_metadata_proj_info_albers

PURPOSE: Add the AEA projection elements node to the global metadata
projection information structure.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error parsing the projection_info elements
SUCCESS         Successful parse of the projection_info values

NOTES:
******************************************************************************/
int add_global_ard_metadata_proj_info_albers
(
    xmlNode *a_node,            /* I: pointer to the element node to process */
    Ard_proj_meta_t *proj_info  /* I: projection info structure */
)
{
    char FUNC_NAME[] = "add_global_ard_metadata_proj_info_albers";
                                  /* func name */
    char errmsg[STR_SIZE];        /* error message */
    xmlNode *cur_node = NULL;     /* pointer to the current node */
    xmlNode *child_node = NULL;   /* pointer to the child node */

    /* Make sure the projection type specified matches the projection
       parameters type */
    if (proj_info->proj_type != ARD_GCTP_ALBERS_PROJ)
    {
        sprintf (errmsg, "Projection type is not AEA so the fact that "
            "albers_proj_params exists is a mismatch in the "
            "projection_information.");
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (ERROR);
    }

    /* Process the siblings in the projection information */
    for (cur_node = a_node->children; cur_node;
         cur_node = xmlNextElementSibling (cur_node))
    {
        /* Set up the child pointer */
        child_node = cur_node->children;

        if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "standard_parallel1"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Error processing global_metadata:"
                    "projection_information:albers_proj_params element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
    
            /* Copy the content of the child node into the value for this
               field */
            proj_info->standard_parallel1 =
                atof ((const char *) child_node->content);
        }
        else if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "standard_parallel2"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Error processing global_metadata:"
                    "projection_information:albers_proj_params element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
    
            /* Copy the content of the child node into the value for this
               field */
            proj_info->standard_parallel2 =
                atof ((const char *) child_node->content);
        }
        else if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "central_meridian"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Error processing global_metadata:"
                    "projection_information:albers_proj_params element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
    
            /* Copy the content of the child node into the value for this
               field */
            proj_info->central_meridian =
                atof ((const char *) child_node->content);
        }
        else if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "origin_latitude"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Error processing global_metadata:"
                    "projection_information:albers_proj_params element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
    
            /* Copy the content of the child node into the value for this
               field */
            proj_info->origin_latitude =
                atof ((const char *) child_node->content);
        }
        else if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "false_easting"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Processing global_metadata:"
                    "projection_information:albers_proj_params element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
    
            /* Copy the content of the child node into the value for this
               field */
            proj_info->false_easting =
                atof ((const char *) child_node->content);
        }
        else if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "false_northing"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Error processing global_metadata:"
                    "projection_information:albers_proj_params element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
    
            /* Copy the content of the child node into the value for this
               field */
            proj_info->false_northing =
                atof ((const char *) child_node->content);
        }
        else
        {
            sprintf (errmsg, "Unknown albers_proj_params element: %s",
                cur_node->name);
            ard_error_handler (false, FUNC_NAME, errmsg);
        }
    }

    return (SUCCESS);
}


/******************************************************************************
MODULE:  add_global_ard_metadata_proj_info_ps

PURPOSE: Add the Polar Stereographic projection elements node to the global
metadata projection information structure.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error parsing the projection_info elements
SUCCESS         Successful parse of the projection_info values

NOTES:
******************************************************************************/
int add_global_ard_metadata_proj_info_ps
(
    xmlNode *a_node,            /* I: pointer to the element node to process */
    Ard_proj_meta_t *proj_info  /* I: projection info structure */
)
{
    char FUNC_NAME[] = "add_global_ard_metadata_proj_info_ps";
                                  /* function name */
    char errmsg[STR_SIZE];        /* error message */
    xmlNode *cur_node = NULL;     /* pointer to the current node */
    xmlNode *child_node = NULL;   /* pointer to the child node */

    /* Make sure the projection type specified matches the projection
       parameters type */
    if (proj_info->proj_type != ARD_GCTP_PS_PROJ)
    {
        sprintf (errmsg, "Error: projection type is not PS so the fact that "
            "ps_proj_params exists is a mismatch in projection_information.");
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (ERROR);
    }

    /* Process the siblings in the projection information */
    for (cur_node = a_node->children; cur_node;
         cur_node = xmlNextElementSibling (cur_node))
    {
        /* Set up the child pointer */
        child_node = cur_node->children;

        if (xmlStrEqual (cur_node->name, (const xmlChar *) "longitude_pole"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Error processing global_metadata:"
                    "projection_information:ps_proj_params element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
    
            /* Copy the content of the child node into the value for this
               field */
            proj_info->longitude_pole =
                atof ((const char *) child_node->content);
        }
        else if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "latitude_true_scale"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Error processing global_metadata:"
                    "projection_information:ps_proj_params element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
    
            /* Copy the content of the child node into the value for this
               field */
            proj_info->latitude_true_scale =
                atof ((const char *) child_node->content);
        }
        else if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "false_easting"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Error processing global_metadata:"
                    "projection_information:ps_proj_params element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
    
            /* Copy the content of the child node into the value for this
               field */
            proj_info->false_easting =
                atof ((const char *) child_node->content);
        }
        else if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "false_northing"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Error processing global_metadata:"
                    "projection_information:ps_proj_params element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
    
            /* Copy the content of the child node into the value for this
               field */
            proj_info->false_northing =
                atof ((const char *) child_node->content);
        }
        else
        {
            sprintf (errmsg, "Unknown ps_proj_params element: %s",
                cur_node->name);
            ard_error_handler (false, FUNC_NAME, errmsg);
        }
    }

    return (SUCCESS);
}


/******************************************************************************
MODULE:  add_global_ard_metadata_proj_info_sin

PURPOSE: Add the SIN projection elements node to the global metadata projection
information structure.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error parsing the projection_info elements
SUCCESS         Successful parse of the projection_info values

NOTES:
******************************************************************************/
int add_global_ard_metadata_proj_info_sin
(
    xmlNode *a_node,            /* I: pointer to the element node to process */
    Ard_proj_meta_t *proj_info  /* I: projection info structure */
)
{
    char FUNC_NAME[] = "add_global_ard_metadata_proj_info_sin"; /* func name */
    char errmsg[STR_SIZE];        /* error message */
    xmlNode *cur_node = NULL;     /* pointer to the current node */
    xmlNode *child_node = NULL;   /* pointer to the child node */

    /* Make sure the projection type specified matches the projection
       parameters type */
    if (proj_info->proj_type != ARD_GCTP_SIN_PROJ)
    {
        sprintf (errmsg, "Projection type is not SIN so the fact that "
            "sin_proj_params exists is a mismatch in the "
            "projection_information.");
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (ERROR);
    }

    /* Process the siblings in the projection information */
    for (cur_node = a_node->children; cur_node;
         cur_node = xmlNextElementSibling (cur_node))
    {
        /* Set up the child pointer */
        child_node = cur_node->children;

        if (xmlStrEqual (cur_node->name, (const xmlChar *) "sphere_radius"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Error processing global_metadata:"
                    "projection_information:sin_proj_params element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
    
            /* Copy the content of the child node into the value for this
               field */
            proj_info->sphere_radius =
                atof ((const char *) child_node->content);
        }
        else if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "central_meridian"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Error processing global_metadata:"
                    "projection_information:sin_proj_params element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
    
            /* Copy the content of the child node into the value for this
               field */
            proj_info->central_meridian =
                atof ((const char *) child_node->content);
        }
        else if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "false_easting"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Processing global_metadata:"
                    "projection_information:sin_proj_params element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
    
            /* Copy the content of the child node into the value for this
               field */
            proj_info->false_easting =
                atof ((const char *) child_node->content);
        }
        else if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "false_northing"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Error processing global_metadata:"
                    "projection_information:sin_proj_params element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
    
            /* Copy the content of the child node into the value for this
               field */
            proj_info->false_northing =
                atof ((const char *) child_node->content);
        }
        else
        {
            sprintf (errmsg, "Unknown sin_proj_params element: %s",
                cur_node->name);
            ard_error_handler (false, FUNC_NAME, errmsg);
        }
    }

    return (SUCCESS);
}


/******************************************************************************
MODULE:  add_global_ard_metadata_proj_info_utm

PURPOSE: Add the UTM projection elements node to the global metadata projection
information structure.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error parsing the projection_info elements
SUCCESS         Successful parse of the projection_info values

NOTES:
******************************************************************************/
int add_global_ard_metadata_proj_info_utm
(
    xmlNode *a_node,            /* I: pointer to the element node to process */
    Ard_proj_meta_t *proj_info  /* I: projection info structure */
)
{
    char FUNC_NAME[] = "add_global_ard_metadata_proj_info_utm";
                                  /* function name */
    char errmsg[STR_SIZE];        /* error message */
    xmlNode *cur_node = NULL;     /* pointer to the current node */
    xmlNode *child_node = NULL;   /* pointer to the child node */

    /* Make sure the projection type specified matches the projection
       parameters type */
    if (proj_info->proj_type != ARD_GCTP_UTM_PROJ)
    {
        sprintf (errmsg, "Projection type is not UTM so the fact that "
            "utm_proj_params exists is a mismatch in the "
            "projection_information.");
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (ERROR);
    }

    /* Set up the current and child pointers */
    cur_node = a_node->children;
    child_node = cur_node->children;

    /* Process the zone code for UTM */
    if (xmlStrEqual (cur_node->name, (const xmlChar *) "zone_code"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Error processing global_metadata:"
                "projection_information:utm_proj_params element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this
           field */
        proj_info->utm_zone = atoi ((const char *) child_node->content);
    }
    else
    {
        sprintf (errmsg, "Unknown utm_proj_params element: %s",
            cur_node->name);
        ard_error_handler (false, FUNC_NAME, errmsg);
    }

    return (SUCCESS);
}


/******************************************************************************
MODULE:  add_global_ard_metadata_proj_info

PURPOSE: Add the projection elements node to the global metadata structure.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error parsing the projection_info elements
SUCCESS         Successful parse of the projection_info values

NOTES:
******************************************************************************/
int add_global_ard_metadata_proj_info
(
    xmlNode *a_node,            /* I: pointer to the element node to process */
    Ard_proj_meta_t *proj_info  /* I: projection info structure */
)
{
    char FUNC_NAME[] = "add_global_ard_metadata_proj_info"; /* function name */
    char errmsg[STR_SIZE];        /* error message */
    xmlAttrPtr attr = NULL;       /* pointer to the element attributes */
    xmlNode *cur_node = NULL;     /* pointer to the current node */
    xmlNode *child_node = NULL;   /* pointer to the child node */
    xmlNsPtr ns = NULL;           /* pointer to the namespace */
    xmlChar *attr_val = NULL;     /* attribute value */
    bool is_ul = false;           /* is this the UL corner */
    bool is_lr = false;           /* is this the LR corner */
    double x, y;                  /* x/y values */
    int count;                    /* number of chars copied in snprintf */

    /* Set up the current and child pointers */
    cur_node = a_node;
    ns = cur_node->nsDef;
    child_node = cur_node->children;

    /* Verify the namespace of this node is our ARD namespace.  If it isn't
       then the element won't be added to the metadata structure. */
    if (!xmlStrEqual (ns->href, (const xmlChar *) ARD_NS))
    {
        sprintf (errmsg, "Skipping %s since it is not in the ARD namespace",
            cur_node->name);
        ard_error_handler (false, FUNC_NAME, errmsg);
        return (SUCCESS);
    }

    /* Initialize the datum to no datum */
    proj_info->datum_type = ARD_NODATUM;

    /* Handle the element attributes */
    for (attr = cur_node->properties; attr != NULL; attr = attr->next)
    {
        attr_val = xmlGetProp (cur_node, attr->name);
        if (xmlStrEqual (attr->name, (const xmlChar *) "projection"))
        {
            if (xmlStrEqual (attr_val, (const xmlChar *) "GEO"))
                proj_info->proj_type = ARD_GCTP_GEO_PROJ;
            else if (xmlStrEqual (attr_val, (const xmlChar *) "UTM"))
                proj_info->proj_type = ARD_GCTP_UTM_PROJ;
            else if (xmlStrEqual (attr_val, (const xmlChar *) "PS"))
                proj_info->proj_type = ARD_GCTP_PS_PROJ;
            else if (xmlStrEqual (attr_val, (const xmlChar *) "AEA"))
                proj_info->proj_type = ARD_GCTP_ALBERS_PROJ;
            else if (xmlStrEqual (attr_val, (const xmlChar *) "SIN"))
                proj_info->proj_type = ARD_GCTP_SIN_PROJ;
        }
        else if (xmlStrEqual (attr->name, (const xmlChar *) "datum"))
        {
            if (xmlStrEqual (attr_val, (const xmlChar *) "WGS84"))
                proj_info->datum_type = ARD_WGS84;
            else if (xmlStrEqual (attr_val, (const xmlChar *) "NAD27"))
                proj_info->datum_type = ARD_NAD27;
            else if (xmlStrEqual (attr_val, (const xmlChar *) "NAD83"))
                proj_info->datum_type = ARD_NAD83;
        }
        else if (xmlStrEqual (attr->name, (const xmlChar *) "units"))
        {
            count = snprintf (proj_info->units,
                sizeof (proj_info->units), "%s", (const char *) attr_val);
            if (count < 0 || count >= sizeof (proj_info->units))
            {
                sprintf (errmsg, "Overflow of proj_info->units string");
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
        else
        {
            sprintf (errmsg, "Unknown attribute for element (%s): %s",
                cur_node->name, attr->name);
            ard_error_handler (false, FUNC_NAME, errmsg);
        }
        xmlFree (attr_val);
    }

    /* Process the siblings in the projection information */
    for (cur_node = a_node->children; cur_node;
         cur_node = xmlNextElementSibling (cur_node))
    {
        /* Set up the child pointer */
        child_node = cur_node->children;

        /* Check for the element nodes within the projection_information
           element */
        if (xmlStrEqual (cur_node->name, (const xmlChar *) "corner_point"))
        {
            /* Handle the element attributes */
            x = -9999.0;
            y = -9999.0;
            for (attr = cur_node->properties; attr != NULL; attr = attr->next)
            {
                attr_val = xmlGetProp (cur_node, attr->name);
                if (xmlStrEqual (attr->name, (const xmlChar *) "location"))
                {
                    is_ul = false;
                    is_lr = false;
                    if (xmlStrEqual (attr_val, (const xmlChar *) "UL"))
                        is_ul = true;
                    else if (xmlStrEqual (attr_val, (const xmlChar *) "LR"))
                        is_lr = true;
                    else
                    {
                        sprintf (errmsg, "Unknown corner_point location "
                            "specified (%s). UL and LR expected.", attr_val);
                        ard_error_handler (false, FUNC_NAME, errmsg);
                    }
                }
                else if (xmlStrEqual (attr->name, (const xmlChar *) "x"))
                    x = atof ((const char *) attr_val);
                else if (xmlStrEqual (attr->name, (const xmlChar *) "y"))
                    y = atof ((const char *) attr_val);
                else
                {
                    sprintf (errmsg, "unknown attribute for element (%s): %s",
                        cur_node->name, attr->name);
                    ard_error_handler (false, FUNC_NAME, errmsg);
                }
                xmlFree (attr_val);
            }

            /* Populate the correct corner point */
            if (is_ul)
            {
                proj_info->ul_corner[0] = x;
                proj_info->ul_corner[1] = y;
            }
            else if (is_lr)
            {
                proj_info->lr_corner[0] = x;
                proj_info->lr_corner[1] = y;
            }
        }
        else if (xmlStrEqual (cur_node->name, (const xmlChar *) "grid_origin"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Processing global_metadata:"
                    "projection_information element: %s.", cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }

            /* Copy the content of the child node into the value for this
               field */
            count = snprintf (proj_info->grid_origin,
                sizeof (proj_info->grid_origin), "%s", 
                (const char *) child_node->content);
            if (count < 0 || count >= sizeof (proj_info->grid_origin))
            {
                sprintf (errmsg, "Overflow of proj_info->grid_origin");
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
        else if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "utm_proj_params"))
        {
            /* Handle the projection-specific parameters */
            if (add_global_ard_metadata_proj_info_utm (cur_node, proj_info))
            {
                sprintf (errmsg, "Processing projection_information:"
                    "utm_proj_params elements");
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
        else if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "ps_proj_params"))
        {
            /* Handle the projection-specific parameters */
            if (add_global_ard_metadata_proj_info_ps (cur_node, proj_info))
            {
                sprintf (errmsg, "Processing projection_information:"
                    "ps_proj_params elements");
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
        else if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "albers_proj_params"))
        {
            /* Handle the projection-specific parameters */
            if (add_global_ard_metadata_proj_info_albers (cur_node, proj_info))
            {
                sprintf (errmsg, "Processing projection_information:"
                    "albers_proj_params elements");
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
        else if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "sin_proj_params"))
        {
            /* Handle the projection-specific parameters */
            if (add_global_ard_metadata_proj_info_sin (cur_node, proj_info))
            {
                sprintf (errmsg, "Processing projection_information:"
                    "sin_proj_params elements");
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
        else
        {
            sprintf (errmsg, "Unknown projection information element: %s",
                cur_node->name);
            ard_error_handler (false, FUNC_NAME, errmsg);
        }
    }  /* end for cur_node */

    return (SUCCESS);
}


/******************************************************************************
MODULE:  add_global_ard_metadata_bounding_coords

PURPOSE: Add the bounding coords elements node to the global metadata structure

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error parsing the bounding_coords elements
SUCCESS         Successful parse of the bounding_coords values

NOTES:
******************************************************************************/
int add_global_ard_metadata_bounding_coords
(
    xmlNode *a_node,            /* I: pointer to the element node to process */
    double bounding_coords[4]   /* O: bounding coordinates */
)
{
    char FUNC_NAME[] = "add_global_ard_metadata_bounding_coords";
                                  /* function name */
    char errmsg[STR_SIZE];        /* error message */
    xmlNode *cur_node = NULL;     /* pointer to the current node */
    xmlNode *child_node = NULL;   /* pointer to the child node */
    xmlNsPtr ns = NULL;           /* pointer to the namespace */
    int indx;                     /* index into the bounding coords array */

    /* Set up the current and child pointers */
    cur_node = a_node;
    ns = cur_node->nsDef;
    child_node = cur_node->children;

    /* Verify the namespace of this node is our ARD namespace.  If it isn't
       then the element won't be added to the metadata structure. */
    if (!xmlStrEqual (ns->href, (const xmlChar *) ARD_NS))
    {
        sprintf (errmsg, "Skipping %s since it is not in the ARD namespace",
            cur_node->name);
        ard_error_handler (false, FUNC_NAME, errmsg);
        return (SUCCESS);
    }

    /* Look for the ARD global metadata bounding coordinates elements and
       process them */
    indx = 0;
    if (xmlStrEqual (cur_node->name, (const xmlChar *) "west"))
        indx = ARD_WEST;
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "east"))
        indx = ARD_EAST;
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "north"))
        indx = ARD_NORTH;
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "south"))
        indx = ARD_SOUTH;
    else
    {
        sprintf (errmsg, "Unknown bounding coords element: %s", cur_node->name);
        ard_error_handler (false, FUNC_NAME, errmsg);
    }

    /* Expect the child node to be a text node containing the value of
       this field */
    if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
    {
        sprintf (errmsg, "Processing global_metadata element: %s.",
            cur_node->name);
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (ERROR);
    }

    /* Copy the content of the child node into the value for this field */
    bounding_coords[indx] = atof ((const char *) child_node->content);

    return (SUCCESS);
}


/******************************************************************************
MODULE:  add_global_scene_metadata

PURPOSE: Add the current element node to the scene-based global metadata
structure.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error parsing the global_metadata elements
SUCCESS         Successful parse of the global_metadata values

NOTES:
******************************************************************************/
int add_global_scene_metadata
(
    xmlNode *a_node,            /* I: pointer to the element node to process */
    Ard_global_scene_meta_t *gmeta /* I: scene-based global metadata struct */
)
{
    char FUNC_NAME[] = "add_global_scene_metadata";   /* function name */
    char errmsg[STR_SIZE];        /* error message */
    xmlAttrPtr attr = NULL;       /* pointer to the element attributes */
    xmlNode *cur_node = NULL;     /* pointer to the current node */
    xmlNode *child_node = NULL;   /* pointer to the child node */
    xmlNsPtr ns = NULL;           /* pointer to the namespace */
    xmlChar *attr_val = NULL;     /* attribute value */
    int count;                    /* number of chars copied in snprintf */

    /* Set up the current and child pointers */
    cur_node = a_node;
    ns = cur_node->nsDef;
    child_node = cur_node->children;

    /* Verify the namespace of this node is our ARD namespace.  If it isn't
       then the element won't be added to the metadata structure. */
    if (!xmlStrEqual (ns->href, (const xmlChar *) ARD_NS))
    {
        sprintf (errmsg, "Skipping %s since it is not in the ARD namespace",
            cur_node->name);
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (SUCCESS);
    }

    /* Look for the scene-based global metadata elements and process them */
    if (xmlStrEqual (cur_node->name, (const xmlChar *) "data_provider"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->data_provider, sizeof (gmeta->data_provider),
            "%s", (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->data_provider))
        {
            sprintf (errmsg, "Overflow of gmeta->data_provider string");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "satellite"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->satellite, sizeof (gmeta->satellite), "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->satellite))
        {
            sprintf (errmsg, "Overflow of gmeta->satellite string");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "instrument"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->instrument, sizeof (gmeta->instrument), "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->instrument))
        {
            sprintf (errmsg, "Overflow of gmeta->instrument string");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "acquisition_date"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->acquisition_date,
            sizeof (gmeta->acquisition_date), "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->acquisition_date))
        {
            sprintf (errmsg, "Overflow of gmeta->acquisition_date string");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name,
        (const xmlChar *) "scene_center_time"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->scene_center_time,
            sizeof (gmeta->scene_center_time), "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->scene_center_time))
        {
            sprintf (errmsg, "Overflow of gmeta->scene_center_time string");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name,
        (const xmlChar *) "level1_production_date"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->level1_production_date,
            sizeof (gmeta->level1_production_date), "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->level1_production_date))
        {
            sprintf (errmsg, "Overflow of gmeta->level1_production_date");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "wrs"))
    {
        /* Handle the element attributes */
        for (attr = cur_node->properties; attr != NULL; attr = attr->next)
        {
            attr_val = xmlGetProp (cur_node, attr->name);
            if (xmlStrEqual (attr->name, (const xmlChar *) "system"))
                gmeta->wrs_system = atoi ((const char *) attr_val);
            else if (xmlStrEqual (attr->name, (const xmlChar *) "path"))
                gmeta->wrs_path = atoi ((const char *) attr_val);
            else if (xmlStrEqual (attr->name, (const xmlChar *) "row"))
                gmeta->wrs_row = atoi ((const char *) attr_val);
            else
            {
                sprintf (errmsg, "WARNING: unknown attribute for element "
                    "(%s): %s\n", cur_node->name, attr->name);
                ard_error_handler (false, FUNC_NAME, errmsg);
            }
            xmlFree (attr_val);
        }
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "request_id"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->request_id, sizeof (gmeta->request_id), "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->request_id))
        {
            sprintf (errmsg, "Overflow of gmeta->request_id string");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "scene_id"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->scene_id, sizeof (gmeta->scene_id), "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->scene_id))
        {
            sprintf (errmsg, "Overflow of gmeta->scene_id string");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "product_id"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->product_id, sizeof (gmeta->product_id), "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->product_id))
        {
            sprintf (errmsg, "Overflow of gmeta->product_id string");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "elevation_source"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE)
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Use the attribute value to set the elevation_src */
        if (xmlStrEqual (child_node->content, (const xmlChar *) "NED"))
            gmeta->elevation_src = ARD_NED;
        else if (xmlStrEqual (child_node->content, (const xmlChar *) "SRTM"))
            gmeta->elevation_src = ARD_SRTM;
        else if (xmlStrEqual (child_node->content, (const xmlChar *) "GTOPO30"))
            gmeta->elevation_src = ARD_GTOPO30;
        else if (xmlStrEqual (child_node->content, (const xmlChar *) "GLS2000"))
            gmeta->elevation_src = ARD_GLS2000;
        else if (xmlStrEqual (child_node->content, (const xmlChar *) "RAMP"))
            gmeta->elevation_src = ARD_RAMP;
        else
        {
            sprintf (errmsg, "WARNING: unknown option for element (%s): "
                "%s\n", cur_node->name, child_node->content);
            ard_error_handler (false, FUNC_NAME, errmsg);
        }
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "sensor_mode"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE)
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Use the attribute value to set the sensor_mode */
        if (xmlStrEqual (child_node->content, (const xmlChar *) "BUMPER"))
            gmeta->sensor_mode = ARD_BUMPER;
        else if (xmlStrEqual (child_node->content, (const xmlChar *) "SAM"))
            gmeta->sensor_mode = ARD_SAM;
        else
        {
            sprintf (errmsg, "WARNING: unknown option for element (%s): "
                "%s\n", cur_node->name, child_node->content);
            ard_error_handler (false, FUNC_NAME, errmsg);
        }
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "ephemeris_type"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE)
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Use the attribute value to set the ephemeris_type */
        if (xmlStrEqual (child_node->content, (const xmlChar *) "DEFINITIVE"))
            gmeta->ephemeris_type = ARD_DEFINITIVE;
        else if (xmlStrEqual (child_node->content,
            (const xmlChar *) "PREDICTIVE"))
            gmeta->ephemeris_type = ARD_PREDICTIVE;
        else
        {
            sprintf (errmsg, "WARNING: unknown option for element (%s): "
                "%s\n", cur_node->name, child_node->content);
            ard_error_handler (false, FUNC_NAME, errmsg);
        }
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "cpf_name"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->cpf_name, sizeof (gmeta->cpf_name), "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->cpf_name))
        {
            sprintf (errmsg, "Overflow of gmeta->cpf_name string");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name,
        (const xmlChar *) "lpgs_metadata_file"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->lpgs_metadata_file,
            sizeof (gmeta->lpgs_metadata_file), "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->lpgs_metadata_file))
        {
            sprintf (errmsg, "Overflow of gmeta->lpgs_metadata_file string");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name,
        (const xmlChar *) "geometric_rmse_model"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        gmeta->geometric_rmse_model = atof ((const char *) child_node->content);
    }
    else if (xmlStrEqual (cur_node->name,
        (const xmlChar *) "geometric_rmse_model_x"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        gmeta->geometric_rmse_model_x =
            atof ((const char *) child_node->content);
    }
    else if (xmlStrEqual (cur_node->name,
        (const xmlChar *) "geometric_rmse_model_y"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        gmeta->geometric_rmse_model_y =
            atof ((const char *) child_node->content);
    }
    else
    {
        sprintf (errmsg, "Unknown element (%s) in the global_metadata",
            cur_node->name);
        ard_error_handler (false, FUNC_NAME, errmsg);
    }

    return (SUCCESS);
}


/******************************************************************************
MODULE:  add_global_tile_metadata

PURPOSE: Add the current element node to the tile-based global metadata
structure.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error parsing the global_metadata elements
SUCCESS         Successful parse of the global_metadata values

NOTES:
******************************************************************************/
int add_global_tile_metadata
(
    xmlNode *a_node,            /* I: pointer to the element node to process */
    Ard_global_tile_meta_t *gmeta /* I: tile-based global metadata struct */
)
{
    char FUNC_NAME[] = "add_global_tile_metadata";   /* function name */
    char errmsg[STR_SIZE];        /* error message */
    xmlAttrPtr attr = NULL;       /* pointer to the element attributes */
    xmlNode *cur_node = NULL;     /* pointer to the current node */
    xmlNode *child_node = NULL;   /* pointer to the child node */
    xmlNsPtr ns = NULL;           /* pointer to the namespace */
    xmlChar *attr_val = NULL;     /* attribute value */
    int count;                    /* number of chars copied in snprintf */

    /* Set up the current and child pointers */
    cur_node = a_node;
    ns = cur_node->nsDef;
    child_node = cur_node->children;

    /* Verify the namespace of this node is our ARD namespace.  If it isn't
       then the element won't be added to the metadata structure. */
    if (!xmlStrEqual (ns->href, (const xmlChar *) ARD_NS))
    {
        sprintf (errmsg, "Skipping %s since it is not in the ARD namespace",
            cur_node->name);
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (SUCCESS);
    }

    /* Look for the scene-based global metadata elements and process them */
    if (xmlStrEqual (cur_node->name, (const xmlChar *) "data_provider"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->data_provider, sizeof (gmeta->data_provider),
            "%s", (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->data_provider))
        {
            sprintf (errmsg, "Overflow of gmeta->data_provider string");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "satellite"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE)
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->satellite, sizeof (gmeta->satellite), "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->satellite))
        {
            sprintf (errmsg, "Overflow of gmeta->satellite string");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "instrument"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE)
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->instrument, sizeof (gmeta->instrument), "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->instrument))
        {
            sprintf (errmsg, "Overflow of gmeta->instrument string");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name,
        (const xmlChar *) "level1_collection"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE)
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->level1_collection,
            sizeof (gmeta->level1_collection), "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->level1_collection))
        {
            sprintf (errmsg, "Overflow of gmeta->level1_collection string");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "ard_version"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE)
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->ard_version, sizeof (gmeta->ard_version), "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->ard_version))
        {
            sprintf (errmsg, "Overflow of gmeta->ard_version string");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "region"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE)
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->region, sizeof (gmeta->region), "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->region))
        {
            sprintf (errmsg, "Overflow of gmeta->region string");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "acquisition_date"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->acquisition_date,
            sizeof (gmeta->acquisition_date), "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->acquisition_date))
        {
            sprintf (errmsg, "Overflow of gmeta->acquisition_date string");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "product_id"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->product_id, sizeof (gmeta->product_id), "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->product_id))
        {
            sprintf (errmsg, "Overflow of gmeta->product_id");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name,
        (const xmlChar *) "production_date"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        count = snprintf (gmeta->production_date,
            sizeof (gmeta->production_date), "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= sizeof (gmeta->production_date))
        {
            sprintf (errmsg, "Overflow of gmeta->production_date");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name,
        (const xmlChar *) "bounding_coordinates"))
    {
        /* Process the siblings in the bounding coordinates */
        for (cur_node = a_node->children; cur_node;
             cur_node = xmlNextElementSibling (cur_node))
        {
            if (add_global_ard_metadata_bounding_coords (cur_node,
                gmeta->bounding_coords))
            {
                sprintf (errmsg, "Processing bounding_coordinates element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
    }
    else if (xmlStrEqual (cur_node->name,
        (const xmlChar *) "projection_information"))
    {
        /* Process the elements within the projection information */
        if (add_global_ard_metadata_proj_info (cur_node, &gmeta->proj_info))
        {
            sprintf (errmsg, "Processing projection_information elements");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
    }
    else if (xmlStrEqual (cur_node->name,
        (const xmlChar *) "orientation_angle"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        gmeta->orientation_angle = atof ((const char *) child_node->content);
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "tile_grid"))
    {
        /* Handle the element attributes */
        for (attr = cur_node->properties; attr != NULL; attr = attr->next)
        {
            attr_val = xmlGetProp (cur_node, attr->name);
            if (xmlStrEqual (attr->name, (const xmlChar *) "h"))
                gmeta->htile = atoi ((const char *) attr_val);
            else if (xmlStrEqual (attr->name, (const xmlChar *) "v"))
                gmeta->vtile = atoi ((const char *) attr_val);
            else
            {
                sprintf (errmsg, "WARNING: unknown attribute for element "
                    "(%s): %s\n", cur_node->name, attr->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
            xmlFree (attr_val);
        }
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "scene_count"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE)
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        gmeta->scene_count = atoi ((const char *) child_node->content);
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "cloud_cover"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE)
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        gmeta->cloud_cover = atof ((const char *) child_node->content);
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "cloud_shadow"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE)
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        gmeta->cloud_shadow = atof ((const char *) child_node->content);
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "snow_ice"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE)
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        gmeta->snow_ice = atof ((const char *) child_node->content);
    }
    else if (xmlStrEqual (cur_node->name, (const xmlChar *) "fill"))
    {
        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE)
        {
            sprintf (errmsg, "Processing global_metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the value for this field */
        gmeta->fill = atof ((const char *) child_node->content);
    }
    else
    {
        sprintf (errmsg, "Unknown element (%s) in the global_metadata",
            cur_node->name);
        ard_error_handler (false, FUNC_NAME, errmsg);
    }

    return (SUCCESS);
}


/******************************************************************************
MODULE:  add_ard_band_metadata_bitmap_description

PURPOSE: Adds the bit elements to the bitmap description to the band metadata
structure.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error parsing the bit elements
SUCCESS         Successful parse of the bit values

NOTES:
1. Memory is allocated in the band metadata for the number of bits in the
   bitmap description.
******************************************************************************/
int add_ard_band_metadata_bitmap_description
(
    xmlNode *a_node,            /* I/O: pointer to the element node to
                                        process */
    Ard_band_meta_t *bmeta      /* I: band metadata structure for current
                                      band in the bands structure */
)
{
    char FUNC_NAME[] = "add_ard_band_metadata_bitmap_description";
                                  /* func name */
    char errmsg[STR_SIZE];        /* error message */
    xmlNode *cur_node = NULL;     /* pointer to the current node */
    xmlNode *child_node = NULL;   /* pointer to the child node */
    int nbits = 0;                /* number of bits in the bitmap description */
    int count;                    /* number of chars copied in snprintf */

    /* Count the number of siblings which are bit descriptions */
    for (cur_node = a_node; cur_node;
         cur_node = xmlNextElementSibling (cur_node))
    {
        /* If this is a bit element then count it */
        if (xmlStrEqual (cur_node->name, (const xmlChar *) "bit"))
            nbits++;
    }

    /* Allocate memory in the band structure for the number of bits */
    if (allocate_ard_bitmap_metadata (bmeta, nbits) != SUCCESS)
    {
        sprintf (errmsg, "Allocating memory to the band structure for %d "
            "bits in the bitmap description.", nbits);
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (ERROR);
    }

    /* Process the siblings as long as they are bit elements.  Assume the XML
       file has a description for every bit number inclusive from 0 to nbits-1
       therefore the bit num attribute will not be stored. */
    nbits = 0;
    for (cur_node = a_node; cur_node;
         cur_node = xmlNextElementSibling (cur_node))
    {
        /* Set up the child pointer */
        child_node = cur_node->children;

        /* If this isn't a bit element then skip to the next one */
        if (!xmlStrEqual (cur_node->name, (const xmlChar *) "bit"))
            continue;

        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Error processing band metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the array of bits in the
           bitmap_description */
        count = snprintf (bmeta->bitmap_description[nbits], STR_SIZE, "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= STR_SIZE)
        {
            sprintf (errmsg, "Overflow of bmeta->bitmap_description[nbits]");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Increment the bit count */
        nbits++;
    }

    /* Successful processing */
    return (SUCCESS);
}


/******************************************************************************
MODULE:  add_ard_band_metadata_class_values

PURPOSE: Adds the class elements to the band metadata structure.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error parsing the class elements
SUCCESS         Successful parse of the class values

NOTES:
1. Memory is allocated in the band metadata for the number of classes in the
   class values.
******************************************************************************/
int add_ard_band_metadata_class_values
(
    xmlNode *a_node,            /* I/O: pointer to the element node to
                                        process */
    Ard_band_meta_t *bmeta      /* I: band metadata structure for current
                                      band in the bands structure */
)
{
    char FUNC_NAME[] = "add_ard_band_metadata_class_values"; /* func name */
    char errmsg[STR_SIZE];        /* error message */
    xmlNode *cur_node = NULL;     /* pointer to the current node */
    xmlNode *child_node = NULL;   /* pointer to the child node */
    xmlAttrPtr attr = NULL;       /* pointer to the element attributes */
    xmlChar *attr_val = NULL;     /* attribute value */
    int nclass = 0;               /* number of classes in the class values */
    int count;                    /* number of chars copied in snprintf */

    /* Count the number of siblings which are class descriptions */
    for (cur_node = a_node; cur_node;
         cur_node = xmlNextElementSibling (cur_node))
    {
        /* If this is a class element then count it */
        if (xmlStrEqual (cur_node->name, (const xmlChar *) "class"))
            nclass++;
    }

    /* Allocate memory in the band structure for the number of classes */
    if (allocate_ard_class_metadata (bmeta, nclass) != SUCCESS)
    {
        sprintf (errmsg, "Allocating memory to the band structure for %d "
            "classes in the class_values.", nclass);
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (ERROR);
    }

    /* Process the siblings as long as they are class elements */
    nclass = 0;
    for (cur_node = a_node; cur_node;
         cur_node = xmlNextElementSibling (cur_node))
    {
        /* Set up the child pointer */
        child_node = cur_node->children;

        /* If this isn't a class element then skip to the next one */
        if (!xmlStrEqual (cur_node->name, (const xmlChar *) "class"))
            continue;

        /* Handle the element attributes */
        for (attr = cur_node->properties; attr != NULL; attr = attr->next)
        {
            attr_val = xmlGetProp (cur_node, attr->name);
            if (xmlStrEqual (attr->name, (const xmlChar *) "num"))
                bmeta->class_values[nclass].class =
                    atoi ((const char *) attr_val);
            else
            {
                sprintf (errmsg, "WARNING: unknown attribute for element "
                    "(%s): %s\n", cur_node->name, attr->name);
                ard_error_handler (false, FUNC_NAME, errmsg);
            }
            xmlFree (attr_val);
        }

        /* Expect the child node to be a text node containing the value of
           this field */
        if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
        {
            sprintf (errmsg, "Error processing band metadata element: %s.",
                cur_node->name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Copy the content of the child node into the array of classes in the
           class_values */
        count = snprintf (bmeta->class_values[nclass].description,
            STR_SIZE, "%s",
            (const char *) child_node->content);
        if (count < 0 || count >= STR_SIZE)
        {
            sprintf (errmsg,
                "Overflow of bmeta->class_values[nclass].description");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Increment the class count */
        nclass++;
    }

    /* Successful processing */
    return (SUCCESS);
}


/******************************************************************************
MODULE:  add_ard_band_metadata

PURPOSE: Add the current band element node to the current band metadata
structure and process the childrend of this node.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error parsing the band metadata elements
SUCCESS         Successful parse of the band metadata values

NOTES:
******************************************************************************/
int add_ard_band_metadata
(
    xmlNode *a_node,            /* I: pointer to the element node to process */
    Ard_band_meta_t *bmeta      /* I: band metadata structure for current
                                      band in the bands structure */
)
{
    char FUNC_NAME[] = "add_ard_band_metadata";   /* function name */
    char errmsg[STR_SIZE];        /* error message */
    xmlAttrPtr attr = NULL;       /* pointer to the element attributes */
    xmlNode *cur_node = NULL;     /* pointer to the current node */
    xmlNode *child_node = NULL;   /* pointer to the child node */
    xmlNsPtr ns = NULL;           /* pointer to the namespace */
    xmlChar *attr_val = NULL;     /* attribute value */
    int count;                    /* number of chars copied in snprintf */

    /* Set up the current and child pointers */
    cur_node = a_node;
    ns = cur_node->nsDef;

    /* Verify the namespace of this node is our ARD namespace.  If it isn't
       then the element won't be added to the metadata structure. */
    if (!xmlStrEqual (ns->href, (const xmlChar *) ARD_NS))
    {
        sprintf (errmsg, "Skipping %s since it is not in the ARD namespace",
            cur_node->name);
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (SUCCESS);
    }

    /* Handle the element attributes for the band element */
    for (attr = cur_node->properties; attr != NULL; attr = attr->next)
    {
        attr_val = xmlGetProp (cur_node, attr->name);
        if (xmlStrEqual (attr->name, (const xmlChar *) "product"))
        {
            count = snprintf (bmeta->product, sizeof (bmeta->product),
                "%s", (const char *) attr_val);
            if (count < 0 || count >= sizeof (bmeta->product))
            {
                sprintf (errmsg, "Overflow of bmeta->product string");
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
        else if (xmlStrEqual (attr->name, (const xmlChar *) "source"))
        {
            count = snprintf (bmeta->source, sizeof (bmeta->source),
                "%s", (const char *) attr_val);
            if (count < 0 || count >= sizeof (bmeta->source))
            {
                sprintf (errmsg, "Overflow of bmeta->source string");
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
        else if (xmlStrEqual (attr->name, (const xmlChar *) "name"))
        {
            count = snprintf (bmeta->name, sizeof (bmeta->name),
                "%s", (const char *) attr_val);
            if (count < 0 || count >= sizeof (bmeta->name))
            {
                sprintf (errmsg, "Overflow of bmeta->name string");
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
        else if (xmlStrEqual (attr->name, (const xmlChar *) "category"))
        {
            count = snprintf (bmeta->category, sizeof (bmeta->category),
                "%s", (const char *) attr_val);
            if (count < 0 || count >= sizeof (bmeta->category))
            {
                sprintf (errmsg, "Overflow of bmeta->category string");
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
        else if (xmlStrEqual (attr->name, (const xmlChar *) "data_type"))
        {
            if (xmlStrEqual (attr_val, (const xmlChar *) "INT8"))
                bmeta->data_type = ARD_INT8;
            else if (xmlStrEqual (attr_val, (const xmlChar *) "UINT8"))
                bmeta->data_type = ARD_UINT8;
            else if (xmlStrEqual (attr_val, (const xmlChar *) "INT16"))
                bmeta->data_type = ARD_INT16;
            else if (xmlStrEqual (attr_val, (const xmlChar *) "UINT16"))
                bmeta->data_type = ARD_UINT16;
            else if (xmlStrEqual (attr_val, (const xmlChar *) "INT32"))
                bmeta->data_type = ARD_INT32;
            else if (xmlStrEqual (attr_val, (const xmlChar *) "UINT32"))
                bmeta->data_type = ARD_UINT32;
            else if (xmlStrEqual (attr_val, (const xmlChar *) "FLOAT32"))
                bmeta->data_type = ARD_FLOAT32;
            else if (xmlStrEqual (attr_val, (const xmlChar *) "FLOAT64"))
                bmeta->data_type = ARD_FLOAT64;
        }
        else if (xmlStrEqual (attr->name, (const xmlChar *) "nlines"))
            bmeta->nlines = atoi ((const char *) attr_val);
        else if (xmlStrEqual (attr->name, (const xmlChar *) "nsamps"))
            bmeta->nsamps = atoi ((const char *) attr_val);
        else if (xmlStrEqual (attr->name, (const xmlChar *) "fill_value"))
            bmeta->fill_value = atoi ((const char *) attr_val);
        else if (xmlStrEqual (attr->name,
            (const xmlChar *) "saturate_value"))
            bmeta->saturate_value = atoi ((const char *) attr_val);
        else if (xmlStrEqual (attr->name, (const xmlChar *) "scale_factor"))
            bmeta->scale_factor = atof ((const char *) attr_val);
        else if (xmlStrEqual (attr->name, (const xmlChar *) "add_offset"))
            bmeta->add_offset = atof ((const char *) attr_val);
        else
        {
            sprintf (errmsg, "WARNING: unknown attribute for element (%s): "
                "%s\n", cur_node->name, attr->name);
            ard_error_handler (false, FUNC_NAME, errmsg);
        }
        xmlFree (attr_val);
    }

    /* Process the children of this node; start with the first child then
       process it's siblings */
    for (cur_node = a_node->children; cur_node;
         cur_node = xmlNextElementSibling (cur_node))
    {
        child_node = cur_node->children;
        if (xmlStrEqual (cur_node->name, (const xmlChar *) "short_name"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Processing band metadata element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }

            /* Copy the content of the child node into value for this field */
            count = snprintf (bmeta->short_name, sizeof (bmeta->short_name),
                "%s", (const char *) child_node->content);
            if (count < 0 || count >= sizeof (bmeta->short_name))
            {
                sprintf (errmsg, "Overflow of bmeta->short_name string");
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
        else if (xmlStrEqual (cur_node->name, (const xmlChar *) "long_name"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Processing band metadata element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }

            /* Copy the content of the child node into value for this field */
            count = snprintf (bmeta->long_name, sizeof (bmeta->long_name),
                "%s", (const char *) child_node->content);
            if (count < 0 || count >= sizeof (bmeta->long_name))
            {
                sprintf (errmsg, "Overflow of bmeta->long_name string");
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
        else if (xmlStrEqual (cur_node->name, (const xmlChar *) "file_name"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Processing band metadata element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }

            /* Copy the content of the child node into value for this field */
            count = snprintf (bmeta->file_name, sizeof (bmeta->file_name),
                "%s", (const char *) child_node->content);
            if (count < 0 || count >= sizeof (bmeta->file_name))
            {
                sprintf (errmsg, "Overflow of bmeta->file_name string");
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
        else if (xmlStrEqual (cur_node->name, (const xmlChar *) "pixel_size"))
        {
            /* Handle the element attributes */
            for (attr = cur_node->properties; attr != NULL; attr = attr->next)
            {
                attr_val = xmlGetProp (cur_node, attr->name);
                if (xmlStrEqual (attr->name, (const xmlChar *) "x"))
                    bmeta->pixel_size[0] =
                        (double) atof ((const char *) attr_val);
                else if (xmlStrEqual (attr->name, (const xmlChar *) "y"))
                    bmeta->pixel_size[1] =
                        (double) atof ((const char *) attr_val);
                else if (xmlStrEqual (attr->name, (const xmlChar *) "units"))
                {
                    count = snprintf (bmeta->pixel_units,
                        sizeof (bmeta->pixel_units), "%s",
                        (const char *) attr_val);
                    if (count < 0 || count >= sizeof (bmeta->pixel_units))
                    {
                        sprintf (errmsg, "Overflow of bmeta->pixel_units");
                        ard_error_handler (true, FUNC_NAME, errmsg);
                        return (ERROR);
                    }
                }
                else
                {
                    sprintf (errmsg, "WARNING: unknown attribute for element "
                        "(%s): %s\n", cur_node->name, attr->name);
                    ard_error_handler (false, FUNC_NAME, errmsg);
                }
                xmlFree (attr_val);
            }
        }
        else if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "resample_method"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Processing band metadata element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }

            /* Use the attribute value to set the resample_method */
            if (xmlStrEqual (child_node->content,
                (const xmlChar *) "cubic convolution"))
                bmeta->resample_method = ARD_CC;
            else if (xmlStrEqual (child_node->content,
                (const xmlChar *) "nearest neighbor"))
                bmeta->resample_method = ARD_NN;
            else if (xmlStrEqual (child_node->content,
                (const xmlChar *) "bilinear"))
                bmeta->resample_method = ARD_BI;
            else if (xmlStrEqual (child_node->content,
                (const xmlChar *) "none"))
                bmeta->resample_method = ARD_NONE;
            else
            {
                sprintf (errmsg, "WARNING: unknown option for element (%s): "
                    "%s\n", cur_node->name, child_node->content);
                ard_error_handler (false, FUNC_NAME, errmsg);
            }
        }
        else if (xmlStrEqual (cur_node->name, (const xmlChar *) "data_units"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Processing band metadata element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }

            /* Copy the content of the child node into value for this field */
            count = snprintf (bmeta->data_units, sizeof (bmeta->data_units),
                "%s", (const char *) child_node->content);
            if (count < 0 || count >= sizeof (bmeta->data_units))
            {
                sprintf (errmsg, "Overflow of bmeta->data_units string");
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
        else if (xmlStrEqual (cur_node->name, (const xmlChar *) "valid_range"))
        {
            /* Handle the element attributes */
            for (attr = cur_node->properties; attr != NULL; attr = attr->next)
            {
                attr_val = xmlGetProp (cur_node, attr->name);
                if (xmlStrEqual (attr->name, (const xmlChar *) "min"))
                    bmeta->valid_range[0] = atof ((const char *) attr_val);
                else if (xmlStrEqual (attr->name, (const xmlChar *) "max"))
                    bmeta->valid_range[1] = atof ((const char *) attr_val);
                else
                {
                    sprintf (errmsg, "WARNING: unknown attribute for element "
                        "(%s): %s\n", cur_node->name, attr->name);
                    ard_error_handler (false, FUNC_NAME, errmsg);
                }
                xmlFree (attr_val);
            }
        }
        else if (xmlStrEqual (cur_node->name, (const xmlChar *) "app_version"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Processing band metadata element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }

            /* Copy the content of the child node into value for this field */
            count = snprintf (bmeta->app_version, sizeof (bmeta->app_version),
                "%s", (const char *) child_node->content);
            if (count < 0 || count >= sizeof (bmeta->app_version))
            {
                sprintf (errmsg, "Overflow of bmeta->app_version string");
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
        else if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "production_date"))
        {
            /* Expect the child node to be a text node containing the value of
               this field */
            if (child_node == NULL || child_node->type != XML_TEXT_NODE) 
            {
                sprintf (errmsg, "Processing band metadata element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }

            /* Copy the content of the child node into value for this field */
            count = snprintf (bmeta->production_date,
                sizeof (bmeta->production_date), "%s",
                (const char *) child_node->content);
            if (count < 0 || count >= sizeof (bmeta->production_date))
            {
                sprintf (errmsg, "Overflow of bmeta->production_date string");
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
        else if (xmlStrEqual (cur_node->name,
            (const xmlChar *) "bitmap_description"))
        {
            if (add_ard_band_metadata_bitmap_description (cur_node->children,
                bmeta) != SUCCESS)
            {
                sprintf (errmsg, "Processing bitmap_description element: "
                    "%s.", cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
        else if (xmlStrEqual (cur_node->name, (const xmlChar *) "class_values"))
        {
            if (add_ard_band_metadata_class_values (cur_node->children, bmeta)
                != SUCCESS)
            {
                sprintf (errmsg, "Processing class_values element: %s.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }
        else
        {
            sprintf (errmsg, "Unknown element (%s) in the band metadata",
                cur_node->name);
            ard_error_handler (false, FUNC_NAME, errmsg);
        }
    } /* for siblings */

    return (SUCCESS);
}


/******************************************************************************
MODULE:  parse_ard_xml_into_struct

PURPOSE: Parse the XML document data into the ARD metadata structure.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error parsing the metadata elements
SUCCESS         Successful parse of the metadata values

NOTES:
1. Uses a stack of character strings to keep track of the nodes that have
   been parsed.  The stack must be allocated before calling this routine.
******************************************************************************/
int parse_ard_xml_into_struct
(
    xmlNode *a_node,           /* I: pointer to the current node */
    Ard_meta_t *ard_meta,      /* I: ARD metadata structure to be filled */
    int *top_of_stack,         /* I: pointer to top of the stack */
    char **stack               /* I: stack to use for parsing */
)
{
    char FUNC_NAME[] = "parse_ard_xml_into_struct";  /* function name */
    char errmsg[STR_SIZE];        /* error message */
    char *curr_stack_element = NULL;  /* element popped from the stack */
    xmlNode *cur_node = NULL;    /* pointer to the current node */
    xmlNode *sib_node = NULL;    /* pointer to the sibling node */
    static int nbands = 0;       /* number of bands in tile/scene container */
    static bool tile_metadata = false;  /* are we parsing the tile-specific
                                    metadata section of the ARD metadata? */
    static bool scene_metadata = false; /* are we parsing the scene-specific
                                    metadata section of the ARD metadata? */
    static bool global_metadata = false;  /* are we parsing the global metadata
                                    section of the ARD metadata? */
    static bool bands_metadata = false;   /* are we parsing the bands metadata
                                    section of the ARD metadata? */
    static int cur_band = 0;     /* current band being processed in the bands
                                    metadata section (zero-based) */
    static int cur_scene = -1;   /* current scene being processed in the XML
                                    metadata (zero-based) */
    bool skip_child;             /* boolean to specify the children of this
                                    node should not be processed */
    Ard_band_meta_t *bmeta = NULL;  /* pointer to tile/scene band metadata */
    Ard_tile_meta_t *tile_meta = &ard_meta->tile_meta;
                                 /* pointer to tile-specific metadata */
    static Ard_scene_meta_t *scene_meta = NULL;
                                 /* ptr to array of scene-specific metadata */

    /* Start at the input node and traverse the tree, visiting all the children
       and siblings */
    for (cur_node = a_node; cur_node;
         cur_node = xmlNextElementSibling (cur_node))
    {
        /* Process the children of this element unless otherwise specified */
        skip_child = false;

        /* Only print the ELEMENT node types */
        if (cur_node->type == XML_ELEMENT_NODE) 
        {
            /* Push the element to the stack and turn the booleans on if this
               is either the global metadata or the band metadata in either
               the tile- or scene- specific metadata */
            //printf ("***Pushed %s\n", cur_node->name); fflush (stdout);
            if (push (top_of_stack, stack, (const char *) cur_node->name))
            {
                sprintf (errmsg, "Pushing element '%s' to the stack.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }

            /* Turn the boolean on if this is the tile metadata container. Flag
               an issue if we have already traversed into the tile metadata. */
            if (xmlStrEqual (cur_node->name,
                (const xmlChar *) "tile_metadata"))
            {
                if (tile_metadata)
                {
                    sprintf (errmsg, "Current element node is '%s' however we "
                        "are already in the tile_metadata section.",
                        cur_node->name);
                    ard_error_handler (true, FUNC_NAME, errmsg);
                    return (ERROR);
                }

                /* Turn on the tile metadata and reinitialize the number of
                   bands */
                tile_metadata = true;
                nbands = 0;
            }

            /* Turn the boolean on if this is the scene metadata container.
               Flag an issue if we have already traversed into the scene
               metadata. Reinit the number of bands. */
            if (xmlStrEqual (cur_node->name,
                (const xmlChar *) "scene_metadata"))
            {
                if (scene_metadata)
                {
                    sprintf (errmsg, "Current element node is '%s' however we "
                        "are already in the scene_metadata section.",
                        cur_node->name);
                    ard_error_handler (true, FUNC_NAME, errmsg);
                    return (ERROR);
                }
                cur_scene++;  /* add to zero-based scene count */
                if (cur_scene >= MAX_TOTAL_SCENES)
                {
                    sprintf (errmsg, "Current scene count (%d) exceeds the max "
                        "total scenes (%d).\n", cur_scene+1, MAX_TOTAL_SCENES);
                    ard_error_handler (true, FUNC_NAME, errmsg);
                    return (ERROR);
                }

                /* Turn on the scene metadata and reinitialize the number of
                   bands */
                scene_metadata = true;
                nbands = 0;

                /* Set up the scene metadata pointer for the current scene */
                scene_meta = &ard_meta->scene_meta[cur_scene];
            }

            /* Turn the boolean on if this is the global_metadata. Flag an
               issue if we have already traversed into the global metadata
               for the current tile/scene section. */
            if (xmlStrEqual (cur_node->name,
                (const xmlChar *) "global_metadata"))
            {
                if (global_metadata)
                {
                    sprintf (errmsg, "Current element node is '%s' however we "
                        "are already in the global_metadata section for either "
                        "the tile or scene section.", cur_node->name);
                    ard_error_handler (true, FUNC_NAME, errmsg);
                    return (ERROR);
                }
                global_metadata = true;
            }

            /* Turn the boolean on if this is the bands metadata. Flag an
               issue if we have already traversed into the bands metadata for
               the current tile/scene section.  Count the number of band
               elements in this structure, then allocate memory for the
               nbands. */
            if (xmlStrEqual (cur_node->name, (const xmlChar *) "bands"))
            {
                if (bands_metadata)
                {
                    sprintf (errmsg, "Current element node is '%s' however we "
                        "are already in the bands section for either the tile "
                        "or scene section.", cur_node->name);
                    ard_error_handler (true, FUNC_NAME, errmsg);
                    return (ERROR);
                }
                bands_metadata = true;
                cur_band = 0;  /* reset to zero for start of band count */

                /* Count the number of siblings which are band elements */
                nbands = 0;
                for (sib_node = cur_node->children; sib_node;
                     sib_node = xmlNextElementSibling (sib_node))
                {
                    /* If this is a band element then count it */
                    if (xmlStrEqual (sib_node->name, (const xmlChar *) "band"))
                        nbands++;
                }

                /* Allocate the bands based on whether we are in the tile or
                   scene metadata */
                if (tile_metadata)
                {
                    if (allocate_ard_band_metadata (tile_meta, NULL, nbands) !=
                        SUCCESS)
                    {   /* Error messages already printed */
                        return (ERROR);
                    }
                }
                else if (scene_metadata)
                {
                    if (allocate_ard_band_metadata (NULL, scene_meta, nbands) !=
                        SUCCESS)
                    {   /* Error messages already printed */
                        return (ERROR);
                    }
                }
            }

            /* Print out the name of the element */
            xmlAttrPtr attr;     /* pointer to the element attributes */
            //printf ("node type: Element, name: %s\n", cur_node->name);

            /* Print out the attribute properties for this element */
            for (attr = cur_node->properties; attr != NULL; attr = attr->next)
            {
                xmlChar *v = xmlGetProp (cur_node, attr->name);
                //printf (" @%s=%s ", attr->name, v);
                xmlFree (v);
            }
            //printf ("\n"); fflush (stdout);

            /* If we are IN the global metadata (don't process the actual
               global_metadata element) then consume this node and add the
               information to the tile/scene global metadata structure */
            if (global_metadata && !xmlStrEqual (cur_node->name,
                (const xmlChar *) "global_metadata"))
            {
                /* Global metadata for tile-based and scene-based metadata is
                   slightly different */
                if (tile_metadata)
                {
                    if (add_global_tile_metadata (cur_node,
                        &tile_meta->tile_global))
                    {
                        sprintf (errmsg, "Consuming tile-based global_metadata "
                            "element '%s'.", cur_node->name);
                        ard_error_handler (true, FUNC_NAME, errmsg);
                        return (ERROR);
                    }
                }
                else if (scene_metadata)
                {
                    if (add_global_scene_metadata (cur_node,
                        &scene_meta->scene_global))
                    {
                        sprintf (errmsg, "Consuming scene-based "
                            "global_metadata element '%s'.", cur_node->name);
                        ard_error_handler (true, FUNC_NAME, errmsg);
                        return (ERROR);
                    }
                }

                /* Skip processing the children of this node, since they
                   will be handled by the global metadata parser */
                skip_child = true;
            }

            /* If we are IN the bands metadata and at a band element, then
               consume this node and add the information to the band metadata
               structure for the current band */
            if (bands_metadata && xmlStrEqual (cur_node->name,
                (const xmlChar *) "band"))
            {
                if (cur_band >= nbands)
                {
                    sprintf (errmsg, "Number of bands consumed already "
                        "reached the total number of bands allocated for this "
                        "scene/tile container (%d).", nbands);
                    ard_error_handler (true, FUNC_NAME, errmsg);
                    return (ERROR);
                }

                /* Add current band to the tile/scene-based band metadata */
                if (tile_metadata)
                    bmeta = &tile_meta->band[cur_band++];
                else if (scene_metadata)
                    bmeta = &scene_meta->band[cur_band++];
                if (add_ard_band_metadata (cur_node, bmeta))
                {
                    sprintf (errmsg, "Consuming band metadata element '%s'.",
                        cur_node->name);
                    ard_error_handler (true, FUNC_NAME, errmsg);
                    return (ERROR);
                }

                /* Skip processing the children of this node, since they
                   will be handled by the global metadata parser */
                skip_child = true;
            }
        }
        else if (cur_node->type == XML_TEXT_NODE) 
        {
            /* Print out the text for the element */
            //printf ("   node type: Text, content: %s\n", cur_node->content);
        }

        /* Parse the children of this node if they haven't been consumed
           elsewhere */
        if (!skip_child)
        {
            if (parse_ard_xml_into_struct (cur_node->children, ard_meta,
                top_of_stack, stack))
            {
                sprintf (errmsg, "Parsing the children of this element '%s'.",
                    cur_node->name);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
        }

        /* Done with the element and its siblings so pop the element name off
           the stack */
        if (cur_node->type == XML_ELEMENT_NODE)
        {
            curr_stack_element = pop (top_of_stack, stack);
            if (curr_stack_element == NULL)
            {
                sprintf (errmsg, "Popping elements off the stack.");
                ard_error_handler (true, FUNC_NAME, errmsg);
                return (ERROR);
            }
            //printf ("***Popped %s\n", curr_stack_element); fflush (stdout);

            if (!strcmp (curr_stack_element, "global_metadata"))
                global_metadata = false;
            if (!strcmp (curr_stack_element, "bands"))
                bands_metadata = false;
            if (!strcmp (curr_stack_element, "tile_metadata"))
                tile_metadata = false;
            if (!strcmp (curr_stack_element, "scene_metadata"))
                scene_metadata = false;
        }
    }  /* for cur_node */

    /* Set the number of scenes based on the number of scene_metadata that
       were parsed */
    ard_meta->nscenes = cur_scene + 1;

    return (SUCCESS);
}


/******************************************************************************
MODULE:  parse_ard_metadata

PURPOSE: Parse the input metadata file and populate the associated ARD metadata
file.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error parsing the metadata elements
SUCCESS         Successful parse of the metadata values

NOTES:
1. Uses a stack of character strings to keep track of the nodes that have been
   found in the metadata document.
2. For debugging purposes
   xmlDocDump (stderr, doc);
   can be used to dump/print the XML doc to the screen.
3. Input ARD metadata structure needs to be initialized via
   init_ard_metadata_struct.
******************************************************************************/
int parse_ard_metadata
(
    char *metafile,       /* I: input metadata file or URL */
    Ard_meta_t *ard_meta  /* I: input ARD metadata structure which has been
                                initialized via init_ard_metadata_struct */
)
{
    char FUNC_NAME[] = "parse_ard_metadata";  /* function name */
    char errmsg[STR_SIZE];    /* error message */
    xmlTextReaderPtr reader;  /* reader for the XML file */
    xmlDocPtr doc = NULL;     /* document tree pointer */
    xmlNodePtr current=NULL;  /* pointer to the current node */
    int status;               /* return status */
    int nodeType;             /* node type (element, text, attribute, etc.) */
    int top_of_stack;         /* top of the stack */
    int count;                /* number of chars copied in snprintf */
    char **stack = NULL;      /* stack to keep track of elements in the tree */

    /* Establish the reader for this metadata file */
    reader = xmlNewTextReaderFilename (metafile);
    if (reader == NULL)
    {
        sprintf (errmsg, "Setting up reader for %s", metafile);
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (ERROR);
    }

    /* Use the reader to parse the XML file, looking at each of the nodes,
       until the entire file has been parsed.  Start by reading the first
       node in the file. */
    status = xmlTextReaderRead (reader);
    while (status == 1)
    {
        /* Determine what kind of node the reader is at (element, end element,
           attribute, text/white space) and handle the information as desired */
        nodeType = xmlTextReaderNodeType (reader);
        if (nodeType == -1)
        {
            sprintf (errmsg, "Getting node type");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
        switch (nodeType)
        {
            case XML_READER_TYPE_ELEMENT:
            {  /* Node is an element (ex. <global_metadata> */
                xmlNodePtr node=NULL;
                if (doc==NULL)
                {
                    doc=xmlNewDoc (BAD_CAST "1.0");
                }

                /* Get the URI defining the namespace for this node */
                if (xmlTextReaderConstNamespaceUri (reader) != NULL)
                {
                    /* Search the namespace for a document */
                    xmlNsPtr ns = xmlSearchNs (doc, current,
                        xmlTextReaderConstNamespaceUri(reader));

                    /* Create a tree node for this element in the XML file
                       using the element name */
                    node = xmlNewNode (ns, xmlTextReaderConstName(reader));

                    /* If the namespace is empty (i.e. root) then create a
                       new namespace pointer with this node */
                    if (ns == NULL)
                    {
                        ns = xmlNewNs (node,
                            xmlTextReaderConstNamespaceUri(reader),
                            xmlTextReaderConstPrefix(reader));
                    }
                }
                else
                {
                    /* Create a tree node for this element in the XML file
                       using the element name */
                    node = xmlNewNode (0, xmlTextReaderConstName(reader));
                }

                /* Set the element as the root if appropriate otherwise add
                   it as a child to the previous element */
                if (current == NULL)
                {
                    xmlDocSetRootElement (doc, node);
                }
                else
                {
                    xmlAddChild (current, node);
                }
                current = node;

                /* If the element has attributes, then handle them */
                if (xmlTextReaderHasAttributes (reader))
                {
                    /* Get the number of attributes and then process each one */
                    int i;
                    int n_att = xmlTextReaderAttributeCount (reader);
                    for (i = 0; i < n_att; i++)
                    {
                        /* Read each attribute, obtain the name and value,
                           then add it as a property for this node in the
                           tree */
                        const xmlChar *k = NULL;
                        xmlChar *v = NULL;
                        xmlTextReaderMoveToAttributeNo (reader, i);
                        k = xmlTextReaderConstName (reader);
                        v = xmlTextReaderValue (reader);
                        if (xmlTextReaderConstNamespaceUri (reader) != NULL)
                        {
                            if (!xmlStrEqual (
                                xmlTextReaderConstNamespaceUri(reader),
                                BAD_CAST "http://www.w3.org/2000/xmlns/"))
                            {
                                /* Search the namespace for the document */
                                xmlNsPtr ns = xmlSearchNs (doc, current,
                                    xmlTextReaderConstNamespaceUri(reader));
                                if (ns == NULL)
                                {
                                    ns = xmlNewNs (node,
                                        xmlTextReaderConstNamespaceUri(reader),
                                        xmlTextReaderConstPrefix(reader));
                                }

                                /* Create a new property tagged with this
                                   namespace and carried by this node */
                                xmlNewNsProp (current, ns,
                                    xmlTextReaderConstLocalName(reader), v);
                            }
                         }
                         else
                         {
                            /* Add the attribute as a property of the node
                               in the tree */
                            xmlNewProp (current, k, v);
                         }

                         /* Free the XML value pointer */
                         xmlFree (v);
                    }

                    /* We are done with the attributes so go to the current
                       attribute node */
                    xmlTextReaderMoveToElement (reader);
                }

                /* If this is an empty element, then return to the parent */
                if (xmlTextReaderIsEmptyElement(reader))
                    current = current->parent;
                break;
            }  /* End: Node is an element */

            case XML_READER_TYPE_END_ELEMENT:
            {  /* Node is an end element (ex. </global_metadata>, so return
                  to the parent */
                current = current->parent;
                break;
            }

            case XML_READER_TYPE_TEXT:
            {  /* Node is text or white space */
                /* Read the value of the text and add it as text for the
                   node, which is then added as a child to the tree */
                const xmlChar *v = xmlTextReaderConstValue (reader);
                xmlNodePtr node = xmlNewDocText (doc, v);
                xmlAddChild (current, node);
                break;
            }
        }  /* end switch */

        /* Read the next node */
        status = xmlTextReaderRead (reader);
    }  /* end while */
    if (status != 0)
    {
        sprintf (errmsg, "Failed to parse %s", metafile);
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (ERROR);
    }

    /* If the document tree is not NULL then send in the root node of the
       tree to be parsed and read into the ARD metadata structure */
    if (doc != NULL)
    {
        /* Store the namespace for the overall metadata file */
        xmlNsPtr ns = xmlDocGetRootElement(doc)->nsDef;
        count = snprintf (ard_meta->meta_namespace,
            sizeof (ard_meta->meta_namespace), "%s", (const char *) ns->href);
        if (count < 0 || count >= sizeof (ard_meta->meta_namespace))
        {
            sprintf (errmsg, "Overflow of ard_meta->meta_namespace string");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Initialize the stack to hold the elements */
        if (init_stack (&top_of_stack, &stack))
        {
            sprintf (errmsg, "Initializing the stack.");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }
        //print_element_names (xmlDocGetRootElement (doc));

        /* Parse the XML document into our ARD metadata structure */
        if (parse_ard_xml_into_struct (xmlDocGetRootElement(doc), ard_meta,
            &top_of_stack, stack))
        {
            sprintf (errmsg, "Parsing the metadata file into the ARD metadata "
                "structure.");
            ard_error_handler (true, FUNC_NAME, errmsg);
            return (ERROR);
        }

        /* Clean up the XML document and the stack */
        xmlFreeDoc (doc);
        free_stack (&stack);
    }

    /* Free the reader and associated memory */
    xmlFreeTextReader (reader);
    xmlCleanupParser();
    xmlMemoryDump();

    return (SUCCESS);
}


/*****************************************************************************
FILE: write_ard_metadata.c
  
PURPOSE: Contains functions for writing/appending the ARD metadata files.

PROJECT:  Land Satellites Data System Science Research and Development (LSRD)
at the USGS EROS

LICENSE TYPE:  NASA Open Source Agreement Version 1.3

NOTES:
  1. The XML metadata format written via this library follows the ARD metadata
     format found in the ARD DFCB.  The schema for the ARD metadata format is
     available at
     http://espa.cr.usgs.gov/schema/ard_metadata_v1_0.xsd.
*****************************************************************************/

#include <math.h>
#include "write_ard_metadata.h"

/******************************************************************************
MODULE:  write_ard_proj_metadata

PURPOSE: Write the ARD projection metadata to the open XML file

RETURN VALUE: N/A

NOTES:
******************************************************************************/
void write_ard_proj_metadata
(
    Ard_proj_meta_t *proj_info, /* I: pointer to the projection information */
    FILE *fptr               /* I: file pointer to the open XML metadata file */
)
{
    char myproj[STR_SIZE];   /* projection type string */
    char mydatum[STR_SIZE];  /* datum string */

    /* Write the global metadata - projection information */
    switch (proj_info->proj_type)
    {
        case ARD_GCTP_GEO_PROJ: strcpy (myproj, "GEO"); break;
        case ARD_GCTP_UTM_PROJ: strcpy (myproj, "UTM"); break;
        case ARD_GCTP_ALBERS_PROJ: strcpy (myproj, "ALBERS"); break;
        case ARD_GCTP_PS_PROJ: strcpy (myproj, "PS"); break;
        case ARD_GCTP_SIN_PROJ: strcpy (myproj, "SIN"); break;
        default: strcpy (myproj, "undefined"); break;
    }

    if (proj_info->datum_type != ARD_NODATUM)
    {
        switch (proj_info->datum_type)
        {
            case ARD_WGS84: strcpy (mydatum, "WGS84"); break;
            case ARD_NAD27: strcpy (mydatum, "NAD27"); break;
            case ARD_NAD83: strcpy (mydatum, "NAD83"); break;
        }
        fprintf (fptr,
            "        <projection_information projection=\"%s\" datum=\"%s\" "
            "units=\"%s\">\n", myproj, mydatum, proj_info->units);
    }
    else
    {
        fprintf (fptr,
            "        <projection_information projection=\"%s\" units=\"%s\">\n",
            myproj, proj_info->units);
    }

    fprintf (fptr,
        "            <corner_point location=\"UL\" x=\"%lf\" y=\"%lf\"/>\n"
        "            <corner_point location=\"LR\" x=\"%lf\" y=\"%lf\"/>\n"
        "            <grid_origin>%s</grid_origin>\n",
        proj_info->ul_corner[0], proj_info->ul_corner[1],
        proj_info->lr_corner[0], proj_info->lr_corner[1],
        proj_info->grid_origin);

    /* UTM-specific parameters */
    if (proj_info->proj_type == ARD_GCTP_UTM_PROJ)
    {
        fprintf (fptr,
            "            <utm_proj_params>\n"
            "                <zone_code>%d</zone_code>\n"
            "            </utm_proj_params>\n",
            proj_info->utm_zone);
    }

    /* ALBERS-specific parameters */
    if (proj_info->proj_type == ARD_GCTP_ALBERS_PROJ)
    {
        fprintf (fptr,
            "            <albers_proj_params>\n"
            "                <standard_parallel1>%lf</standard_parallel1>\n"
            "                <standard_parallel2>%lf</standard_parallel2>\n"
            "                <central_meridian>%lf</central_meridian>\n"
            "                <origin_latitude>%lf</origin_latitude>\n"
            "                <false_easting>%lf</false_easting>\n"
            "                <false_northing>%lf</false_northing>\n"
            "            </albers_proj_params>\n",
            proj_info->standard_parallel1,
            proj_info->standard_parallel2,
            proj_info->central_meridian,
            proj_info->origin_latitude,
            proj_info->false_easting,
            proj_info->false_northing);
    }

    /* PS-specific parameters */
    if (proj_info->proj_type == ARD_GCTP_PS_PROJ)
    {
        fprintf (fptr,
            "            <ps_proj_params>\n"
            "                <longitude_pole>%lf</longitude_pole>\n"
            "                <latitude_true_scale>%lf</latitude_true_scale>\n"
            "                <false_easting>%lf</false_easting>\n"
            "                <false_northing>%lf</false_northing>\n"
            "            </ps_proj_params>\n",
            proj_info->longitude_pole,
            proj_info->latitude_true_scale,
            proj_info->false_easting,
            proj_info->false_northing);
    }

    /* SIN-specific parameters */
    if (proj_info->proj_type == ARD_GCTP_SIN_PROJ)
    {
        fprintf (fptr,
            "            <sin_proj_params>\n"
            "                <sphere_radius>%lf</sphere_radius>\n"
            "                <central_meridian>%lf</central_meridian>\n"
            "                <false_easting>%lf</false_easting>\n"
            "                <false_northing>%lf</false_northing>\n"
            "            </sin_proj_params>\n",
            proj_info->sphere_radius,
            proj_info->central_meridian,
            proj_info->false_easting,
            proj_info->false_northing);
    }

    fprintf (fptr,
        "        </projection_information>\n");
}


/******************************************************************************
MODULE:  write_ard_band_metadata

PURPOSE: Write the ARD band metadata structure to the open XML file

RETURN VALUE: N/A

NOTES:
******************************************************************************/
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
)
{
    char my_dtype[STR_SIZE]; /* data type string */
    char my_rtype[STR_SIZE]; /* resampling type string */
    int i, j;                /* looping variables */

    /* Write the bands metadata */
    if (!skip_bands_cntnr)
    {
        fprintf (fptr,
            "    <bands>\n");
    }

    /* Write the bands themselves.  Make sure the optional parameters have
       been specified and are not fill, otherwise don't write them out. */
    for (i = 0; i < nbands; i++)
    {
        switch (bmeta[i].data_type)
        {
            case ARD_INT8: strcpy (my_dtype, "INT8"); break;
            case ARD_UINT8: strcpy (my_dtype, "UINT8"); break;
            case ARD_INT16: strcpy (my_dtype, "INT16"); break;
            case ARD_UINT16: strcpy (my_dtype, "UINT16"); break;
            case ARD_INT32: strcpy (my_dtype, "INT32"); break;
            case ARD_UINT32: strcpy (my_dtype, "UINT32"); break;
            case ARD_FLOAT32: strcpy (my_dtype, "FLOAT32"); break;
            case ARD_FLOAT64: strcpy (my_dtype, "FLOAT64"); break;
            default: strcpy (my_dtype, "undefined"); break;
        }

        switch (bmeta[i].resample_method)
        {
            case ARD_CC: strcpy (my_rtype, "cubic convolution"); break;
            case ARD_NN: strcpy (my_rtype, "nearest neighbor"); break;
            case ARD_BI: strcpy (my_rtype, "bilinear"); break;
            case ARD_NONE: strcpy (my_rtype, "none"); break;
            default: strcpy (my_rtype, "undefined"); break;
        }

        /* Start with the required band attributes */
        fprintf (fptr,
            "        <band product=\"%s\" ", bmeta[i].product);

        if (strcmp (bmeta[i].source, ARD_STRING_META_FILL))
            fprintf (fptr, "source=\"%s\" ", bmeta[i].source);

        fprintf (fptr, "name=\"%s\" category=\"%s\" data_type=\"%s\"",
            bmeta[i].name, bmeta[i].category, my_dtype);

        /* If nlines is valid then assume nsamps is also valid since they go
           hand-in-hand */
        if (bmeta[i].nlines != ARD_INT_META_FILL)
            fprintf (fptr, " nlines=\"%d\" nsamps=\"%d\"",
                bmeta[i].nlines, bmeta[i].nsamps);

        /* Handle the rest of the optional attributes */
        if (bmeta[i].fill_value != ARD_INT_META_FILL)
            fprintf (fptr, " fill_value=\"%ld\"", bmeta[i].fill_value);
        if (bmeta[i].saturate_value != ARD_INT_META_FILL)
            fprintf (fptr, " saturate_value=\"%d\"", bmeta[i].saturate_value);
        if (fabs (bmeta[i].scale_factor-ARD_FLOAT_META_FILL) > ARD_EPSILON)
            fprintf (fptr, " scale_factor=\"%f\"", bmeta[i].scale_factor);
        if (fabs (bmeta[i].add_offset-ARD_FLOAT_META_FILL) > ARD_EPSILON)
            fprintf (fptr, " add_offset=\"%f\"", bmeta[i].add_offset);

        /* Close out the band and attributes */
        fprintf (fptr, ">\n");

        /* Print the required band elements */
        fprintf (fptr,
            "            <short_name>%s</short_name>\n"
            "            <long_name>%s</long_name>\n"
            "            <file_name>%s</file_name>\n"
            "            <pixel_size x=\"%g\" y=\"%g\" units=\"%s\"/>\n"
            "            <resample_method>%s</resample_method>\n"
            "            <data_units>%s</data_units>\n",
            bmeta[i].short_name, bmeta[i].long_name, bmeta[i].file_name,
            bmeta[i].pixel_size[0], bmeta[i].pixel_size[1],
            bmeta[i].pixel_units, my_rtype, bmeta[i].data_units);

        /* Print the optional band elements */
        if (fabs (bmeta[i].valid_range[0] - ARD_FLOAT_META_FILL) >
            ARD_EPSILON &&
            fabs (bmeta[i].valid_range[1] - ARD_FLOAT_META_FILL) >
            ARD_EPSILON)
        {
            fprintf (fptr,
                "            <valid_range min=\"%f\" max=\"%f\"/>\n",
                bmeta[i].valid_range[0], bmeta[i].valid_range[1]);
        }

        if (bmeta[i].nbits != ARD_INT_META_FILL && bmeta[i].nbits > 0)
        {
            fprintf (fptr,
                "            <bitmap_description>\n");
            for (j = 0; j < bmeta[i].nbits; j++)
            {
                fprintf (fptr,
                    "                <bit num=\"%d\">%s</bit>\n",
                    j, bmeta[i].bitmap_description[j]);
            }
            fprintf (fptr,
                "            </bitmap_description>\n");
        }

        if (bmeta[i].nclass != ARD_INT_META_FILL && bmeta[i].nclass > 0)
        {
            fprintf (fptr,
                "            <class_values>\n");
            for (j = 0; j < bmeta[i].nclass; j++)
            {
                fprintf (fptr,
                    "                <class num=\"%d\">%s</class>\n",
                     bmeta[i].class_values[j].class,
                     bmeta[i].class_values[j].description);
            }
            fprintf (fptr,
                "            </class_values>\n");
        }

        /* Close out the current band */
        if (strcmp (bmeta[i].app_version, ARD_STRING_META_FILL))
        {
            fprintf (fptr,
                "            <app_version>%s</app_version>\n",
                bmeta[i].app_version);
        }

        fprintf (fptr,
            "            <production_date>%s</production_date>\n"
            "        </band>\n",
            bmeta[i].production_date);
    }

    /* End bands metadata unless otherwise specified */
    if (!skip_bands_cntnr)
    {
        fprintf (fptr,
            "    </bands>\n");
    }
}


/******************************************************************************
MODULE:  write_ard_metadata

PURPOSE: Write the ARD metadata structure to the specified XML metadata file

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error writing the metadata file
SUCCESS         Successfully wrote the metadata file

NOTES:
  1. If the XML file specified already exists, it will be overwritten.
  2. Use this routine to create a new metadata file.  To append bands to an
     existing metadata file, use append_tile_bands_ard_metadata.
  3. It is recommended that validate_meta be used after writing the XML file
     to make sure the new file is valid against the ARD schema.
******************************************************************************/
int write_ard_metadata
(
    Ard_meta_t *ard_meta,      /* I: input ARD metadata structure to be written
                                     to XML */
    char *xml_file             /* I: name of the XML metadata file to be
                                     written to or overwritten */
)
{
    char FUNC_NAME[] = "write_ard_metadata";   /* function name */
    char errmsg[STR_SIZE];   /* error message */
    char myelev[STR_SIZE];   /* elevation source string */
    char mysensor[STR_SIZE]; /* sensor mode string */
    char myephem[STR_SIZE];  /* ephemeris type string */
    int i;                   /* looping variables */
    FILE *fptr = NULL;       /* file pointer to the XML metadata file */
    Ard_global_tile_meta_t *tile_gmeta = &ard_meta->tile_meta.tile_global;
                             /* ptr to tile-based global metadata structure */
    Ard_global_scene_meta_t *scene_gmeta = NULL;
                             /* ptr to scene-based global metadata structure */

    /* Open the metadata XML file for write or rewrite privelages */
    fptr = fopen (xml_file, "w");
    if (fptr == NULL)
    {
        sprintf (errmsg, "Opening %s for write access.", xml_file);
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (ERROR);
    }

    /* Write the overall header */
    fprintf (fptr,
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\n"
        "<ard_metadata version=\"%s\"\n"
        "xmlns=\"%s\"\n"
        "xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
        "xsi:schemaLocation=\"%s %s\">\n\n", ARD_SCHEMA_VERSION, ARD_NS,
        ARD_SCHEMA_LOCATION, ARD_SCHEMA);

    /* Write the tile metadata header */
    fprintf (fptr,
        "<tile_metadata>\n");

    /* Write the global tile metadata */
    fprintf (fptr,
        "    <global_metadata>\n"
        "        <data_provider>%s</data_provider>\n"
        "        <acquisition_date>%s</acquisition_date>\n"
        "        <product_id>%s</product_id>\n"
        "        <production_date>%s</production_date>\n"
        "        <bounding_coordinates>\n"
        "            <west>%lf</west>\n"
        "            <east>%lf</east>\n"
        "            <north>%lf</north>\n"
        "            <south>%lf</south>\n"
        "        </bounding_coordinates>\n",
        tile_gmeta->data_provider, tile_gmeta->acquisition_date,
        tile_gmeta->product_id, tile_gmeta->production_date,
        tile_gmeta->bounding_coords[ARD_WEST],
        tile_gmeta->bounding_coords[ARD_EAST],
        tile_gmeta->bounding_coords[ARD_NORTH],
        tile_gmeta->bounding_coords[ARD_SOUTH]);

    /* Write the projection information */
    write_ard_proj_metadata (&tile_gmeta->proj_info, fptr);

    /* Continue with the global metadata */
    fprintf (fptr,
        "        <orientation_angle>%f</orientation_angle>\n"
        "        <tile_grid h=\"%03d\" v=\"%03d\"/>\n"
        "        <scene_count>%d</scene_count>\n"
        "        <cloud_cover>%f</cloud_cover>\n"
        "        <cloud_shadow>%f</cloud_shadow>\n"
        "        <snow_ice>%f</snow_ice>\n"
        "        <fill>%f</fill>\n",
        tile_gmeta->orientation_angle, tile_gmeta->htile, tile_gmeta->vtile,
        tile_gmeta->scene_count, tile_gmeta->cloud_cover,
        tile_gmeta->cloud_shadow, tile_gmeta->snow_ice, tile_gmeta->fill);

    /* End global tile metadata */
    fprintf (fptr,
        "    </global_metadata>\n\n");

    /* Write the tile-based band metadata and close the bands */
    write_ard_band_metadata (ard_meta->tile_meta.nbands,
        ard_meta->tile_meta.band, fptr, false);

    /* End tile metadata */
    fprintf (fptr,
        "</tile_metadata>\n");

    /* Loop through each scene in the overall tile and write the scene-based
       metadata */
    for (i = 0; i < ard_meta->nscenes; i++)
    {
        /* Get the global metadata for the current scene */
        scene_gmeta = &ard_meta->scene_meta[i].scene_global;

        /* Write the scene metadata header */
        fprintf (fptr,
            "\n<scene_metadata>\n"
            "    <index>%d</index>\n", i+1);

        /* Get the elevation source type */
        switch (scene_gmeta->elevation_src)
        {
            case ARD_NED: strcpy (myelev, "NED"); break;
            case ARD_SRTM: strcpy (myelev, "SRTM"); break;
            case ARD_GTOPO30: strcpy (myelev, "GTOPO30"); break;
            case ARD_GLS2000: strcpy (myelev, "GLS2000"); break;
            case ARD_RAMP: strcpy (myelev, "RAMP"); break;
            default: strcpy (myelev, "undefined"); break;
        }

        switch (scene_gmeta->sensor_mode)
        {
            case ARD_SAM: strcpy (mysensor, "SAM"); break;
            case ARD_BUMPER: strcpy (mysensor, "BUMPER"); break;
            default: strcpy (mysensor, "undefined"); break;
        }

        switch (scene_gmeta->ephemeris_type)
        {
            case ARD_DEFINITIVE: strcpy (myephem, "DEFINITIVE"); break;
            case ARD_PREDICTIVE: strcpy (myephem, "PREDICTIVE"); break;
            default: strcpy (myephem, "undefined"); break;
        }

        /* Write the global scene metadata */
        fprintf (fptr,
            "    <global_metadata>\n"
            "        <data_provider>%s</data_provider>\n"
            "        <satellite>%s</satellite>\n"
            "        <instrument>%s</instrument>\n"
            "        <acquisition_date>%s</acquisition_date>\n"
            "        <scene_center_time>%s</scene_center_time>\n"
            "        <level1_production_date>%s</level1_production_date>\n"
            "        <wrs system=\"%d\" path=\"%d\" row=\"%d\"/>\n"
            "        <request_id>%s</request_id>\n"
            "        <scene_id>%s</scene_id>\n"
            "        <product_id>%s</product_id>\n"
            "        <elevation_source>%s</elevation_source>\n"
            "        <sensor_mode>%s</sensor_mode>\n"
            "        <ephemeris_type>%s</ephemeris_type>\n"
            "        <cpf_name>%s</cpf_name>\n"
            "        <lpgs_metadata_file>%s</lpgs_metadata_file>\n"
            "        <geometric_rmse_model>%f</geometric_rmse_model>\n"
            "        <geometric_rmse_model_x>%f</geometric_rmse_model_x>\n"
            "        <geometric_rmse_model_y>%f</geometric_rmse_model_y>\n"
            "    </global_metadata>\n\n",
            scene_gmeta->data_provider, scene_gmeta->satellite,
            scene_gmeta->instrument, scene_gmeta->acquisition_date,
            scene_gmeta->scene_center_time, scene_gmeta->level1_production_date,
            scene_gmeta->wrs_system, scene_gmeta->wrs_path,
            scene_gmeta->wrs_row, scene_gmeta->request_id,
            scene_gmeta->scene_id, scene_gmeta->product_id, myelev, mysensor,
            myephem, scene_gmeta->cpf_name, scene_gmeta->lpgs_metadata_file,
            scene_gmeta->geometric_rmse_model,
            scene_gmeta->geometric_rmse_model_x,
            scene_gmeta->geometric_rmse_model_y);

        /* Write the scene-based band metadata and close the bands */
        write_ard_band_metadata (ard_meta->scene_meta[i].nbands,
            ard_meta->scene_meta[i].band, fptr, false);

        /* End scene metadata */
        fprintf (fptr,
            "</scene_metadata>\n");
    } /* end nscenes */

    /* End of the overall ARD metadata container */
    fprintf (fptr,
        "</ard_metadata>\n");

    /* Close the XML file */
    fclose (fptr);

    /* Successful generation */
    return (SUCCESS);
}


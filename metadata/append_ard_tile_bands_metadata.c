/*****************************************************************************
FILE: append_ard_tile_bands_metadata
  
PURPOSE: Contains functions for writing/appending bands to the tile metadata
within the ARD metadata file.

PROJECT:  Land Satellites Data System Science Research and Development (LSRD)
at the USGS EROS

LICENSE TYPE:  NASA Open Source Agreement Version 1.3

NOTES:
  1. The XML metadata format written via this library follows the ARD metadata
     format found in the ARD DFCB.  The schema for the ARD metadata format is
     available at
     http://espa.cr.usgs.gov/schema/ard/ard_metadata_v1_0.xsd.
*****************************************************************************/

#include <math.h>
#include "write_ard_metadata.h"

/******************************************************************************
MODULE:  append_ard_tile_bands_metadata

PURPOSE: Write the ARD metadata structure to the specified XML metadata file
and append the specified bands to the tile_metadata, after the existing bands
in the tile_metadata.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error writing/appending the metadata file
SUCCESS         Successfully wrote the metadata file

NOTES:
  1. If the XML file specified already exists, it will be overwritten.
  2. Use this routine to append bands to an existing metadata file. This
     routine will basically overwrite the existing metadata file.
  3. It is recommended that validate_meta be used after writing the XML file
     to make sure the new file is valid against the ARD schema.
******************************************************************************/
int append_ard_tile_bands_metadata
(
    Ard_meta_t *ard_meta,      /* I: original ARD metadata structure to be
                                     appended to */
    int nbands_append,         /* I: number of bands to be appended */
    Ard_band_meta_t *bmeta,    /* I: pointer to the array of tile bands
                                     to be appended to the XML metadata */
    char *xml_file             /* I: name of the XML metadata file to be
                                     written to or overwritten */
)
{
    char FUNC_NAME[] = "append_tile_bands_ard_metadata";   /* function name */
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
        "        <satellite>%s</satellite>\n"
        "        <instrument>%s</instrument>\n"
        "        <level1_collection>%s</level1_collection>\n"
        "        <ard_version>%s</ard_version>\n"
        "        <region>%s</region>\n"
        "        <acquisition_date>%s</acquisition_date>\n"
        "        <product_id>%s</product_id>\n"
        "        <production_date>%s</production_date>\n"
        "        <bounding_coordinates>\n"
        "            <west>%lf</west>\n"
        "            <east>%lf</east>\n"
        "            <north>%lf</north>\n"
        "            <south>%lf</south>\n"
        "        </bounding_coordinates>\n",
        tile_gmeta->data_provider, tile_gmeta->satellite,
        tile_gmeta->instrument, tile_gmeta->level1_collection,
        tile_gmeta->ard_version, tile_gmeta->region,
        tile_gmeta->acquisition_date, tile_gmeta->product_id,
        tile_gmeta->production_date,
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

    /* Handle the bands container information directly since we are appending */
    fprintf (fptr,
        "    <bands>\n");

    /* Write the original tile-based band metadata, but don't write the bands
       container information */
    write_ard_band_metadata (ard_meta->tile_meta.nbands,
        ard_meta->tile_meta.band, fptr, true);

    /* Write the new tile-based band metadata, but don't write the bands
       container information */
    write_ard_band_metadata (nbands_append, bmeta, fptr, true);

    /* End bands metadata */
    fprintf (fptr,
        "    </bands>\n");

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
            "        <elevation_source>%s</elevation_source>\n",
            scene_gmeta->data_provider, scene_gmeta->satellite,
            scene_gmeta->instrument, scene_gmeta->acquisition_date,
            scene_gmeta->scene_center_time, scene_gmeta->level1_production_date,
            scene_gmeta->wrs_system, scene_gmeta->wrs_path,
            scene_gmeta->wrs_row, scene_gmeta->request_id,
            scene_gmeta->scene_id, scene_gmeta->product_id, myelev);

        if (strcmp (mysensor, "undefined"))
            fprintf (fptr,
                "        <sensor_mode>%s</sensor_mode>\n", mysensor);

        if (strcmp (myephem, "undefined"))
            fprintf (fptr,
                "        <ephemeris_type>%s</ephemeris_type>\n", myephem);

        fprintf (fptr,
            "        <cpf_name>%s</cpf_name>\n"
            "        <lpgs_metadata_file>%s</lpgs_metadata_file>\n",
            scene_gmeta->cpf_name, scene_gmeta->lpgs_metadata_file);

        if (fabs (scene_gmeta->geometric_rmse_model - ARD_FLOAT_META_FILL) >
            ARD_EPSILON)
        {
            fprintf (fptr,
                "        <geometric_rmse_model>%f</geometric_rmse_model>\n",
                scene_gmeta->geometric_rmse_model);
        }

        if (fabs (scene_gmeta->geometric_rmse_model_x - ARD_FLOAT_META_FILL) >
            ARD_EPSILON)
        {
            fprintf (fptr,
                "        <geometric_rmse_model_x>%f</geometric_rmse_model_x>\n",
                scene_gmeta->geometric_rmse_model_x);
        }

        if (fabs (scene_gmeta->geometric_rmse_model_y - ARD_FLOAT_META_FILL) >
            ARD_EPSILON)
        {
            fprintf (fptr,
                "        <geometric_rmse_model_y>%f</geometric_rmse_model_y>\n",
                scene_gmeta->geometric_rmse_model_y);
        }

        fprintf (fptr,
            "    </global_metadata>\n\n");

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


/*****************************************************************************
FILE: ard_tiff_io.c
  
PURPOSE: Contains functions for opening/closing ARD Tiff files as well as
reading/writing to Tiff files.  The Tiff-level read/writes will be
tile-oriented I/O.

PROJECT:  Land Satellites Data System Science Research and Development (LSRD)
at the USGS EROS

LICENSE TYPE:  NASA Open Source Agreement Version 1.3

NOTES:
  1. The XML metadata format read via this library follows the ARD metadata
     format found in the ARD DFCB.  The schema for the ARD metadata format is
     available at
     https://landsat.usgs.gov/ard/ard_metadata_vX_X.xsd
*****************************************************************************/

#include "ard_tiff_io.h"

/* define the read/write formats to be used for opening a file */
/* TIFF_READ_FORMAT, TIFF_WRITE_FORMAT, TIFF_READ_WRITE_FORMAT */
const char ard_tiff_format[][3] = {"r", "w", "a"};


/******************************************************************************
MODULE: ard_set_geotiff_datum

PURPOSE: Sets the GeoTiff tags for the datum used

RETURN VALUE:
Type = int
ERROR        An unknown datum was specified
SUCCESS      Writing of datum geolocation tags was successful

NOTES:
*****************************************************************************/
int ard_set_geotiff_datum
(
    GTIF *gtif,        /* I: GeoTiff file pointer */
    int datum_type,    /* I: datum type (see ARD_* in gctp_defines.h */
    char *citation     /* I/O: string for geo citation tag (updated) */
)
{
    char FUNC_NAME[] = "ard_set_geotiff_datum"; /* function name */
    char errmsg[STR_SIZE];      /* error message */

    switch (datum_type)
    {
        case (ARD_WGS84):
            strcat (citation, "WGS 1984");
            GTIFKeySet (gtif, GeogGeodeticDatumGeoKey, TYPE_SHORT, 1,
                Datum_WGS84);
            GTIFKeySet (gtif, GeographicTypeGeoKey, TYPE_SHORT, 1,
                GCS_WGS_84);
            break;

        case (ARD_NAD83):
            strcat (citation, "North American Datum 1983");
            GTIFKeySet (gtif, GeogGeodeticDatumGeoKey, TYPE_SHORT, 1,
                Datum_North_American_Datum_1983);
            GTIFKeySet (gtif, GeographicTypeGeoKey, TYPE_SHORT, 1,
                GCS_NAD83);
            break;

        case (ARD_NAD27):
            strcat (citation, "North American Datum 1927");
            GTIFKeySet (gtif, GeogGeodeticDatumGeoKey, TYPE_SHORT, 1,
                Datum_North_American_Datum_1927);
            GTIFKeySet (gtif, GeographicTypeGeoKey, TYPE_SHORT, 1,
                GCS_NAD27);
            break;

        default:
            sprintf (errmsg, "Unsupported datum %d", datum_type);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return ERROR;
    }

    return SUCCESS;
}


/******************************************************************************
MODULE: ard_set_geotiff_tags

PURPOSE: Sets the GeoTiff tags for the current Tiff and GeoTiff pointers

RETURN VALUE:
Type = int
ERROR        An error occurred writing geolocation tags to the Tiff and
             GeoTiff files
SUCCESS      Writing of geolocation tags was successful

NOTES:
*****************************************************************************/
int ard_set_geotiff_tags
(
    TIFF *tif,                   /* I: pointer to Tiff file */
    Ard_band_meta_t *bmeta,      /* I: band metadata */
    Ard_proj_meta_t *proj_info   /* I: global projection information */
)
{
    char FUNC_NAME[] = "ard_set_geotiff_tags"; /* function name */
    char errmsg[STR_SIZE];      /* error message */
    char nors;                  /* north or south UTM zone */
    char citation[STR_SIZE];    /* string for geo citation tag */
    char gt_citation_text[STR_SIZE];  /* UTM GTCitationText */
    double tiepoints[6];        /* corner tie points for projection */
    double pixelscale[3];       /* same as pixel size */
    int nors_set;               /* north or south UTM zone codes */
    int zone;                   /* abs UTM zone number */
    int linear_value = Linear_Meter;  /* default linear value */
    GTIF *gtif = NULL;          /* GeoTiff file pointer */

    int UTMWGS84_ZoneCodes[2][60] = { /* zone code for UTM WGS84 projections */
        {PCS_WGS84_UTM_zone_1N,
         PCS_WGS84_UTM_zone_2N,
         PCS_WGS84_UTM_zone_3N,
         PCS_WGS84_UTM_zone_4N,
         PCS_WGS84_UTM_zone_5N,
         PCS_WGS84_UTM_zone_6N,
         PCS_WGS84_UTM_zone_7N,
         PCS_WGS84_UTM_zone_8N,
         PCS_WGS84_UTM_zone_9N,
         PCS_WGS84_UTM_zone_10N,
         PCS_WGS84_UTM_zone_11N,
         PCS_WGS84_UTM_zone_12N,
         PCS_WGS84_UTM_zone_13N,
         PCS_WGS84_UTM_zone_14N,
         PCS_WGS84_UTM_zone_15N,
         PCS_WGS84_UTM_zone_16N,
         PCS_WGS84_UTM_zone_17N,
         PCS_WGS84_UTM_zone_18N,
         PCS_WGS84_UTM_zone_19N,
         PCS_WGS84_UTM_zone_20N,
         PCS_WGS84_UTM_zone_21N,
         PCS_WGS84_UTM_zone_22N,
         PCS_WGS84_UTM_zone_23N,
         PCS_WGS84_UTM_zone_24N,
         PCS_WGS84_UTM_zone_25N,
         PCS_WGS84_UTM_zone_26N,
         PCS_WGS84_UTM_zone_27N,
         PCS_WGS84_UTM_zone_28N,
         PCS_WGS84_UTM_zone_29N,
         PCS_WGS84_UTM_zone_30N,
         PCS_WGS84_UTM_zone_31N,
         PCS_WGS84_UTM_zone_32N,
         PCS_WGS84_UTM_zone_33N,
         PCS_WGS84_UTM_zone_34N,
         PCS_WGS84_UTM_zone_35N,
         PCS_WGS84_UTM_zone_36N,
         PCS_WGS84_UTM_zone_37N,
         PCS_WGS84_UTM_zone_38N,
         PCS_WGS84_UTM_zone_39N,
         PCS_WGS84_UTM_zone_40N,
         PCS_WGS84_UTM_zone_41N,
         PCS_WGS84_UTM_zone_42N,
         PCS_WGS84_UTM_zone_43N,
         PCS_WGS84_UTM_zone_44N,
         PCS_WGS84_UTM_zone_45N,
         PCS_WGS84_UTM_zone_46N,
         PCS_WGS84_UTM_zone_47N,
         PCS_WGS84_UTM_zone_48N,
         PCS_WGS84_UTM_zone_49N,
         PCS_WGS84_UTM_zone_50N,
         PCS_WGS84_UTM_zone_51N,
         PCS_WGS84_UTM_zone_52N,
         PCS_WGS84_UTM_zone_53N,
         PCS_WGS84_UTM_zone_54N,
         PCS_WGS84_UTM_zone_55N,
         PCS_WGS84_UTM_zone_56N,
         PCS_WGS84_UTM_zone_57N,
         PCS_WGS84_UTM_zone_58N,
         PCS_WGS84_UTM_zone_59N,
         PCS_WGS84_UTM_zone_60N},
        {PCS_WGS84_UTM_zone_1S,
         PCS_WGS84_UTM_zone_2S,
         PCS_WGS84_UTM_zone_3S,
         PCS_WGS84_UTM_zone_4S,
         PCS_WGS84_UTM_zone_5S,
         PCS_WGS84_UTM_zone_6S,
         PCS_WGS84_UTM_zone_7S,
         PCS_WGS84_UTM_zone_8S,
         PCS_WGS84_UTM_zone_9S,
         PCS_WGS84_UTM_zone_10S,
         PCS_WGS84_UTM_zone_11S,
         PCS_WGS84_UTM_zone_12S,
         PCS_WGS84_UTM_zone_13S,
         PCS_WGS84_UTM_zone_14S,
         PCS_WGS84_UTM_zone_15S,
         PCS_WGS84_UTM_zone_16S,
         PCS_WGS84_UTM_zone_17S,
         PCS_WGS84_UTM_zone_18S,
         PCS_WGS84_UTM_zone_19S,
         PCS_WGS84_UTM_zone_20S,
         PCS_WGS84_UTM_zone_21S,
         PCS_WGS84_UTM_zone_22S,
         PCS_WGS84_UTM_zone_23S,
         PCS_WGS84_UTM_zone_24S,
         PCS_WGS84_UTM_zone_25S,
         PCS_WGS84_UTM_zone_26S,
         PCS_WGS84_UTM_zone_27S,
         PCS_WGS84_UTM_zone_28S,
         PCS_WGS84_UTM_zone_29S,
         PCS_WGS84_UTM_zone_30S,
         PCS_WGS84_UTM_zone_31S,
         PCS_WGS84_UTM_zone_32S,
         PCS_WGS84_UTM_zone_33S,
         PCS_WGS84_UTM_zone_34S,
         PCS_WGS84_UTM_zone_35S,
         PCS_WGS84_UTM_zone_36S,
         PCS_WGS84_UTM_zone_37S,
         PCS_WGS84_UTM_zone_38S,
         PCS_WGS84_UTM_zone_39S,
         PCS_WGS84_UTM_zone_40S,
         PCS_WGS84_UTM_zone_41S,
         PCS_WGS84_UTM_zone_42S,
         PCS_WGS84_UTM_zone_43S,
         PCS_WGS84_UTM_zone_44S,
         PCS_WGS84_UTM_zone_45S,
         PCS_WGS84_UTM_zone_46S,
         PCS_WGS84_UTM_zone_47S,
         PCS_WGS84_UTM_zone_48S,
         PCS_WGS84_UTM_zone_49S,
         PCS_WGS84_UTM_zone_50S,
         PCS_WGS84_UTM_zone_51S,
         PCS_WGS84_UTM_zone_52S,
         PCS_WGS84_UTM_zone_53S,
         PCS_WGS84_UTM_zone_54S,
         PCS_WGS84_UTM_zone_55S,
         PCS_WGS84_UTM_zone_56S,
         PCS_WGS84_UTM_zone_57S,
         PCS_WGS84_UTM_zone_58S,
         PCS_WGS84_UTM_zone_59S,
         PCS_WGS84_UTM_zone_60S}
    };     

    int UTMNAD27_ZoneCodes[] =      /* zone code for UTM NAD27 projections */
        {0,
         0,
         PCS_NAD27_UTM_zone_3N,
         PCS_NAD27_UTM_zone_4N,
         PCS_NAD27_UTM_zone_5N,
         PCS_NAD27_UTM_zone_6N,
         PCS_NAD27_UTM_zone_7N,
         PCS_NAD27_UTM_zone_8N,
         PCS_NAD27_UTM_zone_9N,
         PCS_NAD27_UTM_zone_10N,
         PCS_NAD27_UTM_zone_11N,
         PCS_NAD27_UTM_zone_12N,
         PCS_NAD27_UTM_zone_13N,
         PCS_NAD27_UTM_zone_14N,
         PCS_NAD27_UTM_zone_15N,
         PCS_NAD27_UTM_zone_16N,
         PCS_NAD27_UTM_zone_17N,
         PCS_NAD27_UTM_zone_18N,
         PCS_NAD27_UTM_zone_19N,
         PCS_NAD27_UTM_zone_20N,
         PCS_NAD27_UTM_zone_21N,
         PCS_NAD27_UTM_zone_22N};

    int UTMNAD83_ZoneCodes[] =      /* zone code for UTM NAD83 projections */
        {0,
         0,
         PCS_NAD83_UTM_zone_3N,
         PCS_NAD83_UTM_zone_4N,
         PCS_NAD83_UTM_zone_5N,
         PCS_NAD83_UTM_zone_6N,
         PCS_NAD83_UTM_zone_7N,
         PCS_NAD83_UTM_zone_8N,
         PCS_NAD83_UTM_zone_9N,
         PCS_NAD83_UTM_zone_10N,
         PCS_NAD83_UTM_zone_11N,
         PCS_NAD83_UTM_zone_12N,
         PCS_NAD83_UTM_zone_13N,
         PCS_NAD83_UTM_zone_14N,
         PCS_NAD83_UTM_zone_15N,
         PCS_NAD83_UTM_zone_16N,
         PCS_NAD83_UTM_zone_17N,
         PCS_NAD83_UTM_zone_18N,
         PCS_NAD83_UTM_zone_19N,
         PCS_NAD83_UTM_zone_20N,
         PCS_NAD83_UTM_zone_21N,
         PCS_NAD83_UTM_zone_22N,
         PCS_NAD83_UTM_zone_23N};

    /* Handle the Tiff geolocation tags */
    /* UL corner
       NOTE: according to the Geotiff documentation, only one tiepoint
       (the UL corner) is specified. */
    /* Since we are using RasterPixelIsPoint for the RasterTypeGeoKey, the
       UL corner point needs to be the center of the pixel */
    tiepoints[0] = 0.0;
    tiepoints[1] = 0.0;
    tiepoints[2] = 0.0;
    tiepoints[5] = 0.0;

    if (strcmp (proj_info->grid_origin, "CENTER"))
    {  /* projection corners represent center of the pixel */
        tiepoints[3] = proj_info->ul_corner[0];
        tiepoints[4] = proj_info->ul_corner[1];
    }
    else
    {  /* projection corners represent UL corner of the pixel */
        tiepoints[3] = proj_info->ul_corner[0] + 0.5 * bmeta->pixel_size[0];
        tiepoints[4] = proj_info->ul_corner[1] - 0.5 * bmeta->pixel_size[1];
    }
    TIFFSetField (tif, TIFFTAG_GEOTIEPOINTS, 6, tiepoints);

    /* Pixel size */
    pixelscale[0] = bmeta->pixel_size[0];
    pixelscale[1] = bmeta->pixel_size[1];
    pixelscale[2] = 0.0;
    TIFFSetField (tif, TIFFTAG_GEOPIXELSCALE, 3, pixelscale);

    /* Set up a GeoTiff file descriptor */
    gtif = GTIFNew (tif);
    if (gtif == NULL)
    {
        sprintf (errmsg, "Unable to initialize the GeoTiff file descriptor");
        ard_error_handler (true, FUNC_NAME, errmsg);
        return ERROR;
    }

    /* Handle the GeoTiff geolocation tags */
    switch (proj_info->proj_type)
    {
        case (ARD_GCTP_GEO_PROJ):
            GTIFKeySet (gtif, GTModelTypeGeoKey, TYPE_SHORT, 1,
                ModelTypeGeographic);
            GTIFKeySet (gtif, GTRasterTypeGeoKey, TYPE_SHORT, 1,
                RasterPixelIsPoint);
            GTIFKeySet (gtif, GeogAngularUnitsGeoKey, TYPE_SHORT, 1,
                Angular_Degree);
            strcpy (citation, "Geographic (Longitude, Latitude) ");
            ard_set_geotiff_datum (gtif, proj_info->datum_type, citation);
            GTIFKeySet (gtif, GTCitationGeoKey, TYPE_ASCII, 1, citation);
            break;

        case (ARD_GCTP_UTM_PROJ):
            if (proj_info->utm_zone < 0)     /* South */
            {
                nors = 'S';
                nors_set = 1;
                zone = abs (proj_info->utm_zone);
            }
            else                             /* North */
            {
                nors = 'N';
                nors_set = 0;
                zone = proj_info->utm_zone;
            }

            if (proj_info->datum_type == ARD_WGS84) /* WGS84 */
            {
                sprintf (gt_citation_text, "UTM Zone %d %c with WGS84", zone,
                    nors);
                zone -= 1; /* zero base */

                GTIFKeySet (gtif, GTModelTypeGeoKey, TYPE_SHORT, 1,
                    ModelTypeProjected);
                GTIFKeySet (gtif, GTRasterTypeGeoKey, TYPE_SHORT, 1,
                    RasterPixelIsPoint);
                GTIFKeySet (gtif, GTCitationGeoKey, TYPE_ASCII, 0,
                    gt_citation_text);
                GTIFKeySet (gtif, GeogLinearUnitsGeoKey, TYPE_SHORT, 1,
                    linear_value);
                GTIFKeySet (gtif, GeogAngularUnitsGeoKey, TYPE_SHORT, 1,
                    Angular_Degree);
                GTIFKeySet (gtif, ProjectedCSTypeGeoKey, TYPE_SHORT, 1,
                    UTMWGS84_ZoneCodes[nors_set][zone]);
            }
            else if (proj_info->datum_type == ARD_NAD27 &&
                    (zone >= 3 && zone <= 22) &&
                     nors == 'N') /* NAD27 (only valid are 3N to 22N) */
            {
                sprintf (gt_citation_text, "UTM Zone %d %c with NAD27", zone,
                    nors);
                zone -= 1; /* zero base */

                GTIFKeySet (gtif, GTModelTypeGeoKey, TYPE_SHORT, 1,
                    ModelTypeProjected);
                GTIFKeySet (gtif, GTRasterTypeGeoKey, TYPE_SHORT, 1,
                    RasterPixelIsPoint);
                GTIFKeySet (gtif, GTCitationGeoKey, TYPE_ASCII, 0,
                    gt_citation_text);
                GTIFKeySet (gtif, GeogLinearUnitsGeoKey, TYPE_SHORT, 1,
                    linear_value);
                GTIFKeySet (gtif, GeogAngularUnitsGeoKey, TYPE_SHORT, 1,
                    Angular_Degree);
                GTIFKeySet (gtif, ProjectedCSTypeGeoKey, TYPE_SHORT, 1,
                    UTMNAD27_ZoneCodes[zone]);
            }
            else if (proj_info->datum_type == ARD_NAD83 &&
                    (zone >= 3 && zone <= 23) &&
                     nors == 'N') /* NAD83 (only valid are 3N to 23N) */
            {
                sprintf (gt_citation_text, "UTM Zone %d %c with NAD83", zone,
                    nors);
                zone -= 1; /* zero base */

                GTIFKeySet (gtif, GTModelTypeGeoKey, TYPE_SHORT, 1,
                    ModelTypeProjected);
                GTIFKeySet (gtif, GTRasterTypeGeoKey, TYPE_SHORT, 1,
                    RasterPixelIsPoint);
                GTIFKeySet (gtif, GTCitationGeoKey, TYPE_ASCII, 0,
                    gt_citation_text);
                GTIFKeySet (gtif, GeogLinearUnitsGeoKey, TYPE_SHORT, 1,
                    linear_value);
                GTIFKeySet (gtif, GeogAngularUnitsGeoKey, TYPE_SHORT, 1,
                    Angular_Degree);
                GTIFKeySet (gtif, ProjectedCSTypeGeoKey, TYPE_SHORT, 1,
                    UTMNAD83_ZoneCodes[zone]);
            }
            break;

        case (ARD_GCTP_ALBERS_PROJ):
            GTIFKeySet (gtif, ProjCoordTransGeoKey, TYPE_SHORT, 1,
                CT_AlbersEqualArea);
            GTIFKeySet (gtif, GTModelTypeGeoKey, TYPE_SHORT, 1,
                ModelTypeProjected);
            GTIFKeySet (gtif, GTRasterTypeGeoKey, TYPE_SHORT, 1,
                RasterPixelIsPoint);
            strcpy (citation, "Albers|");
            ard_set_geotiff_datum (gtif, proj_info->datum_type, citation);
            GTIFKeySet (gtif, GTCitationGeoKey, TYPE_ASCII, 0, citation);
            GTIFKeySet (gtif, GeogLinearUnitsGeoKey, TYPE_SHORT, 1,
                linear_value);
            GTIFKeySet (gtif, GeogAngularUnitsGeoKey, TYPE_SHORT, 1,
                Angular_Degree);
            GTIFKeySet (gtif, ProjectedCSTypeGeoKey, TYPE_SHORT, 1,
                KvUserDefined);
            GTIFKeySet (gtif, ProjectionGeoKey, TYPE_SHORT, 1, KvUserDefined);
            GTIFKeySet (gtif, ProjLinearUnitsGeoKey, TYPE_SHORT, 1,
                linear_value);
            GTIFKeySet (gtif, ProjStdParallel1GeoKey, TYPE_DOUBLE, 1,
                proj_info->standard_parallel1);
            GTIFKeySet (gtif, ProjStdParallel2GeoKey, TYPE_DOUBLE, 1,
                proj_info->standard_parallel2);
            GTIFKeySet (gtif, ProjNatOriginLongGeoKey, TYPE_DOUBLE, 1,
                proj_info->central_meridian);
            GTIFKeySet (gtif, ProjNatOriginLatGeoKey, TYPE_DOUBLE, 1,
                proj_info->origin_latitude);
            GTIFKeySet (gtif, ProjFalseEastingGeoKey, TYPE_DOUBLE, 1,
                proj_info->false_easting);
            GTIFKeySet (gtif, ProjFalseNorthingGeoKey, TYPE_DOUBLE, 1,
                proj_info->false_northing);
            GTIFKeySet (gtif, ProjFalseOriginLongGeoKey, TYPE_DOUBLE, 1,
                (double) 0.0);
            GTIFKeySet (gtif, ProjFalseOriginLatGeoKey, TYPE_DOUBLE, 1,
                (double) 0.0);
            break;

        case (ARD_GCTP_PS_PROJ):
            GTIFKeySet (gtif, ProjCoordTransGeoKey, TYPE_SHORT, 1,
                CT_PolarStereographic);
            GTIFKeySet (gtif, GTModelTypeGeoKey, TYPE_SHORT, 1,
                ModelTypeProjected);
            GTIFKeySet (gtif, GTRasterTypeGeoKey, TYPE_SHORT, 1,
                RasterPixelIsPoint);
            strcpy (citation, "PS|");
            ard_set_geotiff_datum (gtif, proj_info->datum_type, citation);
            GTIFKeySet (gtif, GTCitationGeoKey, TYPE_ASCII, 0, citation);
            GTIFKeySet (gtif, GeogLinearUnitsGeoKey, TYPE_SHORT, 1,
                linear_value);
            GTIFKeySet (gtif, GeogAngularUnitsGeoKey, TYPE_SHORT, 1,
                Angular_Degree);
            GTIFKeySet (gtif, ProjectedCSTypeGeoKey, TYPE_SHORT, 1,
                KvUserDefined);
            GTIFKeySet (gtif, ProjectionGeoKey, TYPE_SHORT, 1, KvUserDefined);
            GTIFKeySet (gtif, ProjLinearUnitsGeoKey, TYPE_SHORT, 1,
                linear_value);
            GTIFKeySet (gtif, ProjStraightVertPoleLongGeoKey, TYPE_DOUBLE, 1,
                proj_info->longitude_pole);
            GTIFKeySet (gtif, ProjNatOriginLatGeoKey, TYPE_DOUBLE, 1,
                proj_info->latitude_true_scale);
            GTIFKeySet (gtif, ProjFalseEastingGeoKey, TYPE_DOUBLE, 1,
                proj_info->false_easting);
            GTIFKeySet (gtif, ProjFalseNorthingGeoKey, TYPE_DOUBLE, 1,
                proj_info->false_northing);
            break;

        case (ARD_GCTP_SIN_PROJ):
            GTIFKeySet (gtif, ProjCoordTransGeoKey, TYPE_SHORT, 1,
                CT_Sinusoidal);
            GTIFKeySet (gtif, GTModelTypeGeoKey, TYPE_SHORT, 1,
                ModelTypeProjected );
            GTIFKeySet (gtif, GTRasterTypeGeoKey, TYPE_SHORT, 1,
                RasterPixelIsPoint);
            strcpy( citation, "SINUSOIDAL|" );
            ard_set_geotiff_datum (gtif, proj_info->datum_type, citation);
            GTIFKeySet (gtif, GTCitationGeoKey, TYPE_ASCII, 0, citation);
            GTIFKeySet (gtif, GeogLinearUnitsGeoKey, TYPE_SHORT, 1,
                linear_value);
            GTIFKeySet (gtif, GeogAngularUnitsGeoKey, TYPE_SHORT, 1,
                Angular_Degree);
            GTIFKeySet (gtif, ProjectedCSTypeGeoKey, TYPE_SHORT, 1,
                KvUserDefined);
            GTIFKeySet (gtif, ProjLinearUnitsGeoKey, TYPE_SHORT, 1,
                linear_value);
            GTIFKeySet (gtif, ProjNatOriginLongGeoKey, TYPE_DOUBLE, 1,
                proj_info->central_meridian);
            GTIFKeySet (gtif, ProjFalseEastingGeoKey, TYPE_DOUBLE, 1,
                proj_info->false_easting);
            GTIFKeySet (gtif, ProjFalseNorthingGeoKey, TYPE_DOUBLE, 1,
                proj_info->false_northing);
            break;

        default:
            sprintf (errmsg, "Unsupported projection type %d.",
                proj_info->proj_type);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return ERROR;
    }

    /* Write the GeoTiff tags and close the GeoTiff file descriptor.  Keys
       are ultimately written when the Tiff file pointer is closed. */
    GTIFWriteKeys (gtif);
    GTIFFree (gtif);

    return SUCCESS;
}


/******************************************************************************
MODULE: ard_set_tiff_tags

PURPOSE: Sets the Tiff tags for the current Tiff pointer

RETURN VALUE:
Type = N/A

NOTES:
1. Compression is turned on and set to Adobe deflate.
2. Tiling is used and the size of the tiles is passed into the routine.
*****************************************************************************/
void ard_set_tiff_tags
(
    TIFF *tif,        /* I: pointer to Tiff file */
    int data_type,    /* I: data type of this band (see ARD_* in
                            ard_metadata.h) */
    int nlines,       /* I: number of lines in image */
    int nsamps,       /* I: number of samples in image */
    int t_nlines,     /* I: number of lines per tile */
    int t_nsamps      /* I: number of samples per tile */
)
{
    int samps_per_pixel = 1;    /* number of samples per pixel */

    /* Set compression to deflate */
    TIFFSetField (tif, TIFFTAG_COMPRESSION, COMPRESSION_ADOBE_DEFLATE);

    /* Turn on the tiling */
    TIFFSetField (tif, TIFFTAG_TILEWIDTH, t_nsamps);
    TIFFSetField (tif, TIFFTAG_TILELENGTH, t_nlines);

    /* Set the Tiff tags based on the input and some known defaults */
    TIFFSetField (tif, TIFFTAG_SOFTWARE, "ESPA");
    TIFFSetField (tif, TIFFTAG_IMAGEWIDTH, nsamps);
    TIFFSetField (tif, TIFFTAG_IMAGELENGTH, nlines);
    TIFFSetField (tif, TIFFTAG_SAMPLESPERPIXEL, samps_per_pixel);
    TIFFSetField (tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_CONTIG);
    TIFFSetField (tif, TIFFTAG_PHOTOMETRIC, PHOTOMETRIC_MINISBLACK);
    TIFFSetField (tif, TIFFTAG_PREDICTOR, PREDICTOR_HORIZONTAL);

    switch (data_type)
    {
        case ARD_INT8:
            TIFFSetField (tif, TIFFTAG_BITSPERSAMPLE, 8);
            TIFFSetField (tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT);
            break;
        case ARD_UINT8:
            TIFFSetField (tif, TIFFTAG_BITSPERSAMPLE, 8);
            TIFFSetField (tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
            break;
        case ARD_INT16:
            TIFFSetField (tif, TIFFTAG_BITSPERSAMPLE, 16);
            TIFFSetField (tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT);
            break;
        case ARD_UINT16:
            TIFFSetField (tif, TIFFTAG_BITSPERSAMPLE, 16);
            TIFFSetField (tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
            break;
        case ARD_INT32:
            TIFFSetField (tif, TIFFTAG_BITSPERSAMPLE, 32);
            TIFFSetField (tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_INT);
            break;
        case ARD_UINT32:
            TIFFSetField (tif, TIFFTAG_BITSPERSAMPLE, 32);
            TIFFSetField (tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_UINT);
            break;
        case ARD_FLOAT32:
            TIFFSetField (tif, TIFFTAG_BITSPERSAMPLE, 32);
            TIFFSetField (tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
            break;
        case ARD_FLOAT64:
            TIFFSetField (tif, TIFFTAG_BITSPERSAMPLE, 64);
            TIFFSetField (tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
            break;
    }
}


/******************************************************************************
MODULE: ard_open_tiff

PURPOSE: Opens a Tiff file for specified read/write/append binary access.
 
RETURN VALUE:
Type = FILE *
Value        Description
-----        -----------
NULL         Error opening the specified file for read specified access
non-NULL     FILE pointer to the opened file

NOTES:
*****************************************************************************/
TIFF *ard_open_tiff
(
    char *tiff_file,     /* I: name of the input Tiff file to be opened */
    char *access_type    /* I: string for the access type for reading the
                               input file; use the ard_tiff_format array at
                               the top of this file */
)
{
    char FUNC_NAME[] = "ard_open_tiff"; /* function name */
    char errmsg[STR_SIZE];    /* error message */
    TIFF *tif = NULL;    /* pointer to the Tiff file */

    /* Open the file with the specified access type */
    tif = XTIFFOpen (tiff_file, access_type);
    if (tif == NULL)
    {
        sprintf (errmsg, "Opening Tiff file %s with %s access.", tiff_file,
            access_type);
        ard_error_handler (true, FUNC_NAME, errmsg);
        return NULL;
    }

    /* Return the file pointer */
    return tif;
}


/******************************************************************************
MODULE: ard_close_tiff

PURPOSE: Close the Tiff file
 
RETURN VALUE:
Type = N/A

NOTES:
*****************************************************************************/
void ard_close_tiff
(
    TIFF *tif    /* I: pointer to Tiff file to be closed */
)
{
    XTIFFClose (tif);
}


/******************************************************************************
MODULE: ard_write_tiff

PURPOSE: Writes the entire Tiff file as tile-oriented and compressed
 
RETURN VALUE:
Type = int
Value        Description
-----        -----------
ERROR        An error occurred writing data to the Tiff file
SUCCESS      Writing was successful

NOTES:
1. It is expected the Tiff file will have tiling specified and the tile size
   is already identified for the Tiff pointer (see set_tiff_tags).
2. It is assumed the compression is already specified as well
   (see set_tiff_tags).
*****************************************************************************/
int ard_write_tiff
(
    TIFF *tif,       /* I: pointer to the Tiff file */
    int data_type,   /* I: data type of the array to be written (see
                           Ard_data_type in ard_metadata.h) */
    int nlines,      /* I: number of lines to write to the file */
    int nsamps,      /* I: number of samples to write to the file */
    void *img_buf    /* I: array of nlines * nsamps * size to be written to the
                           Tiff file */
)
{
    char FUNC_NAME[] = "ard_write_tiff"; /* function name */
    char errmsg[STR_SIZE];   /* error message */
    int line, samp;         /* looping variables for full image */
    int t_line;             /* looping variable for tile */
    int curr_pix;           /* current pixel in the full image */
    int curr_tile_pix;      /* current pixel in the tile */
    int img_nlines;         /* number of lines in the Tiff file */
    int img_nsamps;         /* number of samples in the Tiff file */
    int t_nlines = 0;       /* number of lines in each tile */
    int t_nsamps = 0;       /* number of samples in each tile */
    int copy_nsamps;        /* how many samples from the tile will be copied
                               to the full image */
    int8_t *int8_ptr = NULL;       /* pointer for int8 data types */
    int8_t *int8_t_ptr = NULL;     /* pointer for int8 tile data types */
    uint8_t *uint8_ptr = NULL;     /* pointer for uint8 data types */
    uint8_t *uint8_t_ptr = NULL;   /* pointer for uint8 tile data types */
    int16_t *int16_ptr = NULL;     /* pointer for int16 data types */
    int16_t *int16_t_ptr = NULL;   /* pointer for int16 tile data types */
    uint16_t *uint16_ptr = NULL;   /* pointer for uint16 data types */
    uint16_t *uint16_t_ptr = NULL; /* pointer for uint16 tile data types */
    int32_t *int32_ptr = NULL;     /* pointer for int32 data types */
    int32_t *int32_t_ptr = NULL;   /* pointer for int32 tile data types */
    uint32_t *uint32_ptr = NULL;   /* pointer for uint32 data types */
    uint32_t *uint32_t_ptr = NULL; /* pointer for uint32 tile data types */
    float *float_ptr = NULL;       /* pointer for float data types */
    float *float_t_ptr = NULL;     /* pointer for float tile data types */
    double *double_ptr = NULL;     /* pointer for double data types */
    double *double_t_ptr = NULL;   /* pointer for double tile data types */
    tdata_t t_buf = NULL;          /* tile data buffer (void ptr from TIFF) */

    /* Get the size of the image as well as the size of each tile */
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &img_nsamps);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &img_nlines);
    TIFFGetField(tif, TIFFTAG_TILEWIDTH, &t_nsamps);
    TIFFGetField(tif, TIFFTAG_TILELENGTH, &t_nlines);

    /* If the size of the tile is invalid, then tiling hasn't been turned
       on for this image (which is expected) */
    if (t_nsamps <= 0 || t_nlines <= 0)
    {
        sprintf (errmsg, "Tiff is not a tile-oriented image");
        ard_error_handler (true, FUNC_NAME, errmsg);
        return ERROR;
    }

    /* If the size of the image doesn't match that of the user-specified
       size (and the size of the input image buffer), then it's an error */
    if (img_nsamps != nsamps || img_nlines != nlines)
    {
        sprintf (errmsg, "User-specified size (%d lines x %d samps) doesn't "
            "match Tiff image size (%d lines x %d samps)", nlines, nsamps,
            img_nlines, img_nsamps);
        ard_error_handler (true, FUNC_NAME, errmsg);
        return ERROR;
    }

    /* Allocate space for the tile buffer */
    t_buf = _TIFFmalloc (TIFFTileSize (tif));
    if (t_buf == NULL)
    {
        sprintf (errmsg, "Unable to allocate memory for the tile buffer");
        ard_error_handler (true, FUNC_NAME, errmsg);
        return ERROR;
    }

    /* Set up the data type specific pointers */
    switch (data_type)
    {
        case ARD_INT8:
            int8_ptr = img_buf;
            int8_t_ptr = t_buf;
            break;
        case ARD_UINT8:
            uint8_ptr = img_buf;
            uint8_t_ptr = t_buf;
            break;
        case ARD_INT16:
            int16_ptr = img_buf;
            int16_t_ptr = t_buf;
            break;
        case ARD_UINT16:
            uint16_ptr = img_buf;
            uint16_t_ptr = t_buf;
            break;
        case ARD_INT32:
            int32_ptr = img_buf;
            int32_t_ptr = t_buf;
            break;
        case ARD_UINT32:
            uint32_ptr = img_buf;
            uint32_t_ptr = t_buf;
            break;
        case ARD_FLOAT32:
            float_ptr = img_buf;
            float_t_ptr = t_buf;
            break;
        case ARD_FLOAT64:
            double_ptr = img_buf;
            double_t_ptr = t_buf;
            break;
        default:
            sprintf (errmsg, "Unsupported data type %d", data_type);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return ERROR;
    }

    /* Tile the data from the Tiff file. Use the void pointer to point to
       the location of the current line in the data type specific pointer. */
    for (line = 0; line < nlines; line += t_nlines)
    {
        for (samp = 0; samp < nsamps; samp += t_nsamps)
        {
            /* Chop the full-sized image into the tiles */
            for (t_line = 0; t_line < t_nlines; t_line++)
            {
                /* Make sure this line is within the image */
                if (line + t_line >= nlines)
                    break;

                /* Set up the location of the current line, samp in the image
                   as well as the tile buffers */
                curr_pix = (line + t_line) * nsamps + samp;
                curr_tile_pix = t_line * t_nsamps;

                /* Determine how many samples to copy to the tile.  If this
                   is the last tile in the line, then we won't be copying the
                   data to fill the entire tile. */
                copy_nsamps = nsamps - samp;
                if (copy_nsamps > t_nsamps)
                    copy_nsamps = t_nsamps;

                /* Copy the data */
                switch (data_type)
                {
                    case ARD_INT8:
                        memcpy (&int8_t_ptr[curr_tile_pix], &int8_ptr[curr_pix],
                            copy_nsamps * sizeof (int8));
                        break;

                    case ARD_UINT8:
                        memcpy (&uint8_t_ptr[curr_tile_pix],
                                &uint8_ptr[curr_pix],
                                copy_nsamps * sizeof (uint8));
                        break;

                    case ARD_INT16:
                        memcpy (&int16_t_ptr[curr_tile_pix],
                                &int16_ptr[curr_pix],
                                copy_nsamps * sizeof (int16));
                        break;

                    case ARD_UINT16:
                        memcpy (&uint16_t_ptr[curr_tile_pix],
                                &uint16_ptr[curr_pix],
                                copy_nsamps * sizeof (uint16));
                        break;

                    case ARD_INT32:
                        memcpy (&int32_t_ptr[curr_tile_pix],
                                &int32_ptr[curr_pix],
                                copy_nsamps * sizeof (int32));
                        break;

                    case ARD_UINT32:
                        memcpy (&uint32_t_ptr[curr_tile_pix],
                                &uint32_ptr[curr_pix],
                                copy_nsamps * sizeof (uint32));
                        break;

                    case ARD_FLOAT32:
                        memcpy (&float_t_ptr[curr_tile_pix],
                                &float_ptr[curr_pix],
                                copy_nsamps * sizeof (float));
                        break;

                    case ARD_FLOAT64:
                        memcpy (&double_t_ptr[curr_tile_pix],
                                &double_ptr[curr_pix],
                                copy_nsamps * sizeof (double));
                        break;
                }
            }  /* for t_line */

            /* Write the current tile (i.e. write the tile containing the
               current x,y which should be the UL corner of the tile) */
            if (TIFFWriteTile (tif, t_buf, samp, line, 0 /*z*/, 0) < 0)
            {
                sprintf (errmsg, "Writing Tiff file for line, samp: %d, %d.",
                    line, samp);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return ERROR;
            }
        }  /* samp */
    }  /* line */

    /* Free the tile buffer */
    _TIFFfree (t_buf);

    return SUCCESS;
}


/******************************************************************************
MODULE: ard_read_tiff

PURPOSE: Reads the entire Tiff file, which should be tile-oriented
 
RETURN VALUE:
Type = int
Value        Description
-----        -----------
ERROR        An error occurred reading data from the Tiff file
SUCCESS      Reading was successful

NOTES:
*****************************************************************************/
int ard_read_tiff
(
    TIFF *tif,       /* I: pointer to the Tiff file */
    int data_type,   /* I: data type of the array to be read (see
                           Ard_data_type in ard_metadata.h) */
    int nlines,      /* I: number of lines to read from the file */
    int nsamps,      /* I: number of samples to read from the file */
    void *img_buf    /* O: array of nlines * nsamps * size to be read from the
                           Tiff file (sufficient space should already have
                           been allocated) */
)
{
    char FUNC_NAME[] = "ard_read_tiff"; /* function name */
    char errmsg[STR_SIZE];  /* error message */
    int line, samp;         /* looping variables for full image */
    int t_line;             /* looping variable for tile */
    int curr_pix;           /* current pixel in the full image */
    int curr_tile_pix;      /* current pixel in the tile */
    int img_nlines;         /* number of lines in the Tiff file */
    int img_nsamps;         /* number of samples in the Tiff file */
    int t_nlines = 0;       /* number of lines in each tile */
    int t_nsamps = 0;       /* number of samples in each tile */
    int copy_nsamps;        /* how many samples from the tile will be copied
                               to the full image */
    int8_t *int8_ptr = NULL;       /* pointer for int8 data types */
    int8_t *int8_t_ptr = NULL;     /* pointer for int8 tile data types */
    uint8_t *uint8_ptr = NULL;     /* pointer for uint8 data types */
    uint8_t *uint8_t_ptr = NULL;   /* pointer for uint8 tile data types */
    int16_t *int16_ptr = NULL;     /* pointer for int16 data types */
    int16_t *int16_t_ptr = NULL;   /* pointer for int16 tile data types */
    uint16_t *uint16_ptr = NULL;   /* pointer for uint16 data types */
    uint16_t *uint16_t_ptr = NULL; /* pointer for uint16 tile data types */
    int32_t *int32_ptr = NULL;     /* pointer for int32 data types */
    int32_t *int32_t_ptr = NULL;   /* pointer for int32 tile data types */
    uint32_t *uint32_ptr = NULL;   /* pointer for uint32 data types */
    uint32_t *uint32_t_ptr = NULL; /* pointer for uint32 tile data types */
    float *float_ptr = NULL;       /* pointer for float data types */
    float *float_t_ptr = NULL;     /* pointer for float tile data types */
    double *double_ptr = NULL;     /* pointer for double data types */
    double *double_t_ptr = NULL;   /* pointer for double tile data types */
    tdata_t t_buf = NULL;          /* tile data buffer (void ptr from TIFF) */
uint32_t val;

    /* Get the size of the image as well as the size of each tile */
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &img_nsamps);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &img_nlines);
    TIFFGetField(tif, TIFFTAG_TILEWIDTH, &t_nsamps);
    TIFFGetField(tif, TIFFTAG_TILELENGTH, &t_nlines);

    /* If the size of the tile is invalid, then this isn't a tile-oriented
       image */
    if (t_nsamps <= 0 || t_nlines <= 0)
    {
        sprintf (errmsg, "Tiff is not a tile-oriented image");
        ard_error_handler (true, FUNC_NAME, errmsg);
        return ERROR;
    }

    /* If the size of the image doesn't match that of the user-specified
       size (and the size of the input image buffer), then it's an error */
    if (img_nsamps != nsamps || img_nlines != nlines)
    {
        sprintf (errmsg, "User-specified size (%d lines x %d samps) doesn't "
            "match Tiff image size (%d lines x %d samps)", nlines, nsamps,
            img_nlines, img_nsamps);
        ard_error_handler (true, FUNC_NAME, errmsg);
        return ERROR;
    }

    /* Allocate space for the tile buffer */
    t_buf = _TIFFmalloc (TIFFTileSize (tif));
    if (t_buf == NULL)
    {
        sprintf (errmsg, "Unable to allocate memory for the tile buffer");
        ard_error_handler (true, FUNC_NAME, errmsg);
        return ERROR;
    }

    /* Set up the data type specific pointers */
    switch (data_type)
    {
        case ARD_INT8:
            int8_ptr = img_buf;
            int8_t_ptr = t_buf;
            break;
        case ARD_UINT8:
            uint8_ptr = img_buf;
            uint8_t_ptr = t_buf;
            break;
        case ARD_INT16:
            int16_ptr = img_buf;
            int16_t_ptr = t_buf;
            break;
        case ARD_UINT16:
            uint16_ptr = img_buf;
            uint16_t_ptr = t_buf;
            break;
        case ARD_INT32:
            int32_ptr = img_buf;
            int32_t_ptr = t_buf;
            break;
        case ARD_UINT32:
            uint32_ptr = img_buf;
            uint32_t_ptr = t_buf;
            break;
        case ARD_FLOAT32:
            float_ptr = img_buf;
            float_t_ptr = t_buf;
            break;
        case ARD_FLOAT64:
            double_ptr = img_buf;
            double_t_ptr = t_buf;
            break;
        default:
            sprintf (errmsg, "Unsupported data type %d", data_type);
            ard_error_handler (true, FUNC_NAME, errmsg);
            return ERROR;
    }

    /* Read the data from the Tiff file. Use the void pointer to point to
       the location of the current line in the data type specific pointer. */
    for (line = 0; line < nlines; line += t_nlines)
    {
        for (samp = 0; samp < nsamps; samp += t_nsamps)
        {
            /* Read the current tile (i.e. read the tile containing the
               current x,y which should be the UL corner of the tile) */
            if (TIFFReadTile (tif, t_buf, samp, line, 0 /*z*/, 0) < 0)
            {
                sprintf (errmsg, "Reading Tiff file for line, samp: %d, %d.",
                    line, samp);
                ard_error_handler (true, FUNC_NAME, errmsg);
                return ERROR;
            }

            /* Copy the tile into the full-sized image */
            for (t_line = 0; t_line < t_nlines; t_line++)
            {
                /* Make sure this line is within the image */
                if (line + t_line >= nlines)
                    break;

                /* Set up the location of the current line, samp in the image
                   as well as the tile buffers */
                curr_pix = (line + t_line) * nsamps + samp;
                curr_tile_pix = t_line * t_nsamps;

                /* Determine how many samples to copy from the tile.  If this
                   is the last tile in the line, then we won't be copying the
                   entire tile. */
                copy_nsamps = nsamps - samp;
                if (copy_nsamps > t_nsamps)
                    copy_nsamps = t_nsamps;

                /* Copy the data */
                switch (data_type)
                {
                    case ARD_INT8:
                        memcpy (&int8_ptr[curr_pix], &int8_t_ptr[curr_tile_pix],
                            copy_nsamps * sizeof (int8));
                        break;

                    case ARD_UINT8:
                        memcpy (&uint8_ptr[curr_pix],
                                &uint8_t_ptr[curr_tile_pix],
                                copy_nsamps * sizeof (uint8));
                        break;

                    case ARD_INT16:
                        memcpy (&int16_ptr[curr_pix],
                                &int16_t_ptr[curr_tile_pix],
                                copy_nsamps * sizeof (int16));
                        break;

                    case ARD_UINT16:
                        memcpy (&uint16_ptr[curr_pix],
                                &uint16_t_ptr[curr_tile_pix],
                                copy_nsamps * sizeof (uint16));
                        break;

                    case ARD_INT32:
                        memcpy (&int32_ptr[curr_pix],
                                &int32_t_ptr[curr_tile_pix],
                                copy_nsamps * sizeof (int32));
                        break;

                    case ARD_UINT32:
                        memcpy (&uint32_ptr[curr_pix],
                                &uint32_t_ptr[curr_tile_pix],
                                copy_nsamps * sizeof (uint32));
                        break;

                    case ARD_FLOAT32:
                        memcpy (&float_ptr[curr_pix],
                                &float_t_ptr[curr_tile_pix],
                                copy_nsamps * sizeof (float));
                        break;

                    case ARD_FLOAT64:
                        memcpy (&double_ptr[curr_pix],
                                &double_t_ptr[curr_tile_pix],
                                copy_nsamps * sizeof (double));
                        break;
                }
            }  /* for t_line */
        }  /* samp */
    }  /* line */

    /* Free the tile buffer */
    _TIFFfree (t_buf);

    return SUCCESS;
}


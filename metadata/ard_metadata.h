/*****************************************************************************
FILE: ard_metadata.h
  
PURPOSE: Contains ARD metadata related defines and structures

PROJECT:  Land Satellites Data System Science Research and Development (LSRD)
at the USGS EROS

LICENSE TYPE:  NASA Open Source Agreement Version 1.3

NOTES:
*****************************************************************************/

#ifndef ARD_METADATA_H
#define ARD_METADATA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlschemastypes.h>
#include "ard_error_handler.h"
#include "ard_gctp_defines.h"

/* Defines - Namespace only contains the major version number (i.e. 1 for 1.x),
   but the schema version will contain the major and minor version number
   (i.e. 1.2) */
#define LIBXML_SCHEMAS_ENABLED
#define ARD_SCHEMA_VERSION "1.0"
#define ARD_NS "http://ard.cr.usgs.gov/v1"
#define ARD_SCHEMA_LOCATION "http://ard.cr.usgs.gov/v1"
#define ARD_SCHEMA "http://espa.cr.usgs.gov/schema/ard/ard_metadata_v1_0.xsd"
#define LOCAL_ARD_SCHEMA "/usr/local/ard-product-libraries/schema/ard_metadata_v1_0.xsd"

/* Data types */
enum Ard_data_type
{
    ARD_INT8, ARD_UINT8, ARD_INT16, ARD_UINT16, ARD_INT32, ARD_UINT32,
    ARD_FLOAT32, ARD_FLOAT64
};

/* Resampling types */
enum Ard_resampling_type
{
    ARD_CC, ARD_NN, ARD_BI, ARD_NONE
};

/* Geographic types */
enum Ard_geographic_type
{
    ARD_WEST, ARD_EAST, ARD_NORTH, ARD_SOUTH
};

/* Elevation source types */
enum Ard_elevation_type
{
    ARD_NED, ARD_SRTM, ARD_GTOPO30, ARD_GLS2000, ARD_RAMP
};

/* Sensor mode types */
enum Ard_sensor_modes
{
    ARD_BUMPER, ARD_SAM
};

/* Ephemeris types */
enum Ard_ephem_type
{
    ARD_DEFINITIVE, ARD_PREDICTIVE
};


/* Local define to specify the maximum total bands and product types.  This
   should be sufficient for ARD products. */
#define MAX_TOTAL_BANDS 100
#define MAX_TOTAL_PRODUCT_TYPES 50

/* Each tile will have a maximum of 3 scenes covering the tile -- current,
   previous, and next */
#define MAX_TOTAL_SCENES 3

/* Local defines for fill or not used data values in the metadata */
#define ARD_INT_META_FILL -3333
#define ARD_FLOAT_META_FILL -3333.00
#define ARD_STRING_META_FILL "undefined"
#define ARD_EPSILON 0.00001

/* Structures to support the global and band metadata information stored in
   the ARD metadata file */
typedef struct
{
    int class;                    /* class value */
    char description[STR_SIZE];   /* class description */
} Ard_class_t;

typedef struct
{
    int proj_type;        /* projection number (see GCTP_* in gctp_defines.h) */
    int datum_type;       /* datum type (see ARD_* in gctp_defines.h) */
    char units[STR_SIZE]; /* projection units (degrees, meters) */
    double ul_corner[2];  /* projection UL x, y (store center of the pixel
                             for multi-res products) */
    double lr_corner[2];  /* projection LR x, y (store center of the pixel
                             for multi-res products) */
    char grid_origin[STR_SIZE];  /* origin of the gridded data (UL, CENTER) */

    /* UTM projection parameters */
    int utm_zone;         /* UTM zone; use a negative number if this is a
                             southern zone */
    /* PS projection parameters */
    double longitude_pole;
    double latitude_true_scale;
    double false_easting;
    double false_northing;

    /* ALBERS projection parameters */
    double standard_parallel1;
    double standard_parallel2;
    double central_meridian;
    double origin_latitude;
    /* double false_easting;   -- already defined */
    /* double false_northing;  -- already defined */

    /* SIN projection parameters */
    double sphere_radius;
    /* double central_meridian; -- already defined */
    /* double false_easting; -- already defined */
    /* double false_northing; -- already defined */

} Ard_proj_meta_t;

typedef struct
{
    char data_provider[STR_SIZE]; /* name of the original data provider */
    char satellite[STR_SIZE];     /* name of the satellite (LANDSAT_4,
                                     LANDSAT_5, LANDSAT_7, LANDSAT_8) */
    char instrument[STR_SIZE];    /* name of instrument (MSS, TM, ETM+,
                                     OLI/TIRS, AQUA, TERRA, ...) */
    char level1_collection[STR_SIZE]; /* Level-1 collection number */
    char ard_version[STR_SIZE];   /* ARD version number */
    char region[STR_SIZE];        /* ARD region - CU, HI, AK, etc. */
    char acquisition_date[STR_SIZE]; /* date of scene acquisition (yyyy-mm-dd)*/
    char product_id[STR_SIZE];       /* product ID */
    char production_date[STR_SIZE];  /* date the tile was processed */
    double bounding_coords[4];    /* geographic west, east, north, south */
    Ard_proj_meta_t proj_info;    /* projection information structure */
    float orientation_angle;      /* orientation angle of the scene (degrees) */
    int htile;                    /* ARD horizontal tile number */
    int vtile;                    /* ARD vertical tile number */
    int scene_count;              /* number of scenes in this tile */
    float cloud_cover;            /* percentage of cloud cover */
    float cloud_shadow;           /* percentage of cloud shadow */
    float snow_ice;               /* percentage of snow/ice */
    float fill;                   /* percentage of fill */
} Ard_global_tile_meta_t;

typedef struct
{
    char data_provider[STR_SIZE]; /* name of the original data provider */
    char satellite[STR_SIZE];     /* name of the satellite (LANDSAT_4,
                                     LANDSAT_5, LANDSAT_7, LANDSAT_8) */
    char instrument[STR_SIZE];    /* name of instrument (MSS, TM, ETM+,
                                     OLI/TIRS, AQUA, TERRA, ...) */
    char acquisition_date[STR_SIZE]; /* date of scene acquisition (yyyy-mm-dd)*/
    char scene_center_time[STR_SIZE];  /* GMT time at scene center */
    char level1_production_date[STR_SIZE];  /* date the scene was processed
                                               to a level 1 product */
    int wrs_system;               /* 1 or 2 */
    int wrs_path;                 /* WRS path of this scene */
    int wrs_row;                  /* WRS row of this scene */
    char request_id[STR_SIZE];    /* request ID */
    char scene_id[STR_SIZE];      /* scene ID */
    char product_id[STR_SIZE];    /* product ID */
    enum Ard_elevation_type elevation_src; /* elevation source */
    enum Ard_sensor_modes sensor_mode;     /* sensor mode */
    enum Ard_ephem_type ephemeris_type;    /* ephemeris type */
    char cpf_name[STR_SIZE];      /* name of Landsat CPF file */
    char lpgs_metadata_file[STR_SIZE]; /* name of LPGS metadata file */
    float geometric_rmse_model;   /* overall geometric RMSE */
    float geometric_rmse_model_x; /* x-direction geometric RMSE */
    float geometric_rmse_model_y; /* y-direction geometric RMSE */
} Ard_global_scene_meta_t;

typedef struct
{
    char product[STR_SIZE];      /* product type */
    char source[STR_SIZE];       /* source type (level1, toa_refl, sr_refl) */
    char name[STR_SIZE];         /* band name */
    char category[STR_SIZE];     /* category type (image, qa, browse, index) */
    enum Ard_data_type data_type;  /* data type of this band */
    int nlines;                  /* number of lines in the dataset */
    int nsamps;                  /* number of samples in the dataset */
    long fill_value;             /* use long to support long data types */
    int saturate_value;          /* saturation value (for Landsat) */
    float scale_factor;          /* scaling factor */
    float add_offset;            /* offset to be added */
    char short_name[STR_SIZE];   /* short band name */
    char long_name[STR_SIZE];    /* long band name */
    char file_name[STR_SIZE];    /* raw binary file name for this band w/o the
                                    pathname */
    double pixel_size[2];        /* pixel size x, y */
    char pixel_units[STR_SIZE];  /* units for pixel size (meters, degrees) */
    enum Ard_resampling_type resample_method;
                                 /* resampling method for this band */
    char data_units[STR_SIZE];   /* units of data stored in this band */
    float valid_range[2];        /* min, max valid value for this band */
    int nbits;                   /* number of bits in bitmap_description */
    char **bitmap_description;   /* support bit mapping description;
                                    0-based going from right to left in the
                                    binary representation; assume the XML file
                                    has a description for every bit number
                                    inclusive from 0 to nbits-1 */
    int nclass;                  /* number of classes in class_values */
    Ard_class_t *class_values;   /* support class value descriptions */
    char app_version[STR_SIZE];  /* version of the application which produced
                                    the current band */
    char production_date[STR_SIZE];  /* date the band was produced */
} Ard_band_meta_t;

typedef struct
{
    Ard_global_tile_meta_t tile_global;    /* global metadata */
    int nbands;                /* number of bands in the metadata file */
    Ard_band_meta_t *band;     /* array of band metadata */
} Ard_tile_meta_t;

typedef struct
{
    Ard_global_scene_meta_t scene_global;  /* global metadata */
    int nbands;                /* number of bands in the metadata file */
    Ard_band_meta_t *band;     /* array of band metadata */
} Ard_scene_meta_t;

typedef struct
{
    char meta_namespace[STR_SIZE];  /* namespace for this metadata file */
    Ard_tile_meta_t tile_meta;      /* tile-specific metadata */
    int nscenes;                    /* number of scenes in the tile metadata */
    Ard_scene_meta_t scene_meta[MAX_TOTAL_SCENES];
                                    /* scene-specific metadata for each of
                                       the scenes in the tile */
} Ard_meta_t;


/* Prototypes */
int validate_ard_xml_file
(
    char *meta_file           /* I: name of metadata file to be validated */
);

void init_ard_tile_metadata_struct
(
    Ard_tile_meta_t *tile_meta /* I: pointer to ARD tile_metadata structure to
                                     be initialized */
);

void init_ard_metadata_struct
(
    Ard_meta_t *ard_meta      /* I: pointer to ARD metadata structure to be
                                    initialized */
);

int allocate_ard_band_metadata
(
    Ard_tile_meta_t *tile_meta,   /* I: pointer to tile-specific metadata,
                                        NULL if initializing scene metadata */
    Ard_scene_meta_t *scene_meta, /* I: pointer to scene-specific metadata,
                                        NULL if initializing tile metadata */
    int nbands     /* I: number of bands to allocate for the tile-specific or
                         scene-specific band field */
);

int allocate_ard_class_metadata
(
    Ard_band_meta_t *band_meta,  /* I: pointer to band metadata structure */
    int nclass                   /* I: number of classes to allocate for the
                                       band metadata */
);

int allocate_ard_bitmap_metadata
(
    Ard_band_meta_t *band_meta,  /* I: pointer to band metadata structure */
    int nbits                    /* I: number of bits to allocate for the
                                       bitmap metadata */
);

void free_ard_band_metadata
(
    int nbands,            /* I: number of bands in the metadata file */
    Ard_band_meta_t *band  /* I/O: array of band metadata to be freed */
);

void free_ard_metadata
(
    Ard_meta_t *ard_meta     /* I: pointer to ARD metadata structure */
);

void print_element_names
(
    xmlNode *a_node   /* I: pointer to the current node in the tree to start
                            printing */
);

#endif

/*****************************************************************************
FILE: test_read_ard.c
  
PURPOSE: Contains functions for reading/writing the ARD tiles as part of
testing the GeoTiff/Tiff libraries.

PROJECT:  Land Satellites Data System Science Research and Development (LSRD)
at the USGS EROS

LICENSE TYPE:  NASA Open Source Agreement Version 1.3

NOTES:
  1. The XML metadata format read via this library follows the ARD metadata
     format found in the ARD DFCB.  The schema for the ARD metadata format is
     available at
     https://landsat.usgs.gov/ard/ard_metadata_vX_X.xsd
*****************************************************************************/
#include <getopt.h>
#include "ard_tiff_io.h"

/******************************************************************************
MODULE: usage

PURPOSE: Prints the usage information for this application.

RETURN VALUE:
Type = None

NOTES:
******************************************************************************/
void usage ()
{
    printf ("test_read_ard parses the XML, reads the Tiff files, and writes "
            "back out the GeoTiff test files to duplicate each band.\n\n");
    printf ("usage: test_read_ard --xml=xml_filename\n");

    printf ("\nwhere the following parameters are required:\n");
    printf ("    -xml: name of the input XML metadata file which follows "
            "the ARD schema\n");
    printf ("\nExample: test_read_ard "
            "--xml=LT05_CU_003009_20110702_20170430_C01_V01_SR\n");
}


/******************************************************************************
MODULE:  get_args

PURPOSE:  Gets the command-line arguments and validates that the required
arguments were specified.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error getting the command-line arguments or a command-line
                argument and associated value were not specified
SUCCESS         No errors encountered

NOTES:
  1. Memory is allocated for the input and output files.  All of these should
     be character pointers set to NULL on input.  The caller is responsible
     for freeing the allocated memory upon successful return.
******************************************************************************/
short get_args
(
    int argc,             /* I: number of cmd-line args */
    char *argv[],         /* I: string of cmd-line args */
    char **xml_infile     /* O: address of input XML filename */
)
{
    int c;                           /* current argument index */
    int option_index;                /* index for the command-line option */
    char errmsg[STR_SIZE];           /* error message */
    char FUNC_NAME[] = "get_args";   /* function name */
    static struct option long_options[] =
    {
        {"xml", required_argument, 0, 'i'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };

    /* Loop through all the cmd-line options */
    opterr = 0;   /* turn off getopt_long error msgs as we'll print our own */
    while (1)
    {
        /* optstring in call to getopt_long is empty since we will only
           support the long options */
        c = getopt_long (argc, argv, "", long_options, &option_index);
        if (c == -1)
        {   /* Out of cmd-line options */
            break;
        }

        switch (c)
        {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if (long_options[option_index].flag != 0)
                    break;
     
            case 'h':  /* help */
                usage ();
                return (ERROR);
                break;

            case 'i':  /* XML infile */
                *xml_infile = strdup (optarg);
                break;
     
            case '?':
            default:
                sprintf (errmsg, "Unknown option %s", argv[optind-1]);
                ard_error_handler (true, FUNC_NAME, errmsg);
                usage ();
                return (ERROR);
                break;
        }
    }

    /* Make sure the infile was specified */
    if (*xml_infile == NULL)
    {
        sprintf (errmsg, "XML input file is a required argument");
        ard_error_handler (true, FUNC_NAME, errmsg);
        usage ();
        return (ERROR);
    }

    return (SUCCESS);
}


/******************************************************************************
MODULE:  main

PURPOSE:  Test program for ARD products to parse the input XML, loop through
the GeoTiff bands, read the Tiff, write the Tiff, and write the Tiff/GeoTiff
geolocation tags.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error processing the ARD product
SUCCESS         No errors encountered

NOTES:
1. This routine requires an 'output' dirctory to exist in the same directory
   as the ARD Tile data being processed.
2. This routine loops through the bands listed in the XML file, reads them,
   then writes the same band to the 'output' directory.  Unless the SR.tar,
   BT.tar, and TA.tar files have all been untarred, then not all the bands in
   the XML file will be available.
******************************************************************************/
int main (int argc, char** argv)
{
    char FUNC_NAME[] = "test_read_ard"; /* function name */
    char errmsg[STR_SIZE];             /* error message */
    char outname[STR_SIZE];            /* output band name */
    char *xml_infile = NULL;           /* input XML filename */
    int status;                        /* return status */
    int i;                             /* looping variable */
    int t_nlines = 0;                  /* number of lines in each tile */
    int t_nsamps = 0;                  /* number of samples in each tile */
    void *band_buffer = NULL;          /* image buffer for the current band */
    Ard_meta_t xml_metadata;           /* XML metadata structure to be
                                          populated by reading the XML
                                          metadata file */
    Ard_global_tile_meta_t *gmeta = NULL;  /* pointer to the global tile
                                          metadata structure */
    Ard_band_meta_t *bmeta = NULL;     /* pointer to current band metadata */
    TIFF *tif_fptr = NULL;             /* file pointer for Tiff file */

    /* Read the command-line arguments */
    if (get_args (argc, argv, &xml_infile) != SUCCESS)
    {   /* get_args already printed the error message */
        exit (EXIT_FAILURE);
    }

    /* Validate the input metadata file */
    if (validate_ard_xml_file (xml_infile) != SUCCESS)
    {  /* Error messages already written */
        exit (EXIT_FAILURE);
    }

    /* Initialize the metadata structure */
    init_ard_metadata_struct (&xml_metadata);

    /* Parse the metadata file into our internal metadata structure; also
       allocates space as needed for various pointers in the global and band
       metadata */
    if (parse_ard_metadata (xml_infile, &xml_metadata) != SUCCESS)
    {  /* Error messages already written */
        exit (EXIT_FAILURE);
    }
    gmeta = &xml_metadata.tile_meta.tile_global;

    /* Loop through each band in the XML file */
    for (i = 0; i < xml_metadata.tile_meta.nbands; i++)
    {
        /* Allocate memory for the current band */
        bmeta = &xml_metadata.tile_meta.band[i];
        printf ("Processing band %d: %s\n", i, bmeta->file_name);
        switch (bmeta->data_type)
        {
            case ARD_INT8:
                band_buffer = calloc (bmeta->nlines * bmeta->nsamps,
                    sizeof (int8_t));
                break;
            case ARD_UINT8:
                band_buffer = calloc (bmeta->nlines * bmeta->nsamps,
                    sizeof (uint8_t));
                break;
            case ARD_INT16:
                band_buffer = calloc (bmeta->nlines * bmeta->nsamps,
                    sizeof (int16_t));
                break;
            case ARD_UINT16:
                band_buffer = calloc (bmeta->nlines * bmeta->nsamps,
                    sizeof (uint16_t));
                break;
            case ARD_INT32:
                band_buffer = calloc (bmeta->nlines * bmeta->nsamps,
                    sizeof (int32_t));
                break;
            case ARD_UINT32:
                band_buffer = calloc (bmeta->nlines * bmeta->nsamps,
                    sizeof (uint32_t));
                break;
            case ARD_FLOAT32:
                band_buffer = calloc (bmeta->nlines * bmeta->nsamps,
                    sizeof (float));
                break;
            case ARD_FLOAT64:
                band_buffer = calloc (bmeta->nlines * bmeta->nsamps,
                    sizeof (double));
                break;
        }
        if (band_buffer == NULL)
        {
            sprintf (errmsg, "Unable to allocate memory for the image data");
            ard_error_handler (true, FUNC_NAME, errmsg);
            exit (EXIT_FAILURE);
        }

        /* Open the current band as a Tiff file for reading. Save some tags
           regarding tile size for later. */
        tif_fptr = ard_open_tiff (bmeta->file_name, "r");
        if (tif_fptr == NULL)
        {  /* Error messages already written */
            exit (EXIT_FAILURE);
        }
        TIFFGetField(tif_fptr, TIFFTAG_TILEWIDTH, &t_nsamps);
        TIFFGetField(tif_fptr, TIFFTAG_TILELENGTH, &t_nlines);

        /* Read the current band */
        status = ard_read_tiff (tif_fptr, bmeta->data_type, bmeta->nlines,
            bmeta->nsamps, band_buffer);
        if (status != SUCCESS)
        {
            sprintf (errmsg, "Error reading the Tiff file %s",
                bmeta->file_name);
            ard_error_handler (true, FUNC_NAME, errmsg);
            exit (EXIT_FAILURE);
        }

        /* Close the input Tiff file */
        ard_close_tiff (tif_fptr);

        /* Open the output band as a Tiff file for writing */
        sprintf (outname, "output/%s", bmeta->file_name);
        tif_fptr = ard_open_tiff (outname, "w");
        if (tif_fptr == NULL)
        {  /* Error messages already written */
            exit (EXIT_FAILURE);
        }

        /* Set the Tiff tags before writing so the Tiff library knows the
           specifics of the band.  Pull the tile size from the input Tiff
           and use that for the output Tiff. */
        ard_set_tiff_tags (tif_fptr, bmeta->data_type, bmeta->nlines,
            bmeta->nsamps, t_nlines, t_nsamps);

        /* Write the current band to the output directory */
        status = ard_write_tiff (tif_fptr, bmeta->data_type, bmeta->nlines,
            bmeta->nsamps, band_buffer);
        if (status != SUCCESS)
        {
            sprintf (errmsg, "Error writing the Tiff file %s", outname);
            ard_error_handler (true, FUNC_NAME, errmsg);
            exit (EXIT_FAILURE);
        }

        /* Write the GeoTiff tags */
        status = ard_set_geotiff_tags (tif_fptr, bmeta, &gmeta->proj_info);
        if (status != SUCCESS)
        {
            sprintf (errmsg, "Error writing the GeoTiff tags for %s", outname);
            ard_error_handler (true, FUNC_NAME, errmsg);
            exit (EXIT_FAILURE);
        }

        /* Close the output Tiff file */
        ard_close_tiff (tif_fptr);

        /* Free the image buffer */
        free (band_buffer);
    }

    /* Free the metadata structure */
    free_ard_metadata (&xml_metadata);

    /* Free the pointers */
    free (xml_infile);

    /* Successful completion */
    exit (EXIT_SUCCESS);
}

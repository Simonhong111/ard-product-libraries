/*****************************************************************************
FILE: test_append_xml

PURPOSE: Tests writing appended bands for the tile_metadata to the ARD XML file.

PROJECT:  Land Satellites Data System Science Research and Development (LSRD)
at the USGS EROS

LICENSE TYPE:  NASA Open Source Agreement Version 1.3

NOTES:
*****************************************************************************/
#include <getopt.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "ard_metadata.h"
#include "ard_error_handler.h"
#include "parse_ard_metadata.h"
#include "append_ard_tile_bands_metadata.h"


/******************************************************************************
MODULE: usage

PURPOSE: Prints the usage information for this application.

RETURN VALUE:
Type = None

NOTES:
******************************************************************************/
void usage ()
{
    printf ("test_append_xml parses the input XML file and then writes it "
            "back out to a new XML file, adding a few random bands to the "
            "tile_metadata as defined in this test source code.");
    printf ("usage: test_append_xml "
            "--xml=input_ard_metadata_filename\n");

    printf ("\nwhere the following parameters are required:\n");
    printf ("    -xml: name of the input ARD XML metadata file which follows "
            "the ARD schema (format defined in the ARD DFCB)\n");

    printf ("\nExample: test_append_xml"
            "--xml=LE07_CU_019002_19991006_20170307_C01_V01.xml\n");
    printf ("This reads the input XML and then writes it back out as "
            "{base_xml_name}_new.xml, but appends a few user-provided band "
            "to the tile_metadata as provided in the source code.\n");
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

            case 'i':  /* XML file */
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

    /* Make sure the input XML file was specified */
    if (*xml_infile == NULL)
    {
        sprintf (errmsg, "Input XML file is a required argument");
        ard_error_handler (true, FUNC_NAME, errmsg);
        usage ();
        return (ERROR);
    }

    return (SUCCESS);
}


/******************************************************************************
MODULE:  main

PURPOSE: Grabs the input XML file, parses it, writes it back out, and appends
the new bands to the tile_metadata as provided in this source code.

RETURN VALUE:
Type = int
Value           Description
-----           -----------
ERROR           Error validating the XML file
SUCCESS         No errors encountered

NOTES:
******************************************************************************/
int main (int argc, char** argv)
{
    char FUNC_NAME[] = "test_append_xml";  /* function name */
    char errmsg[STR_SIZE];       /* error message */
    char *cptr = NULL;           /* pointer to file extension */
    char *xml_infile = NULL;     /* input XML filename */
    char xml_outfile[STR_SIZE];  /* output XML filename */
    int nbands_append;           /* number of new bands to append */
    Ard_meta_t ard_meta;         /* XML metadata structure to be populated by
                                    reading the input XML metadata file */
    Ard_tile_meta_t new_tile_meta; /* tile-specific metadata for new bands */

    /* Read the command-line arguments */
    if (get_args (argc, argv, &xml_infile) != SUCCESS)
    {   /* get_args already printed the error message */
        exit (ERROR);
    }
    printf ("TEST parsing and re-writing of XML file %s\n", xml_infile);

    /* Validate the input metadata file */
    if (validate_ard_xml_file (xml_infile) != SUCCESS)
    {  /* Error messages already written */
        return (ERROR);
    }

    /* Initialize the metadata structure */
    init_ard_metadata_struct (&ard_meta);

    /* Parse the metadata file into our internal metadata structure; also
       allocates space as needed for various pointers in the global and band
       metadata */
    if (parse_ard_metadata (xml_infile, &ard_meta) != SUCCESS)
    {  /* Error messages already written */
        return (ERROR);
    }

    /* Determine the new output file name */
    strcpy (xml_outfile, xml_infile);
    cptr = strchr (xml_outfile, '.');
    strcpy (cptr, "_new.xml");

    /* Initialize new bands to be appended to the tile_metadata */
    nbands_append = 3;
    if (allocate_ard_band_metadata (&new_tile_meta, NULL, nbands_append) !=
        SUCCESS)
    {
        sprintf (errmsg, "Allocating %d new bands for ARD tile metadata",
            nbands_append);
        ard_error_handler (true, FUNC_NAME, errmsg);
        return (ERROR);
    }

    /* TODO - Add band information to the 3 bands, if desired.  For now the
       bands will just be the default/initialized values. */

    /* Write the metadata to a new output XML file, appending the additional
       bands for the tile_metadata */
    printf ("Writing/appending ARD metadata to %s\n", xml_outfile);
    if (append_ard_tile_bands_metadata (&ard_meta, nbands_append,
        new_tile_meta.band, xml_outfile) != SUCCESS)
    {  /* Error messages already written */
        return (ERROR);
    }

    /* Free the input XML metadata */
    free_ard_metadata (&ard_meta);

    /* Free the new bands for the tile_metadata */
    free_ard_band_metadata (new_tile_meta.nbands, new_tile_meta.band);

    /* Free the pointers */
    free (xml_infile);

    /* Successful completion */
    printf ("File successfully parsed and re-written\n");
    exit (SUCCESS);
}

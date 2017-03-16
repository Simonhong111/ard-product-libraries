/*****************************************************************************
FILE: test_parse_xml

PURPOSE: Tests the parsing of the ARD XML file.

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


/******************************************************************************
MODULE: usage

PURPOSE: Prints the usage information for this application.

RETURN VALUE:
Type = None

NOTES:
******************************************************************************/
void usage ()
{
    printf ("test_parse_xml parses the input XML file");
    printf ("usage: test_parse_xml --xml=input_ard_metadata_filename\n");

    printf ("\nwhere the following parameters are required:\n");
    printf ("    -xml: name of the input ARD XML metadata file which follows "
            "the ARD schema (format defined in the ARD DFCB)\n");

    printf ("\nExample: test_parse_xml "
            "--xml=LE07_CU_019002_19991006_20170307_C01_V01.xml\n");
    printf ("This parses the specified ARD XML file.\n");
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

    /* Make sure the XML file was specified */
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

PURPOSE: Grabs the input XML file and parses it.

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
//    char FUNC_NAME[] = "test_parse_xml";  /* function name */
//    char errmsg[STR_SIZE];       /* error message */
    char *xml_infile = NULL;     /* input XML filename */
    Ard_meta_t ard_meta;         /* XML metadata structure to be populated by
                                    reading the input XML metadata file */

    /* Read the command-line arguments */
    if (get_args (argc, argv, &xml_infile) != SUCCESS)
    {   /* get_args already printed the error message */
        exit (ERROR);
    }
    printf ("TEST parsing of XML file %s\n", xml_infile);

    /* Validate the input metadata file */
    if (validate_xml_file (xml_infile) != SUCCESS)
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

    /* Print some items */
    printf ("ARD metadata namespace: %s\n", ard_meta.meta_namespace);
    printf ("Number of scenes in the tile: %d\n", ard_meta.nscenes);

    /* Free the input XML metadata */
    free_ard_metadata (&ard_meta);

    /* Free the pointers */
    free (xml_infile);

    /* Successful completion */
    printf ("File successfully parsed\n");
    exit (SUCCESS);
}

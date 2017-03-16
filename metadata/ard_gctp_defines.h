/*****************************************************************************
FILE: ard_gctp_defines.h
  
PURPOSE: Contains ARD internal defines for GCTP projections

PROJECT:  Land Satellites Data System Science Research and Development (LSRD)
at the USGS EROS

LICENSE TYPE:  NASA Open Source Agreement Version 1.3

NOTES:
*****************************************************************************/

#ifndef ARD_GCTP_DEFINES_H
#define ARD_GCTP_DEFINES_H

/* GCTP projection numbers for ESPA projections (match proj.h from GCTP) */
#define ARD_GCTP_GEO_PROJ    0
#define ARD_GCTP_UTM_PROJ    1
#define ARD_GCTP_ALBERS_PROJ 3
#define ARD_GCTP_PS_PROJ     6
#define ARD_GCTP_SIN_PROJ    16

/* GCTP spheroid numbers (match sphdz.c in GCTP) */
#define ARD_GCTP_CLARKE_1866  0
#define ARD_GCTP_GRS80        8
#define ARD_GCTP_WGS84        12
#define ARD_GCTP_MODIS_SPHERE 21

/* Local defines to use for the datum */
#define ARD_NAD27   225
#define ARD_NAD83   219
#define ARD_WGS84   317
#define ARD_NODATUM -1

/* Define the semi-major axis (meters), semi-minor axis (meters), and the
   inverse flattening for each of the spheroids.
   Obtained from NGA Reference Ellipsoid Parameters via
   http://geoengine.nga.mil/geospatial/SW_TOOLS/NIMAMUSE/webinter/geotrans2/help/elliptab.htm */
#define ARD_GCTP_CLARKE_1866_SEMI_MAJOR 6378206.4
#define ARD_GCTP_CLARKE_1866_SEMI_MINOR 6356583.8
#define ARD_GCTP_CLARKE_1866_INV_FLATTENING 294.9786982

#define ARD_GCTP_GRS80_SEMI_MAJOR 6378137.0
#define ARD_GCTP_GRS80_SEMI_MINOR 6356752.3141
#define ARD_GCTP_GRS80_INV_FLATTENING 298.257222101

#define ARD_GCTP_WGS84_SEMI_MAJOR 6378137.0
#define ARD_GCTP_WGS84_SEMI_MINOR 6356752.3142
#define ARD_GCTP_WGS84_INV_FLATTENING 298.257223563

#endif

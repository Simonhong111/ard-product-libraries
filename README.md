## ARD-PRODUCT-LIBRARIES Version 1.3.0 Release Notes
Release Date: March 2018

The ARD product library project contains libraries and tools for working with the ARD XML metadata. It currently supports Landsat 4-8.

### Support Information
This project is unsupported software provided by the U.S. Geological Survey (USGS) Earth Resources Observation and Science (EROS) Land Satellite Data Systems (LSDS) Project. For questions regarding products produced by this source code, please contact us at custserv@usgs.gov.

### Disclaimer
This software is preliminary or provisional and is subject to revision. It is being provided to meet the need for timely best science. The software has not received final approval by the U.S. Geological Survey (USGS). No warranty, expressed or implied, is made by the USGS or the U.S. Government as to the functionality of the software and related material nor shall the fact of release constitute any such warranty. The software is provided on the condition that neither the USGS nor the U.S. Government shall be held liable for any damages resulting from the authorized or unauthorized use of the software.

### Downloads
ard-product-library source code

    git clone https://github.com/USGS-EROS/ard-product-library.git

See git tag [version_1.3.0]

### Dependencies
  * XML2 libraries -- ftp://xmlsoft.org/libxml2/
  * JBIG libraries -- http://www.cl.cam.ac.uk/~mgk25/jbigkit/
  * LZMA libraries -- http://www.7-zip.org/sdk.html
  * SZIP libraries -- http://www.compressconsult.com/szip/
  * ZLIB libraries -- http://www.zlib.net/
  * TIFF libraries (3.8.2 or most current) -- ftp://ftp.remotesensing.org/pub/libtiff/
  * GeoTIFF libraries (1.2.5 or most current) -- ftp://ftp.remotesensing.org/pub/geotiff/libgeotiff/

### Installation
  * Install dependent libraries.  Many of these come standard with the Linux distribution.
  * Set up environment variables.  Can create an environment shell file or add the following to your bash shell.  For C shell, use 'setenv VAR "directory"'.  
  ```
    export TIFFINC="path_to_TIFF_include_files"
    export TIFFLIB="path_to_TIFF_libraries"
    export GEOTIFF_INC="path_to_GEOTIFF_include_files"
    export GEOTIFF_LIB="path_to_GEOTIFF_libraries"
    export XML2INC="path_to_XML2_include_files"
    export XML2LIB="path_to_XML2_libraries"
    export JBIGINC="path_to_JBIG_include_files"
    export JBIGLIB="path_to_JBIG_libraries"
    export ZLIBINC="path_to_ZLIB_include_files"
    export ZLIBLIB="path_to_ZLIB_libraries"    
    export LZMAINC="path_to_LZMA_include_files"
    export LZMALIB="path_to_LZMA_libraries"
    export ARDINC="path_to_ard_product_library_include_directory"
    export ARDLIB="path_to_ard_product_library_binary_lib_directory"
  ```
  Define $PREFIX to point to the directory in which you want the executables, static data, etc. to be installed.
  ```
    export PREFIX="path_to_directory_for_ard_product_library_build_data"
   ```

* Install ARD product libraries and tools by downloading the source from Downloads above.  Goto the src directory and build the source code there. ARDINC and ARDLIB above refer to the include and lib directories created by building this source code using make followed by make install.

  Note: on some platforms, the JBIG library may be needed for the XML library support, if it isn't already installed.  If so, then the JBIGLIB environment variable needs to point to the location of the JBIG library.

### Linking these libraries for other applications
The following is an example of how to link these libraries into your source
code. Depending on your needs, some of these libraries may not be needed for
your application or other ARD product libraries may need to be added.
```
 -L$(ARDLIB) -l_ard_metadata -l_ard_common \
 -L$(XML2LIB) -lxml2 \
 -L$(JBIGLIB) -ljbig \
 -L$(ZLIBLIB) -lz \
 -L$(LZMALIB) -llzma \
 -lm
```

### Verification Data

### User Manual

### Product Guide 
## Release Notes
  * Changes in this release will help support the Level-3 products such as
    Burned Area and fractional Snow Covered Area (fSCA), which are based on
    temporal stacks of data versus a single product from one particular
    date/time.  These products will only have tile-based metadata and the
    scene-based metadata will not exist or even apply to the product.
  * The acquisition date is an optional element, since the Level-3 products
    often encompass a range of dates versus one single acquisition date.
  * Added a date range as an optional element in the global metadata.
  * Added a description element to allow the author to describe the product
    or point to a URL with more information about the product.
  * Made instrument, scene_count, cloud_cover, cloud_shadow, snow_ice, and fill
    optional elements, since they won't be utilized for the Level-3 products
    based on a temporal stack.

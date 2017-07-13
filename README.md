## ARD-PRODUCT-LIBRARIES Version 1.1.2 Release Notes
Release Date: August 2017

The ARD product library project contains libraries and tools for working with the ARD XML metadata. It currently supports Landsat 4-8.

This project is hosted by the US Geological Survey (USGS) Earth Resources Observation and Science (EROS) Land Satellite Data Systems (LSDS) Science Research and Development (LSRD) Project. For questions regarding this source code, please contact the Landsat Contact Us page and specify USGS CDR/ECV in the "Regarding" section. https://landsat.usgs.gov/contactus.php

### Downloads
ard-product-library source code

    git clone https://github.com/USGS-EROS/ard-product-library.git

See git tag [version_1.1.2]

### Dependencies
  * XML2 libraries -- ftp://xmlsoft.org/libxml2/
  * JBIG libraries -- http://www.cl.cam.ac.uk/~mgk25/jbigkit/
  * LZMA libraries -- http://www.7-zip.org/sdk.html
  * SZIP libraries -- http://www.compressconsult.com/szip/
  * ZLIB libraries -- http://www.zlib.net/

### Installation
  * Install dependent libraries.  Many of these come standard with the Linux distribution.
  * Set up environment variables.  Can create an environment shell file or add the following to your bash shell.  For C shell, use 'setenv VAR "directory"'.  
  ```
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
  * Changed the URL where the ARD schema will officially be located.  This
    location is now https://landsat.usgs.gov/ard/.  The previous http is now
    https.

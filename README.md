## ARD-PRODUCT-LIBRARIES Version 1.0.0 Release Notes
Release Date: March 2017

The ARD product library project contains libraries and tools for working with the ARD XML metadata. It currently supports Landsat 4-8.

This project is hosted by the US Geological Survey (USGS) Earth Resources Observation and Science (EROS) Land Satellite Data Systems (LSDS) Science Research and Development (LSRD) Project. For questions regarding this source code, please contact the Landsat Contact Us page and specify USGS CDR/ECV in the "Regarding" section. https://landsat.usgs.gov/contactus.php

### Downloads
ard-product-library source code

    git clone https://github.com/USGS-EROS/ard-product-library.git

See git tag [version_1.0.0]

### Dependencies
  * GCTP libraries (obtained from the GCTP directory in the HDF-EOS2 source code)
  * CURL libraries (7.48.0 or most current) -- https://curl.haxx.se/download
  * IDN libraries (1.32 or most current) -- ftp://ftp.gnu.org/gnu/libidn
  * JPEG libraries (version 6b) -- http://www.ijg.org/files/
  * ZLIB libraries (version 1.2.8) -- http://zlib.net/
  * XML2 libraries -- ftp://xmlsoft.org/libxml2/
  * JBIG libraries -- http://www.cl.cam.ac.uk/~mgk25/jbigkit/
  * LZMA libraries -- http://www.7-zip.org/sdk.html
  * SZIP libraries -- http://www.compressconsult.com/szip/

NOTE: The HDF-EOS2 link currently provides the source for the HDF4, JPEG, and ZLIB libraries in addition to the HDF-EOS2 library.

### Installation
  * Install dependent libraries.  Many of these come standard with the Linux distribution.
  * Set up environment variables.  Can create an environment shell file or add the following to your bash shell.  For C shell, use 'setenv VAR "directory"'.  Note: If the HDF library was configured and built with szip support, then the user will also need to add an environment variable for SZIP include (SZIPINC) and library (SZIPLIB) files.
  ```
    export XML2INC="path_to_XML2_include_files"
    export XML2LIB="path_to_XML2_libraries"
    export JBIGINC="path_to_JBIG_include_files"
    export JBIGLIB="path_to_JBIG_libraries"
    export ZLIBINC="path_to_ZLIB_include_files"
    export ZLIBLIB="path_to_ZLIB_libraries"    
    export SZIPINC="path_to_SZIP_include_files"
    export SZIPLIB="path_to_SZIP_libraries"    
    export CURLINC="path_to_CURL_include_files"
    export CURLLIB="path_to_CURL_libraries"
    export LZMAINC="path_to_LZMA_include_files"
    export LZMALIB="path_to_LZMA_libraries"
    export IDNINC="path_to_IDN_include_files"
    export IDNLIB="path_to_IDN_libraries"
    export ESPAINC="path_to_format_converter_raw_binary_include_directory"
    export ESPALIB="path_to_format_converter_raw_binary_lib_directory"
  ```
  Define $PREFIX to point to the directory in which you want the executables, static data, etc. to be installed.
  ```
    export PREFIX="path_to_directory_for_format_converter_build_data"
   ```

* Install ARD product libraries and tools by downloading the source from Downloads above.  Goto the src directory and build the source code there. ESPAINC and ESPALIB above refer to the include and lib directories created by building this source code using make followed by make install.

  Note: on some platforms, the JBIG library may be needed for the XML library support, if it isn't already installed.  If so, then the JBIGLIB environment variable needs to point to the location of the JBIG library.

### Linking these libraries for other applications
The following is an example of how to link these libraries into your
source code. Depending on your needs, some of these libraries may not
be needed for your application or other espa product formatter libraries may need to be added.
```
 -L$(ESPALIB) -l_espa_format_conversion -l_espa_raw_binary -l_espa_common \
 -L$(XML2LIB) -lxml2 \
 -L$(HDFEOS_LIB) -lhdfeos -L$(HDFEOS_GCTPLIB) -lGctp \
 -L$(HDFLIB) -lmfhdf -ldf -L$(JPEGLIB) -ljpeg -L$(JBIGLIB) -ljbig \
 -L$(ZLIBLIB) -lz -lm
```

### Verification Data

### User Manual

### Product Guide

## Release Notes
  * Initial version of this software.

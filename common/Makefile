#-----------------------------------------------------------------------------
# Makefile
#
# for ARD common libraries
#-----------------------------------------------------------------------------
.PHONY: all install-headers install-lib install clean

# Inherit from upper-level make.config
TOP = ..
include $(TOP)/make.config

#-----------------------------------------------------------------------------
# Set up compile options
CC    = gcc
RM    = rm
AR    = ar rcsv
EXTRA = -Wall $(EXTRA_OPTIONS)

# Define the include files
INC = ard_common.h ard_error_handler.h

# Define the source code and object files
SRC = \
      ard_error_handler.c
OBJ = $(SRC:.c=.o)

# Define include paths
INCDIR  = -I.
NCFLAGS = $(EXTRA) $(INCDIR)

# Define the object libraries and paths
# Not used in this library only directory
#EXLIB   =
#MATHLIB =
#LOADLIB = $(EXLIB) $(MATHLIB)

# Define the C library/archive
ARCHIVE = lib_ard_common.a

#-----------------------------------------------------------------------------
all: $(ARCHIVE)

$(ARCHIVE): $(OBJ) $(INC)
	$(AR) $(ARCHIVE) $(OBJ)
	install -d ../lib
	install -d ../include
	install -m 644 $(ARCHIVE) ../lib
	install -m 644 $(INC) ../include

#-----------------------------------------------------------------------------
install-headers:
	install -d $(inc_link_path)
	install -d $(ard_library_inc_install_path)
	@for inc in $(INC); do \
        echo "install -m 644 $$inc $(ard_library_inc_install_path)/$$inc"; \
        install -m 644 $$inc $(ard_library_inc_install_path)/$$inc; \
        echo "ln -sf $(ard_library_link_inc_path)/$$inc $(inc_link_path)/$$inc"; \
        ln -sf $(ard_library_link_inc_path)/$$inc $(inc_link_path)/$$inc; \
        done

#-----------------------------------------------------------------------------
install-lib: all
	install -d $(lib_link_path)
	install -d $(ard_library_lib_install_path)
	install -m 644 $(ARCHIVE) $(ard_library_lib_install_path)
	ln -sf $(ard_library_link_lib_path)/$(ARCHIVE) $(lib_link_path)/$(ARCHIVE)

#-----------------------------------------------------------------------------
install: install-lib install-headers

#-----------------------------------------------------------------------------
clean:
	$(RM) -f *.o $(ARCHIVE)

#-----------------------------------------------------------------------------
$(OBJ): $(INC)

.c.o:
	$(CC) $(NCFLAGS) -c $<


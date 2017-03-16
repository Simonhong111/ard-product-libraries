#-----------------------------------------------------------------------------
# Makefile
# for ARD XML tests
#-----------------------------------------------------------------------------
.PHONY: all install clean

# Inherit from upper-level make.config
TOP = ..
include $(TOP)/make.config

#-----------------------------------------------------------------------------
# Set up compile options
CC    = gcc
RM    = rm
EXTRA = -Wall $(EXTRA_OPTIONS)

# Define the include files
INC =

# Define the source code and object files
SRC1 = test_validate_xml.c
OBJ1 = $(SRC1:.c=.o)

SRC2 = test_parse_xml.c
OBJ2 = $(SRC2:.c=.o)

SRC3 = test_write_xml.c
OBJ3 = $(SRC3:.c=.o)


# Define include paths
INCDIR  = -I. -I../include -I$(XML2INC) -I$(JBIGINC) -I$(ZLIBINC)
NETCDF_INCDIR = -I. -I../include -I$(XML2INC)
          
NCFLAGS = $(EXTRA) $(INCDIR)

# Define the object libraries and paths
MATHLIB = -lm

LIB1   = \
    -L../lib -l_ard_metadata -l_ard_common \
    -L$(XML2LIB) -lxml2 \
    -L$(JBIGLIB) -ljbig \
    -L$(ZLIBLIB) -lz \
    -L$(LZMALIB) -llzma \
    $(MATHLIB)

LIB2   = \
    -L../lib -l_ard_metadata -l_ard_common \
    -L$(XML2LIB) -lxml2 \
    -L$(JBIGLIB) -ljbig \
    -L$(ZLIBLIB) -lz \
    -L$(LZMALIB) -llzma \
    $(MATHLIB)

LIB3   = \
    -L../lib -l_ard_metadata -l_ard_common \
    -L$(XML2LIB) -lxml2 \
    -L$(JBIGLIB) -ljbig \
    -L$(ZLIBLIB) -lz \
    -L$(LZMALIB) -llzma \
    $(MATHLIB)

# Define C executables
EXE1 = $(SRC1:.c=)
EXE2 = $(SRC2:.c=)
EXE3 = $(SRC3:.c=)
ALL_EXES = $(EXE1) $(EXE2) $(EXE3)

#-----------------------------------------------------------------------------
all: $(ALL_EXES)

$(EXE1): $(OBJ1) $(INC)
	$(CC) $(NCFLAGS) -o $(EXE1) $(OBJ1) $(LIB1)

$(EXE2): $(OBJ2) $(INC)
	$(CC) $(NCFLAGS) -o $(EXE2) $(OBJ2) $(LIB2)

$(EXE3): $(OBJ3) $(INC)
	$(CC) $(NCFLAGS) -o $(EXE3) $(OBJ3) $(LIB3)

#-----------------------------------------------------------------------------
install: $(ALL_EXES)
	install -d $(bin_link_path)
	install -d $(raw_binary_bin_install_path)
	install -m 755 $(ALL_EXES) $(raw_binary_bin_install_path)
	@for executable in $(ALL_EXES); do \
        echo "ln -sf $(raw_binary_link_bin_path)/$$executable $(bin_link_path)/$$executable"; \
        ln -sf $(raw_binary_link_bin_path)/$$executable $(bin_link_path)/$$executable; \
        done

#-----------------------------------------------------------------------------
clean:
	$(RM) -f *.o $(ALL_EXES)

#-----------------------------------------------------------------------------
$(OBJ1): $(INC)
$(OBJ2): $(INC)
$(OBJ3): $(INC)

.c.o:
	$(CC) $(NCFLAGS) -c $<

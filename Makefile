#-----------------------------------------------------------------------------
# Makefile
#
# Simple makefile for building and installing ard libraries.
#-----------------------------------------------------------------------------
.PHONY: all install-headers install-lib install clean

LIBDIRS = common \
          metadata \
          io_libs
EXEDIRS = test
DIR_SCHEMA = schema

#-----------------------------------------------------------------------------
all: executables

#-----------------------------------------------------------------------------
libraries:
	@for dir in $(LIBDIRS); do \
        echo "make all in $$dir..."; \
        ($(MAKE) -C $$dir || exit 1); done

#-----------------------------------------------------------------------------
executables: libraries
	@for dir in $(EXEDIRS); do \
        echo "make all in $$dir..."; \
        ($(MAKE) -C $$dir || exit 1); done

#-----------------------------------------------------------------------------
install-headers:
# if the ARDINC environment variable points to the 'include' directory, then
# there is no need to install anything
ifneq ($(ARDINC), $(CURDIR)/include)
	@for dir in $(LIBDIRS); do \
        echo "installing all in $$dir..."; \
        ($(MAKE) -C $$dir install-headers || exit 1); done
else
	echo "$(ARDINC) is the same as the include directory. Include files already installed."
endif

#-----------------------------------------------------------------------------
install-lib: libraries
# if the ARDLIB environment variable points to the 'lib' directory, then
# there is no need to install anything
ifneq ($(ARDLIB), $(CURDIR)/lib)
	@for dir in $(LIBDIRS); do \
        echo "installing all in $$dir..."; \
        ($(MAKE) -C $$dir install-lib || exit 1); done
else
	echo "$(ARDLIB) is the same as the lib directory. Libraries already installed."
endif

#-----------------------------------------------------------------------------
install-executables: executables
	@for dir in $(EXEDIRS); do \
        echo "installing all in $$dir..."; \
        ($(MAKE) -C $$dir install || exit 1); done

#-----------------------------------------------------------------------------
install-schema:
	echo "make install in $(DIR_SCHEMA)"; \
        ($(MAKE) -C $(DIR_SCHEMA) install || exit 1)

#-----------------------------------------------------------------------------
install: install-lib install-headers install-executables install-schema

#-----------------------------------------------------------------------------
clean:
# all directories need to be cleaned
	@for dir in $(LIBDIRS) $(EXEDIRS); do \
        echo "make clean in $$dir..."; \
        ($(MAKE) -C $$dir clean || exit 1); done
	rm -r include lib

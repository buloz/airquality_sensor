#
# Main component makefile.
#
# This Makefile can be left empty. By default, it will take the sources in the 
# src/ directory, compile them and link them into lib(subdirectory_name).a 
# in the build directory. This behaviour is entirely configurable,
# please read the ESP-IDF documents if you need to do this.
#

COMPONENT_SRCDIRS := .
COMPONENT_ADD_INCLUDEDIRS := include

LIBS := algobsec

COMPONENT_ADD_LDFLAGS     := -lbt -L $(COMPONENT_PATH) \
                           $(addprefix -l,$(LIBS))
ALL_LIB_FILES := $(patsubst %,$(COMPONENT_PATH)/lib%.a,$(LIBS))
COMPONENT_ADD_LINKER_DEPS := $(ALL_LIB_FILES)
COMPONENT_SUBMODULES += lib

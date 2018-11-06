# This file is generated by gyp; do not edit.

TOOLSET := target
TARGET := standalone
DEFS_Debug := \
	'-DNODE_GYP_MODULE_NAME=standalone' \
	'-DUSING_UV_SHARED=1' \
	'-DUSING_V8_SHARED=1' \
	'-DV8_DEPRECATION_WARNINGS=1' \
	'-D_DARWIN_USE_64_BIT_INODE=1' \
	'-D_LARGEFILE_SOURCE' \
	'-D_FILE_OFFSET_BITS=64' \
	'-DDEBUG' \
	'-D_DEBUG' \
	'-DV8_ENABLE_CHECKS'

# Flags passed to all source files.
CFLAGS_Debug := \
	-O0 \
	-gdwarf-2 \
	-mmacosx-version-min=10.7 \
	-arch x86_64 \
	-Wall \
	-Wendif-labels \
	-W \
	-Wno-unused-parameter

# Flags passed to only C files.
CFLAGS_C_Debug := \
	-fno-strict-aliasing

# Flags passed to only C++ files.
CFLAGS_CC_Debug := \
	-std=gnu++0x \
	-stdlib=libc++ \
	-fno-rtti \
	-fno-threadsafe-statics \
	-std=c++14 \
	-stdlib=libc++

# Flags passed to only ObjC files.
CFLAGS_OBJC_Debug :=

# Flags passed to only ObjC++ files.
CFLAGS_OBJCC_Debug :=

INCS_Debug := \
	-I/Users/alexsanchez/.node-gyp/8.12.0/include/node \
	-I/Users/alexsanchez/.node-gyp/8.12.0/src \
	-I/Users/alexsanchez/.node-gyp/8.12.0/deps/openssl/config \
	-I/Users/alexsanchez/.node-gyp/8.12.0/deps/openssl/openssl/include \
	-I/Users/alexsanchez/.node-gyp/8.12.0/deps/uv/include \
	-I/Users/alexsanchez/.node-gyp/8.12.0/deps/zlib \
	-I/Users/alexsanchez/.node-gyp/8.12.0/deps/v8/include \
	-I$(srcdir)/.

DEFS_Release := \
	'-DNODE_GYP_MODULE_NAME=standalone' \
	'-DUSING_UV_SHARED=1' \
	'-DUSING_V8_SHARED=1' \
	'-DV8_DEPRECATION_WARNINGS=1' \
	'-D_DARWIN_USE_64_BIT_INODE=1' \
	'-D_LARGEFILE_SOURCE' \
	'-D_FILE_OFFSET_BITS=64'

# Flags passed to all source files.
CFLAGS_Release := \
	-Os \
	-gdwarf-2 \
	-mmacosx-version-min=10.7 \
	-arch x86_64 \
	-Wall \
	-Wendif-labels \
	-W \
	-Wno-unused-parameter

# Flags passed to only C files.
CFLAGS_C_Release := \
	-fno-strict-aliasing

# Flags passed to only C++ files.
CFLAGS_CC_Release := \
	-std=gnu++0x \
	-stdlib=libc++ \
	-fno-rtti \
	-fno-threadsafe-statics \
	-std=c++14 \
	-stdlib=libc++

# Flags passed to only ObjC files.
CFLAGS_OBJC_Release :=

# Flags passed to only ObjC++ files.
CFLAGS_OBJCC_Release :=

INCS_Release := \
	-I/Users/alexsanchez/.node-gyp/8.12.0/include/node \
	-I/Users/alexsanchez/.node-gyp/8.12.0/src \
	-I/Users/alexsanchez/.node-gyp/8.12.0/deps/openssl/config \
	-I/Users/alexsanchez/.node-gyp/8.12.0/deps/openssl/openssl/include \
	-I/Users/alexsanchez/.node-gyp/8.12.0/deps/uv/include \
	-I/Users/alexsanchez/.node-gyp/8.12.0/deps/zlib \
	-I/Users/alexsanchez/.node-gyp/8.12.0/deps/v8/include \
	-I$(srcdir)/.

OBJS := \
	$(obj).target/$(TARGET)/main.o \
	$(obj).target/$(TARGET)/MoveProcessor.o \
	$(obj).target/$(TARGET)/MovementMove.o \
	$(obj).target/$(TARGET)/Piece.o \
	$(obj).target/$(TARGET)/ArmyPiece.o \
	$(obj).target/$(TARGET)/FleetPiece.o \
	$(obj).target/$(TARGET)/ConvoyMove.o \
	$(obj).target/$(TARGET)/Graph.o \
	$(obj).target/$(TARGET)/HoldMove.o \
	$(obj).target/$(TARGET)/Move.o \
	$(obj).target/$(TARGET)/SupportMove.o \
	$(obj).target/$(TARGET)/Territory.o \
	$(obj).target/$(TARGET)/ChildProcess.o

# Add to the list of files we specially track dependencies for.
all_deps += $(OBJS)

# CFLAGS et al overrides must be target-local.
# See "Target-specific Variable Values" in the GNU Make manual.
$(OBJS): TOOLSET := $(TOOLSET)
$(OBJS): GYP_CFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_C_$(BUILDTYPE))
$(OBJS): GYP_CXXFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_CC_$(BUILDTYPE))
$(OBJS): GYP_OBJCFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_C_$(BUILDTYPE)) $(CFLAGS_OBJC_$(BUILDTYPE))
$(OBJS): GYP_OBJCXXFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_CC_$(BUILDTYPE)) $(CFLAGS_OBJCC_$(BUILDTYPE))

# Suffix rules, putting all outputs into $(obj).

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(srcdir)/%.cpp FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# Try building from generated source, too.

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj).$(TOOLSET)/%.cpp FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj)/%.cpp FORCE_DO_CMD
	@$(call do_cmd,cxx,1)

# End of this set of suffix rules
### Rules for final target.
LDFLAGS_Debug := \
	-stdlib=libc++ \
	-Wl,-no_pie \
	-Wl,-search_paths_first \
	-mmacosx-version-min=10.7 \
	-arch x86_64 \
	-L$(builddir) \
	-stdlib=libc++

LIBTOOLFLAGS_Debug := \
	-stdlib=libc++ \
	-Wl,-no_pie \
	-Wl,-search_paths_first

LDFLAGS_Release := \
	-stdlib=libc++ \
	-Wl,-no_pie \
	-Wl,-search_paths_first \
	-mmacosx-version-min=10.7 \
	-arch x86_64 \
	-L$(builddir) \
	-stdlib=libc++

LIBTOOLFLAGS_Release := \
	-stdlib=libc++ \
	-Wl,-no_pie \
	-Wl,-search_paths_first

LIBS :=

$(builddir)/standalone: GYP_LDFLAGS := $(LDFLAGS_$(BUILDTYPE))
$(builddir)/standalone: LIBS := $(LIBS)
$(builddir)/standalone: GYP_LIBTOOLFLAGS := $(LIBTOOLFLAGS_$(BUILDTYPE))
$(builddir)/standalone: LD_INPUTS := $(OBJS)
$(builddir)/standalone: TOOLSET := $(TOOLSET)
$(builddir)/standalone: $(OBJS) FORCE_DO_CMD
	$(call do_cmd,link)

all_deps += $(builddir)/standalone
# Add target alias
.PHONY: standalone
standalone: $(builddir)/standalone

# Add executable to "all" target.
.PHONY: all
all: $(builddir)/standalone

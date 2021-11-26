%global maj_ver 14
%global min_ver 0
%global patch_ver 0
#.global rc_ver 0
%global pre_ver 3

%global pkg_bindir %{_bindir}
%global pkg_libdir %{_libdir}

%global lib64dir %{_prefix}/lib64

Name:           llvm
Version:    	%{maj_ver}.%{min_ver}.%{patch_ver}%{?rc_ver:~rc%{rc_ver}}%{?pre_ver:~pre%{pre_ver}}
Release:        1%{?dist}
Summary:	    The Low Level Virtual Machine

License:        NCSA
URL:    		http://llvm.org

Source0:        llvm-14.0.0-irix-sgug.tar

Requires:	libcxx = %{version}-%{release}
Requires:	libunwind = %{version}-%{release}

# need to fix this, .so requires are showing up, but provides aren't
AutoReqProv:    no

#BuildRequires:  

%description
LLVM is a compiler infrastructure designed for compile-time, link-time,
runtime, and idle-time optimization of programs from arbitrary programming
languages. The compiler infrastructure includes mirror sets of programming
tools as well as libraries with equivalent functionality.

%package -n clang
Summary:    LLVM project C/C++ compiler
Requires:	libunwind = %{version}-%{release}
Requires:	libcxx = %{version}-%{release}
Requires:	llvm = %{version}-%{release}
Requires:	lld = %{version}-%{release}
Requires:   clang-irix-includes-6530 = %{version}-%{release}

%description -n clang
LLVM project C/C++ compiler

%package -n clang-devel
Summary:    LLVM project C/C++ compiler (development files)
Requires:	clang = %{version}-%{release}

%description -n clang-devel
LLVM project C/C++ compiler (development files)

%package -n lld
Summary:    LLVM project linker
Requires:	libunwind = %{version}-%{release}
Requires:	libcxx = %{version}-%{release}
Requires:	llvm = %{version}-%{release}

%description -n lld
LLVM project linker

%package -n lld-devel
Summary:    LLVM project linker (development files)
Requires:	lld = %{version}-%{release}

%description -n lld-devel
LLVM project linker (development files)

%package -n clang-irix-includes-6530
Summary:    Fixed include files for IRIX 6.5.30
Requires:	libunwind = %{version}-%{release}
Requires:	libcxx = %{version}-%{release}
Requires:	llvm = %{version}-%{release}
Provides:   clang-irix-includes(major) = %{maj_ver}

%description -n clang-irix-includes-6530
Fixed include files for Clang on IRIX 6.5.30

%package -n libcxx
Summary:    LLVM project implementation of the C++ standard library
Requires:	libunwind = %{version}-%{release}
# TODO vlad -- not sure why automatic provides are broken
Provides:   libc++.so.1

%description -n libcxx
LLVM project implementation of the C++ standard library

%package -n libcxx-devel
Summary:    LLVM project implementation of the C++ standard library (development files)
Requires:	libcxx = %{version}-%{release}

%description -n libcxx-devel
LLVM project implementation of the C++ standard library (development files)

%package -n libunwind
Summary:    LLVM project implementation of unwind library
Provides:   libunwind.so.1

%description -n libunwind
LLVM project implementation of unwind library

%package devel
Summary:	Libraries and header files for LLVM
Requires:	%{name}%{?_isa} = %{version}-%{release}
Requires:	%{name}-libs%{?_isa} = %{version}-%{release}
# The installed LLVM cmake files will add -ledit to the linker flags for any
# app that requires the libLLVMLineEditor, so we need to make sure
# libedit-devel is available.
#Requires:	libedit-devel
# The installed cmake files reference binaries from llvm-test and llvm-static.
# We tried in the past to split the cmake exports for these binaries out into
# separate files, so that llvm-devel would not need to Require these packages,
# but this caused bugs (rhbz#1773678) and forced us to carry two non-upstream
# patches.
#Requires:	%{name}-static%{?_isa} = %{version}-%{release}
#Requires:	%{name}-test%{?_isa} = %{version}-%{release}
 
###Requires(post):	%{_sbindir}/alternatives
##Requires(postun):	%{_sbindir}/alternatives
 
Provides:	llvm-devel(major) = %{maj_ver}
 
%description devel
This package contains library and header files needed to develop new native
programs that use the LLVM infrastructure.
 
%package doc
Summary:	Documentation for LLVM
BuildArch:	noarch
Requires:	%{name} = %{version}-%{release}
 
%description doc
Documentation for the LLVM compiler infrastructure.
 
%package libs
Summary:	LLVM shared libraries
 
%description libs
Shared libraries for the LLVM compiler infrastructure.
 
%package static
Summary:	LLVM static libraries
Conflicts:	%{name}-devel < 8
 
Provides:	llvm-static(major) = %{maj_ver}
 
%description static
Static libraries for the LLVM compiler infrastructure.

%prep
%setup -q -c

%build
echo No build

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT
mv usr $RPM_BUILD_ROOT

# we don't care about a bunch of things
rm $RPM_BUILD_ROOT%{_bindir}/ld64*
rm $RPM_BUILD_ROOT%{_bindir}/c-index-test

echo PREFIX %{_prefix}
echo BINDIR %{_bindir}
echo LIBDIR %{_libdir}
echo LIB64DIR %{lib64dir}


%files
#%license LICENSE.TXT
# vlad - build man pages
###%exclude %{_mandir}/man1/llvm-config*
###%{_mandir}/man1/*
%{_bindir}/lli
%{_bindir}/opt
%{_bindir}/wasm-ld
%{_bindir}/llvm-*
%{_bindir}/dsymutil
%{_bindir}/diagtool
%{_bindir}/find-all-symbols
%{_bindir}/hmaptool
%{_bindir}/modularize
%{_bindir}/pp-trace
%{_bindir}/sancov
%{_bindir}/sanstats
%exclude %{_bindir}/llvm-config
###%exclude %{pkg_bindir}/llvm-config-%{__isa_bits}
###%exclude %{_bindir}/llvm-config-%{maj_ver}
###%exclude %{pkg_bindir}/llvm-config-%{maj_ver}-%{__isa_bits}
#%exclude %{_bindir}/not
#%exclude %{_bindir}/count
#%exclude %{_bindir}/yaml-bench
#%exclude %{_bindir}/lli-child-target
#%exclude %{_bindir}/llvm-isel-fuzzer
#%exclude %{_bindir}/llvm-opt-fuzzer
%{_datadir}/opt-viewer

%files -n lld
%{_bindir}/lld
%{_bindir}/ld.lld
%{_bindir}/llc
%{_bindir}/lld-link

%files -n lld-devel
%{_includedir}/lld
%{_libdir}/cmake/lld

%files -n clang
%{_bindir}/clang*
%{_bindir}/run-clang-tidy
%{_bindir}/git-clang-format
%{_libdir}/clang/%{maj_ver}.%{min_ver}.%{patch_ver}
%exclude %{_libdir}/clang/%{maj_ver}.%{min_ver}.%{patch_ver}/include-fixed
%{_datadir}/clang

%files -n clang-devel
%{_bindir}/run-clang-tidy
%{_bindir}/git-clang-format
%{_datadir}/clang
%{_includedir}/clang
%{_includedir}/clang-c
%{_includedir}/clang-tidy
%{_libdir}/cmake/clang
%{_libdir}/libclang-cpp.*
%{_libdir}/libclang.*

%files -n clang-irix-includes-6530
%{_libdir}/clang/%{maj_ver}.%{min_ver}.%{patch_ver}/include-fixed

%files -n libcxx
%{_libdir}/libc++*
%{lib64dir}/libc++*

%files -n libcxx-devel
%{_includedir}/c++/v1
%{_includedir}/mips64-sgi-irix6.5-gnuabin32/c++
%{_includedir}/mips64-sgi-irix6.5/c++

%files -n libunwind
%{_libdir}/libunwind*
%{lib64dir}/libunwind*

%files libs
#%license LICENSE.TXT
###%{pkg_libdir}/libLLVM-%{maj_ver}.so
###%{_libdir}/libLLVM-%{maj_ver}.%{min_ver}*.so
###%{_libdir}/libLLVM-%{maj_ver}.so%{?abi_revision:.%{abi_revision}}
%{_libdir}/libLTO.so*
%{pkg_libdir}/libRemarks.so*
 
%files devel
#%license LICENSE.TXT

# we install llvm-config as-is, without a version
###vlad#%ghost %{_bindir}/llvm-config
%{_bindir}/bugpoint
%{_bindir}/llvm-config
%{_bindir}/modularize
%{_bindir}/pp-trace
%{_bindir}/hmaptool
%{_bindir}/split-file
%{_bindir}/verify-uselistorder
###vlad#%{pkg_bindir}/llvm-config-%{__isa_bits}
###vlad#%{_mandir}/man1/llvm-config*
%{_includedir}/llvm
%{_includedir}/llvm-c
###%{_libdir}/libLLVM.so
%{_libdir}/cmake/llvm
###%{pkg_bindir}/llvm-config-%{maj_ver}-%{__isa_bits}
###%ghost %{_bindir}/llvm-config-%{maj_ver}
 
%files doc
#%license LICENSE.TXT
#%doc %{_pkgdocdir}/html
 
%files static
#%license LICENSE.TXT
%{_libdir}/*.a
%exclude %{_libdir}/libc++*
%exclude %{_libdir}/libunwind*
%exclude %{_libdir}/libLLVMTestingSupport.a
 
%changelog


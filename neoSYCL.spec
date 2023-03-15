%global _missing_build_ids_terminate_build 0
%define debug_package %{nil}
%define _prefix /opt/nec/ve/neosycl
Name:           neoSYCL
Version:        0.1
Release:        1%{?dist}
Summary:        neoSYCL a SYCL Implementation for SX-Aurora TUBASA
Group: 		System Environment/Libraries
License:        MIT
Source: 	%{name}-%{version}.tar.gz
Vendor:         NEC Corporation
BuildArch: 	x86_64
Url: 		https://github.com/veos-sxarr-NEC/neoSYCL

%description
neoSYCL is a SYCL Implementation fot SX-Aurora TSUBASA which supports a combination of VH (host code) and VE (device code) and offloads calculations for vectors to VE through AVEO.

%prep
%setup -q -n %{name}-%{version}


%build
mkdir build; cd build
cmake ../ -DBUILD_KERNEL_OUTLINER=ON -DBUILD_VE=ON -DCMAKE_INSTALL_PREFIX=%{_prefix}
make

%install
cd build
%make_install
echo ${RPM_BUILD_ROOT}
mkdir -p ${RPM_BUILD_ROOT}/opt/nec/ve/bin
ln -s %{_prefix}/bin/nsc++ ${RPM_BUILD_ROOT}/opt/nec/ve/bin/nsc++


%files
%defattr(-,root,root,-)
%{_prefix}/bin/nsc++
%{_prefix}/bin/kout
%{_prefix}/include/CL
%{_prefix}/include/neoSYCL
%{_prefix}/../bin/nsc++

%changelog

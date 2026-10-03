%bcond_with unittest
# To make the _libdir can get the right place on the debian
# 1. Shell out to dpkg-architecture to dynamically find the path
%global deb_multiarch %(dpkg-architecture -qDEB_HOST_MULTIARCH 2>/dev/null || echo "")

# 2. Redefine _libdir based on the result
%if "%{deb_multiarch}" != ""
%global _libdir %{_prefix}/lib/%{deb_multiarch}

%endif
Name:       libqmlchewing_plugin

%define debug_package %{nil}

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}
Summary:    Bridge between QML and chewing
Version:    0.1
Release:    3
Group:      Qt/Qt
License:    LGPLv2
URL:        https://github.com/hanhsuan/libqmlchewing-plug
Source0:    %{name}-%{version}.tar.xz
Requires:   (libchewing or libchewing3)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(chewing)

%description
%{summary}.

%prep

%build
%if %{with unittest}
%qtc_qmake5 ./project.pro CONFIG+=unittest
%else
%qtc_qmake5 ./project.pro
%endif
%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}
make install INSTALL_ROOT=%{buildroot}

%files
%defattr(-,root,root,-)
%{_libdir}/qt5/qml/H/H/chewing/libqmlchewing.so
%{_libdir}/qt5/qml/H/H/chewing/qmldir

Name:		manual_h2_client
Version:	0.1.6
Release:	1%{?dist}
Summary:	manual_h2_client

License:	BSD-3-Clause License
URL:		https://github.com/k-noya/%{name}
Source:		https://github.com/k-noya/%{name}/archive/refs/tags/v%{version}.tar.gz

BuildRequires:	gcc-c++, openssl, openssl-devel, cmake, gtest
Requires:	openssl

%description
manual_h2_client is a simple HTTP/2 client library which assembles the HTTP/2 frames manually.
The purpose of the library is to debug a HTTP/2 server.

%prep
%setup -q

%build
cmake -B build \
  -DBUILD_SHARED_LIBS=ON \
  -DCMAKE_INSTALL_LIBDIR=%{buildroot}%{_libdir} \
  -DCMAKE_INSTALL_INCLUDEDIR=i%{buildroot}%{_includedir}
cmake --build build
cmake --build build --target test

%install
cmake --build build --target install

%files
%license LICENSE
%{_libdir}/libmh2c.so
%{_libdir}/libmh2c.so.%{version}

%changelog

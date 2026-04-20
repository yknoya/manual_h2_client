Name:           manual_h2_client
Version:        0.1.6
Release:        1%{?dist}
Summary:        Manual HTTP/2 client library for debugging servers

License:        BSD-3-Clause
URL:            https://github.com/yknoya/%{name}
Source0:        %{url}/archive/refs/tags/v%{version}.tar.gz

BuildRequires:  cmake
BuildRequires:  gcc-c++
BuildRequires:  gtest-devel
BuildRequires:  make
BuildRequires:  openssl-devel

%description
manual_h2_client is a simple HTTP/2 client library which assembles the
HTTP/2 frames manually. The purpose of the library is to debug a HTTP/2
server.

%package devel
Summary:        Development files for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description devel
Headers and development files for building applications against
manual_h2_client.

%prep
%autosetup -n %{name}-%{version}

%build
cmake -S . -B build \
  -DBUILD_SHARED_LIBS=ON \
  -DBUILD_TESTING=ON \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_INSTALL_PREFIX=%{_prefix}
cmake --build build --parallel %{?_smp_build_ncpus}

%install
DESTDIR=%{buildroot} cmake --install build

%check
export LD_LIBRARY_PATH="$PWD/build/lib${LD_LIBRARY_PATH:+:${LD_LIBRARY_PATH}}"
ctest --test-dir build --output-on-failure --parallel %{?_smp_build_ncpus}

%files
%license LICENSE
%doc README.md
%{_libdir}/libmh2c.so.*

%files devel
%{_includedir}/mh2c/
%{_libdir}/libmh2c.so

%changelog

Summary: sense - kill a shellscript after some time
Name: sense
Version: 0.0.1
Release: 1leitrix1
License: Public Domain
Group: System/Tools
URL: http://www.bl0rg.net/software/
Source: sense.tar.gz
BuildRoot: %{_tmppath}/%{name}-%{version}-root

%description
sense - kill a shellscript after some time

%prep
%setup -q -n %{name}

%build
make

%install
rm -rf %{buildroot}
install -d %{buildroot}/usr/bin
install -m 0755 sense %{buildroot}/usr/bin

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%clean

%files
%attr(-,root,root)
/usr/bin/sense

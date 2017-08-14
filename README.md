# OpenEAGGR (Open Equal Area Global GRid)

<img src="Images/OpenEAGGR_logo-01.png" width="200" height="200">

The OpenEAGGR software library is an implementation of a Discrete Global Grid System (DGGS) which models the Earth's surface as a network of equal area cells. Cell networks are available at increasingly finer resolutions to allow locations to be represented by cells up to the order of one square centimetre in area.  The library provides the ability to convert in both directions between WGS84 latitude/longitude locations and cell identifiers. 

Two DGGS implementations are currently supported both using an icosahedron as the Earth model and the Snyder Equal Area projection to map points on the Earth to locations on the icosahedron.  ISEA4T uses an aperture 4 triangular cell system to partition the icosahedron faces along with a hierarchical indexing system to generate the cell Ids. ISEA3H uses an aperture 3 hexagonal cell system with an offset co-ordinate indexing system. 

The OpenEAGGR library includes the following APIs:
- [C/C++](EAGGR/API/eaggr_api.h)
- [Java](EAGGRJava/src/uk/co/riskaware/eaggr/Eaggr.java)
- [Python 2/3](EAGGRPython/eaggr/eaggr.py)

The OpenEAGGR library has been integrated as an extension/plug-in for the following third party software:
- [PostgreSQL/PostGIS](https://www.postgresql.org/)
- [Elasticsearch](http://www.elastic.co/products/elasticsearch)

Full technical details and API examples can be found in the documentation:
- [Software Design Document] (Documents/Software Design Document.pdf)
- [Programmer's Guide] (Documents/Programmer's Guide.pdf)

## Prerequisites
- [MinGW](http://www.mingw.org/) 32 and 64 bit downloads available from [32 bit]( http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win32/Personal%20Builds/mingw-builds/5.1.0/threads-posix/dwarf/) and [64 bit]( http://sourceforge.net/projects/mingw-w64/files/Toolchains%20targetting%20Win64/Personal%20Builds/mingw-builds/5.1.0/threads-posix/seh/) respectively
- [Eclipse Mars CDT](https://eclipse.org/cdt/) with Java and PyDev plugins
- [Java SE 8] (http://www.oracle.com/technetwork/java/javase/downloads/index.html) if using the Java API
- [Python 2 and/or 3] (https://www.python.org/downloads/) if using the python APIs
- [PostgreSQL 9.5](https://www.postgresql.org/) and (optionally) [PostGIS](http://www.postgis.net/) if using the PostGIS extension
- [Elasticsearch 2.4.0](http://www.elastic.co/products/elasticsearch) if using the Elasticsearch plug-in

## Build Instructions

The OpenEAGGR library can be built for 32 or 64 bit Windows or 64 bit Linux. These build instructions are based on building for Windows 7 and Scientific Linux 7.1 (which is derived from RedHat Linux).

For C and C++ projects there are different Windows and Linux equivalents. Where specific projects are mentioned in these instructions the Windows projects are used. If building on Linux use the equivalent projects (suffixed by Linux, for example EAGGR -> EAGGRLinux).

### Install prerequisites

#### Windows prerequisites
The OpenEAGGR project uses MinGW-w64 to build the C++ library and C API in Eclipse. MinGW-w64 includes both 32 and 64 bit versions of MinGW.

1. Download the MinGW compiler(s) as obtained from the links in the pre-requisites.
  - i686-5.1.0-posix-dwarf-rt_v4-rev0.zip - 32-bit
  - x86_64-5.1.0-posix-seh-rt_v4-rev0.zip - 64-bit
2. Extract your chosen zip file to a location on the local machine (e.g. C:\mingw-w64\i686-5.1.0-posix-dwarf-rt_v4-rev0\). It is very important that the path does not include any spaces.
3. Create a new user environment variable called MINGW_HOME and set it to the "mingw32" directory location (e.g. C:\mingw-w64\i686-5.1.0-posix-dwarf-rt_v4-rev0\mingw32).
4. Use the new MINGW_HOME variable to add the "bin" directory location to your user's PATH environment variable (i.e. %MINGW_HOME%\bin).
5. Check MinGW-w64 installed successfully.
  1. Open a command prompt and run: gcc --version.
  2. Check the command does not give any errors. Ensure the version is "5.1.0" and the build type matches the build of MinGW-w64 you selected.
  3. If the version number or build type is not what was expected, check the user and system PATH environment variables for paths to other versions of MinGW and MinGW-w64.

#### Linux prerequisites

1. Open a terminal window.  Install the development tools using 'sudo yum groupinstall "Development Tools" "Development Libraries" "Additional Development"'.

#### Java and Python prerequisites
If you wish to build the Java and Python APIs, perform these extra install steps which relate to the APIs you wish to build (steps apply to both Windows and Linux).

1. Install the Java JDK.
2. Install Python 2.
  1. If you wish to run the tests with Python 2, open a command prompt and run: PATH_TO_PYTHON2.7\Scripts\pip.exe install xmlrunner.
  2. If you wish to package the module into a wheel file, open a command prompt and run: PATH_TO_PYTHON2.7\Scripts\pip.exe install wheel.
3. Install Python 3.
  1. If you wish to run the tests with Python 3, open a command prompt and run: PATH_TO_PYTHON3.4\Scripts\pip.exe install xmlrunner.

### Use the OpenEAGGR codebase in Eclipse

1. Download eclipse and install the Java and PyDev plugins (if required). If wishing to run the unit tests install the C/C++ Unit Testing Support package in Help->Install new software... using the http://download.eclipse.org/releases/mars update site.
2. Run Eclipse and import the EAGGR and EAGGRTestHarness projects (or the Linux versions if building on Linux).  If using the Java or Python projects then also import the EAGGRJava, EAGGRJavaJarTest, EAGGRPython and EAGGRPythonWheelTest projects as required.
  1. When importing the Python projects you may be prompted to configure PyDev for the version of Python you are using.  If you only have one version of Python installed then it should be fine to select "Quick Autoconfig", however if you have multiple versions of Python it is recommended to set the configuration manually.
3. Open the C/C++ perspective.
4. Set the build configuration to Release or Release64, depending on whether you are building for 32-bit or 64-bit platform.
5. Right click the EAGGR project and select 'Build Project'.  Repeat for the EAGGRTestHarness project.
6. If using the Java projects
  1. Ensure that the correct version of Java is being used.  Right click on the EAGGRJava project and select 'Properties'.  Select 'Java Build Path' in the left hand pane and ensure that the JRE System Library is set to a JDK and is the expected version.  Ensure that the JAVA_HOME environment variable is also set to this version of the JDK.
  2. Right click on the EAGGRJava project and select 'Build Project'.
  3. Under the EAGGRJava project, right click on the build.xml file and select 'RunAs...->Ant Build'.  This will perform some tasks that Eclipse does not, for example building the jar file and generating the javadoc files.
  4. Right click on the EAGGRJava project and select 'Refresh'.  This will ensure that the project picks up the jar file from the previous step.  Right click on the project again and select 'Build Project'.
7. If using the PostgreSQL extension
  1. Import the EAGGRPostgres and EAGGRPostgresTestHarness projects.
  2. Right click the EAGGRPostgres project and select 'Build Project'.  Repeat for the EAGGRPostgresTestHarness project.
8. If using the Elasticsearch plug-in
  1. Import the EAGGRElasticsearch project.
  2. Under the EAGGRElasticsearch project, right click on the build.xml file and select 'RunAs...->Ant Build...'. Select the 'deploy' target and run. This will create the jar files that make up the Elasticsearch plug-in and create a zip file containing the jars and configuration files in EAGGRElasticsearch/deploy.

Note: There is no build step required for the Python projects because Python is an interpreted language.

## Integrating the PostgreSQL and Elasticsearch extensions

### PostgreSQL

1. Install the PostgreSQL and (optional) PostGIS prerequisites
  1. Note that the code currently builds against PostgreSQL 9.5.x. It is not possible to use the built extension with any other version. To build against other versions it is necessary to update the EAGGRPostgres/3rdParty/include/postgres and EAGGRPostgres/3rdParty/lib/postgres directories, although the extension has not been tested against any other version.
2. Copy the libraries that are created in the EAGGRPostgres/deploy/CONFIGURATION directory and other supporting files into the PostgreSQL installation as follows. The default installation directory is likely to be PROGRAM FILES/PostgreSQL/9.5. Note that administrator permissions are required to copy files to the Program Files folder.
  1. Copy the EAGGRPostgres/eaggr--x.x.sql and EAGGRPostgres/eaggr.control files to the PostgreSQL share/extension directory.
  2. Copy the lib_eaggr_postgres.dll to the lib directory of PostgreSQL.
  3. Copy the other DLLs to the PostgreSQL bin folder (for Windows) or the /usr/lib64/ directory (for Linux). Note that with PostGIS installed, some of the OpenEAGGR dependency DLLs conflict with the PostGIS DLLs. Ideally PostgreSQL and PostGIS would be recompiled with the same compiler as used to build the OpenEAGGR library, including all options for the third party libraries. If this is not possible then, since the OpenEAGGR DLLs compile against a more complete version of these DLLs, it may be sufficient to back-up the existing DLLs and overwrite them with the ones built with OpenEAGGR. These DLLs are:
    - libgcc_s_seh-1.dll
    - libgdal-20.dll
    - libproj-9.dll
    - libstdc++-6.dll
  4. On Linux copy the EAGGRPostgresLinux/eaggr-x86_64.conf file to /etc/ld.so.conf.d so that the libeaggr.so file will be added to the path.
  5. Note that the PostgreSQL bin directory should be available on the Windows PATH environment variable (usually done by the PostgreSQL installer).
3. For each database that requires the OpenEAGGR extension, open an SQL interface to the database and run the command `CREATE EXTENSION eaggr`.

### Elasticsearch

1. Install Elasticsearch (either download from [Elasticsearch](http://www.elastic.co/products/elasticsearch) or a distribution of version 2.4.0 is available in DGGSElasticsearch/testData/elasticsearch)
  1. Note that the code currently builds against Elasticsearch 2.4.0. It is not possible to use the built plug-in with any other version of Elasticsearch. To build against other versions it is necessary to update the libraries in EAGGRElasticsearch/lib and update the elasticsearch.version property in the plugin-descriptor.properties file, although the plug-in has not been tested against any other version.
2. Open a command window in the bin directory of the Elasticsearch installation. Run 'plugin install file:///PATH_TO_DEPLOY_FOLDER' where PATH_TO_DEPLOY_FOLDER is the path to the EAGGRElasticsearch/deploy folder. The installation process will ask for acceptance of the permissions requested by the plug-in.
3. Elasticsearch can be launched using the batch file in the bin directory of the Elasticsearch distribution.

## Integrating the OpenEAGGR library into client applications

The OpenEAGGR library builds to a .dll/.so shared library with a C API, a .a library file and a set of header files.  Examples of the use of the C API can be found in the system tests that test the API in the source code at EAGGRTestHarness\SystemTests\DLL\DLLTest.cpp.

The Java API for the OpenEAGGR library builds to a .jar file to be included in the client java application.  The java classpath must include the dggs.jar, jna-4.1.0.jar and the folder containing the OpenEAGGR native library.  Examples of the use of the Java API can be found in the unit tests in the source code at EAGGRJava\tests\uk\co\riskaware\eaggr\EaggrTest.java.

The Python API can be packaged as a wheel (.whl) file, which includes everything required to use the OpenEAGGR library in Python, including the DLL.  The wheel file can be installed using the pip Python package manager, and once installed the classes can be imported into the client application like any other Python module.  Examples of the use of the Python API can be found in the unit tests in the source code at EAGGRPython\tests\uk\co\riskaware\eaggr\eaggr_test.py.

The OpenEAGGR library DLL, C API header, Java jar and Python wheel files can be found in the delivery in the \Built Library folder.

## Licencing

The OpenEAGGR library is supplied under the [LGPL v3](COPYING.LESSER) subject to the [End User License Agreement](EULA.md). 

The OpenEAGGR library depends on the following open-source libraries:

| Library                         | Licence    |
| ------------------------------- | ---------- |
| GDAL                            | [X11/MIT](https://opensource.org/licenses/MIT)    |
| JNA                             | [LGPL 2.1](https://opensource.org/licenses/LGPL-2.1)   |
| Proj4                           | [X11/MIT](https://opensource.org/licenses/MIT)    |
| Python (unittest-xml-reporting) | [BSD](https://opensource.org/licenses/BSD-2-Clause)        |
| GoogleTest                      | [BSD](https://opensource.org/licenses/BSD-2-Clause)        |
| Boost                           | [Boost Software License](http://www.boost.org/)        |
| PostgreSQL                      | [PostgreSQL License (similar to BSD/MIT)](https://www.postgresql.org/about/licence/)        |
| Elasticsearch                   | [Apache License](https://opensource.org/licenses/Apache-2.0)        |

## Support

For any support requests or questions about OpenEAGGR please contact openeaggr@riskaware.co.uk.
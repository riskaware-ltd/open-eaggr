#!lua
conan_build_type = "Release"
conan_arch = "x86_64"

conan_includedirs = {"C:/Users/o_raymond/.conan/data/gdal/3.5.2/_/_/package/4616bf325cec74938e9d502ec368231ec9d3b4fd/include",
"C:/Users/o_raymond/.conan/data/libpqxx/7.7.4/_/_/package/ac95d0a5e7599180e9b318a017447461244f2020/include",
"C:/Users/o_raymond/.conan/data/netcdf/4.8.1/_/_/package/a1cc5449594cd99ffd3668b794c1940def6fc693/include",
"C:/Users/o_raymond/.conan/data/gtest/1.13.0/_/_/package/5ad274d83035c78ba2b205e6cf4f1b317aee8e05/include",
"C:/.conan/ef538c/1/include",
"C:/Users/o_raymond/.conan/data/pybind11/2.10.1/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include",
"C:/Users/o_raymond/.conan/data/json-c/0.16/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include",
"C:/Users/o_raymond/.conan/data/libgeotiff/1.7.1/_/_/package/bb2f096bc0109c5c47d97a0977309e3b0d54966f/include",
"C:/Users/o_raymond/.conan/data/geos/3.11.0/_/_/package/198e2712973a87e9ccc17e2638863f62dab3cb85/include",
"C:/Users/o_raymond/.conan/data/giflib/5.2.1/_/_/package/6b00c5dadf5aaaf1ebe71d0b7a714cd765691e83/include",
"C:/Users/o_raymond/.conan/data/libiconv/1.17/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include",
"C:/Users/o_raymond/.conan/data/libpng/1.6.38/_/_/package/ce650d9f1f1c1c0839cf0694a55c1351ddbed859/include",
"C:/Users/o_raymond/.conan/data/qhull/8.0.1/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include",
"C:/Users/o_raymond/.conan/data/libpq/14.5/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include",
"C:/Users/o_raymond/.conan/data/hdf5/1.13.1/_/_/package/28891ed0065342f11bc16b246461e0f70d52d55a/include",
"C:/Users/o_raymond/.conan/data/hdf5/1.13.1/_/_/package/28891ed0065342f11bc16b246461e0f70d52d55a/include/hdf5",
"C:/Users/o_raymond/.conan/data/proj/9.0.1/_/_/package/bb37563544dd478bdaee9944346bb7576bf427f4/include",
"C:/Users/o_raymond/.conan/data/getopt-for-visual-studio/20200201/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include",
"C:/Users/o_raymond/.conan/data/libtiff/4.4.0/_/_/package/ab864d6dcbcbe0fc78f82a6d385877d88cdb58d8/include",
"C:/Users/o_raymond/.conan/data/nlohmann_json/3.11.2/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include",
"C:/Users/o_raymond/.conan/data/sqlite3/3.39.3/_/_/package/1cb7125758648b3fd39bd045f772ec43fd26f71a/include",
"C:/Users/o_raymond/.conan/data/libcurl/7.87.0/_/_/package/bc4c0f3dad1f27ca5668260749730c07bfef0e48/include",
"C:/Users/o_raymond/.conan/data/zlib/1.2.13/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include",
"C:/Users/o_raymond/.conan/data/libdeflate/1.12/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include",
"C:/Users/o_raymond/.conan/data/xz_utils/5.2.5/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include",
"C:/Users/o_raymond/.conan/data/libjpeg/9e/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include",
"C:/Users/o_raymond/.conan/data/jbig/20160605/_/_/package/1c06f8aa5b65435590877732bd94377a1ed95260/include",
"C:/Users/o_raymond/.conan/data/zstd/1.5.2/_/_/package/72381fc5a05da7a9f5e4d254cd58aa074c20b053/include",
"C:/Users/o_raymond/.conan/data/libwebp/1.2.4/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/include",
"C:/Users/o_raymond/.conan/data/openssl/1.1.1s/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include"}
conan_libdirs = {"C:/Users/o_raymond/.conan/data/gdal/3.5.2/_/_/package/4616bf325cec74938e9d502ec368231ec9d3b4fd/lib",
"C:/Users/o_raymond/.conan/data/libpqxx/7.7.4/_/_/package/ac95d0a5e7599180e9b318a017447461244f2020/lib",
"C:/Users/o_raymond/.conan/data/netcdf/4.8.1/_/_/package/a1cc5449594cd99ffd3668b794c1940def6fc693/lib",
"C:/Users/o_raymond/.conan/data/gtest/1.13.0/_/_/package/5ad274d83035c78ba2b205e6cf4f1b317aee8e05/lib",
"C:/Users/o_raymond/.conan/data/pybind11/2.10.1/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/lib",
"C:/Users/o_raymond/.conan/data/json-c/0.16/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib",
"C:/Users/o_raymond/.conan/data/libgeotiff/1.7.1/_/_/package/bb2f096bc0109c5c47d97a0977309e3b0d54966f/lib",
"C:/Users/o_raymond/.conan/data/geos/3.11.0/_/_/package/198e2712973a87e9ccc17e2638863f62dab3cb85/lib",
"C:/Users/o_raymond/.conan/data/giflib/5.2.1/_/_/package/6b00c5dadf5aaaf1ebe71d0b7a714cd765691e83/lib",
"C:/Users/o_raymond/.conan/data/libiconv/1.17/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib",
"C:/Users/o_raymond/.conan/data/libpng/1.6.38/_/_/package/ce650d9f1f1c1c0839cf0694a55c1351ddbed859/lib",
"C:/Users/o_raymond/.conan/data/qhull/8.0.1/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib",
"C:/Users/o_raymond/.conan/data/libpq/14.5/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib",
"C:/Users/o_raymond/.conan/data/hdf5/1.13.1/_/_/package/28891ed0065342f11bc16b246461e0f70d52d55a/lib",
"C:/Users/o_raymond/.conan/data/proj/9.0.1/_/_/package/bb37563544dd478bdaee9944346bb7576bf427f4/lib",
"C:/Users/o_raymond/.conan/data/libtiff/4.4.0/_/_/package/ab864d6dcbcbe0fc78f82a6d385877d88cdb58d8/lib",
"C:/Users/o_raymond/.conan/data/sqlite3/3.39.3/_/_/package/1cb7125758648b3fd39bd045f772ec43fd26f71a/lib",
"C:/Users/o_raymond/.conan/data/libcurl/7.87.0/_/_/package/bc4c0f3dad1f27ca5668260749730c07bfef0e48/lib",
"C:/Users/o_raymond/.conan/data/zlib/1.2.13/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib",
"C:/Users/o_raymond/.conan/data/libdeflate/1.12/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib",
"C:/Users/o_raymond/.conan/data/xz_utils/5.2.5/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib",
"C:/Users/o_raymond/.conan/data/libjpeg/9e/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib",
"C:/Users/o_raymond/.conan/data/jbig/20160605/_/_/package/1c06f8aa5b65435590877732bd94377a1ed95260/lib",
"C:/Users/o_raymond/.conan/data/zstd/1.5.2/_/_/package/72381fc5a05da7a9f5e4d254cd58aa074c20b053/lib",
"C:/Users/o_raymond/.conan/data/libwebp/1.2.4/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/lib",
"C:/Users/o_raymond/.conan/data/openssl/1.1.1s/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib"}
conan_bindirs = {"C:/Users/o_raymond/.conan/data/gdal/3.5.2/_/_/package/4616bf325cec74938e9d502ec368231ec9d3b4fd/bin",
"C:/Users/o_raymond/.conan/data/geos/3.11.0/_/_/package/198e2712973a87e9ccc17e2638863f62dab3cb85/bin",
"C:/Users/o_raymond/.conan/data/giflib/5.2.1/_/_/package/6b00c5dadf5aaaf1ebe71d0b7a714cd765691e83/bin",
"C:/Users/o_raymond/.conan/data/libiconv/1.17/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/bin",
"C:/Users/o_raymond/.conan/data/qhull/8.0.1/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/bin",
"C:/Users/o_raymond/.conan/data/hdf5/1.13.1/_/_/package/28891ed0065342f11bc16b246461e0f70d52d55a/bin",
"C:/Users/o_raymond/.conan/data/proj/9.0.1/_/_/package/bb37563544dd478bdaee9944346bb7576bf427f4/bin",
"C:/Users/o_raymond/.conan/data/sqlite3/3.39.3/_/_/package/1cb7125758648b3fd39bd045f772ec43fd26f71a/bin",
"C:/Users/o_raymond/.conan/data/libcurl/7.87.0/_/_/package/bc4c0f3dad1f27ca5668260749730c07bfef0e48/bin",
"C:/Users/o_raymond/.conan/data/jbig/20160605/_/_/package/1c06f8aa5b65435590877732bd94377a1ed95260/bin",
"C:/Users/o_raymond/.conan/data/openssl/1.1.1s/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/bin"}
conan_libs = {"gdal", "pqxx", "netcdf", "gtest_main", "gmock_main", "gmock", "gtest", "json-c", "geotiff", "geos_c", "geos", "gif", "iconv", "charset", "libpng16", "qhullstatic_r", "libpq", "libpgport", "libpgcommon", "libhdf5_hl_cpp", "libhdf5_cpp", "libhdf5_hl", "libhdf5", "proj", "tiffxx", "tiff", "sqlite3", "libcurl", "zlib", "libdeflatestatic", "lzma", "libjpeg", "jbig", "zstd_static", "webpdecoder", "webpdemux", "webpmux", "webp", "libssl", "libcrypto"}
conan_system_libs = {"wsock32", "secur32", "wldap32", "shell32", "ole32", "crypt32", "ws2_32", "advapi32", "user32", "bcrypt"}
conan_defines = {"LIBJPEG_STATIC", "LZMA_API_STATIC", "CURL_STATICLIB=1", "PROJ_DLL=", "USE_GIF_LIB", "USE_UNSTABLE_GEOS_CPP_API", "TTMATH_NOASM"}
conan_cxxflags = {}
conan_cflags = {}
conan_sharedlinkflags = {}
conan_exelinkflags = {}
conan_frameworks = {}

conan_includedirs_gdal = {"C:/Users/o_raymond/.conan/data/gdal/3.5.2/_/_/package/4616bf325cec74938e9d502ec368231ec9d3b4fd/include"}
conan_libdirs_gdal = {"C:/Users/o_raymond/.conan/data/gdal/3.5.2/_/_/package/4616bf325cec74938e9d502ec368231ec9d3b4fd/lib"}
conan_bindirs_gdal = {"C:/Users/o_raymond/.conan/data/gdal/3.5.2/_/_/package/4616bf325cec74938e9d502ec368231ec9d3b4fd/bin"}
conan_libs_gdal = {"gdal"}
conan_system_libs_gdal = {}
conan_defines_gdal = {}
conan_cxxflags_gdal = {}
conan_cflags_gdal = {}
conan_sharedlinkflags_gdal = {}
conan_exelinkflags_gdal = {}
conan_frameworks_gdal = {}
conan_rootpath_gdal = "C:/Users/o_raymond/.conan/data/gdal/3.5.2/_/_/package/4616bf325cec74938e9d502ec368231ec9d3b4fd"

conan_includedirs_libpqxx = {"C:/Users/o_raymond/.conan/data/libpqxx/7.7.4/_/_/package/ac95d0a5e7599180e9b318a017447461244f2020/include"}
conan_libdirs_libpqxx = {"C:/Users/o_raymond/.conan/data/libpqxx/7.7.4/_/_/package/ac95d0a5e7599180e9b318a017447461244f2020/lib"}
conan_bindirs_libpqxx = {}
conan_libs_libpqxx = {"pqxx"}
conan_system_libs_libpqxx = {"wsock32", "ws2_32"}
conan_defines_libpqxx = {}
conan_cxxflags_libpqxx = {}
conan_cflags_libpqxx = {}
conan_sharedlinkflags_libpqxx = {}
conan_exelinkflags_libpqxx = {}
conan_frameworks_libpqxx = {}
conan_rootpath_libpqxx = "C:/Users/o_raymond/.conan/data/libpqxx/7.7.4/_/_/package/ac95d0a5e7599180e9b318a017447461244f2020"

conan_includedirs_netcdf = {"C:/Users/o_raymond/.conan/data/netcdf/4.8.1/_/_/package/a1cc5449594cd99ffd3668b794c1940def6fc693/include"}
conan_libdirs_netcdf = {"C:/Users/o_raymond/.conan/data/netcdf/4.8.1/_/_/package/a1cc5449594cd99ffd3668b794c1940def6fc693/lib"}
conan_bindirs_netcdf = {}
conan_libs_netcdf = {"netcdf"}
conan_system_libs_netcdf = {}
conan_defines_netcdf = {}
conan_cxxflags_netcdf = {}
conan_cflags_netcdf = {}
conan_sharedlinkflags_netcdf = {}
conan_exelinkflags_netcdf = {}
conan_frameworks_netcdf = {}
conan_rootpath_netcdf = "C:/Users/o_raymond/.conan/data/netcdf/4.8.1/_/_/package/a1cc5449594cd99ffd3668b794c1940def6fc693"

conan_includedirs_gtest = {"C:/Users/o_raymond/.conan/data/gtest/1.13.0/_/_/package/5ad274d83035c78ba2b205e6cf4f1b317aee8e05/include"}
conan_libdirs_gtest = {"C:/Users/o_raymond/.conan/data/gtest/1.13.0/_/_/package/5ad274d83035c78ba2b205e6cf4f1b317aee8e05/lib"}
conan_bindirs_gtest = {}
conan_libs_gtest = {"gtest_main", "gmock_main", "gmock", "gtest"}
conan_system_libs_gtest = {}
conan_defines_gtest = {}
conan_cxxflags_gtest = {}
conan_cflags_gtest = {}
conan_sharedlinkflags_gtest = {}
conan_exelinkflags_gtest = {}
conan_frameworks_gtest = {}
conan_rootpath_gtest = "C:/Users/o_raymond/.conan/data/gtest/1.13.0/_/_/package/5ad274d83035c78ba2b205e6cf4f1b317aee8e05"

conan_includedirs_boost = {"C:/.conan/ef538c/1/include"}
conan_libdirs_boost = {}
conan_bindirs_boost = {}
conan_libs_boost = {}
conan_system_libs_boost = {}
conan_defines_boost = {}
conan_cxxflags_boost = {}
conan_cflags_boost = {}
conan_sharedlinkflags_boost = {}
conan_exelinkflags_boost = {}
conan_frameworks_boost = {}
conan_rootpath_boost = "C:/.conan/ef538c/1"

conan_includedirs_pybind11 = {"C:/Users/o_raymond/.conan/data/pybind11/2.10.1/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include"}
conan_libdirs_pybind11 = {"C:/Users/o_raymond/.conan/data/pybind11/2.10.1/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/lib"}
conan_bindirs_pybind11 = {}
conan_libs_pybind11 = {}
conan_system_libs_pybind11 = {}
conan_defines_pybind11 = {}
conan_cxxflags_pybind11 = {}
conan_cflags_pybind11 = {}
conan_sharedlinkflags_pybind11 = {}
conan_exelinkflags_pybind11 = {}
conan_frameworks_pybind11 = {}
conan_rootpath_pybind11 = "C:/Users/o_raymond/.conan/data/pybind11/2.10.1/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9"

conan_includedirs_json_c = {"C:/Users/o_raymond/.conan/data/json-c/0.16/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include"}
conan_libdirs_json_c = {"C:/Users/o_raymond/.conan/data/json-c/0.16/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib"}
conan_bindirs_json_c = {}
conan_libs_json_c = {"json-c"}
conan_system_libs_json_c = {}
conan_defines_json_c = {}
conan_cxxflags_json_c = {}
conan_cflags_json_c = {}
conan_sharedlinkflags_json_c = {}
conan_exelinkflags_json_c = {}
conan_frameworks_json_c = {}
conan_rootpath_json_c = "C:/Users/o_raymond/.conan/data/json-c/0.16/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab"

conan_includedirs_libgeotiff = {"C:/Users/o_raymond/.conan/data/libgeotiff/1.7.1/_/_/package/bb2f096bc0109c5c47d97a0977309e3b0d54966f/include"}
conan_libdirs_libgeotiff = {"C:/Users/o_raymond/.conan/data/libgeotiff/1.7.1/_/_/package/bb2f096bc0109c5c47d97a0977309e3b0d54966f/lib"}
conan_bindirs_libgeotiff = {}
conan_libs_libgeotiff = {"geotiff"}
conan_system_libs_libgeotiff = {}
conan_defines_libgeotiff = {}
conan_cxxflags_libgeotiff = {}
conan_cflags_libgeotiff = {}
conan_sharedlinkflags_libgeotiff = {}
conan_exelinkflags_libgeotiff = {}
conan_frameworks_libgeotiff = {}
conan_rootpath_libgeotiff = "C:/Users/o_raymond/.conan/data/libgeotiff/1.7.1/_/_/package/bb2f096bc0109c5c47d97a0977309e3b0d54966f"

conan_includedirs_geos = {"C:/Users/o_raymond/.conan/data/geos/3.11.0/_/_/package/198e2712973a87e9ccc17e2638863f62dab3cb85/include"}
conan_libdirs_geos = {"C:/Users/o_raymond/.conan/data/geos/3.11.0/_/_/package/198e2712973a87e9ccc17e2638863f62dab3cb85/lib"}
conan_bindirs_geos = {"C:/Users/o_raymond/.conan/data/geos/3.11.0/_/_/package/198e2712973a87e9ccc17e2638863f62dab3cb85/bin"}
conan_libs_geos = {"geos_c", "geos"}
conan_system_libs_geos = {}
conan_defines_geos = {"USE_UNSTABLE_GEOS_CPP_API", "TTMATH_NOASM"}
conan_cxxflags_geos = {}
conan_cflags_geos = {}
conan_sharedlinkflags_geos = {}
conan_exelinkflags_geos = {}
conan_frameworks_geos = {}
conan_rootpath_geos = "C:/Users/o_raymond/.conan/data/geos/3.11.0/_/_/package/198e2712973a87e9ccc17e2638863f62dab3cb85"

conan_includedirs_giflib = {"C:/Users/o_raymond/.conan/data/giflib/5.2.1/_/_/package/6b00c5dadf5aaaf1ebe71d0b7a714cd765691e83/include"}
conan_libdirs_giflib = {"C:/Users/o_raymond/.conan/data/giflib/5.2.1/_/_/package/6b00c5dadf5aaaf1ebe71d0b7a714cd765691e83/lib"}
conan_bindirs_giflib = {"C:/Users/o_raymond/.conan/data/giflib/5.2.1/_/_/package/6b00c5dadf5aaaf1ebe71d0b7a714cd765691e83/bin"}
conan_libs_giflib = {"gif"}
conan_system_libs_giflib = {}
conan_defines_giflib = {"USE_GIF_LIB"}
conan_cxxflags_giflib = {}
conan_cflags_giflib = {}
conan_sharedlinkflags_giflib = {}
conan_exelinkflags_giflib = {}
conan_frameworks_giflib = {}
conan_rootpath_giflib = "C:/Users/o_raymond/.conan/data/giflib/5.2.1/_/_/package/6b00c5dadf5aaaf1ebe71d0b7a714cd765691e83"

conan_includedirs_libiconv = {"C:/Users/o_raymond/.conan/data/libiconv/1.17/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include"}
conan_libdirs_libiconv = {"C:/Users/o_raymond/.conan/data/libiconv/1.17/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib"}
conan_bindirs_libiconv = {"C:/Users/o_raymond/.conan/data/libiconv/1.17/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/bin"}
conan_libs_libiconv = {"iconv", "charset"}
conan_system_libs_libiconv = {}
conan_defines_libiconv = {}
conan_cxxflags_libiconv = {}
conan_cflags_libiconv = {}
conan_sharedlinkflags_libiconv = {}
conan_exelinkflags_libiconv = {}
conan_frameworks_libiconv = {}
conan_rootpath_libiconv = "C:/Users/o_raymond/.conan/data/libiconv/1.17/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab"

conan_includedirs_libpng = {"C:/Users/o_raymond/.conan/data/libpng/1.6.38/_/_/package/ce650d9f1f1c1c0839cf0694a55c1351ddbed859/include"}
conan_libdirs_libpng = {"C:/Users/o_raymond/.conan/data/libpng/1.6.38/_/_/package/ce650d9f1f1c1c0839cf0694a55c1351ddbed859/lib"}
conan_bindirs_libpng = {}
conan_libs_libpng = {"libpng16"}
conan_system_libs_libpng = {}
conan_defines_libpng = {}
conan_cxxflags_libpng = {}
conan_cflags_libpng = {}
conan_sharedlinkflags_libpng = {}
conan_exelinkflags_libpng = {}
conan_frameworks_libpng = {}
conan_rootpath_libpng = "C:/Users/o_raymond/.conan/data/libpng/1.6.38/_/_/package/ce650d9f1f1c1c0839cf0694a55c1351ddbed859"

conan_includedirs_qhull = {"C:/Users/o_raymond/.conan/data/qhull/8.0.1/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include"}
conan_libdirs_qhull = {"C:/Users/o_raymond/.conan/data/qhull/8.0.1/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib"}
conan_bindirs_qhull = {"C:/Users/o_raymond/.conan/data/qhull/8.0.1/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/bin"}
conan_libs_qhull = {"qhullstatic_r"}
conan_system_libs_qhull = {}
conan_defines_qhull = {}
conan_cxxflags_qhull = {}
conan_cflags_qhull = {}
conan_sharedlinkflags_qhull = {}
conan_exelinkflags_qhull = {}
conan_frameworks_qhull = {}
conan_rootpath_qhull = "C:/Users/o_raymond/.conan/data/qhull/8.0.1/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab"

conan_includedirs_libpq = {"C:/Users/o_raymond/.conan/data/libpq/14.5/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include"}
conan_libdirs_libpq = {"C:/Users/o_raymond/.conan/data/libpq/14.5/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib"}
conan_bindirs_libpq = {}
conan_libs_libpq = {"libpq", "libpgport", "libpgcommon"}
conan_system_libs_libpq = {"ws2_32", "secur32", "advapi32", "shell32", "crypt32", "wldap32"}
conan_defines_libpq = {}
conan_cxxflags_libpq = {}
conan_cflags_libpq = {}
conan_sharedlinkflags_libpq = {}
conan_exelinkflags_libpq = {}
conan_frameworks_libpq = {}
conan_rootpath_libpq = "C:/Users/o_raymond/.conan/data/libpq/14.5/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab"

conan_includedirs_hdf5 = {"C:/Users/o_raymond/.conan/data/hdf5/1.13.1/_/_/package/28891ed0065342f11bc16b246461e0f70d52d55a/include",
"C:/Users/o_raymond/.conan/data/hdf5/1.13.1/_/_/package/28891ed0065342f11bc16b246461e0f70d52d55a/include/hdf5"}
conan_libdirs_hdf5 = {"C:/Users/o_raymond/.conan/data/hdf5/1.13.1/_/_/package/28891ed0065342f11bc16b246461e0f70d52d55a/lib"}
conan_bindirs_hdf5 = {"C:/Users/o_raymond/.conan/data/hdf5/1.13.1/_/_/package/28891ed0065342f11bc16b246461e0f70d52d55a/bin"}
conan_libs_hdf5 = {"libhdf5_hl_cpp", "libhdf5_cpp", "libhdf5_hl", "libhdf5"}
conan_system_libs_hdf5 = {}
conan_defines_hdf5 = {}
conan_cxxflags_hdf5 = {}
conan_cflags_hdf5 = {}
conan_sharedlinkflags_hdf5 = {}
conan_exelinkflags_hdf5 = {}
conan_frameworks_hdf5 = {}
conan_rootpath_hdf5 = "C:/Users/o_raymond/.conan/data/hdf5/1.13.1/_/_/package/28891ed0065342f11bc16b246461e0f70d52d55a"

conan_includedirs_proj = {"C:/Users/o_raymond/.conan/data/proj/9.0.1/_/_/package/bb37563544dd478bdaee9944346bb7576bf427f4/include"}
conan_libdirs_proj = {"C:/Users/o_raymond/.conan/data/proj/9.0.1/_/_/package/bb37563544dd478bdaee9944346bb7576bf427f4/lib"}
conan_bindirs_proj = {"C:/Users/o_raymond/.conan/data/proj/9.0.1/_/_/package/bb37563544dd478bdaee9944346bb7576bf427f4/bin"}
conan_libs_proj = {"proj"}
conan_system_libs_proj = {"shell32", "ole32"}
conan_defines_proj = {"PROJ_DLL="}
conan_cxxflags_proj = {}
conan_cflags_proj = {}
conan_sharedlinkflags_proj = {}
conan_exelinkflags_proj = {}
conan_frameworks_proj = {}
conan_rootpath_proj = "C:/Users/o_raymond/.conan/data/proj/9.0.1/_/_/package/bb37563544dd478bdaee9944346bb7576bf427f4"

conan_includedirs_getopt_for_visual_studio = {"C:/Users/o_raymond/.conan/data/getopt-for-visual-studio/20200201/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include"}
conan_libdirs_getopt_for_visual_studio = {}
conan_bindirs_getopt_for_visual_studio = {}
conan_libs_getopt_for_visual_studio = {}
conan_system_libs_getopt_for_visual_studio = {}
conan_defines_getopt_for_visual_studio = {}
conan_cxxflags_getopt_for_visual_studio = {}
conan_cflags_getopt_for_visual_studio = {}
conan_sharedlinkflags_getopt_for_visual_studio = {}
conan_exelinkflags_getopt_for_visual_studio = {}
conan_frameworks_getopt_for_visual_studio = {}
conan_rootpath_getopt_for_visual_studio = "C:/Users/o_raymond/.conan/data/getopt-for-visual-studio/20200201/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9"

conan_includedirs_libtiff = {"C:/Users/o_raymond/.conan/data/libtiff/4.4.0/_/_/package/ab864d6dcbcbe0fc78f82a6d385877d88cdb58d8/include"}
conan_libdirs_libtiff = {"C:/Users/o_raymond/.conan/data/libtiff/4.4.0/_/_/package/ab864d6dcbcbe0fc78f82a6d385877d88cdb58d8/lib"}
conan_bindirs_libtiff = {}
conan_libs_libtiff = {"tiffxx", "tiff"}
conan_system_libs_libtiff = {}
conan_defines_libtiff = {}
conan_cxxflags_libtiff = {}
conan_cflags_libtiff = {}
conan_sharedlinkflags_libtiff = {}
conan_exelinkflags_libtiff = {}
conan_frameworks_libtiff = {}
conan_rootpath_libtiff = "C:/Users/o_raymond/.conan/data/libtiff/4.4.0/_/_/package/ab864d6dcbcbe0fc78f82a6d385877d88cdb58d8"

conan_includedirs_nlohmann_json = {"C:/Users/o_raymond/.conan/data/nlohmann_json/3.11.2/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9/include"}
conan_libdirs_nlohmann_json = {}
conan_bindirs_nlohmann_json = {}
conan_libs_nlohmann_json = {}
conan_system_libs_nlohmann_json = {}
conan_defines_nlohmann_json = {}
conan_cxxflags_nlohmann_json = {}
conan_cflags_nlohmann_json = {}
conan_sharedlinkflags_nlohmann_json = {}
conan_exelinkflags_nlohmann_json = {}
conan_frameworks_nlohmann_json = {}
conan_rootpath_nlohmann_json = "C:/Users/o_raymond/.conan/data/nlohmann_json/3.11.2/_/_/package/5ab84d6acfe1f23c4fae0ab88f26e3a396351ac9"

conan_includedirs_sqlite3 = {"C:/Users/o_raymond/.conan/data/sqlite3/3.39.3/_/_/package/1cb7125758648b3fd39bd045f772ec43fd26f71a/include"}
conan_libdirs_sqlite3 = {"C:/Users/o_raymond/.conan/data/sqlite3/3.39.3/_/_/package/1cb7125758648b3fd39bd045f772ec43fd26f71a/lib"}
conan_bindirs_sqlite3 = {"C:/Users/o_raymond/.conan/data/sqlite3/3.39.3/_/_/package/1cb7125758648b3fd39bd045f772ec43fd26f71a/bin"}
conan_libs_sqlite3 = {"sqlite3"}
conan_system_libs_sqlite3 = {}
conan_defines_sqlite3 = {}
conan_cxxflags_sqlite3 = {}
conan_cflags_sqlite3 = {}
conan_sharedlinkflags_sqlite3 = {}
conan_exelinkflags_sqlite3 = {}
conan_frameworks_sqlite3 = {}
conan_rootpath_sqlite3 = "C:/Users/o_raymond/.conan/data/sqlite3/3.39.3/_/_/package/1cb7125758648b3fd39bd045f772ec43fd26f71a"

conan_includedirs_libcurl = {"C:/Users/o_raymond/.conan/data/libcurl/7.87.0/_/_/package/bc4c0f3dad1f27ca5668260749730c07bfef0e48/include"}
conan_libdirs_libcurl = {"C:/Users/o_raymond/.conan/data/libcurl/7.87.0/_/_/package/bc4c0f3dad1f27ca5668260749730c07bfef0e48/lib"}
conan_bindirs_libcurl = {"C:/Users/o_raymond/.conan/data/libcurl/7.87.0/_/_/package/bc4c0f3dad1f27ca5668260749730c07bfef0e48/bin"}
conan_libs_libcurl = {"libcurl"}
conan_system_libs_libcurl = {"ws2_32"}
conan_defines_libcurl = {"CURL_STATICLIB=1"}
conan_cxxflags_libcurl = {}
conan_cflags_libcurl = {}
conan_sharedlinkflags_libcurl = {}
conan_exelinkflags_libcurl = {}
conan_frameworks_libcurl = {}
conan_rootpath_libcurl = "C:/Users/o_raymond/.conan/data/libcurl/7.87.0/_/_/package/bc4c0f3dad1f27ca5668260749730c07bfef0e48"

conan_includedirs_zlib = {"C:/Users/o_raymond/.conan/data/zlib/1.2.13/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include"}
conan_libdirs_zlib = {"C:/Users/o_raymond/.conan/data/zlib/1.2.13/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib"}
conan_bindirs_zlib = {}
conan_libs_zlib = {"zlib"}
conan_system_libs_zlib = {}
conan_defines_zlib = {}
conan_cxxflags_zlib = {}
conan_cflags_zlib = {}
conan_sharedlinkflags_zlib = {}
conan_exelinkflags_zlib = {}
conan_frameworks_zlib = {}
conan_rootpath_zlib = "C:/Users/o_raymond/.conan/data/zlib/1.2.13/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab"

conan_includedirs_libdeflate = {"C:/Users/o_raymond/.conan/data/libdeflate/1.12/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include"}
conan_libdirs_libdeflate = {"C:/Users/o_raymond/.conan/data/libdeflate/1.12/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib"}
conan_bindirs_libdeflate = {}
conan_libs_libdeflate = {"libdeflatestatic"}
conan_system_libs_libdeflate = {}
conan_defines_libdeflate = {}
conan_cxxflags_libdeflate = {}
conan_cflags_libdeflate = {}
conan_sharedlinkflags_libdeflate = {}
conan_exelinkflags_libdeflate = {}
conan_frameworks_libdeflate = {}
conan_rootpath_libdeflate = "C:/Users/o_raymond/.conan/data/libdeflate/1.12/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab"

conan_includedirs_xz_utils = {"C:/Users/o_raymond/.conan/data/xz_utils/5.2.5/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include"}
conan_libdirs_xz_utils = {"C:/Users/o_raymond/.conan/data/xz_utils/5.2.5/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib"}
conan_bindirs_xz_utils = {}
conan_libs_xz_utils = {"lzma"}
conan_system_libs_xz_utils = {}
conan_defines_xz_utils = {"LZMA_API_STATIC"}
conan_cxxflags_xz_utils = {}
conan_cflags_xz_utils = {}
conan_sharedlinkflags_xz_utils = {}
conan_exelinkflags_xz_utils = {}
conan_frameworks_xz_utils = {}
conan_rootpath_xz_utils = "C:/Users/o_raymond/.conan/data/xz_utils/5.2.5/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab"

conan_includedirs_libjpeg = {"C:/Users/o_raymond/.conan/data/libjpeg/9e/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include"}
conan_libdirs_libjpeg = {"C:/Users/o_raymond/.conan/data/libjpeg/9e/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib"}
conan_bindirs_libjpeg = {}
conan_libs_libjpeg = {"libjpeg"}
conan_system_libs_libjpeg = {}
conan_defines_libjpeg = {"LIBJPEG_STATIC"}
conan_cxxflags_libjpeg = {}
conan_cflags_libjpeg = {}
conan_sharedlinkflags_libjpeg = {}
conan_exelinkflags_libjpeg = {}
conan_frameworks_libjpeg = {}
conan_rootpath_libjpeg = "C:/Users/o_raymond/.conan/data/libjpeg/9e/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab"

conan_includedirs_jbig = {"C:/Users/o_raymond/.conan/data/jbig/20160605/_/_/package/1c06f8aa5b65435590877732bd94377a1ed95260/include"}
conan_libdirs_jbig = {"C:/Users/o_raymond/.conan/data/jbig/20160605/_/_/package/1c06f8aa5b65435590877732bd94377a1ed95260/lib"}
conan_bindirs_jbig = {"C:/Users/o_raymond/.conan/data/jbig/20160605/_/_/package/1c06f8aa5b65435590877732bd94377a1ed95260/bin"}
conan_libs_jbig = {"jbig"}
conan_system_libs_jbig = {}
conan_defines_jbig = {}
conan_cxxflags_jbig = {}
conan_cflags_jbig = {}
conan_sharedlinkflags_jbig = {}
conan_exelinkflags_jbig = {}
conan_frameworks_jbig = {}
conan_rootpath_jbig = "C:/Users/o_raymond/.conan/data/jbig/20160605/_/_/package/1c06f8aa5b65435590877732bd94377a1ed95260"

conan_includedirs_zstd = {"C:/Users/o_raymond/.conan/data/zstd/1.5.2/_/_/package/72381fc5a05da7a9f5e4d254cd58aa074c20b053/include"}
conan_libdirs_zstd = {"C:/Users/o_raymond/.conan/data/zstd/1.5.2/_/_/package/72381fc5a05da7a9f5e4d254cd58aa074c20b053/lib"}
conan_bindirs_zstd = {}
conan_libs_zstd = {"zstd_static"}
conan_system_libs_zstd = {}
conan_defines_zstd = {}
conan_cxxflags_zstd = {}
conan_cflags_zstd = {}
conan_sharedlinkflags_zstd = {}
conan_exelinkflags_zstd = {}
conan_frameworks_zstd = {}
conan_rootpath_zstd = "C:/Users/o_raymond/.conan/data/zstd/1.5.2/_/_/package/72381fc5a05da7a9f5e4d254cd58aa074c20b053"

conan_includedirs_libwebp = {"C:/Users/o_raymond/.conan/data/libwebp/1.2.4/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/include"}
conan_libdirs_libwebp = {"C:/Users/o_raymond/.conan/data/libwebp/1.2.4/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7/lib"}
conan_bindirs_libwebp = {}
conan_libs_libwebp = {"webpdecoder", "webpdemux", "webpmux", "webp"}
conan_system_libs_libwebp = {}
conan_defines_libwebp = {}
conan_cxxflags_libwebp = {}
conan_cflags_libwebp = {}
conan_sharedlinkflags_libwebp = {}
conan_exelinkflags_libwebp = {}
conan_frameworks_libwebp = {}
conan_rootpath_libwebp = "C:/Users/o_raymond/.conan/data/libwebp/1.2.4/_/_/package/638042d3fd356128c913f5e725646a0c2af264c7"

conan_includedirs_openssl = {"C:/Users/o_raymond/.conan/data/openssl/1.1.1s/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/include"}
conan_libdirs_openssl = {"C:/Users/o_raymond/.conan/data/openssl/1.1.1s/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/lib"}
conan_bindirs_openssl = {"C:/Users/o_raymond/.conan/data/openssl/1.1.1s/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab/bin"}
conan_libs_openssl = {"libssl", "libcrypto"}
conan_system_libs_openssl = {"crypt32", "ws2_32", "advapi32", "user32", "bcrypt"}
conan_defines_openssl = {}
conan_cxxflags_openssl = {}
conan_cflags_openssl = {}
conan_sharedlinkflags_openssl = {}
conan_exelinkflags_openssl = {}
conan_frameworks_openssl = {}
conan_rootpath_openssl = "C:/Users/o_raymond/.conan/data/openssl/1.1.1s/_/_/package/3fb49604f9c2f729b85ba3115852006824e72cab"

function conan_basic_setup()
    configurations{conan_build_type}
    architecture(conan_arch)
    includedirs{conan_includedirs}
    libdirs{conan_libdirs}
    links{conan_libs}
    links{conan_system_libs}
    links{conan_frameworks}
    defines{conan_defines}
    bindirs{conan_bindirs}
end

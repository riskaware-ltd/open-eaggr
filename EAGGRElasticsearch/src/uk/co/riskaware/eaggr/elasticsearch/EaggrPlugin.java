/*
 * Copyright (c) Riskaware 2016
 *
 * This file is part of OpenEAGGR.
 * 
 * OpenEAGGR is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * OpenEAGGR is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * A copy of the GNU Lesser General Public License is available in COPYING.LESSER
 * or can be found at <http://www.gnu.org/licenses/>.
 */
package uk.co.riskaware.eaggr.elasticsearch;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.security.AccessController;
import java.security.PrivilegedAction;

import org.elasticsearch.ElasticsearchException;
import org.elasticsearch.indices.IndicesModule;
import org.elasticsearch.plugins.Plugin;

import com.google.common.io.ByteStreams;

/**
 * The EAGGR Elasticsearch plug-in
 * 
 * @author i_bush
 *
 */
public class EaggrPlugin extends Plugin {

    private static final String PATH_TO_TEMP_LIBRARIES = System.getProperty("java.io.tmpdir") + File.separator
            + "uk.co.riskaware.eaggr.elasticsearch";

    @Override
    public String description() {
        return "EAGGR plugin for elasticsearch";
    }

    @Override
    public String name() {
        return "EAGGR";
    }

    /**
     * Registers mappers and query parsers with the indices module
     * 
     * @param indicesModule
     *            the module to add the mappers and parsers
     */
    public void onModule(final IndicesModule indicesModule) {

        // Need to unpack the libraries from the jar and load them
        // For some reason in the Elasticsearch plugin architecture JNA is unable to find the dll dependencies
        // TODO: Currently can only have one process running EAGGR at the same time (due to shared location to unpack
        // libraries)
        loadLibrariesFromJar();

        indicesModule.registerMapper(EaggrPointFieldMapper.CONTENT_TYPE, new EaggrPointFieldMapper.TypeParser());
        indicesModule.registerMapper(EaggrShapeFieldMapper.CONTENT_TYPE, new EaggrShapeFieldMapper.TypeParser());
        indicesModule.registerQueryParser(EaggrQueryParser.class);
    }

    private void loadLibrariesFromJar() {

        // Build the path to the libraries for this platform based on os architecture
        final boolean isWindows = System.getProperty("os.name").contains("Windows");
        final boolean is64Bit = System.getProperty("os.arch").contains("64");
        String libPathInJar = "/";
        if (!isWindows) {
            libPathInJar = libPathInJar + "linux-x86-64";
        } else {
            if (is64Bit) {
                libPathInJar = libPathInJar + "win32-x86-64";
            } else {
                libPathInJar = libPathInJar + "win32-x86";
            }
        }

        // Create the folder to contain the extracted libraries
        final File tempDirectory = new File(PATH_TO_TEMP_LIBRARIES);
        tempDirectory.mkdirs();

        // Load libraries in dependency order
        if (isWindows) {
            loadLibrary(libPathInJar, "libproj-9.dll");
            loadLibrary(libPathInJar, "libwinpthread-1.dll");
            loadLibrary(libPathInJar, "libgcc_s_seh-1.dll");
            loadLibrary(libPathInJar, "libstdc++-6.dll");
            loadLibrary(libPathInJar, "libgdal-20.dll");
            loadLibrary(libPathInJar, "eaggr.dll");
        } else {
            loadLibrary(libPathInJar, "libproj.so");
            loadLibrary(libPathInJar, "libgdal.so");
            loadLibrary(libPathInJar, "libeaggr.so");
        }
    }

    private void loadLibrary(final String pathToSourceLibraries, final String libraryName) {

        AccessController.doPrivileged(new PrivilegedAction<Void>() {

            @Override
            public Void run() {

                try {
                    // Stream the contents of the library from the jar to the temporary folder
                    final InputStream in = EaggrPlugin.class
                            .getResourceAsStream(pathToSourceLibraries + "/" + libraryName);
                    final File fileOut = new File(PATH_TO_TEMP_LIBRARIES + "/" + libraryName);
                    final OutputStream out = new FileOutputStream(fileOut);

                    ByteStreams.copy(in, out);
                    in.close();
                    out.close();
                    System.load(fileOut.toString());
                } catch (final IOException e) {
                    throw new ElasticsearchException("Failed to load EAGGR libraries", e);
                }

                return null;
            }
        });
    }
}

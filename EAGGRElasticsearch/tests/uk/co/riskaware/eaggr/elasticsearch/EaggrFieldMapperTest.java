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

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.Calendar;
import java.util.HashSet;
import java.util.Set;

import org.apache.http.HttpResponse;
import org.apache.http.client.ClientProtocolException;
import org.apache.http.client.HttpClient;
import org.apache.http.client.config.RequestConfig;
import org.apache.http.client.methods.HttpDelete;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.client.methods.HttpPut;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.HttpClients;
import org.json.JSONObject;
import org.junit.After;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;

import com.google.common.io.CharStreams;

import uk.co.riskaware.eaggr.enums.DggsModel;

public class EaggrFieldMapperTest {

    private static final int HTTP_OK = 200;
    private static final int HTTP_CREATED = 201;

    private static final RequestConfig REQUEST_CONFIG = RequestConfig.custom().setSocketTimeout(5000)
            .setConnectTimeout(5000).setConnectionRequestTimeout(5000).build();

    private static Process elasticsearchProcess;
    private static BufferedReader elasticsearchOutputReader;

    private enum IndexType {
        POINT, SHAPE
    };

    @BeforeClass
    public static void setUp() {
        try {
            final String elasticsearchPath = new File("testData/elasticsearch").getAbsolutePath();

            ProcessBuilder builder = new ProcessBuilder("java", "-Xms256m", "-Xmx1g", "-Djava.awt.headless=true",
                    "-XX:+UseCMSInitiatingOccupancyOnly", "-XX:+HeapDumpOnOutOfMemoryError", "-XX:+DisableExplicitGC",
                    "-Dfile.encoding=UTF-8", "-Djna.nosys=true", "-Delasticsearch", "-Des-foreground=yes",
                    "-Des.path.home=" + elasticsearchPath + "", "-cp", elasticsearchPath + "/lib/*",
                    "org.elasticsearch.bootstrap.Elasticsearch", "start");

            elasticsearchProcess = builder.start();

            long startTimeInMillis = Calendar.getInstance().getTimeInMillis();

            boolean elasticsearchStarted = false;

            elasticsearchOutputReader = new BufferedReader(
                    new InputStreamReader(elasticsearchProcess.getInputStream()));
            String line = null;
            StringBuilder elasticsearchOutput = new StringBuilder();
            while ((line = elasticsearchOutputReader.readLine()) != null) {

                // Add the output to the elasticsearch output string (for error reporting)
                elasticsearchOutput.append(line);
                elasticsearchOutput.append(System.getProperty("line.separator"));

                // This text indicates that the elasticsearch node has started up
                if (line.contains("] started")) {
                    elasticsearchStarted = true;
                    break;
                }

                long timeInMillis = Calendar.getInstance().getTimeInMillis();

                // If elasticsearch has taken longer than 30 seconds to start up fail the test
                if (timeInMillis - startTimeInMillis > 30000) {
                    fail("Elasticsearch failed to start." + System.getProperty("line.separator")
                            + elasticsearchOutput.toString());
                }
            }

            if (!elasticsearchStarted) {
                fail("Elasticsearch failed to start." + System.getProperty("line.separator")
                        + elasticsearchOutput.toString());
            }

        } catch (IOException e) {
            fail("Failed to start elasticsearch process: " + e.getMessage());
        }

    }

    @AfterClass
    public static void cleanUp() {
        // Complete reading process output if there is any remaining
        readElasticsearchProcessOutput();

        try {
            deleteIndex(HttpClients.createDefault());
            elasticsearchOutputReader.close();
            elasticsearchProcess.destroy();
        } catch (IOException e) {
            // Ignore this exception - Elasticsearch is closing down anyway
        }
    }

    @After
    public void pauseForElasticsearchReset() throws InterruptedException {
        // Something asynchonous can cause the tests to hang if the next test starts too soon
        // Sleep for 10 seconds between tests to ensure that Elasticsearch is ready for the next test
        Thread.sleep(10000);
    }

    @Test
    public void testPopulatePointIndex() throws IOException {
        recreateIndex(DggsModel.ISEA4T, IndexType.POINT);

        HttpClient httpClient = HttpClients.createDefault();
        int totalNumberOfDocuments = loadPointTestData(httpClient);

        // Get all documents in the index
        HttpPost httpGetAllDocuments = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetAllDocuments.addHeader("Content-Type", "application/json");
        httpGetAllDocuments.addHeader("Accept", "application/json");
        httpGetAllDocuments.setConfig(REQUEST_CONFIG);

        HttpResponse response = httpClient.execute(httpGetAllDocuments);
        String responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        JSONObject jsonResponse = new JSONObject(responseContent);
        int documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(totalNumberOfDocuments, documentCount);
        httpGetAllDocuments.releaseConnection();
    }

    @Test
    public void testPopulateShapeIndex() throws IOException {
        recreateIndex(DggsModel.ISEA4T, IndexType.SHAPE);

        HttpClient httpClient = HttpClients.createDefault();
        int totalNumberOfDocuments = loadShapeTestData(httpClient);

        // Get all documents in the index
        HttpPost httpGetAllDocuments = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetAllDocuments.addHeader("Content-Type", "application/json");
        httpGetAllDocuments.addHeader("Accept", "application/json");
        httpGetAllDocuments.setConfig(REQUEST_CONFIG);

        HttpResponse response = httpClient.execute(httpGetAllDocuments);
        String responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        JSONObject jsonResponse = new JSONObject(responseContent);
        int documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(totalNumberOfDocuments, documentCount);
        httpGetAllDocuments.releaseConnection();
    }

    @Test
    public void testDggsCellQueryISEA4T() throws IOException {
        recreateIndex(DggsModel.ISEA4T, IndexType.POINT);

        HttpClient httpClient = HttpClients.createDefault();
        int totalNumberOfDocuments = loadPointTestData(httpClient);

        // Get documents with location in a small area
        HttpPost httpGetDocumentInSmallCell = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetDocumentInSmallCell.addHeader("Content-Type", "application/json");
        httpGetDocumentInSmallCell.addHeader("Accept", "application/json");
        httpGetDocumentInSmallCell.setEntity(
                new StringEntity("{\"query\": {\"match\": {\"eaggr.eaggr-dggs-cellid\": \"020010211100300101010\"}}}"));
        httpGetDocumentInSmallCell.setConfig(REQUEST_CONFIG);

        HttpResponse response = httpClient.execute(httpGetDocumentInSmallCell);
        String responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        JSONObject jsonResponse = new JSONObject(responseContent);
        int documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(1, documentCount);
        httpGetDocumentInSmallCell.releaseConnection();

        String city = jsonResponse.getJSONObject("hits").getJSONArray("hits").getJSONObject(0).getJSONObject("_source")
                .getString("city");
        assertEquals("Aberdeen", city);

        // Get documents with location in a medium area
        HttpPost httpGetDocumentInMediumCell = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetDocumentInMediumCell.addHeader("Content-Type", "application/json");
        httpGetDocumentInMediumCell.addHeader("Accept", "application/json");
        httpGetDocumentInMediumCell
                .setEntity(new StringEntity("{\"query\": {\"match\": {\"eaggr.eaggr-dggs-cellid\": \"02000023\"}}}"));
        httpGetDocumentInMediumCell.setConfig(REQUEST_CONFIG);

        response = httpClient.execute(httpGetDocumentInMediumCell);
        responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        jsonResponse = new JSONObject(responseContent);
        documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(4, documentCount);
        httpGetDocumentInMediumCell.releaseConnection();

        Set<String> cities = new HashSet<String>();
        for (int i = 0; i < documentCount; ++i) {
            cities.add(jsonResponse.getJSONObject("hits").getJSONArray("hits").getJSONObject(i).getJSONObject("_source")
                    .getString("city"));
        }

        // Check the cities returned are correct. Validated against the DGGS cell overlaid on Google Earth to ensure
        // that the correct cities are inside the cell
        assertEquals(documentCount, cities.size());
        assertTrue(cities.contains("London"));
        assertTrue(cities.contains("Oxford"));
        assertTrue(cities.contains("Luton"));
        assertTrue(cities.contains("Northampton"));

        // Get documents on whole face
        HttpPost httpGetDocumentsOnFace = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetDocumentsOnFace.addHeader("Content-Type", "application/json");
        httpGetDocumentsOnFace.addHeader("Accept", "application/json");
        httpGetDocumentsOnFace
                .setEntity(new StringEntity("{\"query\": {\"match\": {\"eaggr.eaggr-dggs-cellid\": \"02\"}}}"));
        httpGetDocumentsOnFace.setConfig(REQUEST_CONFIG);

        response = httpClient.execute(httpGetDocumentsOnFace);
        responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        jsonResponse = new JSONObject(responseContent);
        documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(totalNumberOfDocuments, documentCount);
        httpGetDocumentsOnFace.releaseConnection();
    }

    @Test
    public void testDggsShapeQueryISEA4T() throws IOException {
        recreateIndex(DggsModel.ISEA4T, IndexType.SHAPE);

        HttpClient httpClient = HttpClients.createDefault();
        int totalNumberOfDocuments = loadShapeTestData(httpClient);

        // Get documents with location in a medium area
        HttpPost httpGetDocumentInMediumCell = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetDocumentInMediumCell.addHeader("Content-Type", "application/json");
        httpGetDocumentInMediumCell.addHeader("Accept", "application/json");
        httpGetDocumentInMediumCell
                .setEntity(new StringEntity("{\"query\": {\"match\": {\"eaggr.eaggr-dggs-cellid\": \"02000023\"}}}"));
        httpGetDocumentInMediumCell.setConfig(REQUEST_CONFIG);

        HttpResponse response = httpClient.execute(httpGetDocumentInMediumCell);
        String responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        JSONObject jsonResponse = new JSONObject(responseContent);
        int documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(9, documentCount);
        httpGetDocumentInMediumCell.releaseConnection();

        Set<String> cities = new HashSet<String>();
        for (int i = 0; i < documentCount; ++i) {
            cities.add(jsonResponse.getJSONObject("hits").getJSONArray("hits").getJSONObject(i).getJSONObject("_source")
                    .getString("city"));
        }

        // Check the cities returned are correct. Validated against the DGGS cell overlaid on Google Earth to ensure
        // that the correct cities are inside the cell
        assertEquals(documentCount, cities.size());
        assertTrue(cities.contains("London"));
        assertTrue(cities.contains("Oxford"));
        assertTrue(cities.contains("Luton"));
        assertTrue(cities.contains("Northampton"));
        assertTrue(cities.contains("London-Luton"));
        assertTrue(cities.contains("London-Oxford"));
        assertTrue(cities.contains("London-Northampton"));
        assertTrue(cities.contains("London-Oxford-Northampton"));
        assertTrue(cities.contains("London-Luton-Oxford"));

        // Get documents on whole face
        HttpPost httpGetDocumentsOnFace = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetDocumentsOnFace.addHeader("Content-Type", "application/json");
        httpGetDocumentsOnFace.addHeader("Accept", "application/json");
        httpGetDocumentsOnFace
                .setEntity(new StringEntity("{\"query\": {\"match\": {\"eaggr.eaggr-dggs-cellid\": \"02\"}}}"));
        httpGetDocumentsOnFace.setConfig(REQUEST_CONFIG);

        response = httpClient.execute(httpGetDocumentsOnFace);
        responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        jsonResponse = new JSONObject(responseContent);
        documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(totalNumberOfDocuments, documentCount);
        httpGetDocumentsOnFace.releaseConnection();
    }

    @Test
    public void testDggsCellQueryISEA3H() throws IOException {
        recreateIndex(DggsModel.ISEA3H, IndexType.POINT);

        // Recreate the index in ISEA3H mode
        HttpClient httpClient = HttpClients.createDefault();

        int totalNumberOfDocuments = loadPointTestData(httpClient);

        // Get documents with location in a small area
        HttpPost httpGetDocumentInSmallCell = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetDocumentInSmallCell.addHeader("Content-Type", "application/json");
        httpGetDocumentInSmallCell.addHeader("Accept", "application/json");
        httpGetDocumentInSmallCell.setEntity(
                new StringEntity("{\"query\": {\"match\": {\"eaggr.eaggr-dggs-cellid\": \"022560305,-16347\"}}}"));
        httpGetDocumentInSmallCell.setConfig(REQUEST_CONFIG);

        HttpResponse response = httpClient.execute(httpGetDocumentInSmallCell);
        String responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        JSONObject jsonResponse = new JSONObject(responseContent);
        int documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(1, documentCount);
        httpGetDocumentInSmallCell.releaseConnection();

        String city = jsonResponse.getJSONObject("hits").getJSONArray("hits").getJSONObject(0).getJSONObject("_source")
                .getString("city");
        assertEquals("Aberdeen", city);

        // Get documents with location in a medium area
        HttpPost httpGetDocumentInMediumCell = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetDocumentInMediumCell.addHeader("Content-Type", "application/json");
        httpGetDocumentInMediumCell.addHeader("Accept", "application/json");
        httpGetDocumentInMediumCell
                .setEntity(new StringEntity("{\"query\": {\"match\": {\"eaggr.eaggr-dggs-cellid\": \"0207-1,0\"}}}"));
        httpGetDocumentInMediumCell.setConfig(REQUEST_CONFIG);

        response = httpClient.execute(httpGetDocumentInMediumCell);
        responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        jsonResponse = new JSONObject(responseContent);
        documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(4, documentCount);
        httpGetDocumentInMediumCell.releaseConnection();

        Set<String> cities = new HashSet<String>();
        for (int i = 0; i < documentCount; ++i) {
            cities.add(jsonResponse.getJSONObject("hits").getJSONArray("hits").getJSONObject(i).getJSONObject("_source")
                    .getString("city"));
        }

        // Check the cities returned are correct. Validated against the DGGS cell overlaid on Google Earth to ensure
        // that the correct cities are inside the cell
        assertEquals(documentCount, cities.size());
        assertTrue(cities.contains("London"));
        assertTrue(cities.contains("Luton"));
        assertTrue(cities.contains("Reading"));
        assertTrue(cities.contains("Southend-on-Sea"));

        // Get documents on whole face
        HttpPost httpGetDocumentsOnFace = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetDocumentsOnFace.addHeader("Content-Type", "application/json");
        httpGetDocumentsOnFace.addHeader("Accept", "application/json");
        httpGetDocumentsOnFace
                .setEntity(new StringEntity("{\"query\": {\"match\": {\"eaggr.eaggr-dggs-cellid\": \"02000,0\"}}}"));
        httpGetDocumentsOnFace.setConfig(REQUEST_CONFIG);

        response = httpClient.execute(httpGetDocumentsOnFace);
        responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        jsonResponse = new JSONObject(responseContent);
        documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(totalNumberOfDocuments, documentCount);
        httpGetDocumentsOnFace.releaseConnection();
    }

    @Test
    public void testDggsShapeQueryISEA3H() throws IOException {
        recreateIndex(DggsModel.ISEA3H, IndexType.SHAPE);

        // Recreate the index in ISEA3H mode
        HttpClient httpClient = HttpClients.createDefault();

        int totalNumberOfDocuments = loadShapeTestData(httpClient);

        // Get documents with location in a small area
        HttpPost httpGetDocumentInSmallCell = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetDocumentInSmallCell.addHeader("Content-Type", "application/json");
        httpGetDocumentInSmallCell.addHeader("Accept", "application/json");
        httpGetDocumentInSmallCell.setEntity(
                new StringEntity("{\"query\": {\"match\": {\"eaggr.eaggr-dggs-cellid\": \"022560305,-16347\"}}}"));
        httpGetDocumentInSmallCell.setConfig(REQUEST_CONFIG);

        HttpResponse response = httpClient.execute(httpGetDocumentInSmallCell);
        String responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        JSONObject jsonResponse = new JSONObject(responseContent);
        int documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(1, documentCount);
        httpGetDocumentInSmallCell.releaseConnection();

        String city = jsonResponse.getJSONObject("hits").getJSONArray("hits").getJSONObject(0).getJSONObject("_source")
                .getString("city");
        assertEquals("Aberdeen", city);

        // Get documents with location in a medium area
        HttpPost httpGetDocumentInMediumCell = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetDocumentInMediumCell.addHeader("Content-Type", "application/json");
        httpGetDocumentInMediumCell.addHeader("Accept", "application/json");
        httpGetDocumentInMediumCell
                .setEntity(new StringEntity("{\"query\": {\"match\": {\"eaggr.eaggr-dggs-cellid\": \"0207-1,0\"}}}"));
        httpGetDocumentInMediumCell.setConfig(REQUEST_CONFIG);

        response = httpClient.execute(httpGetDocumentInMediumCell);
        responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        jsonResponse = new JSONObject(responseContent);
        documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(7, documentCount);
        httpGetDocumentInMediumCell.releaseConnection();

        Set<String> cities = new HashSet<String>();
        for (int i = 0; i < documentCount; ++i) {
            cities.add(jsonResponse.getJSONObject("hits").getJSONArray("hits").getJSONObject(i).getJSONObject("_source")
                    .getString("city"));
        }

        // Check the cities returned are correct. Validated against the DGGS cell overlaid on Google Earth to ensure
        // that the correct cities are inside the cell
        assertEquals(documentCount, cities.size());
        assertTrue(cities.contains("London"));
        assertTrue(cities.contains("Luton"));
        assertTrue(cities.contains("Reading"));
        assertTrue(cities.contains("Southend-on-Sea"));
        assertTrue(cities.contains("London-Luton"));
        assertTrue(cities.contains("London-Reading"));
        assertTrue(cities.contains("London-Southend-on-Sea"));

        // Get documents on whole face
        HttpPost httpGetDocumentsOnFace = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetDocumentsOnFace.addHeader("Content-Type", "application/json");
        httpGetDocumentsOnFace.addHeader("Accept", "application/json");
        httpGetDocumentsOnFace
                .setEntity(new StringEntity("{\"query\": {\"match\": {\"eaggr.eaggr-dggs-cellid\": \"02000,0\"}}}"));
        httpGetDocumentsOnFace.setConfig(REQUEST_CONFIG);

        response = httpClient.execute(httpGetDocumentsOnFace);
        responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        jsonResponse = new JSONObject(responseContent);
        documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(totalNumberOfDocuments, documentCount);
        httpGetDocumentsOnFace.releaseConnection();
    }

    @Test
    public void testQueryDggsCellByLatLongISEA4T() throws IOException {
        recreateIndex(DggsModel.ISEA4T, IndexType.POINT);

        HttpClient httpClient = HttpClients.createDefault();
        loadPointTestData(httpClient);

        // Get documents with location inside a cell
        HttpPost httpGetDocumentsInCell = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetDocumentsInCell.addHeader("Content-Type", "application/json");
        httpGetDocumentsInCell.addHeader("Accept", "application/json");
        httpGetDocumentsInCell.setEntity(new StringEntity(
                "{\"query\" : {\"eaggr_dggs_cell\" : {\"eaggr.eaggr-dggs-cellid\": {\"lat\": \"51.883333\", \"lon\": \"-0.6258333\", \"accuracy\": \"10000000000\"}}}}"));
        httpGetDocumentsInCell.setConfig(REQUEST_CONFIG);

        HttpResponse response = httpClient.execute(httpGetDocumentsInCell);
        String responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        JSONObject jsonResponse = new JSONObject(responseContent);
        int documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(4, documentCount);

        Set<String> cities = new HashSet<String>();
        for (int i = 0; i < documentCount; ++i) {
            cities.add(jsonResponse.getJSONObject("hits").getJSONArray("hits").getJSONObject(i).getJSONObject("_source")
                    .getString("city"));
        }

        // Check the cities returned are correct. Validated against the DGGS cell overlaid on Google Earth to ensure
        // that the correct cities are inside the cell
        assertEquals(documentCount, cities.size());
        assertTrue(cities.contains("London"));
        assertTrue(cities.contains("Oxford"));
        assertTrue(cities.contains("Luton"));
        assertTrue(cities.contains("Northampton"));
        httpGetDocumentsInCell.releaseConnection();
    }

    @Test
    public void testQueryDggsCellByLatLongISEA3H() throws IOException {
        recreateIndex(DggsModel.ISEA3H, IndexType.POINT);

        HttpClient httpClient = HttpClients.createDefault();
        loadPointTestData(httpClient);

        // Get documents with location inside a cell
        HttpPost httpGetDocumentsInCell = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetDocumentsInCell.addHeader("Content-Type", "application/json");
        httpGetDocumentsInCell.addHeader("Accept", "application/json");
        httpGetDocumentsInCell.setEntity(new StringEntity(
                "{\"query\" : {\"eaggr_dggs_cell\" : {\"eaggr.eaggr-dggs-cellid\": {\"lat\": \"51.883333\", \"lon\": \"-0.6258333\", \"accuracy\": \"30000000000\", \"model\": \"ISEA3H\"}}}}"));
        httpGetDocumentsInCell.setConfig(REQUEST_CONFIG);

        HttpResponse response = httpClient.execute(httpGetDocumentsInCell);
        String responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        JSONObject jsonResponse = new JSONObject(responseContent);
        int documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(4, documentCount);

        Set<String> cities = new HashSet<String>();
        for (int i = 0; i < documentCount; ++i) {
            cities.add(jsonResponse.getJSONObject("hits").getJSONArray("hits").getJSONObject(i).getJSONObject("_source")
                    .getString("city"));
        }

        // Check the cities returned are correct. Validated against the DGGS cell overlaid on Google Earth to ensure
        // that the correct cities are inside the cell
        assertEquals(documentCount, cities.size());
        assertTrue(cities.contains("London"));
        assertTrue(cities.contains("Luton"));
        assertTrue(cities.contains("Reading"));
        assertTrue(cities.contains("Southend-on-Sea"));
        httpGetDocumentsInCell.releaseConnection();
    }

    @Test
    public void testQueryDggsShapeByLatLongISEA4T() throws IOException {
        recreateIndex(DggsModel.ISEA4T, IndexType.SHAPE);

        HttpClient httpClient = HttpClients.createDefault();
        loadShapeTestData(httpClient);

        // Get documents with location inside a cell
        HttpPost httpGetDocumentsInCell = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetDocumentsInCell.addHeader("Content-Type", "application/json");
        httpGetDocumentsInCell.addHeader("Accept", "application/json");
        httpGetDocumentsInCell.setEntity(new StringEntity(
                "{\"query\" : {\"eaggr_dggs_cell\" : {\"eaggr.eaggr-dggs-cellid\": {\"lat\": \"51.883333\", \"lon\": \"-0.6258333\", \"accuracy\": \"10000000000\"}}}}"));
        httpGetDocumentsInCell.setConfig(REQUEST_CONFIG);

        HttpResponse response = httpClient.execute(httpGetDocumentsInCell);
        String responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        JSONObject jsonResponse = new JSONObject(responseContent);
        int documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(9, documentCount);

        Set<String> cities = new HashSet<String>();
        for (int i = 0; i < documentCount; ++i) {
            cities.add(jsonResponse.getJSONObject("hits").getJSONArray("hits").getJSONObject(i).getJSONObject("_source")
                    .getString("city"));
        }

        // Check the cities returned are correct. Validated against the DGGS cell overlaid on Google Earth to ensure
        // that the correct cities are inside the cell
        assertEquals(documentCount, cities.size());
        assertTrue(cities.contains("London"));
        assertTrue(cities.contains("Oxford"));
        assertTrue(cities.contains("Luton"));
        assertTrue(cities.contains("Northampton"));
        assertTrue(cities.contains("London-Luton"));
        assertTrue(cities.contains("London-Oxford"));
        assertTrue(cities.contains("London-Northampton"));
        assertTrue(cities.contains("London-Oxford-Northampton"));
        assertTrue(cities.contains("London-Luton-Oxford"));
        httpGetDocumentsInCell.releaseConnection();
    }

    @Test
    public void testQueryDggsShapeByLatLongISEA3H() throws IOException {
        recreateIndex(DggsModel.ISEA3H, IndexType.SHAPE);

        HttpClient httpClient = HttpClients.createDefault();
        loadShapeTestData(httpClient);

        // Get documents with location inside a cell
        HttpPost httpGetDocumentsInCell = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetDocumentsInCell.addHeader("Content-Type", "application/json");
        httpGetDocumentsInCell.addHeader("Accept", "application/json");
        httpGetDocumentsInCell.setEntity(new StringEntity(
                "{\"query\" : {\"eaggr_dggs_cell\" : {\"eaggr.eaggr-dggs-cellid\": {\"lat\": \"51.883333\", \"lon\": \"-0.6258333\", \"accuracy\": \"30000000000\", \"model\": \"ISEA3H\"}}}}"));
        httpGetDocumentsInCell.setConfig(REQUEST_CONFIG);

        HttpResponse response = httpClient.execute(httpGetDocumentsInCell);
        String responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        JSONObject jsonResponse = new JSONObject(responseContent);
        int documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(7, documentCount);

        Set<String> cities = new HashSet<String>();
        for (int i = 0; i < documentCount; ++i) {
            cities.add(jsonResponse.getJSONObject("hits").getJSONArray("hits").getJSONObject(i).getJSONObject("_source")
                    .getString("city"));
        }

        // Check the cities returned are correct. Validated against the DGGS cell overlaid on Google Earth to ensure
        // that the correct cities are inside the cell
        assertEquals(documentCount, cities.size());
        assertTrue(cities.contains("London"));
        assertTrue(cities.contains("Luton"));
        assertTrue(cities.contains("Reading"));
        assertTrue(cities.contains("Southend-on-Sea"));
        assertTrue(cities.contains("London-Luton"));
        assertTrue(cities.contains("London-Reading"));
        assertTrue(cities.contains("London-Southend-on-Sea"));
        httpGetDocumentsInCell.releaseConnection();
    }

    @Test
    public void testGeoPointQuery() throws IOException {
        recreateIndex(DggsModel.ISEA4T, IndexType.POINT);

        HttpClient httpClient = HttpClients.createDefault();
        int totalNumberOfDocuments = loadPointTestData(httpClient);

        // Get documents with location in a small area
        HttpPost httpGetDocumentInSmallCell = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetDocumentInSmallCell.addHeader("Content-Type", "application/json");
        httpGetDocumentInSmallCell.addHeader("Accept", "application/json");
        httpGetDocumentInSmallCell.setEntity(new StringEntity(
                "{\"query\": {\"filtered\" : {\"query\" : {\"match_all\" : {}},\"filter\" : {\"geo_distance\" : {\"distance\" : \"1km\",\"eaggr\" : {\"lat\" : 51.45,\"lon\" : -2.583333}}}}}}"));
        httpGetDocumentInSmallCell.setConfig(REQUEST_CONFIG);

        HttpResponse response = httpClient.execute(httpGetDocumentInSmallCell);
        String responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        JSONObject jsonResponse = new JSONObject(responseContent);
        int documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(1, documentCount);
        httpGetDocumentInSmallCell.releaseConnection();

        String city = jsonResponse.getJSONObject("hits").getJSONArray("hits").getJSONObject(0).getJSONObject("_source")
                .getString("city");
        assertEquals("Bristol", city);

        // Get documents with location in a medium area
        HttpPost httpGetDocumentInMediumCell = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetDocumentInMediumCell.addHeader("Content-Type", "application/json");
        httpGetDocumentInMediumCell.addHeader("Accept", "application/json");
        httpGetDocumentInMediumCell.setEntity(new StringEntity(
                "{\"query\": {\"filtered\" : {\"query\" : {\"match_all\" : {}},\"filter\" : {\"geo_distance\" : {\"distance\" : \"60km\",\"eaggr\" : {\"lat\" : 51.883333,\"lon\" : -0.6258333}}}}}}"));
        httpGetDocumentInMediumCell.setConfig(REQUEST_CONFIG);

        response = httpClient.execute(httpGetDocumentInMediumCell);
        responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        jsonResponse = new JSONObject(responseContent);
        documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(5, documentCount);
        httpGetDocumentInMediumCell.releaseConnection();

        Set<String> cities = new HashSet<String>();
        for (int i = 0; i < documentCount; ++i) {
            cities.add(jsonResponse.getJSONObject("hits").getJSONArray("hits").getJSONObject(i).getJSONObject("_source")
                    .getString("city"));
        }

        // Check the cities returned are correct. Check distances using
        // http://www.movable-type.co.uk/scripts/latlong.html.
        assertEquals(documentCount, cities.size());
        assertTrue(cities.contains("London"));
        assertTrue(cities.contains("Oxford"));
        assertTrue(cities.contains("Luton"));
        assertTrue(cities.contains("Northampton"));
        assertTrue(cities.contains("Reading"));

        // Get documents on whole face
        HttpPost httpGetDocumentsOnFace = new HttpPost("http://localhost:9200/eaggr/_search?pretty");
        httpGetDocumentsOnFace.addHeader("Content-Type", "application/json");
        httpGetDocumentsOnFace.addHeader("Accept", "application/json");
        httpGetDocumentsOnFace.setEntity(new StringEntity(
                "{\"query\": {\"filtered\" : {\"query\" : {\"match_all\" : {}},\"filter\" : {\"geo_distance\" : {\"distance\" : \"600km\",\"eaggr\" : {\"lat\" : 51.883333,\"lon\" : -0.6258333}}}}}}"));
        httpGetDocumentsOnFace.setConfig(REQUEST_CONFIG);

        response = httpClient.execute(httpGetDocumentsOnFace);
        responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        jsonResponse = new JSONObject(responseContent);
        documentCount = jsonResponse.getJSONObject("hits").getInt("total");
        assertEquals(totalNumberOfDocuments, documentCount);
        httpGetDocumentsOnFace.releaseConnection();
    }

    private static void readElasticsearchProcessOutput() {
        try {
            String line;
            while (elasticsearchOutputReader.ready() && (line = elasticsearchOutputReader.readLine()) != null) {
                System.out.println(line);
            }
        } catch (IOException e) {
            // This shouldn't happen but log it if it does
            e.printStackTrace();
        }
    }

    private static void deleteIndex(HttpClient httpClient) throws ClientProtocolException, IOException {
        HttpDelete httpDelete = new HttpDelete("http://localhost:9200/eaggr");
        httpDelete.setConfig(REQUEST_CONFIG);
        httpClient.execute(httpDelete);
        httpDelete.releaseConnection();
    }

    private void recreateIndex(DggsModel dggsModel, IndexType indexType) {
        HttpClient httpClient = HttpClients.createDefault();
        try {
            deleteIndex(httpClient);

            switch (indexType) {
                case POINT:
                    createPointIndex(httpClient, dggsModel);
                    break;
                case SHAPE:
                    createShapeIndex(httpClient, dggsModel);
            }
        } catch (IOException e) {
            fail("Failed to recreate the elasticsearch index");
        }
    }

    private void createPointIndex(HttpClient httpClient, DggsModel model) throws ClientProtocolException, IOException {

        HttpPut httpPut = new HttpPut("http://localhost:9200/eaggr");
        httpPut.addHeader("Content-Type", "application/json");
        httpPut.addHeader("Accept", "application/json");
        httpPut.setConfig(REQUEST_CONFIG);

        httpPut.setEntity(new StringEntity(
                "{ \"mappings\": { \"city\": { \"properties\": { \"city\": {\"type\": \"string\"}, \"eaggr\": {\"type\": \"eaggr_geo_point\", \"accuracy\": 100, \"model\": \""
                        + model.name() + "\"}}}}}"));

        HttpResponse response = httpClient.execute(httpPut);

        assertEquals(HTTP_OK, response.getStatusLine().getStatusCode());

        httpPut.releaseConnection();
    }

    private void createShapeIndex(HttpClient httpClient, DggsModel model) throws ClientProtocolException, IOException {

        HttpPut httpPut = new HttpPut("http://localhost:9200/eaggr");
        httpPut.addHeader("Content-Type", "application/json");
        httpPut.addHeader("Accept", "application/json");
        httpPut.setConfig(REQUEST_CONFIG);

        httpPut.setEntity(new StringEntity(
                "{ \"mappings\": { \"city\": { \"properties\": { \"city\": {\"type\": \"string\"}, \"eaggr\": {\"type\": \"eaggr_geo_shape\", \"accuracy\": 100, \"model\": \""
                        + model.name() + "\"}}}}}"));

        HttpResponse response = httpClient.execute(httpPut);

        assertEquals(HTTP_OK, response.getStatusLine().getStatusCode());

        httpPut.releaseConnection();
    }

    private int loadPointTestData(HttpClient httpClient) throws IOException {

        File file = new File("testData/CityData.csv");
        BufferedReader reader = new BufferedReader(new FileReader(file));

        int numberOfDocuments = 0;
        try {
            String postFormat = "{\"city\": \"%s\", \"eaggr\": {\"lat\": \"%f\", \"lon\": \"%f\"}}";

            String cityLine;
            while ((cityLine = reader.readLine()) != null) {

                HttpPost httpPostAddDocument = new HttpPost("http://localhost:9200/eaggr/city");
                httpPostAddDocument.addHeader("Content-Type", "application/json");
                httpPostAddDocument.addHeader("Accept", "application/json");

                String[] cityData = cityLine.split(",");
                httpPostAddDocument.setEntity(new StringEntity(String.format(postFormat, cityData[0],
                        Double.parseDouble(cityData[1]), Double.parseDouble(cityData[2]))));

                HttpResponse response = httpClient.execute(httpPostAddDocument);
                assertEquals(HTTP_CREATED, response.getStatusLine().getStatusCode());
                ++numberOfDocuments;

                httpPostAddDocument.releaseConnection();
            }

            // Sleep to let elasticsearch create the document index
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            // Ignore this exception - thread interrupted while sleeping
        } finally {
            reader.close();
        }

        // Check that the data has loaded correctly
        HttpPost httpPostSearch = new HttpPost("http://localhost:9200/eaggr/_search?q=*");
        httpPostSearch.addHeader("Content-Type", "application/json");
        httpPostSearch.addHeader("Accept", "application/json");

        HttpResponse response = httpClient.execute(httpPostSearch);
        String responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        JSONObject jsonResponse = new JSONObject(responseContent);
        int documentCount = jsonResponse.getJSONObject("hits").getInt("total");

        assertEquals(numberOfDocuments, documentCount);
        httpPostSearch.releaseConnection();

        return numberOfDocuments;
    }

    private int loadShapeTestData(HttpClient httpClient) throws IOException {

        File file = new File("testData/ShapeData.csv");
        BufferedReader reader = new BufferedReader(new FileReader(file));

        int numberOfDocuments = 0;
        try {
            String postPointFormat = "{\"city\": \"%s\", \"eaggr\": {\"type\": \"%s\", \"coordinates\": [%f, %f]}}";
            String postLinestringFormat = "{\"city\": \"%s\", \"eaggr\": {\"type\": \"%s\", \"coordinates\": [[%f, %f], [%f, %f]]}}";
            String postPolygonFormat = "{\"city\": \"%s\", \"eaggr\": {\"type\": \"%s\", \"coordinates\": [[[%f, %f], [%f, %f], [%f, %f], [%f, %f]]]}}";

            String cityLine;
            while ((cityLine = reader.readLine()) != null) {

                HttpPost httpPostAddDocument = new HttpPost("http://localhost:9200/eaggr/city");
                httpPostAddDocument.addHeader("Content-Type", "application/json");
                httpPostAddDocument.addHeader("Accept", "application/json");

                String[] cityData = cityLine.split(",");

                switch (cityData[0].toLowerCase()) {
                    case "point":
                        httpPostAddDocument.setEntity(
                                new StringEntity(String.format(postPointFormat, cityData[1], cityData[0].toLowerCase(),
                                        Double.parseDouble(cityData[3]), Double.parseDouble(cityData[2]))));
                        break;
                    case "linestring":
                        httpPostAddDocument.setEntity(new StringEntity(
                                String.format(postLinestringFormat, cityData[1], cityData[0].toLowerCase(),
                                        Double.parseDouble(cityData[3]), Double.parseDouble(cityData[2]),
                                        Double.parseDouble(cityData[5]), Double.parseDouble(cityData[4]))));
                        break;
                    case "polygon":
                        httpPostAddDocument.setEntity(new StringEntity(
                                String.format(postPolygonFormat, cityData[1], cityData[0].toLowerCase(),
                                        Double.parseDouble(cityData[3]), Double.parseDouble(cityData[2]),
                                        Double.parseDouble(cityData[5]), Double.parseDouble(cityData[4]),
                                        Double.parseDouble(cityData[7]), Double.parseDouble(cityData[6]),
                                        Double.parseDouble(cityData[9]), Double.parseDouble(cityData[8]))));
                        break;
                    default:
                        fail("Unsupported shape type " + cityData[0]);
                }

                HttpResponse response = httpClient.execute(httpPostAddDocument);
                assertEquals(HTTP_CREATED, response.getStatusLine().getStatusCode());
                ++numberOfDocuments;

                httpPostAddDocument.releaseConnection();
            }

            // Sleep to let elasticsearch create the document index
            Thread.sleep(2000);
        } catch (InterruptedException e) {
            // Ignore this exception - thread interrupted while sleeping
        } finally {
            reader.close();
        }

        // Check that the data has loaded correctly
        HttpPost httpPostSearch = new HttpPost("http://localhost:9200/eaggr/_search?q=*");
        httpPostSearch.addHeader("Content-Type", "application/json");
        httpPostSearch.addHeader("Accept", "application/json");

        HttpResponse response = httpClient.execute(httpPostSearch);
        String responseContent = CharStreams.toString(new InputStreamReader(response.getEntity().getContent()));
        JSONObject jsonResponse = new JSONObject(responseContent);
        int documentCount = jsonResponse.getJSONObject("hits").getInt("total");

        assertEquals(numberOfDocuments, documentCount);
        httpPostSearch.releaseConnection();

        return numberOfDocuments;
    }
}

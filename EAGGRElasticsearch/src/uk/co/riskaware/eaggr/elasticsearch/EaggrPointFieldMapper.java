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

import static org.elasticsearch.index.mapper.MapperBuilders.stringField;
import static org.elasticsearch.index.mapper.core.TypeParsers.parseField;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.apache.lucene.index.IndexOptions;
import org.elasticsearch.ElasticsearchParseException;
import org.elasticsearch.common.Explicit;
import org.elasticsearch.common.Strings;
import org.elasticsearch.common.geo.GeoPoint;
import org.elasticsearch.common.geo.GeoUtils;
import org.elasticsearch.common.settings.Settings;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.elasticsearch.common.xcontent.XContentParser;
import org.elasticsearch.common.xcontent.support.XContentMapValues;
import org.elasticsearch.index.mapper.ContentPath;
import org.elasticsearch.index.mapper.MappedFieldType;
import org.elasticsearch.index.mapper.Mapper;
import org.elasticsearch.index.mapper.MapperParsingException;
import org.elasticsearch.index.mapper.ParseContext;
import org.elasticsearch.index.mapper.core.StringFieldMapper;
import org.elasticsearch.index.mapper.geo.GeoPointFieldMapper;

import com.google.common.collect.Iterators;

import uk.co.riskaware.eaggr.DggsCell;
import uk.co.riskaware.eaggr.Eaggr;
import uk.co.riskaware.eaggr.EaggrException;
import uk.co.riskaware.eaggr.EaggrLibraryException;
import uk.co.riskaware.eaggr.LatLongPoint;
import uk.co.riskaware.eaggr.enums.DggsModel;

/**
 * Class to implement the index mapping for DGGS cells
 * 
 * The class inherits functionality from the elasticsearch built-in geo_point mapper
 * 
 * @author i_bush
 *
 */
public class EaggrPointFieldMapper extends GeoPointFieldMapper {

    // This is the name of the type as it appears in the index mapping
    public static final String CONTENT_TYPE = "eaggr_geo_point";

    // Parameters that can be set on the index mapping
    private static final String ACCURACY = "accuracy";
    private static final String DGGS_MODEL = "model";

    // The key used to store the cell id in the index
    public static final String DGGS_CELLID = "eaggr-dggs-cellid";

    // Mapper that stores the settings of the eaggr cell field
    private final EaggrCellFieldMapper eaggrCellMapper;

    /**
     * Class to build EaggrFieldMapper objects
     * 
     * @author i_bush
     *
     */
    public static class Builder extends GeoPointFieldMapper.Builder {

        private final GeoPointFieldMapper.Builder geoPointBuilder;
        private double accuracy;
        private String dggsModelString;

        /**
         * Constructor
         * 
         * @param name
         *            the name of the field to be parsed
         * @param geoPointBuilder
         *            the builder used to create GeoPointFieldMapper objects
         */
        public Builder(final String name, final GeoPointFieldMapper.Builder geoPointBuilder) {
            super(name);

            this.geoPointBuilder = geoPointBuilder;
        }

        /**
         * Sets the accuracy for the EAGGR cell resolution
         * 
         * @param accuracy
         *            the location accuracy in metres squared
         * @return the builder instance
         */
        public Builder setAccuracy(final double accuracy) {
            this.accuracy = accuracy;
            return this;
        }

        /**
         * Sets the DGGS model to use to generate cells
         * 
         * @param dggsModel
         *            the DGGS model to use
         * @return the builder instance
         */
        public Builder setDggsModel(final String dggsModel) {
            dggsModelString = dggsModel;
            return this;
        }

        @Override
        public EaggrPointFieldMapper build(final Mapper.BuilderContext context) {
            // Build the geo point mapper
            final GeoPointFieldMapper geoPointMapper = geoPointBuilder.includeInAll(true).build(context);

            final ContentPath.Type origPathType = context.path().pathType();
            context.path().pathType(ContentPath.Type.FULL);
            context.path().add(name);

            // Build the mapper to index the DGGS cell ids
            final StringFieldMapper dggsCellMapper = stringField(DGGS_CELLID).index(true).tokenized(false)
                    .includeInAll(true).store(fieldType.stored()).omitNorms(true).indexOptions(IndexOptions.DOCS)
                    .build(context);

            context.path().remove();
            context.path().pathType(origPathType);

            fieldType.setTokenized(false);
            setupFieldType(context);

            // Create the EAGGR cell mapper object and set the accuracy and model type
            final EaggrCellFieldMapper eaggrCellFieldMapper = new EaggrCellFieldMapper(dggsCellMapper);
            eaggrCellFieldMapper.setAccuracy(accuracy);

            if (dggsModelString != null) {
                eaggrCellFieldMapper.setModel(DggsModel.valueOf(dggsModelString));
            }

            return new EaggrPointFieldMapper(name, fieldType, defaultFieldType, context.indexSettings(), origPathType,
                    ignoreMalformed(context), geoPointMapper, eaggrCellFieldMapper,
                    multiFieldsBuilder.build(this, context), copyTo);
        }
    }

    /**
     * Class to parse the index mapping
     * 
     * @author i_bush
     *
     */
    public static class TypeParser extends GeoPointFieldMapper.TypeParser {

        @Override
        public Mapper.Builder< ? , ? > parse(final String name, final Map<String, Object> node,
                final ParserContext parserContext) throws MapperParsingException {

            // Parse the geo point mappings
            final GeoPointFieldMapper.Builder geoPointBuilder = (GeoPointFieldMapper.Builder) super.parse(name, node,
                    parserContext);

            final EaggrPointFieldMapper.Builder builder = new EaggrPointFieldMapper.Builder(name, geoPointBuilder);
            parseField(builder, name, node, parserContext);

            // Loop over the remaining nodes and check for the parameters of the EAGGR mapping
            for (final Iterator<Map.Entry<String, Object>> iterator = node.entrySet().iterator(); iterator.hasNext();) {
                final Map.Entry<String, Object> entry = iterator.next();
                final String propName = Strings.toUnderscoreCase(entry.getKey());
                final Object propNode = entry.getValue();

                // If this node is a parameter for EAGGR mapping then set the value on the builder
                if (propName.equals(ACCURACY)) {
                    builder.setAccuracy(XContentMapValues.nodeDoubleValue(propNode));
                    iterator.remove();
                } else if (propName.equals(DGGS_MODEL)) {
                    builder.setDggsModel(propNode.toString());
                    iterator.remove();
                }
            }

            return builder;
        }
    }

    /**
     * Constructor
     * 
     * @param simpleName
     *            the name of the mapper
     * @param fieldType
     *            the field type
     * @param defaultFieldType
     *            the default field type
     * @param indexSettings
     *            the index settings
     * @param pathType
     *            the path type
     * @param ignoreMalformed
     *            whether to ignore malformed point data
     * @param geoPointFieldMapper
     *            the mapper for geo point mappings
     * @param eaggrCellMapper
     *            the mapper for DGGS cell mappings
     * @param multiFields
     *            the multifields processor
     * @param copyTo
     *            list of fields where the current field should be copied to
     */
    public EaggrPointFieldMapper(final String simpleName, final MappedFieldType fieldType,
            final MappedFieldType defaultFieldType, final Settings indexSettings, final ContentPath.Type pathType,
            final Explicit<Boolean> ignoreMalformed, final GeoPointFieldMapper geoPointFieldMapper,
            final EaggrCellFieldMapper eaggrCellMapper, final MultiFields multiFields, final CopyTo copyTo) {
        super(simpleName, fieldType, defaultFieldType, indexSettings, pathType, null, null, null, multiFields,
                ignoreMalformed, copyTo);

        this.eaggrCellMapper = eaggrCellMapper;
    }

    @Override
    protected String contentType() {
        return CONTENT_TYPE;
    }

    @Override
    public Mapper parse(final ParseContext context) throws IOException {

        // This code is based on org.elasticsearch.index.mapper.geo.BaseGeoPointFieldMapper.parse(ParseContext context)
        // in the elasticsearch core
        final ContentPath.Type origPathType = context.path().pathType();
        context.path().pathType(pathType);
        context.path().add(simpleName());

        // Parse the geo point
        // This is replicated here rather than calling the superclass since the latitude and longitude values are
        // required to calculate the DGGS cell ids
        GeoPoint geoPoint = context.parseExternalValue(GeoPoint.class);

        if (geoPoint != null) {
            parse(context, geoPoint, null);
        } else {
            geoPoint = new GeoPoint();
            XContentParser.Token token = context.parser().currentToken();
            if (token == XContentParser.Token.START_ARRAY) {
                token = context.parser().nextToken();
                if (token == XContentParser.Token.START_ARRAY) {
                    // its an array of array of lon/lat [ [1.2, 1.3], [1.4, 1.5] ]
                    while (token != XContentParser.Token.END_ARRAY) {
                        parse(context, GeoUtils.parseGeoPoint(context.parser(), geoPoint), null);
                        token = context.parser().nextToken();
                    }
                } else {
                    // its an array of other possible values
                    if (token == XContentParser.Token.VALUE_NUMBER) {
                        final double lon = context.parser().doubleValue();
                        token = context.parser().nextToken();
                        final double lat = context.parser().doubleValue();
                        while ((token = context.parser().nextToken()) != XContentParser.Token.END_ARRAY)
                            ;
                        parse(context, geoPoint.reset(lat, lon), null);
                    } else {
                        while (token != XContentParser.Token.END_ARRAY) {
                            if (token == XContentParser.Token.VALUE_STRING) {
                                parsePointFromString(context, geoPoint, context.parser().text());
                            } else {
                                parse(context, GeoUtils.parseGeoPoint(context.parser(), geoPoint), null);
                            }
                            token = context.parser().nextToken();
                        }
                    }
                }
            } else if (token == XContentParser.Token.VALUE_STRING) {
                parsePointFromString(context, geoPoint, context.parser().text());
            } else if (token != XContentParser.Token.VALUE_NULL) {
                parse(context, GeoUtils.parseGeoPoint(context.parser(), geoPoint), null);
            }
        }

        context.path().remove();
        context.path().pathType(origPathType);

        // Now create the DGGS cells
        try {
            final Eaggr eaggr = new Eaggr(eaggrCellMapper.getModel());
            final DggsCell cell = eaggr.convertPointToDggsCell(
                    new LatLongPoint(geoPoint.getLat(), geoPoint.getLon(), eaggrCellMapper.getAccuracy()));

            // Generate the cells differently depending on model (optimised for ISEA4T as it is hierarchical)
            switch (eaggrCellMapper.getModel()) {
                case ISEA4T:
                    // Parent cells can be found by reducing the length of the string
                    // Start at length 2 since the cell id must be at least 2 characters for the polyhedron face
                    for (int subLength = 2; subLength <= cell.getCellId().length(); ++subLength) {
                        eaggrCellMapper
                                .parse(context.createExternalValueContext(cell.getCellId().substring(0, subLength)));
                    }
                    break;
                case ISEA3H:
                    // Remember the last cell id
                    String lastCellId = cell.getCellId();

                    // Add the id for the cell at the highest resolution
                    eaggrCellMapper.parse(context.createExternalValueContext(cell.getCellId()));

                    // Aperture of ISEA3H is 3
                    final double aperture = 3.0;

                    // The initial accuracy of the DGGS cell
                    double accuracy = eaggrCellMapper.getAccuracy();
                    DggsCell parentCell = null;

                    // Loop over all parent cells of the cell at the desired resolution
                    // Stop when two accuracies produce the same cell id (this will be the full face of the polyhedron)
                    while (parentCell == null || !parentCell.getCellId().equals(lastCellId)) {
                        // Increase the accuracy area by the aperture
                        accuracy *= aperture;

                        if (parentCell != null) {
                            lastCellId = parentCell.getCellId();
                        }

                        // Get the cell id for this accuracy and add to the index
                        parentCell = eaggr.convertPointToDggsCell(
                                new LatLongPoint(geoPoint.getLat(), geoPoint.getLon(), accuracy));
                        eaggrCellMapper.parse(context.createExternalValueContext(parentCell.getCellId()));
                    }
                    break;
                default:
                    throw new ElasticsearchParseException(
                            "Unrecognised DGGS Model " + eaggrCellMapper.getModel().name());
            }
        } catch (EaggrException | EaggrLibraryException e) {
            throw new ElasticsearchParseException("Failed to create DGGS cell", e);
        }

        return null;
    }

    @Override
    protected void doXContentBody(final XContentBuilder builder, final boolean includeDefaults, final Params params)
            throws IOException {

        super.doXContentBody(builder, includeDefaults, params);

        // Add the additional parameters for EAGGR cell mapping to the builder
        builder.field(ACCURACY, eaggrCellMapper.getAccuracy());
        builder.field(DGGS_MODEL, eaggrCellMapper.getModel().toString());
    }

    @Override
    public Iterator<Mapper> iterator() {
        final List<Mapper> extras = new ArrayList<>();
        extras.add(eaggrCellMapper.getCellMapper());

        return Iterators.concat(super.iterator(), extras.iterator());
    }

    // Parses the geo point from a string (copied from org.elasticsearch.index.mapper.geo.BaseGeoPointFieldMapper)
    private void parsePointFromString(final ParseContext context, final GeoPoint geoPoint, final String point)
            throws IOException {
        if (point.indexOf(',') < 0) {
            parse(context, geoPoint.resetFromGeoHash(point), point);
        } else {
            parse(context, geoPoint.resetFromString(point), null);
        }
    }
}

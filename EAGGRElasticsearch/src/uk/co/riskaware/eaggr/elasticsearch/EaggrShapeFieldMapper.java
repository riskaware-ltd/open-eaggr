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

import org.apache.lucene.document.Field;
import org.apache.lucene.index.IndexOptions;
import org.elasticsearch.ElasticsearchParseException;
import org.elasticsearch.common.Explicit;
import org.elasticsearch.common.Strings;
import org.elasticsearch.common.geo.builders.ShapeBuilder;
import org.elasticsearch.common.settings.Settings;
import org.elasticsearch.common.xcontent.XContentBuilder;
import org.elasticsearch.common.xcontent.support.XContentMapValues;
import org.elasticsearch.index.mapper.ContentPath;
import org.elasticsearch.index.mapper.MappedFieldType;
import org.elasticsearch.index.mapper.Mapper;
import org.elasticsearch.index.mapper.MapperParsingException;
import org.elasticsearch.index.mapper.ParseContext;
import org.elasticsearch.index.mapper.core.StringFieldMapper;
import org.elasticsearch.index.mapper.geo.GeoShapeFieldMapper;

import com.google.common.collect.Iterators;
import com.spatial4j.core.shape.Point;
import com.spatial4j.core.shape.Shape;
import com.spatial4j.core.shape.jts.JtsGeometry;
import com.spatial4j.core.shape.jts.JtsPoint;
import com.vividsolutions.jts.geom.Coordinate;
import com.vividsolutions.jts.geom.Geometry;

import uk.co.riskaware.eaggr.DggsCell;
import uk.co.riskaware.eaggr.Eaggr;
import uk.co.riskaware.eaggr.EaggrException;
import uk.co.riskaware.eaggr.EaggrLibraryException;
import uk.co.riskaware.eaggr.LatLongPoint;
import uk.co.riskaware.eaggr.enums.DggsModel;

/**
 * Class to implement the index mapping for DGGS shapes
 * 
 * The class inherits functionality from the elasticsearch built-in geo_shape mapper
 * 
 * @author i_bush
 *
 */
public class EaggrShapeFieldMapper extends GeoShapeFieldMapper {

    // This is the name of the type as it appears in the index mapping
    public static final String CONTENT_TYPE = "eaggr_geo_shape";

    // Parameters that can be set on the index mapping
    private static final String ACCURACY = "accuracy";
    private static final String DGGS_MODEL = "model";

    // Mapper that stores the settings of the eaggr cell field
    private final EaggrCellFieldMapper eaggrCellMapper;

    /**
     * Class to build EaggrFieldMapper objects
     * 
     * @author i_bush
     *
     */
    public static class Builder extends GeoShapeFieldMapper.Builder {

        private final GeoShapeFieldMapper.Builder geoShapeBuilder;
        private double accuracy;
        private String dggsModelString;

        /**
         * Constructor
         * 
         * @param name
         *            the name of the field to be parsed
         * @param geoShapeBuilder
         *            the builder used to create GeoPointFiledMapper objects
         */
        public Builder(final String name, final GeoShapeFieldMapper.Builder geoShapeBuilder) {
            super(name);

            this.geoShapeBuilder = geoShapeBuilder;
        }

        /**
         * Sets the accuracy for the DGGS cell resolution
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
        public EaggrShapeFieldMapper build(final Mapper.BuilderContext context) {

            // Build the geo point mapper
            final GeoShapeFieldMapper geoShapeMapper = geoShapeBuilder.build(context);

            final ContentPath.Type origPathType = context.path().pathType();
            context.path().pathType(ContentPath.Type.FULL);
            context.path().add(name);

            // Build the mapper to index the DGGS cell ids
            final StringFieldMapper dggsCellMapper = stringField(EaggrPointFieldMapper.DGGS_CELLID).index(true)
                    .tokenized(false).includeInAll(true).store(fieldType.stored()).omitNorms(true)
                    .indexOptions(IndexOptions.DOCS).build(context);

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

            return new EaggrShapeFieldMapper(name, fieldType, coerce(context), context.indexSettings(), geoShapeMapper,
                    eaggrCellFieldMapper, multiFieldsBuilder.build(this, context), copyTo);
        }
    }

    /**
     * Class to parse the index mapping
     * 
     * @author i_bush
     *
     */
    public static class TypeParser extends GeoShapeFieldMapper.TypeParser {

        @Override
        public Mapper.Builder< ? , ? > parse(final String name, final Map<String, Object> node,
                final ParserContext parserContext) throws MapperParsingException {

            // Parse the geo shape mappings
            final GeoShapeFieldMapper.Builder geoShapeBuilder = (GeoShapeFieldMapper.Builder) super.parse(name, node,
                    parserContext);

            final EaggrShapeFieldMapper.Builder builder = new EaggrShapeFieldMapper.Builder(name, geoShapeBuilder);
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
     * @param coerce
     *            the coerce flag for the shape mapper
     * @param indexSettings
     *            the index settings
     * @param geoShapeFieldMapper
     *            the mapper for geo point mappings
     * @param eaggrCellMapper
     *            the mapper for DGGS cell mappings
     * @param multiFields
     *            the multifields processor
     * @param copyTo
     *            list of fields where the current field should be copied to
     */
    public EaggrShapeFieldMapper(final String simpleName, final MappedFieldType fieldType,
            final Explicit<Boolean> coerce, final Settings indexSettings, final GeoShapeFieldMapper geoShapeFieldMapper,
            final EaggrCellFieldMapper eaggrCellMapper, final MultiFields multiFields, final CopyTo copyTo) {
        super(simpleName, fieldType, coerce, indexSettings, multiFields, copyTo);

        this.eaggrCellMapper = eaggrCellMapper;
    }

    @Override
    protected String contentType() {
        return CONTENT_TYPE;
    }

    @Override
    public Mapper parse(final ParseContext context) throws IOException {

        // Parse the geo shape
        // This is replicated here rather than calling the superclass since the latitude and longitude values are
        // required to calculate the DGGS cell ids
        Shape shape = context.parseExternalValue(Shape.class);
        if (shape == null) {
            final ShapeBuilder shapeBuilder = ShapeBuilder.parse(context.parser(), this);
            if (shapeBuilder == null) {
                return null;
            }
            shape = shapeBuilder.build();
        }
        if (fieldType().pointsOnly() && !(shape instanceof Point)) {
            throw new MapperParsingException(
                    "[{" + fieldType().names().fullName()
                            + "}] is configured for points only but a " + ((shape instanceof JtsGeometry)
                                    ? ((JtsGeometry) shape).getGeom().getGeometryType() : shape.getClass())
                    + " was found");
        }
        final Field[] fields = fieldType().defaultStrategy().createIndexableFields(shape);
        if (fields == null || fields.length == 0) {
            return null;
        }
        for (final Field field : fields) {
            if (!customBoost()) {
                field.setBoost(fieldType().boost());
            }
            context.doc().add(field);
        }

        if (!(shape instanceof JtsPoint) && !(shape instanceof JtsGeometry)) {
            throw new ElasticsearchParseException("Unsupported shape class " + shape.getClass().getName());
        }

        Geometry geometry = null;
        if (shape instanceof JtsPoint) {
            geometry = ((JtsPoint) shape).getGeom();
        } else if (shape instanceof JtsGeometry) {
            geometry = ((JtsGeometry) shape).getGeom();
        }

        // Now create the DGGS cells
        try {
            final List<DggsCell> cells = new ArrayList<DggsCell>();
            final Eaggr dggs = new Eaggr(eaggrCellMapper.getModel());

            for (final Coordinate coordinate : geometry.getCoordinates()) {
                final DggsCell cell = dggs.convertPointToDggsCell(
                        new LatLongPoint(coordinate.y, coordinate.x, eaggrCellMapper.getAccuracy()));
                cells.add(cell);
            }

            final DggsCell boundingCell = dggs.getBoundingCell(cells.toArray(new DggsCell[0]));

            // Generate the cells differently depending on model (optimised for ISEA4T as it is hierarchical)
            switch (eaggrCellMapper.getModel()) {
                case ISEA4T:
                    // Parent cells can be found by reducing the length of the string
                    // Start at length 2 since the cell id must be at least 2 characters for the polyhedron face
                    for (int subLength = 2; subLength <= boundingCell.getCellId().length(); ++subLength) {
                        eaggrCellMapper.parse(
                                context.createExternalValueContext(boundingCell.getCellId().substring(0, subLength)));
                    }
                    break;
                case ISEA3H:
                    // Remember the last cell id
                    String lastCellId = null;

                    // Aperture of ISEA3H is 3
                    final double aperture = 3.0;

                    // The initial accuracy of the DGGS cell
                    final int boundingCellResolution = Integer.parseInt(boundingCell.getCellId().substring(2, 4));
                    int originalCellResolution = Integer.parseInt(cells.get(0).getCellId().substring(2, 4));

                    final double originalAccuracy = eaggrCellMapper.getAccuracy();

                    double accuracy = originalAccuracy
                            * Math.pow(aperture, originalCellResolution - boundingCellResolution);

                    DggsCell parentCell = boundingCell;

                    final Coordinate firstCoordinate = geometry.getCoordinates()[0];
                    // Loop over all parent cells of the cell at the desired resolution
                    // Stop when two accuracies produce the same cell id (this will be the full face of the
                    // polyhedron)
                    while (lastCellId == null || !parentCell.getCellId().equals(lastCellId)) {
                        // Increase the accuracy area by the aperture
                        accuracy *= aperture;
                        eaggrCellMapper.parse(context.createExternalValueContext(parentCell.getCellId()));

                        lastCellId = parentCell.getCellId();

                        // Get the cell id for this accuracy and add to the index
                        parentCell = dggs.convertPointToDggsCell(
                                new LatLongPoint(firstCoordinate.y, firstCoordinate.x, accuracy));
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

        // Add the additional parameters for DGGS cell mapping to the builder
        builder.field(ACCURACY, eaggrCellMapper.getAccuracy());
        builder.field(DGGS_MODEL, eaggrCellMapper.getModel().toString());
    }

    @Override
    public Iterator<Mapper> iterator() {
        final List<Mapper> extras = new ArrayList<>();
        extras.add(eaggrCellMapper.getCellMapper());

        return Iterators.concat(super.iterator(), extras.iterator());
    }
}

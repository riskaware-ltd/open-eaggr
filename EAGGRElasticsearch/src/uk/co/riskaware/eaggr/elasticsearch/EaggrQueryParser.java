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

import java.io.IOException;

import org.apache.lucene.search.Query;
import org.elasticsearch.common.xcontent.XContentParser;
import org.elasticsearch.index.query.QueryParseContext;
import org.elasticsearch.index.query.QueryParser;
import org.elasticsearch.index.query.QueryParsingException;
import org.elasticsearch.index.search.MatchQuery;

import uk.co.riskaware.eaggr.DggsCell;
import uk.co.riskaware.eaggr.Eaggr;
import uk.co.riskaware.eaggr.EaggrException;
import uk.co.riskaware.eaggr.EaggrLibraryException;
import uk.co.riskaware.eaggr.LatLongPoint;
import uk.co.riskaware.eaggr.enums.DggsModel;

/**
 * Class to parse queries for DGGS cells based on lat/long and accuracy
 * 
 * @author i_bush
 *
 */
public class EaggrQueryParser implements QueryParser {

    // The text used to identify this query
    private static final String NAME = "eaggr_dggs_cell";

    @Override
    public String[] names() {
        return new String[] { NAME };
    }

    @Override
    public Query parse(final QueryParseContext parseContext) throws IOException, QueryParsingException {

        final XContentParser parser = parseContext.parser();
        XContentParser.Token token = parser.nextToken();
        if (token != XContentParser.Token.FIELD_NAME) {
            throw new QueryParsingException(parseContext, "[match] query malformed, no field");
        }

        final String fieldName = parser.currentName();

        final MatchQuery matchQuery = new MatchQuery(parseContext);

        String currentFieldName = null;
        Double latitude = null;
        Double longitude = null;
        Double accuracy = null;
        DggsModel dggsModel = DggsModel.ISEA4T;

        while ((token = parser.nextToken()) != XContentParser.Token.END_OBJECT) {
            if (token == XContentParser.Token.FIELD_NAME) {
                currentFieldName = parser.currentName();
            } else if (token.isValue()) {
                if (currentFieldName.equals("lat")) {
                    if (token == XContentParser.Token.VALUE_STRING) {
                        latitude = Double.parseDouble(parser.text());
                    } else {
                        latitude = parser.numberValue().doubleValue();
                    }
                } else if (currentFieldName.equals("lon")) {
                    if (token == XContentParser.Token.VALUE_STRING) {
                        longitude = Double.parseDouble(parser.text());
                    } else {
                        longitude = parser.numberValue().doubleValue();
                    }
                } else if (currentFieldName.equals("accuracy")) {
                    if (token == XContentParser.Token.VALUE_STRING) {
                        accuracy = Double.parseDouble(parser.text());
                    } else {
                        accuracy = parser.numberValue().doubleValue();
                    }
                } else if (currentFieldName.equals("model")) {
                    if (token == XContentParser.Token.VALUE_STRING) {
                        dggsModel = DggsModel.valueOf(parser.text());
                    }
                }
            }
        }

        if (latitude == null) {
            throw new QueryParsingException(parseContext, "latitude value not found for [{}]", NAME);
        } else if (longitude == null) {
            throw new QueryParsingException(parseContext, "longitude value not found for [{}]", NAME);
        } else if (accuracy == null) {
            throw new QueryParsingException(parseContext, "accuracy value not found for [{}]", NAME);
        }

        if (latitude > 90.0 || latitude < -90.0) {
            throw new QueryParsingException(parseContext, "illegal latitude value [{}] for [{}]", latitude, NAME);
        }
        if (longitude > 180.0 || longitude < -180) {
            throw new QueryParsingException(parseContext, "illegal longitude value [{}] for [{}]", longitude, NAME);
        }

        DggsCell cell;
        try {
            final Eaggr eaggr = new Eaggr(dggsModel);
            cell = eaggr.convertPointToDggsCell(
                    new LatLongPoint(latitude.doubleValue(), longitude.doubleValue(), accuracy.doubleValue()));
        } catch (EaggrException | EaggrLibraryException e) {
            throw new QueryParsingException(parseContext, "failed to create DGGS cell", e);
        }

        final Query query = matchQuery.parse(MatchQuery.Type.PHRASE, fieldName, cell.getCellId());

        return query;
    }
}

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

import org.elasticsearch.index.mapper.ParseContext;
import org.elasticsearch.index.mapper.core.StringFieldMapper;

import uk.co.riskaware.eaggr.enums.DggsModel;

/**
 * Class to hold the settings for the EAGGR cell mapping
 * 
 * @author i_bush
 *
 */
public class EaggrCellFieldMapper {

    // The parameter values for the DGGS cell index
    private double accuracy;
    private DggsModel dggsModel;

    // The mapper to create DGGS cell ids for indexing
    private final StringFieldMapper cellMapper;

    /**
     * Constructor
     * 
     * @param cellMapper
     *            the mapper to create DGGS cell ids for indexing
     */
    public EaggrCellFieldMapper(final StringFieldMapper cellMapper) {
        this.cellMapper = cellMapper;

        // Default to ISEA4T
        this.dggsModel = DggsModel.ISEA4T;
    }

    /**
     * Sets the accuracy for points in the index
     * 
     * @param accuracy
     *            the accuracy in metres squared
     */
    public void setAccuracy(final double accuracy) {
        this.accuracy = accuracy;
    }

    /**
     * @return the accuracy for points in the index in metres squared
     */
    public double getAccuracy() {
        return accuracy;
    }

    /**
     * Sets the model to use
     * 
     * @param dggsModel
     *            the DGGS model to use
     */
    public void setModel(final DggsModel dggsModel) {
        this.dggsModel = dggsModel;
    }

    /**
     * @return the mapper to create DGGS cell ids for indexing
     */
    public DggsModel getModel() {
        return dggsModel;
    }

    /**
     * @return the cell mapper
     */
    public StringFieldMapper getCellMapper() {
        return cellMapper;
    }

    /**
     * Parse the DGGS cell mapping
     * 
     * @param context
     *            the parse context
     * @throws IOException
     *             failed to parse
     */
    public void parse(final ParseContext context) throws IOException {
        cellMapper.parse(context);
    }
}

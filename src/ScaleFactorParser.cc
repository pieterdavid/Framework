#include <cp3_llbb/Framework/interface/ScaleFactorParser.h>

#include <iostream>

#include <boost/property_tree/json_parser.hpp>

void ScaleFactorParser::parse_file(const std::string& file) {

    boost::property_tree::ptree ptree;
    boost::property_tree::read_json(file, ptree);

    size_t dimension = ptree.get<size_t>("dimension", 1);

    std::vector<float> binning_x = get_array(ptree.get_child("binning.x"));

    std::vector<float> binning_y;
    std::vector<float> binning_z;
    if (dimension > 1)
        binning_y = get_array(ptree.get_child("binning.y"));
    if (dimension > 2)
        binning_z = get_array(ptree.get_child("binning.z"));

    bool formula = ptree.get<bool>("formula", false);
    
    m_scale_factor.use_formula = formula;
    if (formula) {
        std::string variable = ptree.get<std::string>("variable");

        if (variable == "x")
            m_scale_factor.formula_variable_index = 0;
        else if (variable == "y")
            m_scale_factor.formula_variable_index = 1;
        else if (variable == "z")
            m_scale_factor.formula_variable_index = 2;
        else
            throw edm::Exception(edm::errors::LogicError, "Unsupported variable: " + variable);
    }
    m_scale_factor.absolute_errors = ptree.get<bool>("absolute_errors", false);

    switch (dimension) {
        case 1:
            if (!formula)
                m_scale_factor.binned.reset(new OneDimensionHistogram<float>(binning_x));
            else
                m_scale_factor.formula.reset(new OneDimensionHistogram<std::shared_ptr<TFormula>, float>(binning_x));
            break;

        case 2:
            if (!formula)
                m_scale_factor.binned.reset(new TwoDimensionsHistogram<float>(binning_x, binning_y));
            else
                m_scale_factor.formula.reset(new TwoDimensionsHistogram<std::shared_ptr<TFormula>, float>(binning_x, binning_y));
            break;

        case 3:
            if (!formula)
                m_scale_factor.binned.reset(new ThreeDimensionsHistogram<float>(binning_x, binning_y, binning_z));
            else
                m_scale_factor.formula.reset(new ThreeDimensionsHistogram<std::shared_ptr<TFormula>, float>(binning_x, binning_y, binning_z));
            break;
    }

    if (formula) {
        parse_data<std::string>(ptree, dimension);
    } else {
        parse_data<float>(ptree, dimension);
    }
}

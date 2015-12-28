#pragma once

#include <cp3_llbb/Framework/interface/Histogram.h>

#include <memory>
#include <TFormula.h>

enum class Variation {
    Nominal = 0,
    Down = 1,
    Up = 2
};

struct ScaleFactor {

    friend class ScaleFactorParser;

    ScaleFactor(ScaleFactor&& rhs) {
        binned = std::move(rhs.binned);
        formula = std::move(rhs.formula);
        use_formula = rhs.use_formula;
        absolute_errors = rhs.absolute_errors;
        formula_variable_index = rhs.formula_variable_index;
    }

    ScaleFactor() = default;

    private:
    template <typename _Value>
        std::vector<_Value> get(Histogram<_Value, float>& h, const std::vector<float>& bins, bool& outOfRange) const {
            std::size_t bin = h.findClosestBin(bins, &outOfRange);
            if (bin == 0) {
                throw std::runtime_error("Failed to found the right bin for a scale-factor. This should not happend");
            }

            return {h.getBinContent(bin), h.getBinErrorLow(bin), h.getBinErrorHigh(bin)};
        }

    bool use_formula = false;

    // Binned data
    std::shared_ptr<Histogram<float>> binned;

    // Formula data
    std::shared_ptr<Histogram<std::shared_ptr<TFormula>, float>> formula;

    bool absolute_errors = false;
    size_t formula_variable_index = -1; // Only used in formula mode

    public:
    std::vector<float> get(const std::vector<float>& variables) const {
        auto relative_to_absolute = [](const std::vector<float>& array) {
            std::vector<float> result(3);
            result[0] = array[0];
            result[1] = array[0] + array[1];
            result[2] = array[0] - array[2];

            return result;
        };

        auto double_errors = [](std::vector<float>& values) {
            values[1] = values[0] + 2 * (values[1] - values[0]);
            values[2] = values[0] - 2 * (values[0] - values[2]);
        };

        bool outOfRange = false;

        if (!use_formula) {
            if (! binned.get())
                return {0., 0., 0.};

            std::vector<float> values = get<float>(*binned.get(), variables, outOfRange);

            if (!absolute_errors) {
                values = relative_to_absolute(values);
            }

            if (outOfRange)
                double_errors(values);

            return values;
        } else {
            if (! formula.get())
                return {0., 0., 0.};

            std::vector<std::shared_ptr<TFormula>> formulas = get<std::shared_ptr<TFormula>>(*formula.get(), variables, outOfRange);
            std::vector<float> values;
            for (auto& formula: formulas) {
                values.push_back(formula->Eval(variables[formula_variable_index]));
            }

            if (!absolute_errors) {
                values = relative_to_absolute(values);
            }

            if (outOfRange)
                double_errors(values);

            return values;
        }
    }

};

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

    /**
     * Type of possible errors: Suppose we have E +- ΔE
     *   - ABSOLUTE = ΔE
     *   - RELATIVE = ΔE / E
     *   - VARIATED = E + ΔE or E - ΔE
     */
    enum class ErrorType {
        ABSOLUTE,
        RELATIVE,
        VARIATED
    };

    friend class ScaleFactorParser;

    ScaleFactor(ScaleFactor&& rhs) {
        binned = std::move(rhs.binned);
        formula = std::move(rhs.formula);
        use_formula = rhs.use_formula;
        error_type = rhs.error_type;
        formula_variable_index = rhs.formula_variable_index;
    }

    ScaleFactor() = default;

    private:
    template <typename _Value>
        std::vector<_Value> get(Histogram<_Value, float>& h, const std::vector<float>& bins, bool& outOfRange) const {
            std::size_t bin = h.findClosestBin(bins, &outOfRange);
            if (bin == 0) {
                std::stringstream msg;
                msg << "Failed to found the right bin for a scale-factor. This should not happend. Bins: [";
                for (float b: bins) {
                    msg << b << ", ";
                }

                msg.seekp(msg.tellp() - 2l);
                msg << "]";

                throw std::runtime_error(msg.str());
            }

            return {h.getBinContent(bin), h.getBinErrorLow(bin), h.getBinErrorHigh(bin)};
        }

    bool use_formula = false;

    // Binned data
    std::shared_ptr<Histogram<float>> binned;

    // Formula data
    std::shared_ptr<Histogram<std::shared_ptr<TFormula>, float>> formula;

    ErrorType error_type;
    size_t formula_variable_index = -1; // Only used in formula mode

    /**
     * Convert relative errors to absolute errors
     **/
    std::vector<float> relative_errors_to_absolute(const std::vector<float>& array) const {
        std::vector<float> result(3);
        result[0] = array[0];
        result[1] = array[0] * (1 + array[1]);
        result[2] = array[0] * (1 - array[2]);

        return result;
    };

    /**
     * Convert variated errors to absolute errors
     **/
    std::vector<float> variated_errors_to_absolute(const std::vector<float>& array) const {
        std::vector<float> result(3);
        result[0] = array[0];
        result[1] = std::abs(array[1] - array[0]);
        result[2] = std::abs(array[0] - array[2]);

        return result;
    };

    std::vector<float> convert_errors(const std::vector<float>& array) const {
        switch (error_type) {
            case ErrorType::ABSOLUTE:
                return array;

            case ErrorType::RELATIVE:
                return relative_errors_to_absolute(array);

            case ErrorType::VARIATED:
                return variated_errors_to_absolute(array);
        }

        throw std::runtime_error("Invalid error type");
    }

    public:
    std::vector<float> get(const std::vector<float>& variables) const {
        static auto double_errors = [](std::vector<float>& values) {
            values[1] *= 2;
            values[2] *= 2;
        };

        bool outOfRange = false;

        if (!use_formula) {
            if (! binned.get())
                return {0., 0., 0.};

            std::vector<float> values = convert_errors(get<float>(*binned.get(), variables, outOfRange));

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

            values = convert_errors(values);

            if (outOfRange)
                double_errors(values);

            return values;
        }
    }

};

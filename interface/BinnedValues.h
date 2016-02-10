#pragma once

#include <cp3_llbb/Framework/interface/Histogram.h>

#include <memory>
#include <TFormula.h>

enum Variation {
    Nominal = 0,
    Down = 1,
    Up = 2
};

struct BinnedValues {

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

    friend class BinnedValuesJSONParser;

    BinnedValues(BinnedValues&& rhs) {
        binned = std::move(rhs.binned);
        formula = std::move(rhs.formula);
        use_formula = rhs.use_formula;
        error_type = rhs.error_type;
        formula_variable_index = rhs.formula_variable_index;
        maximum = rhs.maximum;
        minimum = rhs.minimum;
    }

    BinnedValues() = default;

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

    float maximum;
    float minimum;

    /**
     * Convert relative errors to absolute errors
     **/
    std::vector<float> relative_errors_to_absolute(const std::vector<float>& array) const {
        std::vector<float> result(3);
        result[Nominal] = array[Nominal];
        result[Up] = array[Nominal] * array[Up];
        result[Down] = array[Nominal] * array[Down];

        return result;
    };

    /**
     * Convert variated errors to absolute errors
     **/
    std::vector<float> variated_errors_to_absolute(const std::vector<float>& array) const {
        std::vector<float> result(3);
        result[Nominal] = array[Nominal];
        result[Up] = std::abs(array[Up] - array[Nominal]);
        result[Down] = std::abs(array[Nominal] - array[Down]);

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

    /**
     * Check that the up and down variation are
     * still between the allowed range
     **/
    void clamp(std::vector<float>& array) const {
        if ((array[Nominal] + array[Up]) > maximum) {
            array[Up] = maximum - array[Nominal];
        }

        if ((array[Nominal] - array[Down]) < minimum) {
            array[Down] = -(minimum - array[Nominal]);
        }
    }

    public:
    std::vector<float> get(const std::vector<float>& variables) const {
        static auto double_errors = [](std::vector<float>& values) {
            values[Up] *= 2;
            values[Down] *= 2;
        };

        bool outOfRange = false;

        if (!use_formula) {
            if (! binned.get())
                return {0., 0., 0.};

            std::vector<float> values = convert_errors(get<float>(*binned.get(), variables, outOfRange));

            if (outOfRange)
                double_errors(values);

            clamp(values);

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

            clamp(values);

            return values;
        }
    }

};

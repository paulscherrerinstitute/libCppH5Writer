#include <memory>
#include <unordered_map>

#include "config.hpp"
#include "H5Format.hpp"

using namespace std;
using s_ptr = shared_ptr<h5_base>;

class SfFormat : public H5Format
{
    shared_ptr<unordered_map<string, DATA_TYPE>> input_value_type = NULL;
    shared_ptr<unordered_map<string, boost::any>> default_values = NULL;
    shared_ptr<unordered_map<string, std::string>> dataset_move_mapping = NULL;
    shared_ptr<h5_parent> file_format = NULL;

    public:
        ~SfFormat(){};

        SfFormat(const string& dataset_name, int n_bad_modules)
        {
            // Input values definition type.
            // Which type should be the parameters you receive over the REST api.
            input_value_type.reset(
            new unordered_map<string, DATA_TYPE>({
                {"general/created", NX_DATE_TIME},
                {"general/user", NX_CHAR},
                {"general/process", NX_CHAR},
                {"general/instrument", NX_CHAR}
            }));

            // Default values used in the file format.
            default_values.reset(new std::unordered_map<string, boost::any>(
            {
                {"general/n_bad_modules", n_bad_modules},
                {"general/detector_name", dataset_name}
            }));

            // After format has been writen, where to move the raw datasets.
            dataset_move_mapping.reset(new std::unordered_map<string, string>(
            {
                {config::raw_image_dataset_name, "data/" + dataset_name + "/data"},
                {"pulse_id", "data/" + dataset_name + "/pulse_id"},
                {"frame", "data/" + dataset_name + "/frame"},
                {"is_good_frame", "data/" + dataset_name + "/is_good_frame"},
                {"missing_packets_1", "data/" + dataset_name + "/missing_packets_1"},
                {"missing_packets_2", "data/" + dataset_name + "/missing_packets_2"},
                {"daq_recs", "data/" + dataset_name + "/daq_recs"},
                {"daq_rec", "data/" + dataset_name + "/daq_rec"},
                {"framenum_diff", "data/" + dataset_name + "/framenum_diff"},
                {"pulse_ids", "data/" + dataset_name + "/pulse_ids"},
                {"framenums", "data/" + dataset_name + "/framenums"},
                {"pulse_id_diff", "data/" + dataset_name + "/pulse_id_diff"},
                {"module_number", "data/" + dataset_name + "/module_number"},
                {"module_map", "data/" + dataset_name + "/module_map"},
            }));

            // Definition of the file format.
            file_format.reset(
            new h5_parent("", EMPTY_ROOT, {
                s_ptr(new h5_group("general", {
                    s_ptr(new h5_dataset("created", "general/created", NX_DATE_TIME)),
                    s_ptr(new h5_dataset("user", "general/user", NX_CHAR)),
                    s_ptr(new h5_dataset("process", "general/process", NX_CHAR)),
                    s_ptr(new h5_dataset("instrument", "general/instrument", NX_CHAR)),
                    s_ptr(new h5_dataset("detector_name", "general/detector_name", NX_CHAR)),
                    s_ptr(new h5_dataset("n_bad_modules", "general/n_bad_modules", NX_INT)),
                })),

                s_ptr(new h5_group("data", {
                    s_ptr(new h5_group(dataset_name, {}))
                }))
            }));
        }

        const h5_parent& get_format_definition() const override
        {
            return *file_format;
        }

        const unordered_map<string, boost::any>& get_default_values() const override
        {
            return *default_values;
        }

        void add_calculated_values(unordered_map<string, boost::any>& values) const override
        {
            // No calculated values.
        }

        void add_input_values(unordered_map<string, boost::any>& values,
            const unordered_map<string, boost::any>& input_values) const override
        {
            // Input value mapping is 1:1.
            for (const auto& input_value : input_values) {
                const auto& name = input_value.first;
                const auto& value = input_value.second;

                values[name] = value;
            }
        }

        const std::unordered_map<string, DATA_TYPE>& get_input_value_type() const override
        {
            return *input_value_type;
        }

        const unordered_map<string, string>& get_dataset_move_mapping() const override {
            return *dataset_move_mapping;
        }
};

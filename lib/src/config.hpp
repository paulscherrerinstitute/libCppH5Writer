#include <H5Cpp.h>
#include <string>

#ifndef CONFIG_H
#define CONFIG_H

namespace config
{
    extern int zmq_n_io_threads;
    extern int zmq_receive_timeout;
    extern int zmq_buffer_size_header;
    extern int zmq_buffer_size_data;

    extern size_t ring_buffer_n_slots;
    extern size_t ring_buffer_read_retry_interval;
    extern size_t statistics_buffer_adv_interval;

    extern hsize_t dataset_increase_step;
    extern hsize_t initial_dataset_size;
    extern std::string raw_image_dataset_name;

    extern uint32_t parameters_read_retry_interval;
}

#endif
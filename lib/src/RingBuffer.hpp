#ifndef RINGBUFFER_H
#define RINGBUFFER_H

#include <list>
#include <vector>
#include <map>
#include <mutex>
#include <memory>
#include <string>
#include <boost/any.hpp>
#include <chrono>
#include "date.h"

struct FrameMetadata
{
    // Ring buffer needed data.
    size_t buffer_slot_index;
    size_t frame_bytes_size;
    
    // Image header data.
    uint64_t frame_index;
    std::string endianness;
    std::string type;
    std::vector<size_t> frame_shape;

    // Pass additional header values.
    std::map<std::string, std::shared_ptr<char>> header_values;
};

class RingBuffer
{
    // Initialized in constructor.
    size_t n_slots = 0;
    std::vector<bool> ringbuffer_slots;    

    // Set in initialize().
    size_t slot_size = 0;
    size_t buffer_size = 0;
    char* frame_data_buffer = NULL;
    size_t write_index = 0;
    size_t buffer_used_slots = 0;
    bool ring_buffer_initialized = false;

    std::list< std::shared_ptr<FrameMetadata> > frame_metadata_queue;
    std::mutex frame_metadata_queue_mutex;
    std::mutex ringbuffer_slots_mutex;

    char* get_buffer_slot_address(size_t buffer_slot_index);

    public:
        RingBuffer(size_t n_slots);
        virtual ~RingBuffer();
        void initialize(size_t slot_size);
        
        void write(
            const std::shared_ptr<FrameMetadata> metadata,
            const char* data
        );
        std::pair<std::shared_ptr<FrameMetadata>, char*> read();
        void release(size_t buffer_slot_index);
        bool is_empty();
        bool is_initialized();
        size_t free_slots();
};

#endif

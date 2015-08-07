#pragma once
#include "FramedSource.hh"

#include "event_dispatcher_object.hpp"
#include "media_descriptor.hpp"
#include "media_sample.hpp"
#include "sample_attributes.hpp"
#include "media_sample.hpp"

namespace uvxx { namespace rtsp 
{
    class media_sample;
    class media_descriptor;
}}

namespace uvxx { namespace rtsp { namespace details
{
    using _framed_source_closed_delegate = std::function<void(int stream_id)>;

    class _live_framed_source : public FramedSource
    {
    public:
        explicit _live_framed_source(int stream_id);

        virtual ~_live_framed_source();

    public:
        int stream_id();

        void on_closed_set(_framed_source_closed_delegate source_closed);

        void deliver_sample(const media_sample& sample);

    protected:
        virtual void deliver_sample_override(const media_sample& sample);

    private:
        virtual void doGetNextFrame() override;

    private:
        int _stream_id;

        _framed_source_closed_delegate _on_source_closed;

    };
}}}
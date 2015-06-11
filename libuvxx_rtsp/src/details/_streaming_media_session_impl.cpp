#include "details/_streaming_media_session_impl.hpp"
#include "details/_media_session_impl.hpp"
#include "MediaSession.hh"

using namespace uvxx::pplx;
using namespace uvxx::rtsp;
using namespace uvxx::rtsp::details;

struct _streaming_media_io_state
{
    MediaSubsession* live_subsession;

    _streaming_media_session_impl* _streaming_media_session;
};

_streaming_media_session_impl::_streaming_media_session_impl(const std::vector<media_subsession>& subsessions)
{
    _subsessions = subsessions;
    _buffer.resize(100000);

    for (auto& subsession : _subsessions)
    {
        auto live_subsession = subsession.__media_subsession->live_media_subsession_get();

        FramedSource* subsessionSource = live_subsession->readSource();

        if(!subsessionSource)
        {
            continue;
        }

        subsessionSource->stopGettingFrames();

        /* set a 'BYE' handler for this subsession's RTCP instance: */
        if (live_subsession->rtcpInstance() != nullptr) 
        {
            live_subsession->miscPtr = new _streaming_media_io_state{ live_subsession, this};

            live_subsession->rtcpInstance()->setByeHandler(on_rtcp_bye, this);
        }
    }
}

void _streaming_media_session_impl::on_rtcp_bye(void* client_data)
{

}

uvxx::rtsp::details::_streaming_media_session_impl::~_streaming_media_session_impl()
{
    for (auto& subsession : _subsessions)
    {
        auto live_subsession = subsession.__media_subsession->live_media_subsession_get();

        if (live_subsession->rtcpInstance() != nullptr) 
        {
            if (live_subsession->miscPtr)
            {
                auto state = static_cast<_streaming_media_io_state*>(live_subsession->miscPtr);

                delete state;

                live_subsession->miscPtr = nullptr;
            }

            live_subsession->rtcpInstance()->setByeHandler(on_rtcp_bye, nullptr);
        }
    }
}

void uvxx::rtsp::details::_streaming_media_session_impl::on_frame_callback_set(std::function<bool()> callback)
{
    _on_frame_callback = callback;

    continue_reading();
}

void uvxx::rtsp::details::_streaming_media_session_impl::continue_reading()
{
    for (auto& subsession : _subsessions)
    {
         auto live_subsession = subsession.__media_subsession->live_media_subsession_get();

        FramedSource* subsessionSource = live_subsession->readSource();

        if (subsessionSource == nullptr)
        {
            continue;
        }

        if (subsessionSource->isCurrentlyAwaitingData())
        {
            continue;
        }

        subsessionSource->getNextFrame(_buffer.data(), _buffer.size(), on_after_getting_frame, live_subsession->miscPtr, nullptr, live_subsession->miscPtr);
    }
}

void _streaming_media_session_impl::on_after_getting_frame(void* client_data, unsigned packet_data_size, unsigned truncated_bytes, struct timeval presentation_time, unsigned duration_in_microseconds)
{
    auto io_state = static_cast<_streaming_media_io_state*>(client_data);

    bool continue_reading = io_state->_streaming_media_session->_on_frame_callback();

    if (continue_reading)
    {
        io_state->_streaming_media_session->continue_reading();
    }
}
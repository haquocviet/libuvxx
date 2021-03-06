#include "MediaSession.hh"
#include "media_sample.hpp"
#include "details/_streaming_media_session_impl.hpp"
#include "details/media_framers/_h264_framer.hpp"
#include "details/media_framers/_g711_audio_framer.hpp"
#include "details/media_framers/_g726_audio_framer.hpp"
#include "details/media_framers/_mpa_audio_framer.hpp"
#include "details/media_framers/_mpeg4_framer.hpp"

using namespace uvxx::pplx;
using namespace uvxx::rtsp;
using namespace uvxx::rtsp::details;
using namespace uvxx::rtsp::details::media_framers;

_streaming_media_session_impl::_streaming_media_session_impl(const media_session& session, 
                                                             std::vector<media_subsession> subsessions) :
    _subsessions(std::move(subsessions)),
    _session(session)
{
    for (auto& subsession : _subsessions)
    {
        std::shared_ptr<_media_framer_base> framer;

        auto codec_name = subsession.codec_name();

        if (codec_name == "H264")
        {
            framer = std::make_shared<_h264_framer>(subsession);
        }
        else if(codec_name == "PCMA" || codec_name == "PCMU")
        {
            framer = std::make_shared<_g711_audio_framer>(subsession);
        }
        else if(codec_name.find("G72") == 0 || codec_name.find("g72") == 0)
        {
            framer = std::make_shared<_g726_audio_framer>(subsession);
        }
        else if (codec_name =="MPA" || codec_name == "mpa")
        {
            framer = std::make_shared<_mpa_audio_framer>(subsession);
        }
        else if (codec_name == "MP4V-ES" || codec_name == "mp4-es")
        {
            framer = std::make_shared<_mpeg4_framer>(subsession);
        }
        else
        {
            framer = std::make_shared<_media_framer_base>(subsession);
        }

        _media_framers.push_back(framer);
    }
}

_streaming_media_session_impl::~_streaming_media_session_impl()
{
    
}

void _streaming_media_session_impl::on_sample_set(read_sample_delegate callback)
{
    _on_sample_callback = std::move(callback);

    for (auto& framer : _media_framers)
    {
        framer->on_sample_set(_on_sample_callback);
    }
}

void _streaming_media_session_impl::on_stream_closed_set(stream_closed_delegate callback)
{
    _stream_closed_delegate = std::move(callback);

    for (auto& framer : _media_framers)
    {
        framer->on_stream_closed_set(_stream_closed_delegate);
    }
}

stream_statistics _streaming_media_session_impl::stream_statistics_get(int stream_id) const
{
    for (auto& framer : _media_framers)
    {
        if(framer->stream_number() == stream_id)
        {
            auto stats = framer->qos_stats_get();

            return stream_statistics{ stats.total_kbytes_received(), 
                                      stats.percent_packet_loss(), 
                                      stats.expected_packet_count(),
                                      stats.received_packet_count(),
                                      stats.reset_statistics_interval() };
        }
    }

    throw std::out_of_range("streamid does not exist");
}

media_descriptor _streaming_media_session_impl::media_descriptor_get() const
{
    media_descriptor descriptor;

    for (auto& framer : _media_framers)
    {
        descriptor.add_stream_from_attributes(framer->stream_number(), framer->codec_name(), framer->working_sample());
    }

    return descriptor;
}

void _streaming_media_session_impl::read_stream_sample()
{
    for (auto& framer : _media_framers)
    {
        framer->begin_reading();
    }
}

bool _streaming_media_session_impl::operator=(std::nullptr_t /*rhs*/)
{
    close();

    return true;
}

void _streaming_media_session_impl::close()
{
    _on_sample_callback = nullptr;

    _media_framers.clear();

    _subsessions.clear();
}

bool _streaming_media_session_impl::operator==(std::nullptr_t /*rhs*/)
{
    return !_session ? true : false;
}

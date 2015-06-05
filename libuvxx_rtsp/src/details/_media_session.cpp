#pragma once
#include "details/_media_session.hpp"

using namespace uvxx::uvxx_rtsp::details;

_media_subsession::_media_subsession(MediaSubsession* live_subsession)
{
    _live_subsession = live_subsession;
}

_media_subsession::_media_subsession() : _live_subsession(nullptr)
{

}

_media_subsession::_media_subsession(_media_subsession&& rhs)
{
    _live_subsession = rhs._live_subsession;
    _live_subsession = nullptr;
}

_media_subsession& _media_subsession::operator=(_media_subsession&& rhs)
{
    return std::move(rhs);
}

bool _media_subsession::initiate(int use_special_rtp_offset /*= -1*/)
{
    return _live_subsession->initiate(use_special_rtp_offset);
}

_media_subsession::~_media_subsession()
{

}

_media_session::~_media_session()
{
    if (_live_session)
    {
        Medium::close(_live_session);
    }
}

void _media_session::set_media_session(const std::shared_ptr<UsageEnvironment>& usage_environment, MediaSession* live_session)
{
    reset();

    _live_session = live_session;
    _usage_environment = usage_environment;

    auto iter = std::make_unique<MediaSubsessionIterator>(*_live_session);

    while (auto live_subsession = iter->next())
    {
        if (!live_subsession)
        {
            break;
        }

        _subsessions.emplace_back(_media_subsession(live_subsession));
    }
}

void _media_session::reset()
{
    _subsessions.clear();
    _usage_environment = nullptr;
}

_media_session& _media_session::operator=(_media_session&& rhs)
{
    return std::move(rhs);
}

_media_session::_media_session(_media_session&& rhs)
{
    _live_session = rhs._live_session;
    _live_session = nullptr;

    _subsessions = std::move(_subsessions);
}

_media_session::_media_session() : _live_session(nullptr)
{

}

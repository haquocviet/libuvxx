#include "event_dispatcher_object.hpp"

namespace uvxx { namespace rtsp { namespace details
{
    /* implementation forwards */

    class _media_subsession;

    class _media_session;

    class _rtsp_client_impl;

    using _media_subsession_ptr = std::shared_ptr<_media_subsession>;

    using _media_session_ptr = std::shared_ptr<_media_session>;

}}}

namespace uvxx { namespace rtsp 
{
    class media_subsession : public uvxx::event_dispatcher_object
    {
    public:
        media_subsession(const details::_media_subsession_ptr& _media_subsession);

        media_subsession(const media_subsession&) = default;

        media_subsession& operator=(const media_subsession&) = default;

        media_subsession(media_subsession&& rhs);

        media_subsession& operator=(media_subsession&& rhs);

    public:
        const std::string codec_name_get() const;

    private:
        std::shared_ptr<details::_media_subsession> __media_subsession;
        friend uvxx::rtsp::details::_rtsp_client_impl;
    };

    class media_session : public uvxx::event_dispatcher_object
    {
    public:
        media_session();

        media_session(const details::_media_session_ptr& _media_session);

        media_session(const media_session&) = default;

        media_session& operator=(const media_session&) = default;

        media_session(media_session&& dispatcher);

        media_session& operator=(media_session&& rhs);

    public:

        const std::vector<media_subsession> subsessions_get() const;

        const size_t subsession_count() const;
        
        const media_subsession subsession_get(size_t index) const;

    private:
        details::_media_session_ptr __media_session;

    };
}}
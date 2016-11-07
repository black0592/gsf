#ifndef _GSF_SESSION_HEADER_
#define _GSF_SESSION_HEADER_

#include <event2/bufferevent.h>
#include <stdint.h>

namespace gsf
{
	class Session
	{
    public:
        Session(int32_t id);

        static void read_cb(::bufferevent *bev, void *ctx);
        static void write_cb(::bufferevent *bev, void *ctx);
		static void err_cb(::bufferevent *bev, short what, void *ctx);

        int32_t getid() const { return id_; }
    private:
        uint32_t id_;
	};
}


#endif

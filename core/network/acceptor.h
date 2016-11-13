#ifndef _GSF_ACCEPTOR_HEADER_
#define _GSF_ACCEPTOR_HEADER_

#include <stdint.h>
#include <string>
#include <memory>

#include <event2/bufferevent.h>
#include <event2/listener.h>

namespace gsf
{
	class AcceptHandler;
    class Session;

	struct AcceptorConfig
	{
		int32_t port;
		std::string address;
	};

	class AcceptHandler
	{
	public:
		virtual ~AcceptHandler(){}

        //! 有新的连接进来
		virtual void handler_new_connection(int acceptor_id, int session_id) = 0;
	};

	class Acceptor
	{
	public:
		Acceptor(uint32_t id, const AcceptorConfig &config);
		~Acceptor();

		int open(AcceptHandler *accept_handler);

		int close();

        void handler_new_connect(int32_t session_id);

		AcceptorConfig &get_config();

		uint32_t get_id() const;

	private:
		AcceptorConfig config_;
		AcceptHandler *handler_;

		::evconnlistener *listener_ptr_;

		uint32_t id_;
	};
}


#endif

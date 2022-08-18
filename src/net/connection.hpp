#pragma once

#include <boost/enable_shared_from_this.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/shared_ptr.hpp>
#include <array>
#include "dictionary/dictionary.hpp"

class connection : public boost::enable_shared_from_this<connection>
{

public:

    static constexpr int BUFF_SIZE = 1024;

    using pointer    = boost::shared_ptr<connection>;
    using io_context = boost::asio::io_context;
    using socket     = boost::asio::ip::tcp::socket;
    using streambuf  = boost::asio::streambuf;

    static pointer create( io_context& , dictionary& );

    socket& sck();
    void    start();

private:

    connection( io_context& , dictionary& );

    void process( boost::system::error_code , std::size_t );
    void parse();
    void print_disconnected();

    streambuf   m_buffer;
    socket      m_sck;
    dictionary& m_dictionary;
};
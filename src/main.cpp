#include <iostream>
#include <thread>
#include <boost/asio/io_context.hpp>
#include <boost/asio/post.hpp>
#include <boost/program_options.hpp>
#include <boost/thread.hpp>
#include "net/server.hpp"
#include "dictionary/dictionary.hpp"

namespace cli = boost::program_options;

cli::variables_map parse_args( int argc , char** argv );

int main( int argc , char** argv )
{
    auto config = parse_args( argc , argv );

    int n_threads = config[ "threads" ].as<int>();
    int port      = config[ "port"    ].as<int>();

    if ( n_threads <= 0 )
        n_threads = std::thread::hardware_concurrency();

    std::cout << std::string( 40 , '-' )    << '\n'
              << "In-memory dictionary server starting...\n"
              << std::string( 40 , '-' )    << '\n'
              << " threads : " << n_threads << '\n'
              << " port    : " << port      << '\n'
              << std::string( 40 , '-' )    << std::endl;

    try
    {
        boost::asio::io_context ctx;

        dictionary dict;
        server     srv { ctx , dict , port };

        boost::thread_group threads;

        for ( auto n = 0; n < n_threads; ++n )
            threads.create_thread(
                boost::bind(
                    &boost::asio::io_context::run ,
                    &ctx
                )
            );

        threads.join_all();
    }
    catch ( const std::exception& e )
    {
        std::cerr << e.what() << std::endl;
    }
}

cli::variables_map parse_args( int argc , char** argv )
{
    cli::options_description desc { "Allowed options" };

    desc.add_options()
        ( "help"    , "produce help message" )
        ( "threads" , cli::value<int>()->default_value( 0    ) , "Specify number of executer threads." )
        ( "port"    , cli::value<int>()->default_value( 9424 ) , "Specify port number to listen."      );

    cli::variables_map vm;
    cli::store( cli::parse_command_line( argc , argv , desc ) , vm );
    cli::notify( vm );

    if ( vm.count( "help" ) )
    {
        std::cout << desc << std::endl;

        exit( EXIT_SUCCESS );
    }

    return vm;
}
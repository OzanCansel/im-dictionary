#include "dictionary.hpp"
#include <mutex>

void dictionary::put( const std::string& key , const std::string& value )
{
    std::unique_lock<std::shared_timed_mutex> w( m_mtx );

    m_kv.insert( { key , value } );
}

void dictionary::remove( const std::string& key )
{
    std::unique_lock<std::shared_timed_mutex> w( m_mtx );

    m_kv.erase( key );
}

dictionary::nullable_str dictionary::get( const std::string& key ) const
{
    std::shared_lock<std::shared_timed_mutex> r( m_mtx );

    auto it = m_kv.find( key );

    if ( it == end( m_kv ) )
        return nullable_str {};
    else
        return it->second;
}
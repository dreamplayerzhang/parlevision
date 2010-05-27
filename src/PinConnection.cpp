#include "PinConnection.h"
#include "Types.h"
#include "Pin.h"

using namespace plv;

PinConnection::PinConnection( IOutputPin* producer, IInputPin* consumer ) :
        m_producer( producer ),
        m_consumer( consumer )
{
    assert(m_consumer.isNotNull());
    assert(m_producer.isNotNull());

    m_producer->addConnection(this);

    assert(!m_consumer->isConnected());
    m_consumer->setConnection(this);
    assert( m_consumer->isConnected() );

    // TODO should also be done in release mode at runtime.
    assert( producer->getTypeInfo() == consumer->getTypeInfo() );
}

PinConnection::~PinConnection()
{
    m_producer->removeConnection(this);
    m_consumer->removeConnection(this);

    assert( !m_consumer->isConnected() );
}

bool PinConnection::hasData()
{
    QMutexLocker lock( &m_mutex );
    return !m_queue.empty();
}

int PinConnection::size()
{
    QMutexLocker lock( &m_mutex );
    return static_cast<int>( m_queue.size() );
}

Data* PinConnection::get()
{
    QMutexLocker lock( &m_mutex );
    if( !m_queue.empty() )
    {
        RefPtr<Data> data = m_queue.front();
        Data* d = data.getPtr();
        assert( d->getRefCount() >= 2 );
        d->dec();
        m_queue.pop();
        return d;
    }
    return 0;
}

void PinConnection::put( Data* data )
{
    QMutexLocker lock( &m_mutex );
    data->inc();
    m_queue.push( data );
}

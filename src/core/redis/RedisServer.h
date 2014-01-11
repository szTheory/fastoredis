#pragma once

#include "core/IServer.h"

namespace fastoredis
{
    class RedisServer
            : public IServer
    {
        friend class ServersManager;
        Q_OBJECT
    public:
        virtual void syncWithServer(IServer *src);
        virtual void unSyncFromServer(IServer *src);

    protected:
        virtual void connectEvent(Events::ConnectResponceEvent *ev);
        virtual void disconnectEvent(Events::DisconnectResponceEvent *ev);
        virtual void executeEvent(Events::ExecuteResponceEvent *ev);
        virtual void loadDatabasesInfoEvent(Events::LoadDatabasesInfoResponceEvent *ev);
        virtual void loadDatabaseContentEvent(Events::LoadDatabaseContentResponceEvent *ev);

    private:
        RedisServer(const IDriverPtr &drv, bool isMaster);
    };
}
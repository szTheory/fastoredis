#include "core/redis/redis_infos.h"

#include <ostream>

#include "common/convert2string.h"

namespace fastoredis
{
    DataBaseInfo::DataBaseInfo(const std::string &name, size_t size)
        : name_(name), size_(size)
    {

    }

    ServerInfo::Server::Server::Server()
        : redis_version_(), redis_git_sha1_(), redis_git_dirty_(), redis_mode_(), os_(),
        arch_bits_(0), multiplexing_api_(), gcc_version_() ,process_id_(0),
        run_id_(), tcp_port_(0), uptime_in_seconds_(0), uptime_in_days_(0), lru_clock_(0)
    {

    }

    ServerInfo::Server::Server(const std::string& server_text)
        : redis_version_(), redis_git_sha1_(), redis_git_dirty_(), redis_mode_(), os_(),
        arch_bits_(0), multiplexing_api_(), gcc_version_() ,process_id_(0),
        run_id_(), tcp_port_(0), uptime_in_seconds_(0), uptime_in_days_(0), lru_clock_(0)
    {
        const std::string &src = server_text;
        size_t pos = 0;
        size_t start = 0;
        while((pos = src.find("\r\n", start)) != std::string::npos){
            std::string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            std::string field = line.substr(0, delem);
            std::string value = line.substr(delem + 1);
            if(field == REDIS_VERSION_LABEL){
                redis_version_ = value;
            }
            else if(field == REDIS_GIT_SHA1_LABEL){
                redis_git_sha1_ = value;
            }
            else if(field == REDIS_GIT_DIRTY_LABEL){
                redis_git_dirty_ = value;
            }
            else if(field == REDIS_MODE_LABEL){
                redis_mode_ = value;
            }
            else if(field == REDIS_OS_LABEL){
                os_ = value;
            }
            else if(field == REDIS_ARCH_BITS_LABEL){
                arch_bits_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == REDIS_MULTIPLEXING_API_LABEL){
                multiplexing_api_ = value;
            }
            else if(field == REDIS_GCC_VERSION_LABEL){
                gcc_version_ = value;
            }
            else if(field == REDIS_PROCESS_ID_LABEL){
                process_id_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == REDIS_RUN_ID_LABEL){
                run_id_ = value;
            }
            else if(field == REDIS_TCP_PORT_LABEL){
                tcp_port_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == REDIS_UPTIME_IN_SECONDS_LABEL){
                uptime_in_seconds_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == REDIS_UPTIME_IN_DAYS){
                uptime_in_days_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == REDIS_LRU_CLOCK_LABEL){
                lru_clock_ = common::convertFromString<uint32_t>(value);
            }
            start = pos + 2;
        }
    }

    common::Value* ServerInfo::Server::valueByIndex(unsigned char index) const
    {
        switch (index) {
        case 0:
            return new common::StringValue(redis_version_);
        case 1:
            return new common::StringValue(redis_git_sha1_);
        case 2:
            return new common::StringValue(redis_git_dirty_);
        case 3:
            return new common::StringValue(redis_mode_);
        case 4:
            return new common::StringValue(os_);
        case 5:
            return new common::FundamentalValue(arch_bits_);
        case 6:
            return new common::StringValue(multiplexing_api_);
        case 7:
            return new common::StringValue(gcc_version_);
        case 8:
            return new common::FundamentalValue(process_id_);
        case 9:
            return new common::StringValue(run_id_);
        case 10:
            return new common::FundamentalValue(tcp_port_);
        case 11:
            return new common::FundamentalValue(uptime_in_seconds_);
        case 12:
            return new common::FundamentalValue(uptime_in_days_);
        case 13:
            return new common::FundamentalValue(lru_clock_);
        default:
            NOTREACHED();
            break;
        }
        return NULL;
    }

    ServerInfo::Clients::Clients()
        : connected_clients_(0), client_longest_output_list_(0),
        client_biggest_input_buf_(0), blocked_clients_(0)
    {
    }

    ServerInfo::Clients::Clients(const std::string& client_text)
        : connected_clients_(0), client_longest_output_list_(0),
        client_biggest_input_buf_(0), blocked_clients_(0)
    {
        const std::string &src = client_text;
        size_t pos = 0;
        size_t start = 0;
        while((pos = src.find(("\r\n"), start)) != std::string::npos){
            std::string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            std::string field = line.substr(0, delem);
            std::string value = line.substr(delem + 1);
            if(field == (REDIS_CONNECTED_CLIENTS_LABEL)){
                connected_clients_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_CLIENT_LONGEST_OUTPUT_LIST_LABEL)){
                client_longest_output_list_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_CLIENT_BIGGEST_INPUT_BUF_LABEL)){
                client_biggest_input_buf_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_BLOCKED_CLIENTS_LABEL)){
                blocked_clients_ = common::convertFromString<uint32_t>(value);
            }
            start = pos + 2;
        }
    }

    common::Value* ServerInfo::Clients::valueByIndex(unsigned char index) const
    {
        switch (index) {
        case 0:
            return new common::FundamentalValue(connected_clients_);
        case 1:
            return new common::FundamentalValue(client_longest_output_list_);
        case 2:
            return new common::FundamentalValue(client_biggest_input_buf_);
        case 3:
            return new common::FundamentalValue(blocked_clients_);
        default:
            NOTREACHED();
            break;
        }
        return NULL;
    }

    ServerInfo::Memory::Memory()
        : used_memory_(0), used_memory_human_(), used_memory_rss_(0), used_memory_peak_(0),
          used_memory_peak_human_(), used_memory_lua_(0),mem_fragmentation_ratio_(0), mem_allocator_()
    {


    }

    ServerInfo::Memory::Memory(const std::string &memory_text)
        : used_memory_(0), used_memory_human_(), used_memory_rss_(0), used_memory_peak_(0),
          used_memory_peak_human_(), used_memory_lua_(0),mem_fragmentation_ratio_(0), mem_allocator_()
    {
        const std::string &src = memory_text;
        size_t pos = 0;
        size_t start = 0;
        while((pos = src.find(("\r\n"), start)) != std::string::npos){
            std::string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            std::string field = line.substr(0, delem);
            std::string value = line.substr(delem + 1);
            if(field == (REDIS_USED_MEMORY_LABEL)){
                used_memory_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_USED_MEMORY_HUMAN_LABEL)){
                used_memory_human_ = value;
            }
            else if(field == (REDIS_USED_MEMORY_RSS_LABEL)){
                used_memory_rss_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_USED_MEMORY_PEAK_LABEL)){
                used_memory_peak_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_USED_MEMORY_PEAK_HUMAN_LABEL)){
                used_memory_peak_human_ = value;
            }
            else if(field == (REDIS_USED_MEMORY_LUA_LABEL)){
                used_memory_lua_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_MEM_FRAGMENTATION_RATIO_LABEL)){
                mem_fragmentation_ratio_ = common::convertFromString<float>(value);
            }
            else if(field == (REDIS_MEM_ALLOCATOR_LABEL)){
                mem_allocator_ = value;
            }
            start = pos + 2;
        }
    }

    common::Value* ServerInfo::Memory::valueByIndex(unsigned char index) const
    {
        switch (index) {
        case 0:
            return new common::FundamentalValue(used_memory_);
        case 1:
            return new common::StringValue(used_memory_human_);
        case 2:
            return new common::FundamentalValue(used_memory_rss_);
        case 3:
            return new common::FundamentalValue(used_memory_peak_);
        case 4:
            return new common::StringValue(used_memory_peak_human_);
        case 5:
            return new common::FundamentalValue(used_memory_lua_);
        case 6:
            return new common::FundamentalValue(mem_fragmentation_ratio_);
        case 7:
            return new common::StringValue(mem_allocator_);
        default:
            NOTREACHED();
            break;
        }
        return NULL;
    }

    ServerInfo::Persistence::Persistence()
        : loading_(0), rdb_changes_since_last_save_(0), rdb_bgsave_in_progress_(0),
          rdb_last_save_time_(0), rdb_last_bgsave_status_(),
          rdb_last_bgsave_time_sec_(0), rdb_current_bgsave_time_sec_(0),
          aof_enabled_(0), aof_rewrite_in_progress_(0), aof_rewrite_scheduled_(0),
          aof_last_rewrite_time_sec_(0), aof_current_rewrite_time_sec_(0), aof_last_bgrewrite_status_()
    {

    }

    ServerInfo::Persistence::Persistence(const std::string& persistence_text)
        : loading_(0), rdb_changes_since_last_save_(0), rdb_bgsave_in_progress_(0),
          rdb_last_save_time_(0), rdb_last_bgsave_status_(),
          rdb_last_bgsave_time_sec_(0), rdb_current_bgsave_time_sec_(0),
          aof_enabled_(0), aof_rewrite_in_progress_(0), aof_rewrite_scheduled_(0),
          aof_last_rewrite_time_sec_(0), aof_current_rewrite_time_sec_(0), aof_last_bgrewrite_status_()
    {
        const std::string &src = persistence_text;
        size_t pos = 0;
        size_t start = 0;
        while((pos = src.find(("\r\n"), start)) != std::string::npos){
            std::string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            std::string field = line.substr(0, delem);
            std::string value = line.substr(delem + 1);
            if(field == (REDIS_LOADING_LABEL)){
                loading_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_RDB_CHANGES_SINCE_LAST_SAVE_LABEL)){
                rdb_changes_since_last_save_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_RDB_DGSAVE_IN_PROGRESS_LABEL)){
                rdb_bgsave_in_progress_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_RDB_LAST_SAVE_TIME_LABEL)){
                rdb_last_save_time_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_RDB_LAST_DGSAVE_TIME_SEC_LABEL)){
                rdb_last_bgsave_status_ = value;
            }
            else if(field == (REDIS_RDB_LAST_DGSAVE_TIME_SEC_LABEL)){
                rdb_last_bgsave_time_sec_ = common::convertFromString<int>(value);
            }
            else if(field == (REDIS_RDB_CURRENT_DGSAVE_TIME_SEC_LABEL)){
                rdb_current_bgsave_time_sec_ = common::convertFromString<int>(value);
            }
            else if(field == (REDIS_AOF_ENABLED_LABEL)){
                aof_enabled_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_AOF_REWRITE_IN_PROGRESS_LABEL)){
                aof_rewrite_in_progress_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_AOF_REWRITE_SHEDULED_LABEL)){
                aof_rewrite_scheduled_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_AOF_REWRITE_SHEDULED_LABEL)){
                aof_last_rewrite_time_sec_ = common::convertFromString<int>(value);
            }
            else if(field == (REDIS_AOF_LAST_REWRITE_TIME_SEC_LABEL)){
                aof_last_rewrite_time_sec_ = common::convertFromString<int>(value);
            }
            else if(field == (REDIS_AOF_CURRENT_REWRITE_TIME_SEC_LABEL)){
                aof_current_rewrite_time_sec_ = common::convertFromString<int>(value);
            }
            else if(field == (REDIS_AOF_LAST_DGREWRITE_STATUS_LABEL)){
                aof_last_bgrewrite_status_ = value;
            }
            start = pos + 2;
        }
    }

    common::Value* ServerInfo::Persistence::valueByIndex(unsigned char index) const
    {
        switch (index) {
        case 0:
            return new common::FundamentalValue(loading_);
        case 1:
            return new common::FundamentalValue(rdb_changes_since_last_save_);
        case 2:
            return new common::FundamentalValue(rdb_bgsave_in_progress_);
        case 3:
            return new common::FundamentalValue(rdb_last_save_time_);
        case 4:
            return new common::StringValue(rdb_last_bgsave_status_);
        case 5:
            return new common::FundamentalValue(rdb_last_bgsave_time_sec_);
        case 6:
            return new common::FundamentalValue(rdb_current_bgsave_time_sec_);
        case 7:
            return new common::FundamentalValue(aof_enabled_);
        case 8:
            return new common::FundamentalValue(aof_rewrite_in_progress_);
        case 9:
            return new common::FundamentalValue(aof_rewrite_scheduled_);
        case 10:
            return new common::FundamentalValue(aof_last_rewrite_time_sec_);
        case 11:
            return new common::FundamentalValue(aof_current_rewrite_time_sec_);
        case 12:
            return new common::StringValue(aof_last_bgrewrite_status_);
        default:
            NOTREACHED();
            break;
        }
        return NULL;
    }

    ServerInfo::Stats::Stats()
        : total_connections_received_(0), total_commands_processed_(0),
          instantaneous_ops_per_sec_(0), rejected_connections_(0),
          expired_keys_(0), evicted_keys_(0), keyspace_hits_(0),
          keyspace_misses_(0), pubsub_channels_(0),
          pubsub_patterns_(0), latest_fork_usec_(0)
    {

    }

    ServerInfo::Stats::Stats(const std::string& stats_text)
        : total_connections_received_(0), total_commands_processed_(0),
          instantaneous_ops_per_sec_(0), rejected_connections_(0),
          expired_keys_(0), evicted_keys_(0), keyspace_hits_(0),
          keyspace_misses_(0), pubsub_channels_(0),
          pubsub_patterns_(0), latest_fork_usec_(0)
    {
        const std::string &src = stats_text;
        size_t pos = 0;
        size_t start = 0;
        while((pos = src.find(("\r\n"), start)) != std::string::npos){
            std::string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            std::string field = line.substr(0, delem);
            std::string value = line.substr(delem + 1);
            if(field == (REDIS_TOTAL_CONNECTIONS_RECEIVED_LABEL)){
                total_connections_received_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_TOTAL_COMMANDS_PROCESSED_LABEL)){
                total_commands_processed_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_INSTANTANEOUS_OPS_PER_SEC_LABEL)){
                instantaneous_ops_per_sec_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_REJECTED_CONNECTIONS_LABEL)){
                rejected_connections_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_EXPIRED_KEYS_LABEL)){
                expired_keys_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_EVICTED_KEYS_LABEL)){
                evicted_keys_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_KEYSPACE_HITS_LABEL)){
                keyspace_hits_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_KEYSPACE_MISSES_LABEL)){
                keyspace_misses_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_PUBSUB_CHANNELS_LABEL)){
                pubsub_channels_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_PUBSUB_PATTERNS_LABEL)){
                pubsub_patterns_ = common::convertFromString<uint32_t>(value);
            }
            else if(field == (REDIS_LATEST_FORK_USEC_LABEL)){
                latest_fork_usec_ = common::convertFromString<uint32_t>(value);
            }
            start = pos + 2;
        }
    }

    common::Value* ServerInfo::Stats::valueByIndex(unsigned char index) const
    {
        switch (index) {
        case 0:
            return new common::FundamentalValue(total_connections_received_);
        case 1:
            return new common::FundamentalValue(total_commands_processed_);
        case 2:
            return new common::FundamentalValue(instantaneous_ops_per_sec_);
        case 3:
            return new common::FundamentalValue(rejected_connections_);
        case 4:
            return new common::FundamentalValue(expired_keys_);
        case 5:
            return new common::FundamentalValue(evicted_keys_);
        case 6:
            return new common::FundamentalValue(keyspace_hits_);
        case 7:
            return new common::FundamentalValue(keyspace_misses_);
        case 8:
            return new common::FundamentalValue(pubsub_channels_);
        case 9:
            return new common::FundamentalValue(pubsub_patterns_);
        case 10:
            return new common::FundamentalValue(latest_fork_usec_);
        default:
            NOTREACHED();
            break;
        }
        return NULL;
    }

    ServerInfo::Replication::Replication()
        : role_(), connected_slaves_(0)
    {

    }

    ServerInfo::Replication::Replication(const std::string &replication_text)
        : role_(), connected_slaves_(0)
    {
        const std::string &src = replication_text;
        size_t pos = 0;
        size_t start = 0;

        while((pos = src.find(("\r\n"), start)) != std::string::npos){
            std::string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            std::string field = line.substr(0, delem);
            std::string value = line.substr(delem + 1);
            if(field == (REDIS_ROLE_LABEL)){
                role_ = value;
            }
            else if(field == (REDIS_CONNECTED_SLAVES_LABEL)){
                connected_slaves_ = common::convertFromString<uint32_t>(value);
            }
            start = pos + 2;
        }
    }

    common::Value* ServerInfo::Replication::valueByIndex(unsigned char index) const
    {
        switch (index) {
        case 0:
            return new common::StringValue(role_);
        case 1:
            return new common::FundamentalValue(connected_slaves_);
        default:
            NOTREACHED();
            break;
        }
        return NULL;
    }

    ServerInfo::Cpu::Cpu()
        : used_cpu_sys_(0), used_cpu_user_(0), used_cpu_sys_children_(0), used_cpu_user_children_(0)
    {

    }

    ServerInfo::Cpu::Cpu(const std::string &cpu_text)
        : used_cpu_sys_(0), used_cpu_user_(0), used_cpu_sys_children_(0), used_cpu_user_children_(0)
    {
        const std::string &src = cpu_text;
        size_t pos = 0;
        size_t start = 0;
        while((pos = src.find(("\r\n"), start)) != std::string::npos){
            std::string line = src.substr(start, pos-start);
            size_t delem = line.find_first_of(':');
            std::string field = line.substr(0, delem);
            std::string value = line.substr(delem + 1);
            if(field == (REDIS_USED_CPU_SYS_LABEL)){
                used_cpu_sys_ = common::convertFromString<float>(value);
            }
            else if(field == (REDIS_USED_CPU_USER_LABEL)){
                used_cpu_user_ = common::convertFromString<float>(value);
            }
            else if(field == (REDIS_USED_CPU_SYS_CHILDREN_LABEL)){
                used_cpu_sys_children_ = common::convertFromString<float>(value);
            }
            else if(field == (REDIS_USED_CPU_USER_CHILDREN_LABEL)){
                used_cpu_user_children_ = common::convertFromString<float>(value);
            }
            start = pos + 2;
        }
    }

    common::Value* ServerInfo::Cpu::valueByIndex(unsigned char index) const
    {
        switch (index) {
        case 0:
            return new common::FundamentalValue(used_cpu_sys_);
        case 1:
            return new common::FundamentalValue(used_cpu_user_);
        case 2:
            return new common::FundamentalValue(used_cpu_sys_children_);
        case 3:
            return new common::FundamentalValue(used_cpu_user_children_);
        default:
            NOTREACHED();
            break;
        }
        return NULL;
    }

    common::Value* ServerInfo::Keyspace::valueByIndex(unsigned char index) const
    {
        return NULL;
    }

    ServerInfo::ServerInfo()
    {

    }

    ServerInfo::ServerInfo(const Server &serv, const Clients &clients, const Memory &memory,
                           const Persistence &pers, const Stats &stats, const Replication &repl, const Cpu &cpu, const Keyspace &key)
        : server_(serv), clients_(clients), memory_(memory), persistence_(pers), stats_(stats), replication_(repl), cpu_(cpu), keySp_(key)
    {

    }

    common::Value* ServerInfo::valueByIndexes(unsigned char property, unsigned char field) const
    {
        switch (property) {
        case 0:
            return server_.valueByIndex(field);
        case 1:
            return clients_.valueByIndex(field);
        case 2:
            return memory_.valueByIndex(field);
        case 3:
            return persistence_.valueByIndex(field);
        case 4:
            return stats_.valueByIndex(field);
        case 5:
            return replication_.valueByIndex(field);
        case 6:
            return cpu_.valueByIndex(field);
        case 7:
            return keySp_.valueByIndex(field);
        default:
            NOTREACHED();
            break;
        }
        return NULL;
    }

    std::ostream& operator<<(std::ostream& out, const ServerInfo::Server& value)
    {
        return out << (REDIS_VERSION_LABEL":") << value.redis_version_ << ("\r\n")
                   << (REDIS_GIT_SHA1_LABEL":") << value.redis_git_sha1_ << ("\r\n")
                   << (REDIS_GIT_DIRTY_LABEL":") << value.redis_git_dirty_ << ("\r\n")
                   << (REDIS_MODE_LABEL":") << value.redis_mode_ << ("\r\n")
                   << (REDIS_OS_LABEL":") << value.os_ << ("\r\n")
                   << (REDIS_ARCH_BITS_LABEL":") << value.arch_bits_ << ("\r\n")
                   << (REDIS_MULTIPLEXING_API_LABEL":") << value.multiplexing_api_ << ("\r\n")
                   << (REDIS_GCC_VERSION_LABEL":") << value.gcc_version_ << ("\r\n")
                   << (REDIS_PROCESS_ID_LABEL":") << value.process_id_ << ("\r\n")
                   << (REDIS_RUN_ID_LABEL":") << value.run_id_ << ("\r\n")
                   << (REDIS_TCP_PORT_LABEL":") << value.tcp_port_ << ("\r\n")
                   << (REDIS_UPTIME_IN_SECONDS_LABEL":") << value.uptime_in_seconds_ << ("\r\n")
                   << (REDIS_UPTIME_IN_DAYS":") << value.uptime_in_days_ << ("\r\n")
                   << (REDIS_LRU_CLOCK_LABEL":") << value.lru_clock_ << ("\r\n");
    }

    std::ostream& operator<<(std::ostream& out, const ServerInfo::Clients& value)
    {
        return out << (REDIS_CONNECTED_CLIENTS_LABEL":") << value.connected_clients_ << ("\r\n")
                   << (REDIS_CLIENT_LONGEST_OUTPUT_LIST_LABEL":") << value.client_longest_output_list_ << ("\r\n")
                   << (REDIS_CLIENT_BIGGEST_INPUT_BUF_LABEL":") << value.client_biggest_input_buf_ << ("\r\n")
                   << (REDIS_BLOCKED_CLIENTS_LABEL":") << value.blocked_clients_ << ("\r\n");
    }

    std::ostream& operator<<(std::ostream& out, const ServerInfo::Memory& value)
    {
        return out << (REDIS_USED_MEMORY_LABEL":") << value.used_memory_ << ("\r\n")
                   << (REDIS_USED_MEMORY_HUMAN_LABEL":") << value.used_memory_human_ << ("\r\n")
                   << (REDIS_USED_MEMORY_RSS_LABEL":") << value.used_memory_rss_ << ("\r\n")
                   << (REDIS_USED_MEMORY_PEAK_LABEL":") << value.used_memory_peak_ << ("\r\n")
                   << (REDIS_USED_MEMORY_PEAK_HUMAN_LABEL":") << value.used_memory_peak_human_ << ("\r\n")
                   << (REDIS_USED_MEMORY_LUA_LABEL":") << value.used_memory_lua_ << ("\r\n")
                   << (REDIS_MEM_FRAGMENTATION_RATIO_LABEL":") << value.mem_fragmentation_ratio_ << ("\r\n")
                   << (REDIS_MEM_ALLOCATOR_LABEL":") << value.mem_allocator_ << ("\r\n");
    }

    std::ostream& operator<<(std::ostream& out, const ServerInfo::Persistence& value)
    {
        return out << (REDIS_LOADING_LABEL":") << value.loading_ << ("\r\n")
                   << (REDIS_RDB_CHANGES_SINCE_LAST_SAVE_LABEL":") << value.rdb_changes_since_last_save_ << ("\r\n")
                   << (REDIS_RDB_DGSAVE_IN_PROGRESS_LABEL":") << value.rdb_bgsave_in_progress_ << ("\r\n")
                   << (REDIS_RDB_LAST_SAVE_TIME_LABEL":") << value.rdb_last_save_time_ << ("\r\n")
                   << (REDIS_RDB_LAST_DGSAVE_STATUS_LABEL":") << value.rdb_last_bgsave_status_ << ("\r\n")
                   << (REDIS_RDB_LAST_DGSAVE_TIME_SEC_LABEL":") << value.rdb_last_bgsave_time_sec_ << ("\r\n")
                   << (REDIS_RDB_CURRENT_DGSAVE_TIME_SEC_LABEL":") << value.rdb_current_bgsave_time_sec_ << ("\r\n")
                   << (REDIS_AOF_ENABLED_LABEL":") << value.aof_enabled_ << ("\r\n")
                   << (REDIS_AOF_REWRITE_IN_PROGRESS_LABEL":") << value.aof_rewrite_in_progress_ << ("\r\n")
                   << (REDIS_AOF_REWRITE_SHEDULED_LABEL":") << value.aof_rewrite_scheduled_ << ("\r\n")
                   << (REDIS_AOF_LAST_REWRITE_TIME_SEC_LABEL":") << value.aof_last_rewrite_time_sec_ << ("\r\n")
                   << (REDIS_AOF_CURRENT_REWRITE_TIME_SEC_LABEL":") << value.aof_current_rewrite_time_sec_ << ("\r\n")
                   << (REDIS_AOF_LAST_DGREWRITE_STATUS_LABEL":") << value.aof_last_bgrewrite_status_ << ("\r\n");
    }

    std::ostream& operator<<(std::ostream& out, const ServerInfo::Stats& value)
    {
        return out << (REDIS_TOTAL_CONNECTIONS_RECEIVED_LABEL":") << value.total_connections_received_ << ("\r\n")
                   << (REDIS_TOTAL_COMMANDS_PROCESSED_LABEL":") << value.total_commands_processed_ << ("\r\n")
                   << (REDIS_INSTANTANEOUS_OPS_PER_SEC_LABEL":") << value.instantaneous_ops_per_sec_ << ("\r\n")
                   << (REDIS_REJECTED_CONNECTIONS_LABEL":") << value.rejected_connections_ << ("\r\n")
                   << (REDIS_EXPIRED_KEYS_LABEL":") << value.expired_keys_ << ("\r\n")
                   << (REDIS_EVICTED_KEYS_LABEL":") << value.evicted_keys_ << ("\r\n")
                   << (REDIS_KEYSPACE_HITS_LABEL":") << value.keyspace_hits_ << ("\r\n")
                   << (REDIS_KEYSPACE_MISSES_LABEL":") << value.keyspace_misses_ << ("\r\n")
                   << (REDIS_PUBSUB_CHANNELS_LABEL":") << value.pubsub_channels_ << ("\r\n")
                   << (REDIS_PUBSUB_PATTERNS_LABEL":") << value.pubsub_patterns_ << ("\r\n")
                   << (REDIS_LATEST_FORK_USEC_LABEL":") << value.latest_fork_usec_ << ("\r\n");
    }

    std::ostream& operator<<(std::ostream& out, const ServerInfo::Replication& value)
    {
        return out << (REDIS_ROLE_LABEL":") << value.role_ << ("\r\n")
                   << (REDIS_CONNECTED_SLAVES_LABEL":") << value.connected_slaves_ << ("\r\n");
    }

    std::ostream& operator<<(std::ostream& out, const ServerInfo::Cpu& value)
    {
        return out << (REDIS_USED_CPU_SYS_LABEL":") << value.used_cpu_sys_ << ("\r\n")
                   << (REDIS_USED_CPU_USER_LABEL":") << value.used_cpu_user_ << ("\r\n")
                   << (REDIS_USED_CPU_SYS_CHILDREN_LABEL":") << value.used_cpu_sys_children_ << ("\r\n")
                   << (REDIS_USED_CPU_USER_CHILDREN_LABEL":") << value.used_cpu_user_children_ << ("\r\n");
    }

    std::ostream& operator<<(std::ostream& out, const ServerInfo& value)
    {
        //"# Server", "# Clients", "# Memory", "# Persistence", "# Stats", "# Replication", "# CPU", "# Keyspace"
        return out << (REDIS_SERVER_LABEL"\r\n") << value.server_ << (REDIS_CLIENTS_LABEL"\r\n") << value.clients_ << (REDIS_MEMORY_LABEL"\r\n") << value.memory_
                   << (REDIS_PERSISTENCE_LABEL"\r\n") << value.persistence_ << (REDIS_STATS_LABEL"\r\n") << value.stats_
                   << (REDIS_REPLICATION_LABEL"\r\n") << value.replication_ << (REDIS_CPU_LABEL"\r\n") << value.cpu_ << (REDIS_KEYSPACE_LABEL"\r\n");
    }

    ServerPropertyInfo::ServerPropertyInfo()
    {

    }

    ServerInfo makeServerInfo(const std::string &content)
    {
        ServerInfo result;
        int j = 0;
        std::string word;
        size_t pos = 0;
        for(int i = 0; i < content.size(); ++i)
        {
            char ch = content[i];
            word += ch;
            if(word == redisHeaders[j]){
                if(j+1 != sizeof(redisHeaders)/sizeof(*redisHeaders)){
                    pos = content.find(redisHeaders[j+1], pos);
                }
                else{
                    break;
                }

                if(pos != std::string::npos)
                {
                    std::string part = content.substr(i + 1, pos - i - 1 );
                    switch(j)
                    {
                    case 0:
                        result.server_ = ServerInfo::Server(part);
                        break;
                    case 1:
                        result.clients_ = ServerInfo::Clients(part);
                        break;
                    case 2:
                        result.memory_ = ServerInfo::Memory(part);
                        break;
                    case 3:
                        result.persistence_ = ServerInfo::Persistence(part);
                        break;
                    case 4:
                        result.stats_ = ServerInfo::Stats(part);
                        break;
                    case 5:
                        result.replication_ = ServerInfo::Replication(part);
                        break;
                    case 6:
                        result.cpu_ = ServerInfo::Cpu(part);
                        break;
                    default:
                        break;
                    }
                    i = pos-1;
                    ++j;
                }
                word.clear();
            }
        }
        return result;
    }

    ServerInfo makeServerInfo(const FastoObjectPtr &root)
    {
        const common::string16 content = common::convertToString16(root);
        return makeServerInfo(common::convertToString(content));
    }

    ServerPropertyInfo makeServerProperty(const FastoObjectPtr &root)
    {
        ServerPropertyInfo inf;
        FastoObject::child_container_type childrens = root->childrens();
        for(int i = 0; i < childrens.size(); i+=2){
            const std::string c1 = childrens[i]->toString();
            const std::string c2 = childrens[i+1]->toString();
            inf.propertyes_.push_back(std::make_pair(c1, c2));
        }
        return inf;
    }
}
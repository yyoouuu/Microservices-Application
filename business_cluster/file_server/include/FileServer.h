#pragma once
#include <brpc/server.h>
#include <butil/logging.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string>
#include <fstream>
#include <limits>
#include <uuid/uuid.h>
#include <string>

#include "file.pb.h"
#include "logger.h"
#include "amo_etcd.h" // 服务注册模块封装

namespace amo_file_server
{
    // 功能类
    class FileServerImpl : public mic_ser::FileService
    {
    private:
        std::string _storage_path;
        static constexpr size_t MAX_FILE_SIZE = 50 * 1024 * 1024; // 最大 50 MB 文件

    public:
        using s_ptr = std::shared_ptr<FileServerImpl>;
        FileServerImpl(const std::string &storage_path) : _storage_path(storage_path)
        {
            if (mkdir(_storage_path.c_str(), 0775) != 0 && errno != EEXIST)
            {
                APP_ERROR("Failed to create directory: {}", storage_path);
                return;
            }

            if (_storage_path.back() != '/')
            {
                _storage_path.push_back('/');
            }
        }

    private:
        // 通用文件读取函数
        bool ReadFile(const std::string &filename, std::string &body)
        {
            if (access(filename.c_str(), F_OK) != 0)
            {
                return false;
            }

            std::ifstream file(filename, std::ios::binary | std::ios::ate);
            if (!file.is_open())
            {
                return false;
            }

            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);

            if (size > static_cast<std::streamsize>(MAX_FILE_SIZE))
            {
                APP_ERROR("File too large: {}", filename);
                return false;
            }

            body.resize(size);
            if (!file.read(body.data(), size))
            {
                return false;
            }

            file.close();
            return true;
        }

        // 通用文件写入函数
        bool WriteFile(const std::string &filename, const std::string &body)
        {
            std::ofstream file(filename, std::ios::binary);
            if (!file.is_open())
                return false;

            file.write(body.data(), body.size());
            file.close();
            return true;
        }

        //为文件生成一个唯一uudi作为文件名 以及 文件ID
        inline std::string generateUUID() {
    uuid_t uuid;
    uuid_generate_random(uuid);

    char uuid_str[37]; // 36 chars + '\0'
    uuid_unparse(uuid, uuid_str);

    return std::string(uuid_str);
}
    public:
        // ---------------- 单文件下载 ----------------
        void GetSingleFile(google::protobuf::RpcController *controller,
                           const mic_ser::GetSingleFileReq *request,
                           mic_ser::GetSingleFileRsp *response,
                           google::protobuf::Closure *done) override
        {
            brpc::ClosureGuard rpc_guard(done);

            response->set_request_id(request->request_id());
            std::string file_id = request->file_id();
            std::string filename = _storage_path + file_id;

            std::string body;
            if (!ReadFile(filename, body))
            {
                response->set_success(false);
                response->set_errmsg("Failed to read file: " + file_id);
                APP_ERROR("{} Failed to read file: {}", request->request_id(), filename);
                return;
            }

            response->set_success(true);
            auto *data = response->mutable_file_data();
            data->set_file_id(file_id);
            data->set_file_content(body);

            APP_DEBUG("{} Successfully read file: {}, size: {} bytes",
                      request->request_id(), file_id, body.size());
        }

        // ---------------- 多文件下载 ----------------
        void GetMultiFile(google::protobuf::RpcController *controller,
                          const mic_ser::GetMultiFileReq *request,
                          mic_ser::GetMultiFileRsp *response,
                          google::protobuf::Closure *done) override
        {
            brpc::ClosureGuard rpc_guard(done);
            response->set_request_id(request->request_id());

            bool has_error = false;

            for (int i = 0; i < request->file_id_list_size(); ++i)
            {
                std::string file_id = request->file_id_list(i);
                std::string filename = _storage_path + file_id;

                std::string body;
                if (!ReadFile(filename, body))
                {
                    APP_ERROR("{} Failed to read file: {}", request->request_id(), filename);
                    has_error = true;
                    continue; // 不中断循环，保留已读文件
                }

                auto* file_map = response->mutable_file_data();
auto& file_entry = (*file_map)[file_id];

file_entry.set_file_id(file_id);
file_entry.set_file_content(body);

                APP_DEBUG("{} Successfully read file: {}, size: {} bytes",
                          request->request_id(), file_id, body.size());
            }

            response->set_success(!has_error);
            if (has_error)
            {
                response->set_errmsg("Some files failed to read");
            }
        }

        // ---------------- 单文件上传 ----------------
        void PutSingleFile(google::protobuf::RpcController *controller,
                           const mic_ser::PutSingleFileReq *request,
                           mic_ser::PutSingleFileRsp *response,
                           google::protobuf::Closure *done) override
        {
            brpc::ClosureGuard rpc_guard(done);
            response->set_request_id(request->request_id());

            const auto &file_data = request->file_data();
            std::string file_id = generateUUID();
            std::string filename = _storage_path + file_id;

            if (!WriteFile(filename, file_data.file_content()))
            {
                response->set_success(false);
                response->set_errmsg("Failed to write file: " + file_id);
                APP_ERROR("{} Failed to write file: {}", request->request_id(), filename);
                return;
            }

            auto *info = response->mutable_file_info();
            info->set_file_id(file_id);
            info->set_file_name(file_data.file_name());
            info->set_file_size(file_data.file_size());

            response->set_success(true);
        }

        // ---------------- 多文件上传 ----------------
        void PutMultiFile(google::protobuf::RpcController *controller,
                          const mic_ser::PutMultiFileReq *request,
                          mic_ser::PutMultiFileRsp *response,
                          google::protobuf::Closure *done) override
        {
            brpc::ClosureGuard rpc_guard(done);
            response->set_request_id(request->request_id());

            bool has_error = false;

            for (int i = 0; i < request->file_data_list_size(); ++i)
            {
                const auto &file_data = request->file_data_list(i);
                std::string file_id = generateUUID();
                std::string filename = _storage_path + file_id;

                if (!WriteFile(filename, file_data.file_content()))
                {
                    APP_ERROR("{} Failed to write file index {}: {}", request->request_id(), i, filename);
                    has_error = true;
                    continue; // 不中断上传，保留已成功的文件
                }

                auto *info = response->add_file_info();
                info->set_file_id(file_id);
                info->set_file_name(file_data.file_name());
                info->set_file_size(file_data.file_size());
            }

            response->set_success(!has_error);
            if (has_error)
            {
                response->set_errmsg("Some files failed to write");
            }
        }
    };
    class FileServer
    {
    private:
        // 服务注册对象
        etcd_ns::Registry::s_ptr _register;
        // rpc服务对象
        std::shared_ptr<brpc::Server> _rpc_server;

    public:
        using s_ptr = std::shared_ptr<FileServer>;
        FileServer(etcd_ns::Registry::s_ptr registry,
                   std::shared_ptr<brpc::Server> &rpc_server) : _register(registry), _rpc_server(rpc_server)
        {

        }
        // 搭建RPC服务器，并启动服务器
        void start()
        {
            _rpc_server->RunUntilAskedToQuit();
            APP_INFO("rpc_server启动成功");
        }


    };

    class FileServerBuiler
    {
    private:
        // 服务注册对象
        etcd_ns::Registry::s_ptr _register;
        // rpc服务对象
        std::shared_ptr<brpc::Server> _rpc_server;

    public:
        void make_reg_object(const std::string &host, const std::string &service_name, const std::string &service_host)
        {
            _register = std::make_shared<etcd_ns::Registry>(host, 3);
            _register->registry(service_name, service_host);
        }

        void make_rpc_server(uint16_t port, int32_t timeout, uint8_t num_threads, const std::string &storage_path)
        {

            _rpc_server = std::make_shared<brpc::Server>();
            FileServerImpl *file_service = new FileServerImpl(storage_path);
            int ret = _rpc_server->AddService(file_service, brpc::ServiceOwnership::SERVER_OWNS_SERVICE);
            if (ret == -1)
            {
                APP_ERROR("添加Rpc服务失败！");
                abort();
            }
            else
            {
                APP_INFO("添加Rpc服务成功！");
            }

            brpc::ServerOptions options;

            options.idle_timeout_sec = timeout;
            options.num_threads = num_threads;
            ret = _rpc_server->Start(port, &options);
            if (ret == -1)
            {
                APP_ERROR("服务启动失败！");
                abort();
            }
            else
            {
                APP_INFO("服务启动成功！");
            }
        }
        FileServer::s_ptr build()
        {
            if (!_register)
            {
                APP_ERROR("还未初始化服务注册模块！");
                abort();
            }

            if (!_rpc_server)
            {
                APP_ERROR("还未初始化RPC服务器模块！");
                abort();
            }

            FileServer::s_ptr server = std::make_shared<FileServer>(
                 _register, _rpc_server);
            APP_INFO("语音识别子服务实例构建成功！");

            return server;
        }
    };

} // namespace amo_file_server

project-root/
├── CMakeLists.txt                          # 项目根 CMake 配置
├── third_party/                            # 第三方平台集成
│   ├── baidu_voice/                        # 百度语音
│   │   ├── CMakeLists.txt
│   │   ├── include/
│   │   └── src/
│   └── aliyun/                             # 阿里云
│       ├── CMakeLists.txt
│       ├── include/
│       └── src/
├── gateway/                                # 服务网关
│   ├── CMakeLists.txt
│   ├── http/                               # HTTP 网关
│   │   ├── include/
│   │   └── src/
│   └── websocket/                          # WebSocket 网关
│       ├── include/
│       └── src/
├── business_cluster/                       # 业务集群
│   ├── CMakeLists.txt
│   ├── user/                               # 用户服务
│   │   ├── CMakeLists.txt
│   │   ├── include/
│   │   └── src/
│   ├── friend/                             # 好友服务
│   │   ├── CMakeLists.txt
│   │   ├── include/
│   │   └── src/
│   └── message_forward/                    # 消息转发
│       ├── CMakeLists.txt
│       ├── include/
│       └── src/
├── data_center/                            # 业务数据中心
│   ├── CMakeLists.txt
│   ├── message_storage/                    # 消息存储
│   │   ├── CMakeLists.txt
│   │   ├── include/
│   │   └── src/
│   ├── file_storage/                       # 文件存储
│   │   ├── CMakeLists.txt
│   │   ├── voice/                          # 语音存储
│   │   ├── include/
│   │   └── src/
│   └── ode/                                # ODE 数据模块
│       ├── CMakeLists.txt
│       ├── include/
│       └── src/
├── infrastructure/                         # 基础设施
│   ├── service_discovery/                  # 服务发现
│   │   ├── CMakeLists.txt
│   │   ├── service_register/               # 服务注册
│   │   ├── identity_auth/                  # 身份认证
│   │   ├── include/
│   │   └── src/
│   ├── registry_center/                    # 注册中心
│   │   ├── CMakeLists.txt
│   │   ├── etcd/                           # etcd 客户端
│   │   ├── include/
│   │   └── src/
│   ├── cache/                              # 缓存
│   │   ├── CMakeLists.txt
│   │   ├── redis/                          # Redis 客户端
│   │   ├── include/
│   │   └── src/
│   ├── message_queue/                      # 消息队列
│   │   ├── CMakeLists.txt
│   │   ├── rabbitmq/                       # RabbitMQ 客户端
│   │   ├── include/
│   │   └── src/
│   └── search_engine/                      # 搜索引擎
│       ├── CMakeLists.txt
│       ├── elasticsearch/                  # ES 客户端
│       ├── include/
│       └── src/
├── common/                                 # 公共组件
│   ├── CMakeLists.txt
│   ├── brpc/                               # B.RPC 框架
│   ├── protobuf/                           # Protobuf 定义
│   ├── gflags/                             # GFlags 配置
│   ├── spdlog/                             # SPDLog 日志
│   ├── include/
│   └── src/
├── client/                                 # 客户端请求处理
│   ├── CMakeLists.txt
│   ├── request/                            # 客户端请求
│   ├── push/                               # 服务端推送
│   ├── include/
│   └── src/
└── build_scripts/                          # 构建脚本
    ├── build.sh
    ├── docker/
    └── ci_cd/
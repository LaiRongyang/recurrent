#ifndef _PROTOCOL_H_
#define _PROTOCOL_H_

#include <cstdint>

#define HEX_LENGTH 16
#define PATH_MAX_LEN 260
#define KEY_LENGTH 32
#define MAX_RSA_LEN 256
#define MSG_MAX_LENGTH (8 * 1024)
#define NEMBER_LENGTH (1024 * 3)

/**
 * @enum ErrorCode
 * @brief Error code
 */
#define COM_OK 0x00000000U                   // 成功
#define COM_NO_DEVICE 0x00000001             // 无设备
#define COM_COMMUNICATION_FAILURE 0x00000002 // 通信错误
#define COM_EXECUTE_FAILURE 0x00000003       // 执行失败
#define COM_INVALID_PARAMETER 0x00000004     // 无效参数
#define COM_UNSUPPORT_COMMAND 0x00000005     // 不支持的命令
#define COM_SOFTWARE_ERROR 0x00000006        // 软件错误
#define COM_HARDWARE_ERROR 0x00000007        // 硬件错误
#define COM_PROCEDURE_ERROR 0x00000008       // 程序错误，接口调用顺序不对
#define COM_SPACE_NOT_ENOUGH 0x00000009      // 没有足够的空间
#define COM_DATA_ERROR 0x0000000a            // 数据长度、格式等与与其不符
#define COM_NO_POWER 0x0000000b              // 无权限
#define COM_TIMEOUT 0x0000000c               // 超时错误
#define COM_SYSTEM_ERROR 0x0000000d          // 系统错误
#define COM_CONFIGURE_ERROR 0x0000000e       // 配置读取失败
#define COM_UNKNOWN_ERROR 0x000000ff         // 未定义的错误类型
// OPENSSL错误码
#define COM_SSL_NONE 0x00000101
#define COM_SSL_SSL 0x00000102
#define COM_SSL_WANT_READ 0x00000103
#define COM_SSL_WANT_WRITE 0x00000104
#define COM_SSL_WANT_X509_LOOKUP 0x00000105
#define COM_SSL_SYSCALL 0x00000106 // look at error stack/return
#define COM_SSL_ZERO_RETURN 0x00000107
#define COM_SSL_WANT_CONNECT 0x00000108
#define COM_SSL_WANT_ACCEPT 0x00000109
#define COM_SSL_WANT_ASYNC 0x0000010a
#define COM_SSL_WANT_ASYNC_JOB 0x0000010b
#define COM_SSL_WANT_CLIENT_HELLO_CB 0x0000010c
#define COM_PEM_FORMATE_FAILED 0x0000010d
#define COM_PKCS8_PRIKEY_DECODE 0x0000010e
#define COM_UNSUP_PRIKEY_FORMATE 0x0000010f
#define COM_PKCS1_PRIKEY_DECODE 0x00000110
#define COM_SSL_UNKNOWN_ERROR 0x000001ff // 未定义的错误类型

// read_key
#define COM_ECU_KEY 0x01
#define COM_OTA_KEY_FACTOR 0x02
#define COM_SECOC_KEY 0x03
#define COM_KEY_RSA_PUB 0x04

enum EN_MSG_TYPE
{
    EN_MSG_TYPE_REQ = 0,
    EN_MSG_TYPE_RESP,
    EN_MSG_TYPE_EVENT,
    EN_MSG_TYPE_FILE,
    EN_MSG_TYPE_CREATE,
    EN_MSG_TYPE_DESTORY,
};

enum EN_CMD_ID
{
    EN_CMD_ID_CREATE = 0,  // 创建连接
    EN_CMD_ID_DESTORY,     // 关闭连接
    EN_CMD_ID_OPENDEV,     // 打开芯片
    EN_CMD_ID_CLOSEDEV,    // 关闭芯片
    EN_CMD_ID_CALLBACK,    // 注册回调函数
    EN_CMD_ID_GEN_RAND,    // 获取随机数
    // EN_CMD_ID_GEN_CRS,     // 生成CRS
    EN_CMD_ID_READ_CERT,   // 读取证书
    EN_CMD_ID_WRITE_CERT,  // 写入证书
    EN_CMD_ID_READ_FILE,   // 读取文件
    EN_CMD_ID_WRITE_FILE,  // 写入文件
    EN_CMD_ID_BASE64_EN,   // Base64编码
    EN_CMD_ID_BASE64_DE,   // Base64解码
    EN_CMD_ID_RSA_PRI_CAL, // RSA私钥计算
    EN_CMD_ID_RSA_PUB_CAL, // RSA公钥计算
    // EN_CMD_ID_CER_PUB_CAL,        // 证书公钥计算
    EN_CMD_ID_HMAC_SHA256,        // HMAC-sha256
    EN_CMD_ID_HASH_SHA,           // hash_sha
    EN_CMD_ID_SM3_SHA,            // sm3_hash
    EN_CMD_ID_SM4_CAL,            // sm4计算
    EN_CMD_ID_IMPORT_SYM_KEY,     // 导入对称密钥
    EN_CMD_ID_AES_CALCULATE_INIT, // AES参数初始化
    EN_CMD_ID_AES_CAL,            // AES计算
    EN_CMD_ID_AES_FINAL_CAL,      // AES最后一组加密
    EN_CMD_ID_AES_GCM_EN,         // AES_GCM加密
    EN_CMD_ID_AES_GCM_DE,         // AES_GCM解密
    EN_CMD_ID_IMPORT_RSA_KEY,     // 导入RSA秘钥
    EN_CMD_ID_GEN_RSA_KEY,        // 生成RSA秘钥
    EN_CMD_ID_RSA_SIGN,           // RSA签名
    EN_CMD_ID_RSA_VERIFY,         // RSA签名验证
    EN_CMD_ID_TLS_INIT,           // TLS初始化
    EN_CMD_ID_TLS_DO_HANDSHAKE,   // 客户端跟服务端进行TLS握手验证
    EN_CMD_ID_TLS_SERVER_CREATE,  // 服务端跟客户端进行TLS握手验证
    EN_CMD_ID_TLS_CONNECT,        // 客户端建立tls连接
    EN_CMD_ID_TLS_ACCEPT,         // 服务端建立tls连接
    EN_CMD_ID_TLS_WRITE,          // TLS写入数据
    EN_CMD_ID_TLS_READ,           // TLS读取数据
    EN_CMD_ID_TLS_DESTROY,        // TLS销毁
    EN_CMD_ID_READ_KEY,           // 读取密钥
    EN_CMD_ID_READ_PUB,           // 读取公钥
    EN_CMD_ID_GEN_READ_CRS,       // 生成CRS并读取
};

// 对称秘钥算法类型
enum EN_SYMKEY_TYPE
{
    EN_SYMKEY_TYPE_AES_128 = 0,
    EN_SYMKEY_TYPE_AES_192,
    EN_SYMKEY_TYPE_AES_256,
};

#pragma pack(push, 1)

// 消息头结构
typedef struct tagMsgHead
{
    std::uint32_t msg_size; // 消息长度，包括消息头
    std::int32_t msg_type;          // 消息类型，EN_MSG_TYPE枚举类
    std::int32_t channel_id;        // 通道ID，socket为当前客户端socket id，消息队列为进程id
    std::int32_t session_id;        // 会话ID，由服务端统一分配，从1开始递增
    std::int32_t cmd_id;            // 命令ID，EN_CMD_ID枚举类
    std::int32_t req_id;            // 请求ID，由客户端维护，每个客户端从1开始递增
} MSGHEAD, *LPMSGHEAD;

typedef struct tagReqHead
{
    MSGHEAD msg_head;
    std::uint32_t data_len;
} REQHEAD, *LPREQHEAD;

typedef struct tagRespHead
{
    MSGHEAD msg_head;
    std::uint32_t error_code;
    std::uint32_t data_len;
} RESPHEAD, *LPRESPHEAD;

// 请求生成随机数的帧结构
typedef struct tagReqRandom
{
    std::uint32_t random_len; // 随机数长度
} REQRANDOM, *LPREQRANDOM;

// 返回生成随机数的帧结构
typedef struct tagRespRandom
{
    std::uint32_t data_len; // 随机数长度
    std::int8_t random[0];        // 随机数
} RESPRANDOM, *LPRESPRANDOM;

typedef struct tagReqReadCer
{
    std::uint8_t id;   // 证书索引号
    std::uint8_t type; // 证书类型，EN_CERT_TYPE枚举类，0-der，1-pem
} REQREADCER, *LPREQREADCER;

typedef struct tagRespReadCer
{
    std::uint32_t cer_len; // 证书长度
    std::uint8_t cer[0]; // 证书数据
} RESPREADCER, *LPRESPREADCER;

typedef struct tagReqReadFile
{
    std::uint8_t id; // 文件索引号
} REQREADFILE, *LPREQREADFILE;

typedef struct tagRespReadFile
{
    std::uint32_t data_len; // 文件数据长度
    std::int8_t data[0];          // 文件数据
} RESPREADFILE, *LPRESPREADFILE;

typedef struct tagReqWriteCer
{
    std::uint8_t id;     // 证书索引号
    std::uint8_t type;   // 证书类型，EN_CERT_TYPE枚举类，0-der，1-pem
    std::uint32_t cer_len; // 证书长度
    std::int8_t cer[0];          // 证书数据
} REQWRITECER, *LPREQWRITECER;

typedef struct tagReqWriteFile
{
    std::uint8_t id;      // 文件索引号
    std::uint32_t data_len; // 文件数据长度
    std::int8_t data[0];          // 文件数据
} REQWRITEFILE, *LPREQWRITEFILE;

typedef struct tagReqImportSymKey
{
    std::uint8_t alg;    // 算法类型；EN_ALGORITHM_TYPE枚举类，1为AES-128，2为AES-192，3为AES-256；4为SM4-128 en；5为SM4-128 de；
    std::uint8_t key_id; // 密钥ID; 取值为1-127
    std::int8_t key[KEY_LENGTH]; // 密钥
} REQIMPORTSYMKEY, *LPREQIMPORTSYMKEY;

typedef struct tagReqInitAESCal
{
    std::uint8_t flag;           // 加解密操作类型，EN_OPERATE_TYPE枚举类，0-加密； 1-解密
    std::uint8_t mode;           // 加密分组模式，EN_BLOCK_CIPHER_MODE枚举类，0-ECB； 1-CBC
    std::uint8_t padding;        // 填充模式，EN_PADDING_MODE枚举类，0-不填充；1-PKCS#5；2-PKCS#7（CBC模式此参数有效，ECB模式此参数无效）
    std::uint8_t key_id;         // 密钥ID; 取值为1-127
    std::uint8_t iv[HEX_LENGTH]; // 初始向量（CBC模式此参数有效，ECB模式此参数无效可置为NULL）
    std::uint8_t key_len;        // 密钥长度
    std::int8_t key[KEY_LENGTH];         // 密钥
} REQINITAESCAL, *LPREQINITAESCAL;

typedef struct tagReqAESCal
{
    std::uint32_t input_len; // 待加密/解密的数据的长度
    std::int8_t input_data[0];     // 待加密/解密的数据
} REQAESCAL, *LPREQAESCAL;

typedef struct tagRespAESCal
{
    std::uint32_t output_len; // 已解密/加密的数据的长度
    std::int8_t output_data[0];     // 已解密/加密的数据
} RESPAESCAL, *LPRESPAESCAL;

typedef struct tagRespFinAESCal
{
    std::uint8_t output_len; // 已解密/加密的数据的长度
    std::int8_t output_data[0];      // 已解密/加密的数据
} RESPFINAESCAL, *LPRESPFINAESCAL;

typedef struct tagReqEnAESGCMCal
{
    std::uint8_t opt;                 // 加解密操作类型，EN_OPERATE_TYPE枚举类，0-加密；1-解密
    std::uint8_t key_id;              // 密钥ID; 取值为1-127
    std::uint8_t key_len;             // 密钥长度
    std::int8_t key[KEY_LENGTH];              // 密钥
    std::uint32_t input_len;            // 待加密的数据的长度
    std::uint32_t add_len;              // 附加消息的长度
    std::uint8_t iv_data[HEX_LENGTH]; // 初始向量
    std::int8_t data[0];                      // InputData、AddData
} REQENAESGCMCAL, *LPREQENAESGCMCAL;

typedef struct tagRespEnAESGCMCal
{
    std::uint8_t tag_len;              // 消息认证码的长度
    std::uint8_t tag_data[HEX_LENGTH]; // 消息认证码
    std::uint32_t data_len;              // OutputData的长度
    std::int8_t output_data[0];                // 已加密的数据
} RESPENAESGCMCAL, *LPRESPENAESGCMCAL;

typedef struct tagReqDeAESGCMCalculate
{
    std::uint8_t opt;                  // 加解密操作类型，EN_OPERATE_TYPE枚举类，0-加密； 1-解密
    std::uint8_t key_id;               // 密钥ID; 取值为1-127
    std::uint8_t key_len;              // 密钥长度
    std::int8_t key[KEY_LENGTH];               // 密钥
    std::uint32_t input_len;             // 待解密的数据的长度
    std::uint32_t add_len;               // 附加消息的长度
    std::uint8_t iv_data[HEX_LENGTH];  // 初始化向量
    std::uint8_t tag_len;              // 消息认证码的长度
    std::uint8_t tag_data[HEX_LENGTH]; // 消息认证码
    std::int8_t data[0];                       // InputData、AddData
} REQDEAESGCMCAL, *LPREQDEAESGCMCAL;

typedef struct tagRespDeAESGCMCal
{
    std::uint32_t data_len; // OutputData的长度
    std::int8_t output_data[0];   // 被加密的数据
} RESPDEAESGCMCAL, *LPRESPDEAESGCMCAL;

typedef struct tagReqImportRSAKey
{
    std::uint8_t file_path[PATH_MAX_LEN]; // pem密钥文件路径
} REQIMPORTRSAKEY, *LPREQIMPORTRSAKEY;

typedef struct tagReqRSAPublicCal
{
    std::uint32_t input_len; // 待加密/解密的数据的长度
    std::int8_t input_data[0];     // 待加密/解密的数据
} REQRSAPUBLICCAL, *LPREQRSAPUBLICCAL;

typedef struct tagRespRSAPublicCal
{
    std::uint32_t output_len; // 已解密/加密的数据的长度
    std::int8_t output_data[0];     // 已解密/加密的数据
} RESPRSAPUBLICCAL, *LPRESPRSAPUBLICCAL;

typedef struct tagReqRSAPrivateCal
{
    std::uint32_t input_len; // 待加密/解密的数据的长度
    std::int8_t input_data[0];     // 待加密/解密的数据
} REQRSAPRIVATECAL, *LPREQRSAPRIVATECAL;

typedef struct tagRespRSAPrivateCal
{
    std::uint32_t output_len; // 已解密/加密的数据的长度
    std::int8_t output_data[0];     // 已解密/加密的数据
} RESPRSAPRIVATECAL, *LPRESPRSAPRIVATECAL;

typedef struct tagReqRSASignCal
{
    std::uint8_t hash_type; // 哈希计算的类型，0为SHA-1；1为SHA-256；2为SHA-512
    std::uint8_t padding;   // 填充模式，EN_PADDING_MODE枚举类，0为不填充；6为PKCS#1；7为PKCS#1_OAEP；8为PSS
    std::uint32_t input_len;  // 待签名的数据的长度
    std::int8_t input_data[0];      // 待签名的数据
} REQRSASIGNCAL, *LPREQRSASIGNCAL;

typedef struct tagRespRSASignCAL
{
    std::uint32_t output_len; // 签名的长度
    std::int8_t output_data[0];     // 签名
} RESPRSASIGNCAL, *LPRESPRSASIGNCAL;

typedef struct tagReqVerifySign
{
    std::uint8_t hash_type; // 哈希计算的类型，0为SHA-1；1为SHA-256；2为SHA-512
    std::uint8_t padding;   // 填充模式，EN_PADDING_MODE枚举类，0为不填充；6为PKCS#1；7为PKCS#1_OAEP；8为PSS
    std::uint32_t input_len;  // 被签名的数据的长度
    std::uint32_t sig_len;    // 签名的长度
    std::int8_t data[0];            // InputData、SignData
} REQVERIFYSIGN, *LPREQVERIFYSIGN;

typedef struct tagReqEnBase64
{
    std::uint32_t input_len; // 待编码的数据的长度
    std::int8_t input_data[0];     // 待编码的数据
} REQENBASE64, *LPREQENBASE64;

typedef struct tagRespEnBase64
{
    std::uint32_t output_len; // 已编码的数据的长度
    std::int8_t output_data[0];     // 已编码的数据
} RESPENBASE64, *LPRESPENBASE64;

typedef struct tagReqDeBase64
{
    std::uint32_t input_len; // 已编码的数据的长度
    std::int8_t input_data[0];     // 已编码的数据
} REQDEBASE64, *LPREQDEBASE64;

typedef struct tagRespDeBase64
{
    std::uint32_t output_len;      // 解码后的数据的长度
    std::uint8_t output_data[0]; // 解码后的数据
} RESPDEBASE64, *LPRESPDEBASE64;

typedef struct tagReqGenerateCSR
{
    std::uint32_t input_len; // 生成CSR信息长度
    std::int8_t input_data[0];     // 生成CSR的信息
} REQGENERATECSR, *LPREQGENERATECSR;

typedef struct tagRespGenerateCSR
{
    std::uint32_t out_len; // 生成的CSR数据长度
    std::int8_t out_data[0];     // 生成的CSR数据
} RESPGENERATECSR, *LPRESPGENERATECSR;

typedef struct tagReqGenerateReadCSR
{
    std::uint32_t input_len; // 生成CSR信息长度
    std::int8_t input_data[0];     // 生成CSR的信息
} REQGENERATEReadCSR, *LPREQGENERATEReadCSR;

typedef struct tagRespGenerateReadCSR
{
    std::uint32_t out_len; // 生成的CSR数据长度
    std::int8_t out_data[0];     // 生成的CSR数据
} RESPGENERATEReadCSR, *LPRESPGENERATEReadCSR;

typedef struct tagReqHmac
{
    std::uint32_t key_len;   // 密钥长度
    std::uint32_t input_len; // 输入数据
    std::int8_t input_data[0];     // 密钥及输入数据
} REQHMAC, *LPREQHMAC;

typedef struct tagRespHmac
{
    std::uint32_t out_len;    // 数据长度
    std::int8_t out_data[0]; // HMAC-Sha256计算结果
} RESPHMAC, *LPRESPHMAC;

typedef struct tagReqSM3
{
    std::uint32_t input_len; // 输入数据长度
    std::int8_t input_data[0];     // 输入数据
} REQSM3, *LPREQSM3;

typedef struct tagRespSM3
{
    std::uint32_t out_len;    // 数据长度
    std::int8_t out_data[0]; // Sm3Hash计算结果
} RESPSM3, *LPRESPSM3;

typedef struct tagReqSha
{
    std::uint8_t sha_type; // SHA算法类型，1-SHA1 2-SHA256
    std::uint32_t input_len; // 输入数据长度
    std::int8_t input_data[0];     // 输入数据
} REQSHA, *LPREQSHA;

typedef struct tagRespSha
{
    std::uint32_t out_len;    // 数据长度
    std::int8_t out_data[0]; // sha计算结果
} RESPSHA, *LPRESPSHA;

typedef struct tagReqSM4Calculate
{
    std::uint8_t opt;                 // 加解密操作类型，EN_OPERATE_TYPE枚举类，0-加密； 1-解密
    std::uint8_t mode;                // 指示运算模式，EN_BLOCK_CIPHER_MODE枚举类
    std::uint8_t padding;             // 指示填充模式，EN_PADDING_MODE枚举类
    std::uint8_t key_id;              // 密钥ID, 0-使用外部密钥; others-使用内部对应位置密钥
    std::int8_t key[HEX_LENGTH];              // 密钥
    std::uint8_t iv_data[HEX_LENGTH]; // 初始化向量
    std::uint32_t input_len;            // 待加密的数据的长度
    std::int8_t data[0];                      // InputData
} REQSM4CAL, *LPREQSM4CAL;

typedef struct tagRespSM4Cal
{
    std::uint32_t data_len; // OutputData的长度
    std::int8_t output_data[0];   // 被加密的数据
} RESPSM4CAL, *LPRESPSM4CAL;

typedef struct tagReqTLSShake
{
    std::uint32_t socket_id;
    std::int8_t hostname[PATH_MAX_LEN]; // 主机名
} REQTLSSHAKE, *LPREQTLSSHAKE;

typedef struct tagRespTLSShake
{
    unsigned short handle; // 指针名称
} RESPTLSSHAKE, *LPRESPTLSSHAKE;

typedef struct tagReqTLSSerCreate
{
    std::uint32_t socket_id;
    std::int8_t hostname[PATH_MAX_LEN]; // 主机名
} REQTLSSERCREATE, *LPREQTLSSerCreate;

typedef struct tagRespTLSSerCreate
{
    unsigned short handle; // 指针名称
} RESPTLSSERCREATE, *LPRESPTLSSERCREATE;

typedef struct tagReqTLSConnect
{
    unsigned short handle; // 指针名称
} REQTLSCONNECT, *LPREQTLSCONNECT;

typedef struct tagReqTLSAccept
{
    unsigned short handle; // 指针名称
} REQTLSACCPET, *LPREQTLSACCEPT;

typedef struct tagReqTLSWrite
{
    std::uint32_t writedata_len; // 要写入数据的长度
    unsigned short handle;      // 指针名称
    std::int8_t write_data[0];         // 要写入的数据
} REQTLSWRITE, *LPREQTLSWRITE;

typedef struct tagReqTLSRead
{
    std::uint32_t length;   // 预读取数据的长度
    unsigned short handle; // 指针名称
} REQTLSREAD, *LPREQTLSREAD;

typedef struct tagRespTLSRead
{
    std::uint32_t length; // 读取数据的长度
    std::int8_t read_data[0];   // 读取出的数据
} RESPTLSREAD, *LPRESPTLSREAD;

typedef struct tagReqTLSDestroy
{
    unsigned short handle; // 指针名称
} REQTLSDESTROY, *LPREQTLSDESTROY;

typedef struct tagReqReadKey
{
    std::uint32_t key_id;    // 密钥
    std::uint32_t input_len; // 输入接收密钥缓存key分配内存的长度
} REQREADKEY, *LPREQREADKEY;

typedef struct tagRespReadKey
{
    std::uint32_t output_len; // 输出读取的密钥长度
    std::int8_t key[0];             // 读取的密钥
} RESPREADKEY, *LPRESPREADKEY;

typedef struct tagReqCertPubkeyCal
{
    std::uint8_t type;              // 证书类型，EN_FILE_ID枚举类，0-外部证书；1-DSSAD
    std::int8_t file_path[PATH_MAX_LEN]; // 外部证书路径，type为1时无效
    std::uint32_t input_len;          // 输入数据的长度
    std::int8_t in_data[0];                 // 输入数据
} REQCERTPUBKEYCAL, *LPREQCERTPUBKEYCAL;

typedef struct tagRespCertPubkeyCal
{
    std::uint32_t output_len; // 输出数据的长度
    std::int8_t output_data[0];     // 输出数据
} RESPCERTPUBKEYCAL, *LPRESPCERTPUBKEYCAL;

typedef struct tagRespReadPub
{
    std::uint32_t bits;            //公钥模数长度，1024/2048/3072/4096
    std::uint8_t m[MAX_RSA_LEN]; //靠前存储
    std::uint32_t e;               //可能取值65537
} RESPREADPub, *LPRESPREADPub;

#pragma pack(pop)

#endif
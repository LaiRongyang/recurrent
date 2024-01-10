/**
* @file securityservice.h
* @brief 安全服务引擎
* @details
* @author liu.zhen (liu.zhen30@byd.com) jiang.qi (jiang.qi4@byd.com)
* @version 1.1.0
* @date 2023/03/13 17:30:00
* @copyright Copyright (c) 2023 比亚迪股份有限公司
*/

#ifndef _SECURITY_SERVICE_H_
#define _SECURITY_SERVICE_H_


/**
* @enum ErrorCode
* @brief Error code
*/
#define ERR_CODE_OK                     0x00000000U // 成功
#define ERR_CODE_NO_DEVICE              0x00000001  // 无设备
#define ERR_CODE_COMMUNICATION_FAILURE  0x00000002  // 通信错误
#define ERR_CODE_EXECUTE_FAILURE        0x00000003  // 执行失败
#define ERR_CODE_INVALID_PARAMETER      0x00000004  // 无效参数
#define ERR_CODE_UNSUPPORT_COMMAND      0x00000005  // 不支持的命令
#define ERR_CODE_SOFTWARE_ERROR         0x00000006  // 软件错误
#define ERR_CODE_HARDWARE_ERROR         0x00000007  // 硬件错误
#define ERR_CODE_PROCEDURE_ERROR        0x00000008  // 程序错误，接口调用顺序不对
#define ERR_CODE_SPACE_NOT_ENOUGH       0x00000009  // 没有足够的空间
#define ERR_CODE_DATA_ERROR             0x0000000a  // 数据长度、格式等与与其不符
#define ERR_CODE_NO_POWER               0x0000000b  // 无权限
#define ERR_CODE_TIMEOUT                0x0000000c  // 超时错误
#define ERR_CODE_SYSTEM_ERROR           0x0000000d  // 系统错误
#define ERR_CODE_CONFIGURE_ERROR        0x0000000e  // 配置读取失败
#define ERR_CODE_POINTER_NULL           0x0000000f  // 指针为空
#define ERR_CODE_UNKNOWN                0x000000ff  // 未定义的错误类型


// CSS_read_certificate
#define CERT_ID_ROOT            0x01   // 根证书
#define CERT_ID_SUB             0x02    // 子证书
#define CERT_ID_USER            0x03   // 用户证书
#define CERT_ID_CSR             0x04    //
#define CERT_ID_SERVER          0x05 //服务器证书

// CSS_write_file/CSS_read_file
#define KET_FACTOR_1            0x01
#define KET_FACTOR_2            0x02

// CSS_read_key
#define ECU_KEY                 0x01
#define OTA_KEY_FACTOR          0x02
#define SECOC_KEY               0x03

// 对称密钥索引号
#define KEY_ID_SECOC    0x01 // AES128
#define KEY_ID_PATAM    0x02 // AES128
#define KEY_ID_DATA     0x03 // AES256

typedef void* TLS_HANLDE; 

#ifdef __cplusplus
extern "C" {
#endif

/**
* @brief open_service function
* @details 打开引擎服务
* @return error code
*/
unsigned int CSS_open_service();

/**
* @brief close_service function
* @details 关闭引擎服务
* @return error code
*/
void CSS_close_service();

/**
* @brief CSS_read_file function
* @details 读取文件数据
* @param id [in] 文件ID
* @param data [out] 读取的数据
* @param len [in/out] 读取数据长度
* @return error code
*/
unsigned int CSS_read_file(unsigned int id, unsigned char* data, unsigned int* len);

/**
* @brief CSS_read_certificate function
* @details 读取证书文件
* @param id [in] 证书ID
* @param type [in] 输入的证书数据类型，0-der，1-pem
* @param cert [out] 证书数据
* @param len [in/out] 证书数据长度
* @return error code
*/
unsigned int CSS_read_certificate(unsigned int id, unsigned char type, unsigned char *cert, unsigned int *len);

/**
* @brief CSS_read_key function
* @details 读取密钥
* @param id [in] 密钥ID
* @param key [out] 读取的密钥
* @param len [in/out] 输入接收密钥缓存key分配内存的长度/输出读取的密钥长度
* @return error code
*/
unsigned int CSS_read_key(unsigned int id, unsigned char* key, unsigned int* len);

/**
* @brief CSS_write_file function
* @details 写入文件
* @param id [in] 文件ID
* @param cert [in] 需要写入的数据
* @param len [in] 数据长度
* @return error code
*/
unsigned int CSS_write_file(unsigned int id, unsigned char* data, unsigned int len);

/**
* @brief CSS_generate_random function
* @details 生成随机数
* @param len [in] 随机数长度
* @param rand [out] 随机数
* @return error code
*/
unsigned int CSS_generate_random(unsigned int len, unsigned char* rand);

/**
* @brief CSS_aes_calculate_init function
* @details 初始化AES加解密参数
* @param flag [in] 加解密操作类型，0-加密; 1-解密
* @param mode [in] 加密分组模式，0-ECB; 1-CBC
* @param padding [in] ECB模式此参数无效，CBC模式为填充模式，0-不填充; 1-PKCS#5; 2-PKCS#7
* @param iv [in] ECB模式此参数无效，可置为NULL；CBC模式为初始向量，16字节
* @param key_id [in] 密钥ID: 0-使用外部密钥; others-使用内部密钥
* @param key_len [in] 密钥长度，key_id为0时有效
* @param key [in] 密钥，key_id为0时有效，key_id为其他值时可置为空
* @return error code
*/
unsigned int CSS_aes_calculate_init(int flag, int mode, int padding, unsigned char* iv, int key_id, int key_len, unsigned char* key);

/**
* @brief CSS_aes_calculate function
* @details AES加解密计算
* @param in_data [in] 输入数据
* @param in_len [in] 输入数据长度
* @param out_data [out] 输出数据
* @param out_len [out] 输出数据长度
* @return error code
*/
unsigned int CSS_aes_calculate(unsigned char* in_data, unsigned int in_len, unsigned char* out_data, unsigned int* out_len);

/**
* @brief CSS_aes_calsulate_final function
* @details 结束AES加解密计算，处理最后一组数据
* @param data [out] 输出数据
* @param len [out] 输出数据长度
* @return error code
*/
unsigned int CSS_aes_calculate_final(unsigned char* data, unsigned int* len);

/**
* @brief CSS_aes_gcm function
* @details AES-GCM加解密计算
* @param in_data [in] 输入数据
* @param in_len [in] 输入数据长度
* @param opt [in] 指示加密、解密模式EN_OPERATE_TYPE
* @param key_id [in] 
* @param key [in] 密钥，16字节、24字节或32字节 （128位、192位或256位）
* @param key_len [in] 密钥长度 （16 24 32）
* @param iv [in] 初始化向量
* @param iv_len [in] 初始化向量长度
* @param add [in] CCM模式的附加消息
* @param add_len [in] CCM中附加消息的长度
* @param tag_len [in] tag的长度
* @param tag [in][out] CCM模式中认证值 enOpt=EN_OPERATE_TYPE_ENCRYPT 表示输出 enOpt=EN_OPERATE_TYPE_DECRYPT 表示输入
* @param out_data [out] 输出数据
* @return error code
*/
unsigned int CSS_aes_gcm(unsigned char *in_data, int in_len, int opt, int key_id, unsigned char *key, int key_len, unsigned char *iv, int iv_len, 
                    unsigned char *add, int add_len, int tag_len, unsigned char *tag, unsigned char *out_data);

/**
* @brief CSS_sm4_calculate function
* @details sm4加解密计算
* @param in_data [in] 输入数据
* @param in_len [in] 输入数据长度
* @param opt [in] 指示加密、解密EN_OPERATE_TYPE
* @param mode [in] 指示运算模式，支持ECB，CBC，CFB，OFB，CTR模式
* @param padding [in] 填充模式，EN_PADDING_MODE
* @param key_id [in] 密钥ID: 0-使用外部密钥; others-使用内部对应位置密钥
* @param key [in] 密钥，16字节（128位）
* @param iv [in] 采用ECB模式此参数无效，可置为NULL;其他模式时为初始向量，16字节
* @param out_data [out] 输出数据
* @param out_len [out] 输出数据长度
* @return error code
*/
unsigned int CSS_sm4_calculate(unsigned char* in_data, unsigned int in_len, int opt, int mode, int padding, int key_id, unsigned char* key, 
                            unsigned char* iv, unsigned char* out_data, unsigned int* out_len);

/**
* @brief CSS_rsa_public_calculate function
* @details RSA公钥计算
* @param in_data [in] 输入数据
* @param in_len [in] 输入数据长度
* @param out_data [out] 输出数据
* @param out_len [out] 输出数据长度
* @return error code
*/
unsigned int CSS_rsa_public_calculate(unsigned char* in_data, unsigned int in_len, unsigned char* out_data, unsigned int* out_len);

/**
* @brief CSS_rsa_private_calculate function
* @details RSA私钥计算
* @param in_data [in] 输入数据
* @param in_len [in] 输入数据长度
* @param out_data [out] 输出数据
* @param out_len [out] 输出数据长度
* @return error code
*/
unsigned int CSS_rsa_private_calculate(unsigned char* in_data, unsigned int in_len, unsigned char* out_data, unsigned int* out_len);

/**
* @brief CSS_rsa_sign function
* @details RSA数据签名
* @param in_data [in] 输入数据
* @param in_len [in] 输入数据长度
* @param out_data [out] 输出数据
* @param out_len [out] 输出数据长度
* @return error code
*/
unsigned int CSS_rsa_sign(unsigned char *in_data, unsigned int in_len, unsigned char *out_data, unsigned int *out_len);

/**
* @brief CSS_rsa_verify function
* @details RSA数据签名验签
* @param in_data [in] 输入数据
* @param in_len [in] 输入数据长度
* @param in_sign [in] 输入签名
* @return error code
*/
unsigned int CSS_rsa_verify(unsigned char *in_data, unsigned int in_len, unsigned char *in_sign);

/**
* @brief CSS_base64_en function
* @details base64编码
* @param in_data [in] 输入数据
* @param in_len [in] 输入数据长度
* @param out_data [out] 输出数据
* @param out_len [out] 输出数据长度
* @return error code
*/
unsigned int CSS_base64_en(unsigned char* in_data, unsigned int in_len, char* out_data, unsigned int* out_len);

/**
* @brief CSS_base64_de function
* @details base64解码
* @param in_data [in] 输入数据
* @param in_len [in] 输入数据长度
* @param out_data [out] 输出数据
* @param out_len [out] 输出数据长度
* @return error code
*/
unsigned int CSS_base64_de(char* in_data, unsigned int in_len, unsigned char* out_data, unsigned int* out_len);

/**
* @brief CSS_sha_hash function
* @details SHA运算
* @param type [in] SHA算法类型，1-SHA1 2-SHA256
* @param in_data [in] SHA运算数据
* @param in_len [in] SHA运算数据长度
* @param out_data [out] SHA运算结果，SHA1长度为20个字节，SHA256长度为32个字节
* @return error code
*/
unsigned int CSS_sha_hash(unsigned char type, unsigned char* in_data, unsigned int in_len, unsigned char* out_data);

/**
* @brief CSS_sm3_hash function
* @details SM3运算
* @param in_data [in] 运算输入数据
* @param in_len [in] 运算输入数据长度
* @param out_data [out] 运算输入结果，长度为32个字节
* @return error code
*/
unsigned int CSS_sm3_hash(const unsigned char* in_data, unsigned int in_len, unsigned char* out_data);

/**
* @brief CSS_hmac_sha256 function
* @details HMAC SHA-256运算
* @param key [in] 密钥
* @param key_len [in] 密钥长度
* @param in_data [in] 运算数据
* @param in_len [in] 运算数据长度
* @param out_data [out] 运算输出结果
* @return error code
*/
unsigned int CSS_hmac_sha256(const unsigned char* key, int key_len, unsigned char* in_data, unsigned int in_len, unsigned char* out_data);

/**
* @brief CSS_sm2_public_calculate function
* @details SM2公钥计算
* @param in_data [in] 输入数据
* @param in_len [in] 输入数据长度
* @param out_data [out] 输出数据
* @param out_len [out] 输出数据长度
* @return error code
*/
unsigned int CSS_sm2_public_calculate(unsigned char* in_data, unsigned int in_len, unsigned char* out_data, unsigned int* out_len);

/**
* @brief CSS_sm2_private_calculate function
* @details SM2私钥计算
* @param in_data [in] 输入数据
* @param in_len [in] 输入数据长度
* @param out_data [out] 输出数据
* @param out_len [out] 输出数据长度
* @return error code
*/
unsigned int CSS_sm2_private_calculate(unsigned char* in_data, unsigned int in_len, unsigned char* out_data, unsigned int* out_len);

/**
* @brief CSS_sm2_sign function
* @details SM2数据签名
* @param in_data [in] 输入数据
* @param in_len [in] 输入数据长度
* @param out_data [out] 输出数据
* @param out_len [out] 输出数据长度
* @return error code
*/
unsigned int CSS_sm2_sign(unsigned char* in_data, unsigned int in_len, unsigned char* out_data, unsigned int* out_len);

/**
* @brief CSS_sm2_verify function
* @details SM2数据签名验签
* @param in_data [in] 输入数据
* @param in_len [in] 输入数据长度
* @param in_sign [in] 输入签名
* @return error code
*/
unsigned int CSS_sm2_verify(unsigned char* in_data, unsigned int in_len, unsigned char* in_sign);

#ifdef __cplusplus
}
#endif

#endif  // _SECURITY_SERVICE_H_

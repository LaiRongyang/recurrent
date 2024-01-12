#include <string.h>
#include "securityservice.h"
#include "./../client/security_request.h"
#include "./../common/logger.h"
unsigned int CSS_open_service()
{
    logger::getInstance().logOutStart();
    return CSecurityRequest::getInstance().OpenDevice();
}

void CSS_close_service()
{
    CSecurityRequest::getInstance().CloseDevice();
}
unsigned int CSS_generate_random(unsigned int len, unsigned char *rand)
{
    return CSecurityRequest::getInstance().GenerateRandom(len, rand);
}

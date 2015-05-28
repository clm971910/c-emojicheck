#include "emojicheck.h"


static const unsigned char s_head_byte_tab[256] =
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //0x00
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //0x10
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //0x20
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //0x30
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //0x40
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //0x50
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //0x60
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //1x70
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //1x80
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //1x90
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //1xA0
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, //1xB9
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, //1xC0
    2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, //0xD0
    3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, //0xE0
    4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6  //0xF0
};




/*****************************************************************************
 * 将一个字符的UTF8编码转换成Unicode(UCS-2和UCS-4)编码.
 *
 * 参数:
 *      pInput      指向输入缓冲区, 以UTF-8编码
 *      Unic        指向输出缓冲区, 其保存的数据即是Unicode编码值,
 *                  类型为unsigned long .
 *
 * 返回值:
 *      成功则返回该字符的UTF8编码所占用的字节数; 失败则返回0.
 *
 * 注意:
 *      1. UTF8没有字节序问题, 但是Unicode有字节序要求;
 *         字节序分为大端(Big Endian)和小端(Little Endian)两种;
 *         在Intel处理器中采用小端法表示, 在此采用小端法表示. (低地址存低位)
 *****************************************************************************/
int utf8_to_unicode( const unsigned char * pInput, unsigned long * unicode )
{
    // b1 表示UTF-8编码的pInput中的高字节, b2 表示次高字节, ...
    char b1, b2, b3, b4, b5, b6;
    
    *unicode = 0x0;
    
    int             utfbytes = s_head_byte_tab[ *pInput ];
    unsigned char * pOutput  = (unsigned char *) unicode;
    
    switch ( utfbytes )
    {
        case 1:
            *pOutput = *pInput;
            break;
        case 2:
            b1 = *pInput;
            b2 = *(pInput + 1);
            
            if ( (b2 & 0xE0) != 0x80 )  return 0;
            
            *pOutput     = (b1 << 6) + (b2 & 0x3F);
            *(pOutput+1) = (b1 >> 2) & 0x07;
            
            break;
        case 3:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) )
                return 0;
            
            *pOutput     = (b2 << 6) + (b3 & 0x3F);
            *(pOutput+1) = (b1 << 4) + ((b2 >> 2) & 0x0F);
            
            break;
        case 4:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80) || ((b4 & 0xC0) != 0x80) )
                return 0;
            
            *pOutput     = (b3 << 6) + (b4 & 0x3F);
            *(pOutput+1) = (b2 << 4) + ((b3 >> 2) & 0x0F);
            *(pOutput+2) = ((b1 << 2) & 0x1C)  + ((b2 >> 4) & 0x03);
            
            break;
        case 5:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80) )
                return 0;
            
            *pOutput     = (b4 << 6) + (b5 & 0x3F);
            *(pOutput+1) = (b3 << 4) + ((b4 >> 2) & 0x0F);
            *(pOutput+2) = (b2 << 2) + ((b3 >> 4) & 0x03);
            *(pOutput+3) = (b1 << 6);
            
            break;
        case 6:
            b1 = *pInput;
            b2 = *(pInput + 1);
            b3 = *(pInput + 2);
            b4 = *(pInput + 3);
            b5 = *(pInput + 4);
            b6 = *(pInput + 5);
            
            if ( ((b2 & 0xC0) != 0x80) || ((b3 & 0xC0) != 0x80)
                || ((b4 & 0xC0) != 0x80) || ((b5 & 0xC0) != 0x80)
                || ((b6 & 0xC0) != 0x80) )
                return 0;
            
            *pOutput     = (b5 << 6) + (b6 & 0x3F);
            *(pOutput+1) = (b5 << 4) + ((b6 >> 2) & 0x0F);
            *(pOutput+2) = (b3 << 2) + ((b4 >> 4) & 0x03);
            *(pOutput+3) = ((b1 << 6) & 0x40) + (b2 & 0x3F);
            
            break;
        default:
            return 0;
            break;
    }
    
    return utfbytes;
}

/**
 * 检查字符串src的从 *pos 开始的位置 是否为 emoji 字符，返回 true/false
 *
 * pos 只会修改为下一个字符的起始位置
 */
int check_emoji( const char * src, int slen, int * pos )
{
    unsigned long  uc    = 0;
    int            bytes = utf8_to_unicode( (const unsigned char *)(src + *pos ), &uc );
    
    if ( 0 == bytes ) return -1;
    
    *pos = (*pos) + bytes;
    
    if ( uc >= 0x1F680 && uc <= 0x1F6A4 ) goto successed;
    if ( uc >= 0x1F300 && uc <= 0x1F320 ) goto successed;
    if ( uc >= 0x1F330 && uc <= 0x1F335 ) goto successed;
    if ( uc >= 0x1F337 && uc <= 0x1F37C ) goto successed;
    if ( uc >= 0x1F380 && uc <= 0x1F393 ) goto successed;
    if ( uc >= 0x1F3A0 && uc <= 0x1F3C4 ) goto successed;
    if ( uc >= 0x1F3C6 && uc <= 0x1F3CA ) goto successed;
    if ( uc >= 0x1F3E0 && uc <= 0x1F3F0 ) goto successed;
    if ( uc >= 0x1F500 && uc <= 0x1F53D ) goto successed;
    if ( uc >= 0x1F540 && uc <= 0x1F543 ) goto successed;
    if ( uc >= 0x1F550 && uc <= 0x1F567 ) goto successed;
    if ( uc >= 0x1F5FB && uc <= 0x1F5FF ) goto successed;
    if ( uc >= 0x1F6A5 && uc <= 0x1F6C5 ) goto successed;
    
    if ( uc >= 0x1F400 && uc <= 0x1F4FC )
    {
        if ( uc == 0x1F441 || uc == 0x1F43F || uc == 0x1F4F8 ) return -1;
        
        goto successed;
    }
    
    if ( uc >= 0x1F600 && uc <= 0x1F64F )
    {
        if ( uc < 0x1F641 || uc > 0x1F644 ) return -1;
        
        goto successed;
    }
    
    return -1;
    
successed:
    
    return 0;
}



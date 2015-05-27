
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

static const unsigned int s_utf8_head_mask_xor[] =
{
    0x00,                           /* unused */
    0x7F,                           /* 1 byte */
    0x1F,                           /* 2 bytes */
    0x0F,                           /* 3 bytes */
    0x07,                           /* 4 bytes */
    0x03,                           /* 5 bytes */
    0x01,                           /* 6 bytes */
};


/*
 * func : get a gbk word from a specified position in gbk buffer
 *
 * args : str,slen, input string and its length
 *      : pos, the specified position in str, after calling pos will step one word
 *
 * ret  : 0, reache the end of str
 *      : else, word value
 */
inline unsigned short get_utf8_word(const char * src, int slen, int *pos)
{
    
    unsigned char  ch  = src[ *pos ];
    unsigned int   len = s_head_byte_tab[ ch ];
    
    // 看看越界了没有
    if ( *pos + len > (unsigned int)slen  )
        return 0;
    
    unsigned short value = ch & s_utf8_head_mask_xor[ len ];
    
    for(unsigned int j=1;j<len;j++)
    {
        ch = src[*pos+j];
        value = (value << 6) | (ch & 0x3F);
    }
    
    *pos += len;
    
    return value;
    
}



// 如何判断 emoji 表情
bool string_contain_emoji( const char * start, int slen )
{
    int            pos_h = 0;
    unsigned short hs    = get_utf8_word( start, slen, &pos_h );
    
    if ( 0 == hs ) return false;
    
    int            pos_l = pos_h;
    unsigned short ls    = get_utf8_word( start, slen, &pos_l );
    
    if ( 0 == ls ) return false;
    
    // 把 hs 和 ls 合并起来
    int uc = ((hs - 0xd800) * 0x400) + (ls - 0xdc00) + 0x10000;
    
    if ( uc > 0x1F680 && uc <= 0x1F6A4 ) return true;
    if ( uc > 0x1F300 && uc <= 0x1F320 ) return true;
    if ( uc > 0x1F330 && uc <= 0x1F335 ) return true;
    if ( uc > 0x1F337 && uc <= 0x1F37C ) return true;
    if ( uc > 0x1F380 && uc <= 0x1F393 ) return true;
    if ( uc > 0x1F3A0 && uc <= 0x1F3C4 ) return true;
    if ( uc > 0x1F3C6 && uc <= 0x1F3CA ) return true;
    if ( uc > 0x1F3E0 && uc <= 0x1F3F0 ) return true;
    if ( uc > 0x1F500 && uc <= 0x1F53D ) return true;
    if ( uc > 0x1F540 && uc <= 0x1F543 ) return true;
    if ( uc > 0x1F550 && uc <= 0x1F567 ) return true;
    if ( uc > 0x1F5FB && uc <= 0x1F5FF ) return true;
    if ( uc > 0x1F6A5 && uc <= 0x1F6C5 ) return true;
    
    if ( uc > 0x1F400 && uc <= 0x1F4FC )
    {
        if (uc==0x1F441 || uc==0x1F43F || uc==0x1F4F8)
            return false;
        
        return true;
    }
    
    if ( uc > 0x1F600 && uc <= 0x1F64F )
    {
        if (uc < 0x1F641 || uc > 0x1F644)
            return false;
        
        return true;
    }
    
    return false;
}


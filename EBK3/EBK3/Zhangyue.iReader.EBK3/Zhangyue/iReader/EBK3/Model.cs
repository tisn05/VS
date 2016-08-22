namespace Zhangyue.iReader.EBK3
{
    using System;

    public class Model
    {
        public int CATHY_LZ77_WINDOW_SIZE = 0x10000;
        public int CATHYEBK_CHAPTER_NAME_MAX_SIZE = 0x40;
        public int CATHYEBK_NAME_MAX_SIZE = 0x40;
        public int CATHYEBK_TXT_FILE_MAX_SIZE = 0xA00000;
        public int CATHYEBK_VERSION = 2;

        public uint record_length;
        public uint record_type;
    }
}


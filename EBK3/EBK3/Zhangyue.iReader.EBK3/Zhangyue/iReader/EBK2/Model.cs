namespace Zhangyue.iReader.EBK2
{
    using System;
    using System.Collections.Generic;

    public class Model
    {
        public byte[] bookbuf;
        public int CATHY_LZ77_WINDOW_SIZE = 0x10000;
        public int CATHYEBK_CHAPTER_NAME_MAX_SIZE = 0x40;
        public int CATHYEBK_NAME_MAX_SIZE = 0x40;
        public int CATHYEBK_TXT_FILE_MAX_SIZE = 0xa00000;
        public int CATHYEBK_VERSION = 2;
        public List<string> correctchapterlist = new List<string>();
    }
}


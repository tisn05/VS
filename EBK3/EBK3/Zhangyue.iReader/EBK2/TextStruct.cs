namespace Zhangyue.iReader.EBK2
{
    using System;
    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Sequential)]
    public struct TextStruct
    {
        public int book_id;
        public int head_data_size;
        public int ebk_version;
        public int ebk_size;
        public byte[] book_name;
        public int file_size;
        public int head_compress_size;
        public int first_compress_block_size;
        public int chapter_count;
        public int compress_block_count;
        public int media_count;
        public int media_data_length;
        public int txt_compress_size;
    }
}


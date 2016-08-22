namespace Zhangyue.iReader.EBK3
{
    using System;

    public class ChapterListByteModel
    {
        public byte[] chapter_content_decompress_length6 = new byte[4];
        public byte[] chapter_content_decompresss_offset5 = new byte[4];
        public byte[] chapter_data_block_offset4 = new byte[4];
        public byte[] chapter_data_type3 = new byte[2];
        public byte[] chapter_index1 = new byte[4];
        public byte[] chapter_level2 = new byte[2];
        public byte[] chapter_name_data8;
        public byte[] chapter_name_length7 = new byte[4];
    }
}


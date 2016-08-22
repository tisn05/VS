namespace Zhangyue.iReader.EBK3
{
    using System;
    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Sequential)]
    public struct ChapterListStruct
    {
        public uint chapter_name_length7;
        public uint chapter_index1;
        public ushort chapter_level2;
        public ushort chapter_data_type3;
        public uint chapter_data_block_offset4;
        public uint chapter_content_decompresss_offset5;
        public uint chapter_content_decompress_length6;
        public string chapter_name_data8;
    }
}


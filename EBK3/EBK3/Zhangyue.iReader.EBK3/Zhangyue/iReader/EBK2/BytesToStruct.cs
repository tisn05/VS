namespace Zhangyue.iReader.EBK2
{
    using System;

    public class BytesToStruct
    {
        public static ChapterCompress BytesToChaptercompress(byte[] bytes)
        {
            ChapterCompress compress = new ChapterCompress();
            int offset = 0;
            compress.offset = GetInt32(4, ref offset, bytes);
            compress.length = GetInt32(4, ref offset, bytes);
            return compress;
        }

        public static ChapterStruct BytesToChapterStruct(byte[] bytes)
        {
            ChapterStruct struct2 = new ChapterStruct();
            int offset = 0;
            struct2.chapter_name = GetBytes(0x40, ref offset, bytes);
            struct2.offset = GetInt32(4, ref offset, bytes);
            struct2.lenth = GetInt32(4, ref offset, bytes);
            return struct2;
        }

        public static TextStruct BytesToTextStruct(byte[] bytes)
        {
            TextStruct struct2 = new TextStruct();
            int offset = 0;
            struct2.book_id = GetInt32(4, ref offset, bytes);
            struct2.head_data_size = GetInt16(2, ref offset, bytes);
            struct2.ebk_version = GetInt16(2, ref offset, bytes);
            struct2.ebk_size = GetInt32(4, ref offset, bytes);
            struct2.book_name = GetBytes(0x40, ref offset, bytes);
            struct2.file_size = GetInt32(4, ref offset, bytes);
            struct2.head_compress_size = GetInt32(4, ref offset, bytes);
            struct2.first_compress_block_size = GetInt32(4, ref offset, bytes);
            struct2.chapter_count = GetInt16(2, ref offset, bytes);
            struct2.compress_block_count = GetInt16(2, ref offset, bytes);
            struct2.media_count = GetInt32(4, ref offset, bytes);
            struct2.media_data_length = GetInt32(4, ref offset, bytes);
            struct2.txt_compress_size = GetInt32(4, ref offset, bytes);
            return struct2;
        }

        private static byte[] GetBytes(int length, ref int offset, byte[] buff)
        {
            byte[] buffer = new byte[length];
            Array.Copy(buff, offset, buffer, 0, length);
            offset += length;
            return buffer;
        }

        private static int GetInt16(int length, ref int offset, byte[] buff)
        {
            byte[] buffer = new byte[length];
            Array.Copy(buff, offset, buffer, 0, length);
            offset += length;
            return BitConverter.ToInt16(buffer, 0);
        }

        private static int GetInt32(int length, ref int offset, byte[] buff)
        {
            byte[] buffer = new byte[length];
            Array.Copy(buff, offset, buffer, 0, length);
            offset += length;
            return BitConverter.ToInt32(buffer, 0);
        }
    }
}


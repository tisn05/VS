namespace Zhangyue.iReader.EBK3
{
    using System;

    public class ByteToStructEbk3
    {
        public ChapterListStruct GetBlockFileList(byte[] buffs)
        {
            ChapterListStruct struct2 = new ChapterListStruct();
            int offset = 0;
            struct2.chapter_name_length7 = (uint) GetInt32(4, ref offset, buffs);
            struct2.chapter_index1 = (uint) GetInt32(4, ref offset, buffs);
            struct2.chapter_level2 = (ushort) GetInt16(2, ref offset, buffs);
            struct2.chapter_data_type3 = (ushort) GetInt16(2, ref offset, buffs);
            struct2.chapter_data_block_offset4 = (uint) GetInt32(4, ref offset, buffs);
            struct2.chapter_content_decompresss_offset5 = (uint) GetInt32(4, ref offset, buffs);
            struct2.chapter_content_decompress_length6 = (uint) GetInt32(4, ref offset, buffs);
            return struct2;
        }

        public blockStruct GetBlockStruct(byte[] buffs)
        {
            blockStruct struct2 = new blockStruct();
            int offset = 0;
            struct2.header_key = (uint) GetInt32(4, ref offset, buffs);
            struct2.header_length = (uint) GetInt32(4, ref offset, buffs);
            struct2.decode_key = (uint) GetInt32(4, ref offset, buffs);
            return struct2;
        }

        private static byte[] GetBytes(int length, ref int offset, byte[] buff)
        {
            byte[] buffer = new byte[length];
            Array.Copy(buff, offset, buffer, 0, length);
            offset += length;
            return buffer;
        }

        public ChapterHeadModelStruct GetChapterHeadModelStruct(byte[] buffs)
        {
            ChapterHeadModelStruct struct2 = new ChapterHeadModelStruct();
            int offset = 0;
            struct2.chapter_count = (uint) GetInt32(4, ref offset, buffs);
            struct2.chapter_name_header_length = (uint) GetInt32(4, ref offset, buffs);
            return struct2;
        }

        public DynDataStruct GetDynDataStruct(byte[] buffs)
        {
            DynDataStruct struct2 = new DynDataStruct();
            int offset = 0;
            struct2.dyn_data_length = (uint) GetInt32(4, ref offset, buffs);
            struct2.book_id = (uint) GetInt32(4, ref offset, buffs);
            struct2.chapter_list_offset = (uint) GetInt32(4, ref offset, buffs);
            struct2.chapter_list_length = (uint) GetInt32(4, ref offset, buffs);
            return struct2;
        }

        public EBK3HeadStruct GetHeadStruct(byte[] buffs)
        {
            EBK3HeadStruct struct2 = new EBK3HeadStruct();
            int offset = 0;
            struct2.identifier = GetBytes(4, ref offset, buffs);
            struct2.header_key = (uint) GetInt32(4, ref offset, buffs);
            struct2.header_length = (uint) GetInt32(4, ref offset, buffs);
            struct2.decode_key = (uint) GetInt32(4, ref offset, buffs);
            return struct2;
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


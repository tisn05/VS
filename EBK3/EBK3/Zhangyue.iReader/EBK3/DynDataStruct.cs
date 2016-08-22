namespace Zhangyue.iReader.EBK3
{
    using System;
    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Sequential)]
    public struct DynDataStruct
    {
        public uint dyn_data_length;
        public uint book_id;
        public uint chapter_list_offset;
        public uint chapter_list_length;
    }
}


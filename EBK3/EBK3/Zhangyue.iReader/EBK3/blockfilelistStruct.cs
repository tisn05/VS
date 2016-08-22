namespace Zhangyue.iReader.EBK3
{
    using System;
    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Sequential)]
    public struct blockfilelistStruct
    {
        public uint file_name_length;
        public uint file_offset;
        public uint file_src_length;
        public uint file_compress_length;
    }
}


namespace Zhangyue.iReader.EBK3
{
    using System;
    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Sequential)]
    public struct blockfileStruct
    {
        public uint file_count;
        public uint file_header_length;
    }
}


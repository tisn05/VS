namespace Zhangyue.iReader.EBK3
{
    using System;
    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Sequential)]
    public struct blockStruct
    {
        public uint header_key;
        public uint header_length;
        public uint decode_key;
    }
}


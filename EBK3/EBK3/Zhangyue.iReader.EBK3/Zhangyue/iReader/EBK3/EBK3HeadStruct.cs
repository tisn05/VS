namespace Zhangyue.iReader.EBK3
{
    using System;
    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Sequential)]
    public struct EBK3HeadStruct
    {
        [MarshalAs(UnmanagedType.ByValArray, SizeConst=4)]
        public byte[] identifier;
        public uint header_key;
        public uint header_length;
        public uint decode_key;
    }
}


namespace Zhangyue.iReader.EBK2
{
    using System;
    using System.Runtime.InteropServices;

    [StructLayout(LayoutKind.Sequential)]
    public struct CATHYEBK_MEDIA_DATA_T
    {
        public int media_type;
        public int offset;
        public int data_offset;
        public int length;
    }
}

